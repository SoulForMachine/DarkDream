#include "StdAfx.h"
#include "ModelForm.h"


namespace EntityEditor
{

	ModelForm::ModelForm(void)
	{
		InitializeComponent();
		//
		//TODO: Add the constructor code here
		//
	}

	void ModelForm::SetEntity(Engine::ModelEntity* entity)
	{
		_renderWindow->SetEntity(entity);
	}

	void ModelForm::RedrawAsync()
	{
		InvalidateRect((HWND)_renderWindow->Handle.ToPointer(), 0, FALSE);
	}

	System::Void ModelForm::ModelForm_HandleCreated(System::Object^ sender, System::EventArgs^ e)
	{
		try
		{
			_renderWindow = gcnew RenderWindow(this);
		}
		catch(...)
		{
			_renderWindow = nullptr;
		}
	}

	System::Void ModelForm::ModelForm_HandleDestroyed(System::Object^ sender, System::EventArgs^ e)
	{
	}

	System::Void ModelForm::ModelForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
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

	System::Void ModelForm::ModelForm_Resize(System::Object^  sender, System::EventArgs^  e)
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

	System::Void ModelForm::ModelForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_renderWindow)
		{
			::SendMessage((HWND)_renderWindow->Handle.ToPointer(), WM_MOUSEWHEEL, MAKEWPARAM(0, e->Delta), MAKELPARAM(e->X, e->Y));
		}
	}

}
