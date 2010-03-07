#pragma once

#include "RenderWindow.h"

namespace Engine
{
	class ModelEntity;
}


namespace EntityEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for ModelForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ModelForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		ModelForm(void);

		void SetEntity(Engine::ModelEntity* entity);
		void Redraw()
			{ if(_renderWindow) _renderWindow->Draw(); }
		void RedrawAsync();
		void UpdateFrame()
			{ if(_renderWindow) _renderWindow->UpdateFrame(); }
		void Wireframe(bool wireframe)
			{ if(_renderWindow) _renderWindow->Wireframe(wireframe); }
		void ShowStats(bool stats)
			{ if(_renderWindow) _renderWindow->ShowStats(stats); }
		void ModelChanged()
			{ if(_renderWindow) _renderWindow->ModelChanged(); }
		bool IsRenderingInitialized()
			{ return _renderWindow != nullptr; }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ModelForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		RenderWindow^ _renderWindow;

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
			// ModelForm
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(292, 273);
			this->CloseButton = false;
			this->DockAreas = WeifenLuo::WinFormsUI::Docking::DockAreas::Document;
			this->Name = L"ModelForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->ShowInTaskbar = false;
			this->TabText = L"Model";
			this->Text = L"Model";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->HandleCreated += gcnew System::EventHandler(this, &ModelForm::ModelForm_HandleCreated);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ModelForm::ModelForm_Paint);
			this->HandleDestroyed += gcnew System::EventHandler(this, &ModelForm::ModelForm_HandleDestroyed);
			this->Resize += gcnew System::EventHandler(this, &ModelForm::ModelForm_Resize);
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &ModelForm::ModelForm_MouseWheel);
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void ModelForm_HandleCreated(System::Object^ sender, System::EventArgs^ e);
		private: System::Void ModelForm_HandleDestroyed(System::Object^ sender, System::EventArgs^ e);
		private: System::Void ModelForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		private: System::Void ModelForm_Resize(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ModelForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	};
}
