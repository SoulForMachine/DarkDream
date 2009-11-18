#pragma once

#include "RenderWindow.h"


namespace ParticleEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for ViewportForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ViewportForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		ViewportForm(void);

		void SetParticleSystem(Engine::ParticleSystem* part_sys)
			{ _renderWindow->SetParticleSystem(part_sys); }
		void Redraw()
			{ _renderWindow->Draw(); }
		void RedrawAsync();
		void UpdateFrame()
			{ _renderWindow->UpdateFrame(); }
		void ShowStats(bool stats)
			{ _renderWindow->ShowStats(stats); }
		bool StatsVisible()
			{ return _renderWindow->StatsVisible(); }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ViewportForm()
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
			// ViewportForm
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(292, 273);
			this->CloseButton = false;
			this->DockAreas = WeifenLuo::WinFormsUI::Docking::DockAreas::Document;
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Name = L"ViewportForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->ShowInTaskbar = false;
			this->TabText = L"Particle System";
			this->Text = L"Particle System";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->HandleCreated += gcnew System::EventHandler(this, &ViewportForm::ViewportForm_HandleCreated);
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &ViewportForm::ViewportForm_MouseWheel);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ViewportForm::ViewportForm_Paint);
			this->HandleDestroyed += gcnew System::EventHandler(this, &ViewportForm::ViewportForm_HandleDestroyed);
			this->Resize += gcnew System::EventHandler(this, &ViewportForm::ViewportForm_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void ViewportForm_HandleCreated(System::Object^ sender, System::EventArgs^ e);
		private: System::Void ViewportForm_HandleDestroyed(System::Object^ sender, System::EventArgs^ e);
		private: System::Void ViewportForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
		private: System::Void ViewportForm_Resize(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ViewportForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	};
}
