
#include "stdafx.h"
#include "GrassPanel.h"
#include "UndoManager.h"
#include "ActionPaintGrass.h"
#include "GrassBrush.h"
#include "EM_PaintGrass.h"

using namespace math3d;


namespace MapEditor
{

	EM_PaintGrass::EM_PaintGrass(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager)
			: EditMode(listener, persistent)
	{
		_parameters = gcnew Parameters;
		_parameters->radius = 4.0f;
		_parameters->hardness = 0.6f;
		_parameters->strength = 5.0f;
		_parameters->scale = 1.0f;
		_parameters->texIndex = 0;
		_parameters->posX = 0.0f;
		_parameters->posY = 0.0f;
		_parameters->posZ = 0.0f;
		_parameters->executing = false;

		_panel = gcnew GrassPanel(_parameters);
		_action = nullptr;
		_undoManager = undo_manager;
		_brush = gcnew GrassBrush(_parameters);
		_brush->Init();
		_overTerrain = false;
	}

	EM_PaintGrass::~EM_PaintGrass()
	{
		_brush->Deinit();
	}

	System::Windows::Forms::UserControl^ EM_PaintGrass::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_PaintGrass::GetModeEnum()
	{
		return EditModeEnum::PAINT_GRASS;
	}

	void EM_PaintGrass::MouseMove(int modifiers, int x, int y)
	{
		int vp_x, vp_y, vp_width, vp_height;
		engineAPI->renderSystem->GetRenderer()->GetViewport(vp_x, vp_y, vp_width, vp_height);
		vec3f point;
		_overTerrain = engineAPI->world->GetTerrain().PickTerrainPoint(x, vp_height - y, point) != -1;
		if(_overTerrain)
		{
			_parameters->posX = point.x;
			_parameters->posY = point.y;
			_parameters->posZ = point.z;

			SetCursor(0);
		}
		else
		{
			SetCursor(LoadCursor(0, IDC_ARROW));
		}
	}

	void EM_PaintGrass::LeftButtonDown(int x, int y)
	{
		if(_overTerrain)
		{
			_isExecuting = true;
			_parameters->executing = true;
			_action = gcnew ActionPaintGrass(_parameters);
			_action->BeginAction();
		}
	}

	void EM_PaintGrass::LeftButtonUp(int x, int y)
	{
		if(_isExecuting && _action != nullptr)
		{
			_isExecuting = false;
			_parameters->executing = false;
			_action->EndAction();
			_undoManager->Add(_action);
			_action = nullptr;
		}
	}

	void EM_PaintGrass::KeyDown(int key)
	{
		if(key == VK_ESCAPE)
		{
			if(_isExecuting && _action != nullptr)
			{
				_isExecuting = false;
				_parameters->executing = false;
				_action->CancelAction();
				delete _action;
				_action = nullptr;
			}
		}
		else if(key == VK_OEM_4)
		{
			_parameters->radius -= 0.5f;
			if(_parameters->radius < 1.0f)
				_parameters->radius = 1.0f;
			_panel->UpdateControls();
		}
		else if(key == VK_OEM_6)
		{
			_parameters->radius += 0.5f;
			if(_parameters->radius > 16.0f)
				_parameters->radius = 16.0f;
			_panel->UpdateControls();
		}
		else if(key == VK_OEM_1)
		{
			_parameters->hardness = (int(round(_parameters->hardness * 100.0f)) - 1) / 100.0f;
			if(_parameters->hardness < 0.0f)
				_parameters->hardness = 0.0f;
			_panel->UpdateControls();
		}
		else if(key == VK_OEM_7)
		{
			_parameters->hardness = (int(round(_parameters->hardness * 100.0f)) + 1) / 100.0f;
			if(_parameters->hardness > 1.0f)
				_parameters->hardness = 1.0f;
			_panel->UpdateControls();
		}
		else if(key == VK_OEM_PLUS)
		{
			_parameters->strength += 1.0f;
			if(_parameters->strength > 10.0f)
				_parameters->strength = 10.0f;
			_panel->UpdateControls();
		}
		else if(key == VK_OEM_MINUS)
		{
			_parameters->strength -= 1.0f;
			if(_parameters->strength < 1.0f)
				_parameters->strength = 1.0f;
			_panel->UpdateControls();
		}
	}

	void EM_PaintGrass::Render()
	{
		if(_overTerrain)
			_brush->Draw();
	}

	void EM_PaintGrass::Update(float dt)
	{
		if(_isExecuting && _action != nullptr && _overTerrain)
		{
			_action->Update(dt);
		}
	}

}
