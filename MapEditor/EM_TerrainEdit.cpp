
#include "stdafx.h"
#include "TerrainEditPanel.h"
#include "UndoManager.h"
#include "ActionTerrainEdit.h"
#include "TerrainBrush.h"
#include "EM_TerrainEdit.h"

using namespace math3d;


namespace MapEditor
{

	EM_TerrainEdit::EM_TerrainEdit(UndoManager^ undo_manager)
	{
		_parameters = gcnew Parameters;
		_panel = gcnew TerrainEditPanel(_parameters);
		_action = nullptr;
		_undoManager = undo_manager;
		_brush = gcnew TerrainBrush(_parameters);
		_brush->Init();
		_intersection = false;
	}

	EM_TerrainEdit::~EM_TerrainEdit()
	{
		_brush->Deinit();
	}

	System::Windows::Forms::UserControl^ EM_TerrainEdit::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_TerrainEdit::GetModeEnum()
	{
		return EditModeEnum::TERRAIN_EDIT;
	}

	void EM_TerrainEdit::MouseMove(int modifiers, int x, int y)
	{
		int vp_x, vp_y, vp_width, vp_height;
		engineAPI->renderSystem->GetRenderer()->GetViewport(vp_x, vp_y, vp_width, vp_height);
		vec3f point;
		_intersection = engineAPI->world->GetTerrain().PickTerrainPoint(x, vp_height - y, point);
		if(_intersection)
		{
			_parameters->posX = point.x;
			_parameters->posY = point.y;
			_parameters->posZ = point.z;

			if(_isExecuting && _action != nullptr)
			{
			}
		}
	}

	void EM_TerrainEdit::LeftButtonDown(int x, int y)
	{
		_isExecuting = true;
		_action = gcnew ActionTerrainEdit(_parameters);
		_action->BeginAction();
	}

	void EM_TerrainEdit::LeftButtonUp(int x, int y)
	{
		if(_isExecuting && _action != nullptr)
		{
			_isExecuting = false;
			_action->EndAction();
			_undoManager->Add(_action);
			_action = nullptr;
		}
	}

	void EM_TerrainEdit::KeyDown(int key)
	{
		if(key == VK_ESCAPE)
		{
			if(_isExecuting && _action != nullptr)
			{
				_action->CancelAction();
				delete _action;
			}
		}
	}

	void EM_TerrainEdit::Render()
	{
		if(_intersection)
			_brush->Draw();
	}

	void EM_TerrainEdit::Update()
	{
	}

}
