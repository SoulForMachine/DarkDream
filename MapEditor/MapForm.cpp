#include "StdAfx.h"
#include "EM_View.h"
#include "EM_TerrainEdit.h"
#include "EM_AddPatch.h"
#include "EM_RemovePatch.h"
#include "EM_PlaceObject.h"
#include "EM_LayerEdit.h"
#include "EM_PaintGrass.h"
#include "ToolPanel.h"
#include "UndoManager.h"
#include "MapForm.h"


namespace MapEditor
{

	MapForm::MapForm(EditorCommon::FormDirector^ director)
	{
		InitializeComponent();
		_director = director;
		_undoManager = gcnew UndoManager;
		_currentEditMode = nullptr;
		_persistEditMode = nullptr;
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
			MoveWindow(hwnd, 0, 0, ClientRectangle.Width, ClientRectangle.Height, TRUE);
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

			_editModes = gcnew array<EditMode^>((int)EditMode::EditModeEnum::EDIT_MODE_COUNT);
			_editModes[(int)EditMode::EditModeEnum::VIEW] = gcnew EM_View(this, true);
			_editModes[(int)EditMode::EditModeEnum::TERRAIN_EDIT] = gcnew EM_TerrainEdit(this, true, _undoManager);
			_editModes[(int)EditMode::EditModeEnum::ADD_PATCH] = gcnew EM_AddPatch(this, false, _undoManager);
			_editModes[(int)EditMode::EditModeEnum::REMOVE_PATCH] = gcnew EM_RemovePatch(this, false, _undoManager);
			_editModes[(int)EditMode::EditModeEnum::PLACE_OBJECT] = gcnew EM_PlaceObject(this, true, _undoManager);
			_editModes[(int)EditMode::EditModeEnum::LAYER_EDIT] = gcnew EM_LayerEdit(this, true, _undoManager);
			_editModes[(int)EditMode::EditModeEnum::PAINT_GRASS] = gcnew EM_PaintGrass(this, true, _undoManager);

			SetCurrentEditMode(EditMode::EditModeEnum::VIEW);
			_renderWindow->SetViewParameters(((EM_View^)_editModes[(int)EditMode::EditModeEnum::VIEW])->GetParameters());
		}
		catch(...)
		{
			_renderWindow = nullptr;
		}
	}

	System::Void MapForm::MapForm_HandleDestroyed(System::Object^  sender, System::EventArgs^  e)
	{
		delete _undoManager;
		for each(EditMode^ em in _editModes)
			delete em;
		delete _editModes;
		_currentEditMode = nullptr;
		_persistEditMode = nullptr;
		_renderWindow->DestroyHandle();
		delete _renderWindow;
	}

	System::Void MapForm::MapForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_renderWindow)
		{
			::SendMessage((HWND)_renderWindow->Handle.ToPointer(), WM_MOUSEWHEEL, MAKEWPARAM(0, e->Delta), MAKELPARAM(e->X, e->Y));
		}
	}

	System::Void MapForm::MapForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		::SendMessage((HWND)_renderWindow->Handle.ToPointer(), WM_KEYDOWN, e->KeyValue, 0);
	}

	void MapForm::SetCurrentEditMode(EditMode::EditModeEnum mode)
	{
		if(_currentEditMode != nullptr && _currentEditMode->GetModeEnum() == mode)
			return;

		_currentEditMode = _editModes[(int)mode];
		_currentEditMode->Activate();
		if(_currentEditMode->IsPersistent())
			_persistEditMode = _currentEditMode;

		if(_renderWindow != nullptr)
			_renderWindow->SetEditMode(_currentEditMode);
	}

	void MapForm::EditModeEvent(EditModeEventListener::EMEvent ev)
	{
		if(	ev == EditModeEventListener::EMEvent::EDIT_COMPLETE ||
			ev == EditModeEventListener::EMEvent::EDIT_CANCELED ||
			ev == EditModeEventListener::EMEvent::EDIT_ERROR )
		{
			if(_persistEditMode != nullptr)
				SetCurrentEditMode(_persistEditMode->GetModeEnum());
		}
	}

}
