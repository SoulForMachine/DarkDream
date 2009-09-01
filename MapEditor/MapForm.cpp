#include "StdAfx.h"
#include "EM_View.h"
#include "EM_TerrainEdit.h"
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
			_editModes[(int)EditMode::EditModeEnum::VIEW] = gcnew EM_View;
			_editModes[(int)EditMode::EditModeEnum::TERRAIN_EDIT] = gcnew EM_TerrainEdit(_undoManager);

			_currentEditMode = _editModes[(int)EditMode::EditModeEnum::VIEW];
			_renderWindow->SetEditMode(_currentEditMode);
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

	void MapForm::SetCurrentEditMode(EditMode::EditModeEnum mode)
	{
		if(_currentEditMode != nullptr && _currentEditMode->GetModeEnum() == mode)
			return;

		_currentEditMode = _editModes[(int)mode];

		if(_renderWindow != nullptr)
			_renderWindow->SetEditMode(_currentEditMode);
	}

}
