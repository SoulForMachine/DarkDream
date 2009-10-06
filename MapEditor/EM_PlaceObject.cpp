#include "stdafx.h"
#include "resource.h"
#include "PlaceObjectPanel.h"
#include "ActionPlaceObjects.h"
#include "ActionAddObject.h"
#include "ActionRemoveObjects.h"
#include "EM_PlaceObject.h"

#define CIRCLE_VERTEX_COUNT		64
#define GIZMO_INNER_SCALE		1.0f
#define GIZMO_OUTER_SCALE		2.0f


using namespace System;
using namespace System::Windows;
using namespace System::Drawing;
using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	EM_PlaceObject::EM_PlaceObject(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager)
		: EditMode(listener, persistent)
	{
		_panel = gcnew PlaceObjectPanel(this);
		_undoManager = undo_manager;
		_undoManager->RegisterListener(this);
		_actionPlaceObjs = nullptr;
		_selectedEntities = new(mainPool) List<ModelEntity*>;
		_parameters = gcnew Parameters;
		_parameters->selectedEntities = _selectedEntities;
		_moveStartPoint = new(mainPool) vec3f;
		_moveEndPoint = new(mainPool) vec3f;
		_selecting = false;
		_placing = false;
		_renderer = engineAPI->renderSystem->GetRenderer();

		_vertBufSelRect = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, 4 * sizeof(vec3f), 0, GL::USAGE_DYNAMIC_DRAW);
		ushort indices[] = { 0, 1, 3, 2 };
		_indBufSelRect = _renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, 4 * sizeof(ushort), indices, GL::USAGE_STATIC_DRAW);

		// allocate vertex memory for:
		// - 2 lines
		// - 2 line loops with 64 vertices each
		_vertBufSelMark = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, (CIRCLE_VERTEX_COUNT * 2 + 4) * sizeof(vec3f), 0, GL::USAGE_STATIC_DRAW);
		vec3f* vertices = (vec3f*)_vertBufSelMark->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(vertices)
		{
			vertices[0].set(-1.0f, 0.0f, 0.0f);
			vertices[1].set(1.0f, 0.0f, 0.0f);
			vertices[2].set(0.0f, 0.0f, -1.0f);
			vertices[3].set(0.0f, 0.0f, 1.0f);

			float d = TWO_PI / CIRCLE_VERTEX_COUNT;
			float angle = 0.0f;

			for(int i = 0; i < CIRCLE_VERTEX_COUNT; ++i)
			{
				float x = cos(angle);
				float z = sin(angle);
				vertices[4 + i].set(x * GIZMO_INNER_SCALE, 0.0f, z * GIZMO_INNER_SCALE);
				vertices[4 + CIRCLE_VERTEX_COUNT + i].set(x * GIZMO_OUTER_SCALE, 0.0f, z * GIZMO_OUTER_SCALE);
				angle += d;
			}

			_vertBufSelMark->UnmapBuffer();
		}

		GL::VertexAttribDesc desc[] =
		{
			{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 }
		};
		_vertFmtPos = _renderer->CreateVertexFormat(desc, COUNTOF(desc));

		_vertpSimple2D = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple2D.vp"), true);
		_vertpSimple = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fragpConstColor = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		HINSTANCE hinst = (HINSTANCE)Runtime::InteropServices::Marshal::GetHINSTANCE(GetType()->Module).ToPointer();
		_cursorMove = LoadCursor(hinst, MAKEINTRESOURCE(IDC_CURSOR_MOVE));
		_cursorMoveUD = LoadCursor(hinst, MAKEINTRESOURCE(IDC_CURSOR_MOVE_UD));
		_cursorRotate = LoadCursor(hinst, MAKEINTRESOURCE(IDC_CURSOR_ROTATE));
	}

	EM_PlaceObject::~EM_PlaceObject()
	{
		delete _panel;
		delete _selectedEntities;
		delete _moveStartPoint;
		delete _moveEndPoint;

		_renderer->DestroyBuffer(_vertBufSelRect);
		_renderer->DestroyBuffer(_indBufSelRect);
		_renderer->DestroyBuffer(_vertBufSelMark);
		_renderer->DestroyVertexFormat(_vertFmtPos);
		engineAPI->asmProgManager->ReleaseASMProgram(_vertpSimple2D);
		engineAPI->asmProgManager->ReleaseASMProgram(_vertpSimple);
		engineAPI->asmProgManager->ReleaseASMProgram(_fragpConstColor);
	}

	System::Windows::Forms::UserControl^ EM_PlaceObject::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_PlaceObject::GetModeEnum()
	{
		return EditModeEnum::PLACE_OBJECT;
	}

	void EM_PlaceObject::Activate()
	{
		
	}

	void EM_PlaceObject::MouseMove(int modifiers, int x, int y)
	{
		if(_selecting)
		{
			SetCursor(LoadCursor(0, IDC_ARROW));
			UpdateSelectionRect(x, y);
		}
		else if(_placing)
		{
			switch(_parameters->transformType)
			{
			case TransformType::MOVE_XZ:
				{
					SetCursor(_cursorMove);

					int viewport[4];
					_renderer->GetViewport(viewport);
					engineAPI->world->GetTerrain().PickTerrainPoint(x, viewport[3] - y, *_moveEndPoint);
					_parameters->translX = _moveEndPoint->x - _moveStartPoint->x;
					_parameters->translZ = _moveEndPoint->z - _moveStartPoint->z;
				}
				break;
			case TransformType::MOVE_Y:
				SetCursor(_cursorMoveUD);

				_parameters->translY = (_mouseStartPoint.Y - y) * 0.08f;
				break;
			case TransformType::ROTATE:
				SetCursor(_cursorRotate);

				_parameters->rotateY = (x - _mouseStartPoint.X) * 0.5f;
				break;
			}
		}
		else
		{
			GizmoType gizmo = MouseOverGizmo(x, y);
			switch(gizmo)
			{
			case GizmoType::MOVE_XZ:
				SetCursor(_cursorMove);
				break;
			case GizmoType::MOVE_Y:
				SetCursor(_cursorMoveUD);
				break;
			case GizmoType::ROTATE:
				SetCursor(_cursorRotate);
				break;
			default:
				SetCursor(LoadCursor(0, IDC_ARROW));
			}
		}
	}

	void EM_PlaceObject::LeftButtonDown(int x, int y)
	{
		if(_panel->GetMode() == PlaceObjectPanel::Mode::ADD_OBJECT)
		{
			AddObject(x, y);
			if(!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				_panel->SetMode(PlaceObjectPanel::Mode::PLACE_OBJECT);
			}
		}
		else if(_panel->GetMode() == PlaceObjectPanel::Mode::PLACE_OBJECT)
		{
			GizmoType gizmo = MouseOverGizmo(x, y);
			if(gizmo == GizmoType::NONE)
			{
				// start selecting
				_selectionRect.X = x;
				_selectionRect.Y = y;
				_selectionRect.Width = 0;
				_selectionRect.Height = 0;
				_mouseStartPoint.X = x;
				_mouseStartPoint.Y = y;
				_selecting = true;
			}
			else
			{
				switch(gizmo)
				{
				case GizmoType::MOVE_XZ:
					{
						int viewport[4];
						_renderer->GetViewport(viewport);
						if(engineAPI->world->GetTerrain().PickTerrainPoint(x, viewport[3] - y, *_moveStartPoint) == -1)
							return;
						*_moveEndPoint = *_moveStartPoint;

						_parameters->transformType = TransformType::MOVE_XZ;
						_parameters->translX = 0.0f;
						_parameters->translZ = 0.0f;
					}
					break;
				case GizmoType::MOVE_Y:
					_mouseStartPoint.X = x;
					_mouseStartPoint.Y = y;
					_parameters->transformType = TransformType::MOVE_Y;
					_parameters->translY = 0.0f;
					break;
				case GizmoType::ROTATE:
					_mouseStartPoint.X = x;
					_mouseStartPoint.Y = y;
					_parameters->transformType = TransformType::ROTATE;
					_parameters->rotateY = 0.0f;
					break;
				}

				_actionPlaceObjs = gcnew ActionPlaceObjects(_parameters);
				_placing = _actionPlaceObjs->BeginAction();
				if(!_placing)
				{
					delete _actionPlaceObjs;
					_actionPlaceObjs = nullptr;
				}
			}
		}
	}

	void EM_PlaceObject::LeftButtonUp(int x, int y)
	{
		if(_selecting)
		{
			UpdateSelectionRect(x, y);
			SelectObjects();
			_selecting = false;
		}
		else if(_placing && _actionPlaceObjs != nullptr)
		{
			_actionPlaceObjs->EndAction();
			_undoManager->Add(_actionPlaceObjs);
			_actionPlaceObjs = nullptr;
			_placing = false;
		}
	}

	void EM_PlaceObject::KeyDown(int key)
	{
		switch(key)
		{
		case VK_ESCAPE:
			if(_selecting)
			{
				_selecting = false;
			}
			else if(_placing && _actionPlaceObjs != nullptr)
			{
				_placing = false;
				_actionPlaceObjs->CancelAction();
				delete _actionPlaceObjs;
				_actionPlaceObjs = nullptr;
			}
			else
			{
				_selectedEntities->Clear();
			}
			break;
		case VK_DELETE:
			DeleteObjects();
			break;
		}
	}

	void EM_PlaceObject::UpdateSelectionRect(int x, int y)
	{
		// update selection rect
		Point min_pt, max_pt;

		min_pt.X = Min(_mouseStartPoint.X, x);
		min_pt.Y = Min(_mouseStartPoint.Y, y);
		max_pt.X = Max(_mouseStartPoint.X, x);
		max_pt.Y = Max(_mouseStartPoint.Y, y);

		_selectionRect.Location = min_pt;
		_selectionRect.Width = max_pt.X - min_pt.X;
		if(_selectionRect.Width == 0)
			_selectionRect.Width = 1;
		_selectionRect.Height = max_pt.Y - min_pt.Y;
		if(_selectionRect.Height == 0)
			_selectionRect.Height = 1;

		_selectOne = (_selectionRect.Width == 1 && _selectionRect.Height == 1);
	}

	void EM_PlaceObject::SelectEntity(ModelEntity* entity, SelectMode mode)
	{
		// check if already in list
		for(List<ModelEntity*>::Iterator it = _selectedEntities->Begin(); it != _selectedEntities->End(); ++it)
		{
			if(*it == entity)
			{
				if(mode == SelectMode::INVERT_SELECTION)
					_selectedEntities->Remove(it);
				return;
			}
		}

		_selectedEntities->PushBack(entity);
	}

	EM_PlaceObject::GizmoType EM_PlaceObject::MouseOverGizmo(int x, int y)
	{
		if(_selectedEntities->GetCount() > 0)
		{
			float viewport[4];
			_renderer->GetViewport(viewport);
			mat4f view_proj = engineAPI->world->GetCamera().GetViewProjectionTransform();
			view_proj.inverse();

			for(List<ModelEntity*>::ConstIterator it = _selectedEntities->Begin(); it != _selectedEntities->End(); ++it)
			{
				vec3f center = (*it)->GetPosition();
				vec4f plane(vec3f::y_axis, -center.y);
				vec3f ray_pt;
				unproject(ray_pt, x, (int)viewport[3] - y, view_proj, viewport);
				vec3f ray_dir = ray_pt - engineAPI->world->GetCamera().GetPosition();
				vec3f pt;

				if(intersect_ray_plane(pt, ray_pt, ray_dir, plane))
				{
					float dist = (pt - center).length();
					if(dist <= GIZMO_INNER_SCALE)
					{
						return GizmoType::MOVE_XZ;
					}
					else if(dist < GIZMO_OUTER_SCALE - (GIZMO_OUTER_SCALE - GIZMO_INNER_SCALE) / 2.0f)
					{
						return GizmoType::MOVE_Y;
					}
					else if(dist < GIZMO_OUTER_SCALE * 1.1f)
					{
						return GizmoType::ROTATE;
					}
				}
			}
		}

		return GizmoType::NONE;
	}

	void EM_PlaceObject::Update(float dt)
	{
		if(_placing && _actionPlaceObjs != nullptr)
		{
			_actionPlaceObjs->Update(dt);
		}
	}

	void EM_PlaceObject::Render()
	{
		// render markers/gizmos on selected objects
		if(_selectedEntities->GetCount() > 0)
		{
			_renderer->ActiveVertexFormat(_vertFmtPos);
			_renderer->VertexSource(0, _vertBufSelMark, sizeof(vec3f), 0);
			_renderer->IndexSource(0, GL::TYPE_VOID);
			_renderer->EnableDepthTest(false);
			_renderer->ActiveVertexASMProgram(_vertpSimple->GetASMProgram());
			_renderer->ActiveFragmentASMProgram(_fragpConstColor->GetASMProgram());

			float color[] = { 0.0f, 1.0f, 0.0f, 1.0f };
			_fragpConstColor->GetASMProgram()->LocalParameter(0, color);

			for(List<ModelEntity*>::ConstIterator it = _selectedEntities->Begin(); it != _selectedEntities->End(); ++it)
			{
				mat4f world, wvp;
				world.set_translation((*it)->GetPosition());
				mul(wvp, world, engineAPI->world->GetCamera().GetViewProjectionTransform());
				_vertpSimple->GetASMProgram()->LocalMatrix4x4(0, wvp);

				_renderer->Draw(GL::PRIM_LINES, 0, 4);
				_renderer->Draw(GL::PRIM_LINE_LOOP, 4, CIRCLE_VERTEX_COUNT);
				_renderer->Draw(GL::PRIM_LINE_LOOP, 4 + CIRCLE_VERTEX_COUNT, CIRCLE_VERTEX_COUNT);
			}

			_renderer->EnableDepthTest(true);
		}

		// draw selection rectangle
		if(_selecting)
		{
			float viewport[4];
			_renderer->GetViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

			vec3f* vertices = (vec3f*)_vertBufSelRect->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
			if(vertices)
			{
				vertices[0].set((float)_selectionRect.Left, viewport[3] - _selectionRect.Bottom, 0.0f);
				vertices[1].set((float)_selectionRect.Right, viewport[3] - _selectionRect.Bottom, 0.0f);
				vertices[2].set((float)_selectionRect.Right, viewport[3] - _selectionRect.Top, 0.0f);
				vertices[3].set((float)_selectionRect.Left, viewport[3] - _selectionRect.Top, 0.0f);

				if(_vertBufSelRect->UnmapBuffer())
				{
					_renderer->ActiveVertexFormat(_vertFmtPos);
					_renderer->VertexSource(0, _vertBufSelRect, sizeof(vec3f), 0);
					_renderer->IndexSource(_indBufSelRect, GL::TYPE_UNSIGNED_SHORT);
					_renderer->ActiveVertexASMProgram(_vertpSimple2D->GetASMProgram());
					_renderer->ActiveFragmentASMProgram(_fragpConstColor->GetASMProgram());
					_renderer->EnableDepthTest(false);

					_vertpSimple2D->GetASMProgram()->LocalParameter(0, viewport);

					_renderer->EnableBlending(true);
					_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
					float fill_color[] = { 0.0f, 0.0f, 1.0f, 0.1f };
					_fragpConstColor->GetASMProgram()->LocalParameter(0, fill_color);
					_renderer->DrawIndexed(GL::PRIM_TRIANGLE_STRIP, 0, 4);
					_renderer->EnableBlending(false);

					float outl_color[] = { 0.2f, 0.2f, 0.7f, 1.0f };
					_renderer->IndexSource(0, GL::TYPE_VOID);
					_fragpConstColor->GetASMProgram()->LocalParameter(0, outl_color);
					_renderer->Draw(GL::PRIM_LINE_LOOP, 0, 4);

					_renderer->EnableDepthTest(true);
				}
			}
		}
	}

	void EM_PlaceObject::UndoEvent(UndoEventListener::EventType type, Action^ action)
	{
		if(	(type == UndoEventListener::EventType::UNDO &&
			action->GetActionType() == ActionType::ADD_OBJECT) ||
			(type == UndoEventListener::EventType::REDO &&
			(action->GetActionType() == ActionType::REMOVE_OBJECTS || action->GetActionType() == ActionType::REMOVE_PATCH)) )
		{
			ClearSelection();
		}
	}

	void EM_PlaceObject::ClearSelection()
	{
		_selectedEntities->Clear();
	}

	void EM_PlaceObject::DropSelected()
	{
		_parameters->transformType = TransformType::DROP;
		ActionPlaceObjects^ action = gcnew ActionPlaceObjects(_parameters);
		if(action->BeginAction())
		{
			action->EndAction();
			_undoManager->Add(action);
		}
		else
		{
			delete action;
		}
	}

	void EM_PlaceObject::AddObject(int x, int y)
	{
		if(_panel->GetSelObjectPath() == nullptr)
		{
			Forms::MessageBox::Show("You must select an entity to add.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Information);
			return;
		}

		int vp_x, vp_y, vp_width, vp_height;
		_renderer->GetViewport(vp_x, vp_y, vp_width, vp_height);
		vec3f point;
		if(engineAPI->world->GetTerrain().PickTerrainPoint(x, vp_height - y, point) != -1)
		{
			ActionAddObject^ action = gcnew ActionAddObject(_panel->GetSelObjectPath(), point);
			if(action->BeginAction())
			{
				action->EndAction();
				_undoManager->Add(action);
			}
		}
	}

	void EM_PlaceObject::SelectObjects()
	{
		SelectMode mode;
		if((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
			mode = SelectMode::INVERT_SELECTION;
		else if((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
			mode = SelectMode::ADD_TO_SELECTION;
		else
		{
			mode = SelectMode::NEW_SELECTION;
			_selectedEntities->Clear();
		}

		ModelEntity** vis_ents = new(tempPool) ModelEntity*[World::MAX_NUM_ENTITIES];
		int count = engineAPI->world->GetVisibleEntities(vis_ents, World::MAX_NUM_ENTITIES);
		if(count > 0)
		{
			// make 4 world-space planes from selection rect
			float viewport[4];
			_renderer->GetViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			mat4f inv_view_proj = engineAPI->world->GetCamera().GetViewProjectionTransform();
			inv_view_proj.inverse();

			vec3f points[4]; // world-space points of selection frustum top

			unproject(points[0], _selectionRect.Left, (int)viewport[3] - _selectionRect.Top, inv_view_proj, viewport);
			unproject(points[1], _selectionRect.Left, (int)viewport[3] - _selectionRect.Bottom, inv_view_proj, viewport);
			unproject(points[2], _selectionRect.Right, (int)viewport[3] - _selectionRect.Bottom, inv_view_proj, viewport);
			unproject(points[3], _selectionRect.Right, (int)viewport[3] - _selectionRect.Top, inv_view_proj, viewport);

			const vec3f& cam_pos = engineAPI->world->GetCamera().GetPosition();
			vec3f vec1 = cam_pos - points[0];
			vec3f vec2 = cam_pos - points[2];
			vec4f planes[4]; // left, top, right, bottom

			cross(planes[0].rvec3, points[1] - points[0], points[0] - cam_pos);
			planes[0].rvec3.normalize();
			planes[0].w = -dot(planes[0].rvec3, points[0]);

			cross(planes[1].rvec3, points[0] - cam_pos, points[3] - points[0]);
			planes[1].rvec3.normalize();
			planes[1].w = -dot(planes[1].rvec3, points[0]);

			cross(planes[2].rvec3, points[3] - cam_pos, points[2] - points[3]);
			planes[2].rvec3.normalize();
			planes[2].w = -dot(planes[2].rvec3, points[2]);

			cross(planes[3].rvec3, points[2] - cam_pos, points[1] - points[2]);
			planes[3].rvec3.normalize();
			planes[3].w = -dot(planes[3].rvec3, points[2]);

			if(_selectOne)
			{
				ModelEntity* entity = 0;
				for(int i = 0; i < count; ++i)
				{
					const OBBox& bbox = vis_ents[i]->GetWorldBoundingBox();
					if(BBoxInSelRect(bbox, planes))
					{
						if(entity)
						{
							if((cam_pos - vis_ents[i]->GetPosition()).length() < (cam_pos - entity->GetPosition()).length())
								entity = vis_ents[i];
						}
						else
						{
							entity = vis_ents[i];
						}
					}
				}

				if(entity)
					SelectEntity(entity, mode);
			}
			else
			{
				for(int i = 0; i < count; ++i)
				{
					const OBBox& bbox = vis_ents[i]->GetWorldBoundingBox();
					if(BBoxInSelRect(bbox, planes))
						SelectEntity(vis_ents[i], mode);
				}
			}
		}
		delete[] vis_ents;
	}

	void EM_PlaceObject::DeleteObjects()
	{
		if(_selectedEntities->GetCount() > 0)
		{
			ActionRemoveObjects^ action = gcnew ActionRemoveObjects(*_selectedEntities);
			if(action->BeginAction())
			{
				action->EndAction();
				_undoManager->Add(action);
				_selectedEntities->Clear();
			}
		}
	}

	bool EM_PlaceObject::BBoxInSelRect(const OBBox& bbox, const vec4f planes[4])
	{
		for(int i = 0; i < 4; ++i)
		{
			if(point_to_plane_sgn_dist(bbox.points[0], planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[1], planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[2], planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[3], planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[4], planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[5], planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[6], planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[7], planes[i]) > 0) continue;

			return false;
		}

		return true;
	}

}
