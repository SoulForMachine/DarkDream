#pragma once


namespace ParticleEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	ref class ViewportForm;
	ref class EmitterPanel;
	ref class PropertiesPanel;

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form, public EditorCommon::FormDirector
	{
	public:
		MainForm();

		virtual void FormNotify(System::Windows::Forms::Form^ form, EditorCommon::NotifyMessage msg);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  _mainMenu;
	private: System::Windows::Forms::StatusStrip^  _statusBar;
	protected: 

	protected: 

	private: System::Windows::Forms::ToolStrip^  _mainToolbar;


	private: WeifenLuo::WinFormsUI::Docking::DockPanel^  _dockPanel;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewConsole;

	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileNew;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileOpen;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileSave;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileSaveAs;




	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileExit;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewStats;

	private: System::Windows::Forms::OpenFileDialog^  _openPartSysDialog;
	private: System::Windows::Forms::SaveFileDialog^  _savePartSysDialog;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		WeifenLuo::WinFormsUI::Docking::IDockContent^ GetContentFromPersistString(String^ persistString);

		WeifenLuo::WinFormsUI::Docking::DeserializeDockContent^ _deserializeDockContent;
		EditorCommon::ConsoleForm^ _consoleForm;
		ViewportForm^ _viewportForm;
		EmitterPanel^ _emitterPanel;
		PropertiesPanel^ _propertyPanel;
		bool _viewStats;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			WeifenLuo::WinFormsUI::Docking::DockPanelSkin^  dockPanelSkin9 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelSkin());
			WeifenLuo::WinFormsUI::Docking::AutoHideStripSkin^  autoHideStripSkin9 = (gcnew WeifenLuo::WinFormsUI::Docking::AutoHideStripSkin());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient25 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient57 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripSkin^  dockPaneStripSkin9 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripSkin());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripGradient^  dockPaneStripGradient9 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient58 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient26 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient59 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripToolWindowGradient^  dockPaneStripToolWindowGradient9 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripToolWindowGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient60 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient61 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient27 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient62 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient63 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			this->_mainMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileSaveAs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_menuFileExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuViewConsole = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuViewStats = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_statusBar = (gcnew System::Windows::Forms::StatusStrip());
			this->_mainToolbar = (gcnew System::Windows::Forms::ToolStrip());
			this->_dockPanel = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanel());
			this->_openPartSysDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_savePartSysDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->_mainMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// _mainMenu
			// 
			this->_mainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->viewToolStripMenuItem});
			this->_mainMenu->Location = System::Drawing::Point(0, 0);
			this->_mainMenu->Name = L"_mainMenu";
			this->_mainMenu->Size = System::Drawing::Size(875, 24);
			this->_mainMenu->TabIndex = 0;
			this->_mainMenu->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->_menuFileNew, 
				this->_menuFileOpen, this->_menuFileSave, this->_menuFileSaveAs, this->toolStripMenuItem1, this->_menuFileExit});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// _menuFileNew
			// 
			this->_menuFileNew->Name = L"_menuFileNew";
			this->_menuFileNew->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
			this->_menuFileNew->Size = System::Drawing::Size(152, 22);
			this->_menuFileNew->Text = L"&New";
			this->_menuFileNew->Click += gcnew System::EventHandler(this, &MainForm::_menuFileNew_Click);
			// 
			// _menuFileOpen
			// 
			this->_menuFileOpen->Name = L"_menuFileOpen";
			this->_menuFileOpen->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->_menuFileOpen->Size = System::Drawing::Size(152, 22);
			this->_menuFileOpen->Text = L"&Open";
			this->_menuFileOpen->Click += gcnew System::EventHandler(this, &MainForm::_menuFileOpen_Click);
			// 
			// _menuFileSave
			// 
			this->_menuFileSave->Name = L"_menuFileSave";
			this->_menuFileSave->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->_menuFileSave->Size = System::Drawing::Size(152, 22);
			this->_menuFileSave->Text = L"&Save";
			this->_menuFileSave->Click += gcnew System::EventHandler(this, &MainForm::_menuFileSave_Click);
			// 
			// _menuFileSaveAs
			// 
			this->_menuFileSaveAs->Name = L"_menuFileSaveAs";
			this->_menuFileSaveAs->Size = System::Drawing::Size(152, 22);
			this->_menuFileSaveAs->Text = L"Save &As...";
			this->_menuFileSaveAs->Click += gcnew System::EventHandler(this, &MainForm::_menuFileSaveAs_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(149, 6);
			// 
			// _menuFileExit
			// 
			this->_menuFileExit->Name = L"_menuFileExit";
			this->_menuFileExit->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::F4));
			this->_menuFileExit->Size = System::Drawing::Size(152, 22);
			this->_menuFileExit->Text = L"E&xit";
			this->_menuFileExit->Click += gcnew System::EventHandler(this, &MainForm::_menuFileExit_Click);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_menuViewConsole, 
				this->_menuViewStats});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->viewToolStripMenuItem->Text = L"&View";
			// 
			// _menuViewConsole
			// 
			this->_menuViewConsole->Name = L"_menuViewConsole";
			this->_menuViewConsole->Size = System::Drawing::Size(152, 22);
			this->_menuViewConsole->Text = L"&Console";
			this->_menuViewConsole->Click += gcnew System::EventHandler(this, &MainForm::_menuViewConsole_Click);
			// 
			// _menuViewStats
			// 
			this->_menuViewStats->Name = L"_menuViewStats";
			this->_menuViewStats->Size = System::Drawing::Size(152, 22);
			this->_menuViewStats->Text = L"&Statistics";
			this->_menuViewStats->Click += gcnew System::EventHandler(this, &MainForm::_menuViewStats_Click);
			// 
			// _statusBar
			// 
			this->_statusBar->Location = System::Drawing::Point(0, 562);
			this->_statusBar->Name = L"_statusBar";
			this->_statusBar->Size = System::Drawing::Size(875, 22);
			this->_statusBar->TabIndex = 1;
			this->_statusBar->Text = L"statusStrip1";
			// 
			// _mainToolbar
			// 
			this->_mainToolbar->Location = System::Drawing::Point(0, 24);
			this->_mainToolbar->Name = L"_mainToolbar";
			this->_mainToolbar->Size = System::Drawing::Size(875, 25);
			this->_mainToolbar->TabIndex = 2;
			this->_mainToolbar->Text = L"toolStrip1";
			// 
			// _dockPanel
			// 
			this->_dockPanel->ActiveAutoHideContent = nullptr;
			this->_dockPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->_dockPanel->DockBackColor = System::Drawing::SystemColors::Control;
			this->_dockPanel->DocumentStyle = WeifenLuo::WinFormsUI::Docking::DocumentStyle::DockingSdi;
			this->_dockPanel->Location = System::Drawing::Point(0, 49);
			this->_dockPanel->Name = L"_dockPanel";
			this->_dockPanel->Size = System::Drawing::Size(875, 513);
			dockPanelGradient25->EndColor = System::Drawing::SystemColors::ControlLight;
			dockPanelGradient25->StartColor = System::Drawing::SystemColors::ControlLight;
			autoHideStripSkin9->DockStripGradient = dockPanelGradient25;
			tabGradient57->EndColor = System::Drawing::SystemColors::Control;
			tabGradient57->StartColor = System::Drawing::SystemColors::Control;
			tabGradient57->TextColor = System::Drawing::SystemColors::ControlDarkDark;
			autoHideStripSkin9->TabGradient = tabGradient57;
			dockPanelSkin9->AutoHideStripSkin = autoHideStripSkin9;
			tabGradient58->EndColor = System::Drawing::SystemColors::ControlLightLight;
			tabGradient58->StartColor = System::Drawing::SystemColors::ControlLightLight;
			tabGradient58->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripGradient9->ActiveTabGradient = tabGradient58;
			dockPanelGradient26->EndColor = System::Drawing::SystemColors::Control;
			dockPanelGradient26->StartColor = System::Drawing::SystemColors::Control;
			dockPaneStripGradient9->DockStripGradient = dockPanelGradient26;
			tabGradient59->EndColor = System::Drawing::SystemColors::ControlLight;
			tabGradient59->StartColor = System::Drawing::SystemColors::ControlLight;
			tabGradient59->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripGradient9->InactiveTabGradient = tabGradient59;
			dockPaneStripSkin9->DocumentGradient = dockPaneStripGradient9;
			tabGradient60->EndColor = System::Drawing::SystemColors::ActiveCaption;
			tabGradient60->LinearGradientMode = System::Drawing::Drawing2D::LinearGradientMode::Vertical;
			tabGradient60->StartColor = System::Drawing::SystemColors::GradientActiveCaption;
			tabGradient60->TextColor = System::Drawing::SystemColors::ActiveCaptionText;
			dockPaneStripToolWindowGradient9->ActiveCaptionGradient = tabGradient60;
			tabGradient61->EndColor = System::Drawing::SystemColors::Control;
			tabGradient61->StartColor = System::Drawing::SystemColors::Control;
			tabGradient61->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripToolWindowGradient9->ActiveTabGradient = tabGradient61;
			dockPanelGradient27->EndColor = System::Drawing::SystemColors::ControlLight;
			dockPanelGradient27->StartColor = System::Drawing::SystemColors::ControlLight;
			dockPaneStripToolWindowGradient9->DockStripGradient = dockPanelGradient27;
			tabGradient62->EndColor = System::Drawing::SystemColors::GradientInactiveCaption;
			tabGradient62->LinearGradientMode = System::Drawing::Drawing2D::LinearGradientMode::Vertical;
			tabGradient62->StartColor = System::Drawing::SystemColors::GradientInactiveCaption;
			tabGradient62->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripToolWindowGradient9->InactiveCaptionGradient = tabGradient62;
			tabGradient63->EndColor = System::Drawing::Color::Transparent;
			tabGradient63->StartColor = System::Drawing::Color::Transparent;
			tabGradient63->TextColor = System::Drawing::SystemColors::ControlDarkDark;
			dockPaneStripToolWindowGradient9->InactiveTabGradient = tabGradient63;
			dockPaneStripSkin9->ToolWindowGradient = dockPaneStripToolWindowGradient9;
			dockPanelSkin9->DockPaneStripSkin = dockPaneStripSkin9;
			this->_dockPanel->Skin = dockPanelSkin9;
			this->_dockPanel->TabIndex = 3;
			// 
			// _openPartSysDialog
			// 
			this->_openPartSysDialog->Filter = L"Particle System Files (*.part)|*.part|All Files (*.*)|*.*";
			this->_openPartSysDialog->Title = L"Open Particle System";
			// 
			// _savePartSysDialog
			// 
			this->_savePartSysDialog->DefaultExt = L"part";
			this->_savePartSysDialog->Filter = L"Particle System Files (*.part)|*.part|All Files (*.*)|*.*";
			this->_savePartSysDialog->Title = L"Save Particle System";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(875, 584);
			this->Controls->Add(this->_dockPanel);
			this->Controls->Add(this->_mainToolbar);
			this->Controls->Add(this->_statusBar);
			this->Controls->Add(this->_mainMenu);
			this->MainMenuStrip = this->_mainMenu;
			this->Name = L"MainForm";
			this->Text = L"Dark Dream Particle System Editor";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->_mainMenu->ResumeLayout(false);
			this->_mainMenu->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
	private: System::Void OnIdle(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileNew_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileOpen_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileSave_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileSaveAs_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileExit_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewConsole_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewStats_Click(System::Object^  sender, System::EventArgs^  e);
};
}

