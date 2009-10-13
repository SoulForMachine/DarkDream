#pragma once

#include "MapRenderWindow.h"
#include "EditMode.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MapEditor {

	ref class ToolPanel;
	ref class UndoManager;


	/// <summary>
	/// Summary for MapForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MapForm :	public WeifenLuo::WinFormsUI::Docking::DockContent,
								public EditModeEventListener
	{
	public:
		MapForm(EditorCommon::FormDirector^ director);

		void Redraw()
			{ _renderWindow->Draw(); }
		void RedrawAsync();
		void UpdateFrame()
			{ _renderWindow->UpdateFrame(); }
		void Animate(bool anim)
			{ _renderWindow->Animate(anim); }
		void Wireframe(bool wireframe)
			{ _renderWindow->Wireframe(wireframe); }
		void ShowStats(bool stats)
			{ _renderWindow->ShowStats(stats); }
		void SetViewMode(MapRenderWindow::ViewMode mode)
			{ _renderWindow->SetViewMode(mode); }
		MapRenderWindow::ViewMode GetViewMode()
			{ return _renderWindow->GetViewMode(); }
		void SetCurrentEditMode(EditMode::EditModeEnum mode);
		EditMode^ GetCurrentEditMode()
			{ return _currentEditMode; }
		EditMode^ GetPersistentEditMode()
			{ return _persistEditMode; }
		UndoManager^ GetUndoManager()
			{ return _undoManager; }
		virtual void EditModeEvent(EditModeEventListener::EMEvent ev);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MapForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		EditorCommon::FormDirector^ _director;
		MapRenderWindow^ _renderWindow;
		array<EditMode^>^ _editModes;
		EditMode^ _currentEditMode;
		EditMode^ _persistEditMode;
		UndoManager^ _undoManager;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// MapForm
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(651, 437);
			this->CloseButton = false;
			this->DockAreas = WeifenLuo::WinFormsUI::Docking::DockAreas::Document;
			this->Name = L"MapForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->ShowInTaskbar = false;
			this->TabText = L"Map";
			this->Text = L"Map";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->HandleCreated += gcnew System::EventHandler(this, &MapForm::MapForm_HandleCreated);
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MapForm::MapForm_MouseWheel);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MapForm::MapForm_Paint);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MapForm::MapForm_KeyDown);
			this->HandleDestroyed += gcnew System::EventHandler(this, &MapForm::MapForm_HandleDestroyed);
			this->Resize += gcnew System::EventHandler(this, &MapForm::MapForm_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void MapForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		private: System::Void MapForm_Resize(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MapForm_HandleCreated(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MapForm_HandleDestroyed(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MapForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
		private: System::Void MapForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	};
}
