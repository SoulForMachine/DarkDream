#include "StdAfx.h"
#include "EM_View.h"
#include "EM_TerrainEdit.h"
#include "EM_TerrainPaint.h"
#include "ToolPanel.h"
#include "UndoManager.h"
#include "MapForm.h"


namespace MapEditor
{

	MapForm::MapForm(EditorCommon::FormDirector^ director, ToolPanel^ tool_panel)
	{
		InitializeComponent();
		_director = director;
		_toolPanel = tool_panel;
		_undoManager = gcnew UndoManager;
		SetCurrentEditMode(EditMode::EditModeEnum::VIEW);
	}

	void MapForm::RedrawAsync()
	{
		InvalidateRect((HWND)_renderWindow->Handle.ToPointer(), 0, FALSE);
	}

	System::Void MapForm::MapForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		if(!_renderWindow)
		{
			Drawing::Font^ font = gcnew Drawing::Font("Arial", 14);
			StringFormat^ fmt = gcnew StringFormat;
			fmt->Alignment = StringAlignment::Center;
			fmt->LineAlignment = StringAlignment::Center;

			e->Graphics->DrawString("Failed to initialize renderer!", font, Brushes::Red, ClientRectangle, fmt);
		}
	}

	System::Void MapForm::MapForm_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		if(_renderWindow)
		{
			HWND hwnd = (HWND)_renderWindow->Handle.ToPointer();
			MoveWindow(hwnd, 0, _mainToolbar->Height, ClientRectangle.Width, ClientRectangle.Height, TRUE);
		}
		else
		{
			Refresh();
		}
	}

	System::Void MapForm::MapForm_HandleCreated(System::Object^  sender, System::EventArgs^  e)
	{
		try
		{
			_renderWindow = gcnew MapRenderWindow(this);
			_renderWindow->SetEditMode(_currentEditMode);
		}
		catch(...)
		{
			_renderWindow = nullptr;
		}
	}

	System::Void MapForm::MapForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_renderWindow)
		{
			::SendMessage((HWND)_renderWindow->Handle.ToPointer(), WM_MOUSEWHEEL, MAKEWPARAM(0, e->Delta), MAKELPARAM(e->X, e->Y));
		}
	}

	void MapForm::SetCurrentEditMode(EditMode::EditModeEnum mode)
	{
		if(_currentEditMode != nullptr && _currentEditMode->GetModeEnum() == mode)
			return;

		// reset toolbar button states
		_toolBtnViewMode->Checked = false;
		_toolBtnTerrainEdit->Checked = false;
		_toolBtnTerrainPaint->Checked = false;
		_toolBtnObjectPlacement->Checked = false;
		_toolBtnWaterPatch->Checked = false;
		_toolBtnRoads->Checked = false;
		_toolBtnRivers->Checked = false;
		_toolBtnFogLayers->Checked = false;
		_toolBtnTriggers->Checked = false;
		_toolBtnLights->Checked = false;
		_toolBtnParticleSystems->Checked = false;

		delete _currentEditMode;
		switch(mode)
		{
		case EditMode::EditModeEnum::VIEW:
			_currentEditMode = gcnew EM_View;
			_toolBtnViewMode->Checked = true;
			break;
		case EditMode::EditModeEnum::TERRAIN_EDIT:
			_currentEditMode = gcnew EM_TerrainEdit(_undoManager);
			_toolBtnTerrainEdit->Checked = true;
			break;
		case EditMode::EditModeEnum::TERRAIN_PAINT:
			_currentEditMode = gcnew EM_TerrainPaint(_undoManager);
			_toolBtnTerrainPaint->Checked = true;
			break;
		default:
			assert(0);
		}

		_toolPanel->SetPanel(_currentEditMode->GetPanel());
		if(_renderWindow != nullptr)
			_renderWindow->SetEditMode(_currentEditMode);
	}

	System::Void MapForm::_toolBtnViewMode_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCurrentEditMode(EditMode::EditModeEnum::VIEW);
	}

	System::Void MapForm::_toolBtnTerrainEdit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCurrentEditMode(EditMode::EditModeEnum::TERRAIN_EDIT);
	}

	System::Void MapForm::_toolBtnTerrainPaint_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SetCurrentEditMode(EditMode::EditModeEnum::TERRAIN_PAINT);
	}

	System::Void MapForm::_toolBtnEditGrid_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MapForm::_toolBtnObjectPlacement_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MapForm::_toolBtnWaterPatch_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MapForm::_toolBtnRoads_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MapForm::_toolBtnRivers_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MapForm::_toolBtnFogLayers_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MapForm::_toolBtnTriggers_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

}
