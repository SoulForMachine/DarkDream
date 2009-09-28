#include "stdafx.h"
#include "PlaceObjectPanel.h"
#include "ActionPlaceObjects.h"
#include "ActionAddObject.h"
#include "ActionRemoveObjects.h"
#include "UndoManager.h"
#include "EM_PlaceObject.h"

#define CIRCLE_VERTEX_COUNT		64


using namespace System;
using namespace System::Drawing;
using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	EM_PlaceObject::EM_PlaceObject(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager)
		: EditMode(listener, persistent)
	{
		_panel = gcnew PlaceObjectPanel;
		_undoManager = undo_manager;
		_selectedEntities = new(mainPool) List<ModelEntity*>;
		_selecting = false;
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
				vertices[4 + i].set(x, 0.0f, z);
				vertices[4 + CIRCLE_VERTEX_COUNT + i].set(x * 1.25f, 0.0f, z * 1.25f);
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
	}

	EM_PlaceObject::~EM_PlaceObject()
	{
		delete _panel;
		delete _selectedEntities;

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
		SetCursor(LoadCursor(0, IDC_ARROW));
		UpdateSelectionRect(x, y);
	}

	void EM_PlaceObject::LeftButtonDown(int x, int y)
	{
		if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			AddObject(x, y);
		}
		else
		{
			_selectionRect.X = x;
			_selectionRect.Y = y;
			_selectionRect.Width = 0;
			_selectionRect.Height = 0;
			_selStartPoint.X = x;
			_selStartPoint.Y = y;
			_selecting = true;
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
	}

	void EM_PlaceObject::KeyDown(int key)
	{
		switch(key)
		{
		case VK_ESCAPE:
			_selectedEntities->Clear();
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

		min_pt.X = Min(_selStartPoint.X, x);
		min_pt.Y = Min(_selStartPoint.Y, y);
		max_pt.X = Max(_selStartPoint.X, x);
		max_pt.Y = Max(_selStartPoint.Y, y);

		_selectionRect.Location = min_pt;
		_selectionRect.Width = max_pt.X - min_pt.X;
		_selectionRect.Height = max_pt.Y - min_pt.Y;
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

			float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			_fragpConstColor->GetASMProgram()->LocalParameter(0, color);

			for(List<ModelEntity*>::ConstIterator it = _selectedEntities->Begin(); it != _selectedEntities->End(); ++it)
			{
				const AABBox& bbox = (*it)->GetWorldBoundingBox();
				vec3f center(
					(bbox.maxPt.x + bbox.minPt.x) * 0.5f,
					bbox.minPt.y,
					(bbox.maxPt.z + bbox.minPt.z) * 0.5f);
				float scale = Max(bbox.maxPt.x - bbox.minPt.x, bbox.maxPt.z - bbox.minPt.z) * 0.5f;

				mat4f world, wvp;
				world.set_scale(scale);
				world.translate(center);
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

	void EM_PlaceObject::ClearSelection()
	{
		_selectedEntities->Clear();
	}

	void EM_PlaceObject::AddObject(int x, int y)
	{
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
		_selectedEntities->Clear();
		ModelEntity** vis_ents = new(tempPool) ModelEntity*[World::MAX_NUM_ENTITIES];
		int count = engineAPI->world->GetVisibleEntities(vis_ents, World::MAX_NUM_ENTITIES);
		if(count > 0)
		{
			// make 4 world-space planes from selection rect
			float viewport[4];
			_renderer->GetViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			mat4f inv_view_proj = engineAPI->world->GetCamera().GetViewProjectionTransform();
			inv_view_proj.inverse();

			vec4f planes[4]; // left, top, right, bottom
			vec3f point;

			const vec4f& cp_left = engineAPI->world->GetCamera().GetClipPlane(Camera::CP_LEFT);
			const vec4f& cp_right = engineAPI->world->GetCamera().GetClipPlane(Camera::CP_RIGHT);
			const vec4f& cp_top = engineAPI->world->GetCamera().GetClipPlane(Camera::CP_TOP);
			const vec4f& cp_bottom = engineAPI->world->GetCamera().GetClipPlane(Camera::CP_BOTTOM);

			unproject(point, _selectionRect.Left, (int)viewport[3] - _selectionRect.Bottom, inv_view_proj, viewport);
			planes[0].rvec3 = cp_left.rvec3;
			planes[0].w = -dot(cp_left.rvec3, point);
			planes[1].rvec3 = cp_top.rvec3;
			planes[1].w = -dot(cp_top.rvec3, point);

			unproject(point, _selectionRect.Right, (int)viewport[3] - _selectionRect.Top, inv_view_proj, viewport);
			planes[2].rvec3 = cp_right.rvec3;
			planes[2].w = -dot(cp_right.rvec3, point);
			planes[3].rvec3 = cp_bottom.rvec3;
			planes[3].w = -dot(cp_bottom.rvec3, point);

			for(int i = 0; i < count; ++i)
			{
				const AABBox& bbox = vis_ents[i]->GetWorldBoundingBox();
				if(BBoxInSelRect(bbox, planes))
					_selectedEntities->PushBack(vis_ents[i]);
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

	bool EM_PlaceObject::BBoxInSelRect(const AABBox& bbox, const vec4f planes[4])
	{
		for(int i = 0; i < 4; ++i)
		{
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.minPt.y, bbox.minPt.z), planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.minPt.y, bbox.minPt.z), planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.maxPt.y, bbox.minPt.z), planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.maxPt.y, bbox.minPt.z), planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.minPt.y, bbox.maxPt.z), planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.minPt.y, bbox.maxPt.z), planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.maxPt.y, bbox.maxPt.z), planes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.maxPt.y, bbox.maxPt.z), planes[i]) > 0) continue;

			return false;
		}

		return true;
	}

}
