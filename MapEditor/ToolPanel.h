#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace EditorCommon
{
	interface class FormDirector;
}


namespace MapEditor {

	/// <summary>
	/// Summary for ToolPanel
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ToolPanel : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		ToolPanel(EditorCommon::FormDirector^ director);

		void SetPanel(System::Windows::Forms::UserControl^ panel);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ToolPanel()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		EditorCommon::FormDirector^ _director;
		System::Windows::Forms::UserControl^ _panel;

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
			// ToolPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(350, 302);
			this->DockAreas = static_cast<WeifenLuo::WinFormsUI::Docking::DockAreas>(((WeifenLuo::WinFormsUI::Docking::DockAreas::Float | WeifenLuo::WinFormsUI::Docking::DockAreas::DockLeft) 
				| WeifenLuo::WinFormsUI::Docking::DockAreas::DockRight));
			this->HideOnClose = true;
			this->Name = L"ToolPanel";
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::DockRight;
			this->ShowInTaskbar = false;
			this->TabText = L"Tools";
			this->Text = L"Tools";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
