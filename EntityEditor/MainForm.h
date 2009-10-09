#pragma once


namespace EntityEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	ref class ModelForm;
	ref class EntityForm;
	ref class MaterialForm;
	ref class PropertyForm;
	ref class Entity;

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

		virtual void FormNotify(Form^ form, EditorCommon::NotifyMessage msg);

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

	private:
		WeifenLuo::WinFormsUI::Docking::IDockContent^ GetContentFromPersistString(String^ persistString);
		void ToggleForm(WeifenLuo::WinFormsUI::Docking::DockContent^ form);
		void NewEntity();
		bool LoadEntity();
		bool SaveEntity();
		bool SaveEntityAs();
		bool SaveEntity(String^ file_name);
		bool PromptSave();
		void UpdateTitleBar();
		void OnIdle(System::Object^  sender, System::EventArgs^  e);

		WeifenLuo::WinFormsUI::Docking::DeserializeDockContent^ _deserializeDockContent;
		ModelForm^ _modelForm;
		EditorCommon::ConsoleForm^ _consoleForm;
		EntityForm^ _entityForm;
		MaterialForm^ _materialForm;
		PropertyForm^ _propertyForm;
		Entity^ _entity;
		bool _animate;
		bool _wireframe;
		bool _modelStats;
		bool _skelet;

	private: WeifenLuo::WinFormsUI::Docking::DockPanel^  _dockPanel;
	private: System::Windows::Forms::MenuStrip^  _mainMenu;
	protected: 

	protected: 

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuEntityNew;


	private: System::Windows::Forms::StatusStrip^  _statusBar;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuEntitySave;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuEntitySaveAs;




	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuEntityExit;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuView;


	private: System::Windows::Forms::ToolStripMenuItem^  _mnuViewConsole;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuModelStats;
	private: System::Windows::Forms::ToolStrip^  _mainToolbar;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnNew;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnOpen;



	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuEntityOpen;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuViewEntityPanel;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuViewMaterialPanel;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuViewPropertyPanel;
	private: System::Windows::Forms::SaveFileDialog^  _saveEntityDlg;
	private: System::Windows::Forms::OpenFileDialog^  _openEntityDlg;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnSave;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnConsole;
	private: System::Windows::Forms::ToolStripMenuItem^  renderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuAnimate;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuWireframe;
	private: System::Windows::Forms::ToolStripMenuItem^  _mnuViewSkelet;













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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->_dockPanel = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanel());
			this->_mainMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuEntityNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuEntityOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuEntitySave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuEntitySaveAs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_mnuEntityExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuView = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuViewConsole = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuModelStats = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuViewEntityPanel = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuViewMaterialPanel = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuViewPropertyPanel = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuViewSkelet = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->renderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuAnimate = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_mnuWireframe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_statusBar = (gcnew System::Windows::Forms::StatusStrip());
			this->_mainToolbar = (gcnew System::Windows::Forms::ToolStrip());
			this->_toolBtnNew = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnOpen = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_toolBtnConsole = (gcnew System::Windows::Forms::ToolStripButton());
			this->_saveEntityDlg = (gcnew System::Windows::Forms::SaveFileDialog());
			this->_openEntityDlg = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_mainMenu->SuspendLayout();
			this->_mainToolbar->SuspendLayout();
			this->SuspendLayout();
			// 
			// _dockPanel
			// 
			this->_dockPanel->ActiveAutoHideContent = nullptr;
			this->_dockPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->_dockPanel->DocumentStyle = WeifenLuo::WinFormsUI::Docking::DocumentStyle::DockingWindow;
			this->_dockPanel->Location = System::Drawing::Point(0, 49);
			this->_dockPanel->Name = L"_dockPanel";
			this->_dockPanel->Size = System::Drawing::Size(830, 556);
			this->_dockPanel->TabIndex = 0;
			// 
			// _mainMenu
			// 
			this->_mainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->_mnuView, this->renderToolStripMenuItem});
			this->_mainMenu->Location = System::Drawing::Point(0, 0);
			this->_mainMenu->Name = L"_mainMenu";
			this->_mainMenu->Size = System::Drawing::Size(830, 24);
			this->_mainMenu->TabIndex = 2;
			this->_mainMenu->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->_mnuEntityNew, 
				this->_mnuEntityOpen, this->_mnuEntitySave, this->_mnuEntitySaveAs, this->toolStripMenuItem1, this->_mnuEntityExit});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(47, 20);
			this->fileToolStripMenuItem->Text = L"&Entity";
			// 
			// _mnuEntityNew
			// 
			this->_mnuEntityNew->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuEntityNew.Image")));
			this->_mnuEntityNew->Name = L"_mnuEntityNew";
			this->_mnuEntityNew->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
			this->_mnuEntityNew->Size = System::Drawing::Size(151, 22);
			this->_mnuEntityNew->Text = L"&New";
			this->_mnuEntityNew->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntityNew_Click);
			// 
			// _mnuEntityOpen
			// 
			this->_mnuEntityOpen->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuEntityOpen.Image")));
			this->_mnuEntityOpen->Name = L"_mnuEntityOpen";
			this->_mnuEntityOpen->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->_mnuEntityOpen->Size = System::Drawing::Size(151, 22);
			this->_mnuEntityOpen->Text = L"&Open";
			this->_mnuEntityOpen->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntityOpen_Click);
			// 
			// _mnuEntitySave
			// 
			this->_mnuEntitySave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuEntitySave.Image")));
			this->_mnuEntitySave->Name = L"_mnuEntitySave";
			this->_mnuEntitySave->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->_mnuEntitySave->Size = System::Drawing::Size(151, 22);
			this->_mnuEntitySave->Text = L"&Save";
			this->_mnuEntitySave->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntitySave_Click);
			// 
			// _mnuEntitySaveAs
			// 
			this->_mnuEntitySaveAs->Name = L"_mnuEntitySaveAs";
			this->_mnuEntitySaveAs->Size = System::Drawing::Size(151, 22);
			this->_mnuEntitySaveAs->Text = L"Save &As";
			this->_mnuEntitySaveAs->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntitySaveAs_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(148, 6);
			// 
			// _mnuEntityExit
			// 
			this->_mnuEntityExit->Name = L"_mnuEntityExit";
			this->_mnuEntityExit->Size = System::Drawing::Size(151, 22);
			this->_mnuEntityExit->Text = L"E&xit";
			this->_mnuEntityExit->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntityExit_Click);
			// 
			// _mnuView
			// 
			this->_mnuView->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->_mnuViewConsole, 
				this->_mnuModelStats, this->_mnuViewEntityPanel, this->_mnuViewMaterialPanel, this->_mnuViewPropertyPanel, this->_mnuViewSkelet});
			this->_mnuView->Name = L"_mnuView";
			this->_mnuView->Size = System::Drawing::Size(41, 20);
			this->_mnuView->Text = L"&View";
			this->_mnuView->DropDownOpening += gcnew System::EventHandler(this, &MainForm::_mnuView_DropDownOpening);
			// 
			// _mnuViewConsole
			// 
			this->_mnuViewConsole->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_mnuViewConsole.Image")));
			this->_mnuViewConsole->ImageTransparentColor = System::Drawing::Color::White;
			this->_mnuViewConsole->Name = L"_mnuViewConsole";
			this->_mnuViewConsole->ShortcutKeys = System::Windows::Forms::Keys::F2;
			this->_mnuViewConsole->Size = System::Drawing::Size(178, 22);
			this->_mnuViewConsole->Text = L"&Console";
			this->_mnuViewConsole->Click += gcnew System::EventHandler(this, &MainForm::_mnuViewConsole_Click);
			// 
			// _mnuModelStats
			// 
			this->_mnuModelStats->Name = L"_mnuModelStats";
			this->_mnuModelStats->ShortcutKeys = System::Windows::Forms::Keys::F3;
			this->_mnuModelStats->Size = System::Drawing::Size(178, 22);
			this->_mnuModelStats->Text = L"Model &Statistics";
			this->_mnuModelStats->Click += gcnew System::EventHandler(this, &MainForm::_mnuModelStats_Click);
			// 
			// _mnuViewEntityPanel
			// 
			this->_mnuViewEntityPanel->Name = L"_mnuViewEntityPanel";
			this->_mnuViewEntityPanel->ShortcutKeys = System::Windows::Forms::Keys::F4;
			this->_mnuViewEntityPanel->Size = System::Drawing::Size(178, 22);
			this->_mnuViewEntityPanel->Text = L"&Entity Panel";
			this->_mnuViewEntityPanel->Click += gcnew System::EventHandler(this, &MainForm::_mnuViewEntityPanel_Click);
			// 
			// _mnuViewMaterialPanel
			// 
			this->_mnuViewMaterialPanel->Name = L"_mnuViewMaterialPanel";
			this->_mnuViewMaterialPanel->ShortcutKeys = System::Windows::Forms::Keys::F5;
			this->_mnuViewMaterialPanel->Size = System::Drawing::Size(178, 22);
			this->_mnuViewMaterialPanel->Text = L"&Material Panel";
			this->_mnuViewMaterialPanel->Click += gcnew System::EventHandler(this, &MainForm::_mnuViewMaterialPanel_Click);
			// 
			// _mnuViewPropertyPanel
			// 
			this->_mnuViewPropertyPanel->Name = L"_mnuViewPropertyPanel";
			this->_mnuViewPropertyPanel->ShortcutKeys = System::Windows::Forms::Keys::F6;
			this->_mnuViewPropertyPanel->Size = System::Drawing::Size(178, 22);
			this->_mnuViewPropertyPanel->Text = L"&Property Panel";
			this->_mnuViewPropertyPanel->Click += gcnew System::EventHandler(this, &MainForm::_mnuViewPropertyPanel_Click);
			// 
			// _mnuViewSkelet
			// 
			this->_mnuViewSkelet->Name = L"_mnuViewSkelet";
			this->_mnuViewSkelet->Size = System::Drawing::Size(178, 22);
			this->_mnuViewSkelet->Text = L"Skelet";
			this->_mnuViewSkelet->Click += gcnew System::EventHandler(this, &MainForm::_mnuViewSkelet_Click);
			// 
			// renderToolStripMenuItem
			// 
			this->renderToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_mnuAnimate, 
				this->_mnuWireframe});
			this->renderToolStripMenuItem->Name = L"renderToolStripMenuItem";
			this->renderToolStripMenuItem->Size = System::Drawing::Size(54, 20);
			this->renderToolStripMenuItem->Text = L"&Render";
			// 
			// _mnuAnimate
			// 
			this->_mnuAnimate->Name = L"_mnuAnimate";
			this->_mnuAnimate->Size = System::Drawing::Size(177, 22);
			this->_mnuAnimate->Text = L"&Animate";
			this->_mnuAnimate->Click += gcnew System::EventHandler(this, &MainForm::_mnuAnimate_Click);
			// 
			// _mnuWireframe
			// 
			this->_mnuWireframe->Name = L"_mnuWireframe";
			this->_mnuWireframe->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::W));
			this->_mnuWireframe->Size = System::Drawing::Size(177, 22);
			this->_mnuWireframe->Text = L"&Wireframe";
			this->_mnuWireframe->Click += gcnew System::EventHandler(this, &MainForm::_mnuWireframe_Click);
			// 
			// _statusBar
			// 
			this->_statusBar->Location = System::Drawing::Point(0, 605);
			this->_statusBar->Name = L"_statusBar";
			this->_statusBar->Size = System::Drawing::Size(830, 22);
			this->_statusBar->TabIndex = 3;
			this->_statusBar->Text = L"statusStrip1";
			// 
			// _mainToolbar
			// 
			this->_mainToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->_toolBtnNew, this->_toolBtnOpen, 
				this->_toolBtnSave, this->toolStripSeparator1, this->_toolBtnConsole});
			this->_mainToolbar->Location = System::Drawing::Point(0, 24);
			this->_mainToolbar->Name = L"_mainToolbar";
			this->_mainToolbar->Size = System::Drawing::Size(830, 25);
			this->_mainToolbar->TabIndex = 4;
			// 
			// _toolBtnNew
			// 
			this->_toolBtnNew->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnNew->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnNew.Image")));
			this->_toolBtnNew->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnNew->Name = L"_toolBtnNew";
			this->_toolBtnNew->Size = System::Drawing::Size(23, 22);
			this->_toolBtnNew->Text = L"New Entity";
			this->_toolBtnNew->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntityNew_Click);
			// 
			// _toolBtnOpen
			// 
			this->_toolBtnOpen->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnOpen->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnOpen.Image")));
			this->_toolBtnOpen->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnOpen->Name = L"_toolBtnOpen";
			this->_toolBtnOpen->Size = System::Drawing::Size(23, 22);
			this->_toolBtnOpen->Text = L"Open Entity";
			this->_toolBtnOpen->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntityOpen_Click);
			// 
			// _toolBtnSave
			// 
			this->_toolBtnSave->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnSave.Image")));
			this->_toolBtnSave->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnSave->Name = L"_toolBtnSave";
			this->_toolBtnSave->Size = System::Drawing::Size(23, 22);
			this->_toolBtnSave->Text = L"Save Entity";
			this->_toolBtnSave->Click += gcnew System::EventHandler(this, &MainForm::_mnuEntitySave_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
			// 
			// _toolBtnConsole
			// 
			this->_toolBtnConsole->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnConsole->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnConsole.Image")));
			this->_toolBtnConsole->ImageTransparentColor = System::Drawing::Color::White;
			this->_toolBtnConsole->Name = L"_toolBtnConsole";
			this->_toolBtnConsole->Size = System::Drawing::Size(23, 22);
			this->_toolBtnConsole->Text = L"Console";
			this->_toolBtnConsole->Click += gcnew System::EventHandler(this, &MainForm::_mnuViewConsole_Click);
			// 
			// _saveEntityDlg
			// 
			this->_saveEntityDlg->Filter = L"Entity Files (*.entity)|*.entity|All Files (*.*)|*.*";
			this->_saveEntityDlg->Title = L"Save Entity As";
			this->_saveEntityDlg->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MainForm::fileDialog_FileOk);
			// 
			// _openEntityDlg
			// 
			this->_openEntityDlg->DefaultExt = L"entity";
			this->_openEntityDlg->Filter = L"Entity Files (*.entity)|*.entity|All Files (*.*)|*.*";
			this->_openEntityDlg->Title = L"Open Entity";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(830, 627);
			this->Controls->Add(this->_dockPanel);
			this->Controls->Add(this->_mainToolbar);
			this->Controls->Add(this->_statusBar);
			this->Controls->Add(this->_mainMenu);
			this->MainMenuStrip = this->_mainMenu;
			this->Name = L"MainForm";
			this->Text = L"Dark Dream Entity Editor";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->_mainMenu->ResumeLayout(false);
			this->_mainMenu->PerformLayout();
			this->_mainToolbar->ResumeLayout(false);
			this->_mainToolbar->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
	private: System::Void _mnuViewConsole_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuModelStats_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuView_DropDownOpening(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuViewEntityPanel_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuViewPropertyPanel_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuEntityNew_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuEntityOpen_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuEntitySave_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuEntitySaveAs_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuEntityExit_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void fileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e);
	private: System::Void _mnuAnimate_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuWireframe_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuViewSkelet_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _mnuViewMaterialPanel_Click(System::Object^  sender, System::EventArgs^  e);
};
}

