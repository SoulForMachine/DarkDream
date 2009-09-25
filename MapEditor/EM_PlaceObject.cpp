#include "stdafx.h"
#include "PlaceObjectPanel.h"
#include "ActionPlaceObjects.h"
#include "ActionAddObject.h"
#include "ActionRemoveObjects.h"
#include "UndoManager.h"
#include "EM_PlaceObject.h"

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
	}

	EM_PlaceObject::~EM_PlaceObject()
	{
		delete _panel;
		delete _selectedEntities;
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

		if(_selecting)
		{
			// update selection rect
			Point min_pt, max_pt;
			
			if(_selectionRect.X < x)
			{
				min_pt.X = _selectionRect.X;
				max_pt.X = x;
			}
			else
			{
				min_pt.X = x;
				max_pt.X = _selectionRect.X;
			}

			if(_selectionRect.Y < y)
			{
				min_pt.Y = _selectionRect.Y;
				max_pt.Y = y;
			}
			else
			{
				min_pt.Y = y;
				max_pt.Y = _selectionRect.Y;
			}

			_selectionRect.Location = min_pt;
			_selectionRect.Width = max_pt.X - min_pt.X;
			_selectionRect.Height = max_pt.Y - min_pt.Y;
		}
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
			_selecting = true;
		}
	}

	void EM_PlaceObject::LeftButtonUp(int x, int y)
	{
		if(_selecting)
		{
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

	void EM_PlaceObject::Render()
	{
		if(_selectedEntities->GetCount() > 0)
		{
		}
	}

	void EM_PlaceObject::ClearSelection()
	{
		_selectedEntities->Clear();
	}

	void EM_PlaceObject::AddObject(int x, int y)
	{
		int vp_x, vp_y, vp_width, vp_height;
		engineAPI->renderSystem->GetRenderer()->GetViewport(vp_x, vp_y, vp_width, vp_height);
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

			unproject(point, _selectionRect.Left, (int)viewport[3] - _selectionRect.Top, inv_view_proj, viewport);
			planes[0].rvec3 = cp_left.rvec3;
			planes[0].w = -dot(cp_left.rvec3, point);
			planes[1].rvec3 = cp_top.rvec3;
			planes[1].w = -dot(cp_top.rvec3, point);

			unproject(point, _selectionRect.Right, (int)viewport[3] - _selectionRect.Bottom, inv_view_proj, viewport);
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
	}

	void EM_PlaceObject::DeleteObjects()
	{
		ActionRemoveObjects^ action = gcnew ActionRemoveObjects(*_selectedEntities);
		if(action->BeginAction())
		{
			action->EndAction();
			_undoManager->Add(action);
			_selectedEntities->Clear();
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
