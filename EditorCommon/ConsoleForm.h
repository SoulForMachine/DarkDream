#pragma once


namespace EditorCommon {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	ref class ConsoleWindow;
	interface class FormDirector;

	/// <summary>
	/// Summary for ConsoleForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ConsoleForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		ConsoleForm(FormDirector^ director);

		void RedrawAsync();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ConsoleForm()
		{
			if (components)
			{
				delete components;
			}
		}


	protected: 
		virtual bool IsInputKey(System::Windows::Forms::Keys key_data) override
			{ return true; }

	protected: 
	private: System::ComponentModel::IContainer^  components;

	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		ConsoleWindow^ _consoleWnd;
		FormDirector^ _director;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// ConsoleForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(553, 350);
			this->DoubleBuffered = true;
			this->HideOnClose = true;
			this->Name = L"ConsoleForm";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockBottomAutoHide;
			this->ShowInTaskbar = false;
			this->TabText = L"Console";
			this->Text = L"Console";
			this->HandleCreated += gcnew System::EventHandler(this, &ConsoleForm::ConsoleForm_HandleCreated);
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &ConsoleForm::ConsoleForm_MouseWheel);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ConsoleForm::ConsoleForm_Paint);
			this->Enter += gcnew System::EventHandler(this, &ConsoleForm::ConsoleForm_Enter);
			this->Leave += gcnew System::EventHandler(this, &ConsoleForm::ConsoleForm_Leave);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ConsoleForm::ConsoleForm_KeyPress);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ConsoleForm::ConsoleForm_KeyUp);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ConsoleForm::ConsoleForm_KeyDown);
			this->Resize += gcnew System::EventHandler(this, &ConsoleForm::ConsoleForm_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void ConsoleForm_Resize(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ConsoleForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void ConsoleForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
	private: System::Void ConsoleForm_HandleCreated(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ConsoleForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void ConsoleForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
	private: System::Void ConsoleForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void ConsoleForm_Enter(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ConsoleForm_Leave(System::Object^  sender, System::EventArgs^  e);
	};
}
