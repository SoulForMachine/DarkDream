#pragma once

#include "EditMode.h"


namespace MapEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	ref class MapForm;
	ref class ToolPanel;

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
		void UpdateToolbarButtons();
		void ToggleForm(WeifenLuo::WinFormsUI::Docking::DockContent^ form);

		WeifenLuo::WinFormsUI::Docking::DeserializeDockContent^ _deserializeDockContent;
		EditorCommon::ConsoleForm^ _consoleForm;
		MapForm^ _mapForm;
		ToolPanel^ _toolPanel;
		bool _wireframe;
		bool _viewStats;

	private: System::Windows::Forms::MenuStrip^  _mainMenu;
	protected: 

	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;

	private: System::Windows::Forms::StatusStrip^  _statusBar;


	private: WeifenLuo::WinFormsUI::Docking::DockPanel^  _dockPanel;






	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileNewMap;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileOpenMap;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileSaveMap;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileSaveMapAs;




	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuFileExit;

	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuEditUndo;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuEditRedo;
	private: System::Windows::Forms::ToolStripMenuItem^  regionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuRegionNewTerrain;


	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuToolsOptions;
	private: System::Windows::Forms::OpenFileDialog^  _openFileDialog;
	private: System::Windows::Forms::SaveFileDialog^  _saveFileDialog;
	private: System::Windows::Forms::ToolStrip^  _mainToolbar;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnViewMode;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnTerrainEdit;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnObjectPlacement;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnTriggers;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnParticleSystems;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewWireframe;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewStats;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewTerrainNormals;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem3;

	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewGameView;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuTerrainRemovePatch;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuTerrainNewPatchAtEnd;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewToolPanel;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewConsole;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem4;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuViewEntityBBoxes;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnLayers;
	private: System::Windows::Forms::ToolStripButton^  _toolBtnGrass;





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
			this->_mainMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileNewMap = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileOpenMap = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileSaveMap = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuFileSaveMapAs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_menuFileExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuEditUndo = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuEditRedo = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuViewToolPanel = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuViewConsole = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_menuViewWireframe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuViewStats = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuViewTerrainNormals = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuViewEntityBBoxes = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->_menuViewGameView = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->regionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuRegionNewTerrain = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuTerrainNewPatchAtEnd = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuTerrainRemovePatch = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuToolsOptions = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_statusBar = (gcnew System::Windows::Forms::StatusStrip());
			this->_dockPanel = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanel());
			this->_openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->_mainToolbar = (gcnew System::Windows::Forms::ToolStrip());
			this->_toolBtnViewMode = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnTerrainEdit = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnObjectPlacement = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnTriggers = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnParticleSystems = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnLayers = (gcnew System::Windows::Forms::ToolStripButton());
			this->_toolBtnGrass = (gcnew System::Windows::Forms::ToolStripButton());
			this->_mainMenu->SuspendLayout();
			this->_mainToolbar->SuspendLayout();
			this->SuspendLayout();
			// 
			// _mainMenu
			// 
			this->_mainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->fileToolStripMenuItem, 
				this->editToolStripMenuItem, this->toolStripMenuItem2, this->regionToolStripMenuItem, this->toolsToolStripMenuItem});
			this->_mainMenu->Location = System::Drawing::Point(0, 0);
			this->_mainMenu->Name = L"_mainMenu";
			this->_mainMenu->Size = System::Drawing::Size(860, 24);
			this->_mainMenu->TabIndex = 0;
			this->_mainMenu->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->_menuFileNewMap, 
				this->_menuFileOpenMap, this->_menuFileSaveMap, this->_menuFileSaveMapAs, this->toolStripMenuItem1, this->_menuFileExit});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// _menuFileNewMap
			// 
			this->_menuFileNewMap->Name = L"_menuFileNewMap";
			this->_menuFileNewMap->Size = System::Drawing::Size(159, 22);
			this->_menuFileNewMap->Text = L"&New Map...";
			this->_menuFileNewMap->Click += gcnew System::EventHandler(this, &MainForm::_menuFileNewMap_Click);
			// 
			// _menuFileOpenMap
			// 
			this->_menuFileOpenMap->Name = L"_menuFileOpenMap";
			this->_menuFileOpenMap->Size = System::Drawing::Size(159, 22);
			this->_menuFileOpenMap->Text = L"&Open Map...";
			this->_menuFileOpenMap->Click += gcnew System::EventHandler(this, &MainForm::_menuFileOpenMap_Click);
			// 
			// _menuFileSaveMap
			// 
			this->_menuFileSaveMap->Name = L"_menuFileSaveMap";
			this->_menuFileSaveMap->Size = System::Drawing::Size(159, 22);
			this->_menuFileSaveMap->Text = L"&Save Map";
			this->_menuFileSaveMap->Click += gcnew System::EventHandler(this, &MainForm::_menuFileSaveMap_Click);
			// 
			// _menuFileSaveMapAs
			// 
			this->_menuFileSaveMapAs->Name = L"_menuFileSaveMapAs";
			this->_menuFileSaveMapAs->Size = System::Drawing::Size(159, 22);
			this->_menuFileSaveMapAs->Text = L"Save Map As...";
			this->_menuFileSaveMapAs->Click += gcnew System::EventHandler(this, &MainForm::_menuFileSaveMapAs_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(156, 6);
			// 
			// _menuFileExit
			// 
			this->_menuFileExit->Name = L"_menuFileExit";
			this->_menuFileExit->Size = System::Drawing::Size(159, 22);
			this->_menuFileExit->Text = L"E&xit";
			this->_menuFileExit->Click += gcnew System::EventHandler(this, &MainForm::_menuFileExit_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_menuEditUndo, 
				this->_menuEditRedo});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->editToolStripMenuItem->Text = L"&Edit";
			// 
			// _menuEditUndo
			// 
			this->_menuEditUndo->Name = L"_menuEditUndo";
			this->_menuEditUndo->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Z));
			this->_menuEditUndo->Size = System::Drawing::Size(148, 22);
			this->_menuEditUndo->Text = L"&Undo";
			this->_menuEditUndo->Click += gcnew System::EventHandler(this, &MainForm::_menuEditUndo_Click);
			// 
			// _menuEditRedo
			// 
			this->_menuEditRedo->Name = L"_menuEditRedo";
			this->_menuEditRedo->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Y));
			this->_menuEditRedo->Size = System::Drawing::Size(148, 22);
			this->_menuEditRedo->Text = L"&Redo";
			this->_menuEditRedo->Click += gcnew System::EventHandler(this, &MainForm::_menuEditRedo_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->_menuViewToolPanel, 
				this->_menuViewConsole, this->toolStripMenuItem4, this->_menuViewWireframe, this->_menuViewStats, this->_menuViewTerrainNormals, 
				this->_menuViewEntityBBoxes, this->toolStripMenuItem3, this->_menuViewGameView});
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(41, 20);
			this->toolStripMenuItem2->Text = L"&View";
			// 
			// _menuViewToolPanel
			// 
			this->_menuViewToolPanel->Name = L"_menuViewToolPanel";
			this->_menuViewToolPanel->Size = System::Drawing::Size(192, 22);
			this->_menuViewToolPanel->Text = L"&Tool Panel";
			this->_menuViewToolPanel->Click += gcnew System::EventHandler(this, &MainForm::_menuViewToolPanel_Click);
			// 
			// _menuViewConsole
			// 
			this->_menuViewConsole->Name = L"_menuViewConsole";
			this->_menuViewConsole->Size = System::Drawing::Size(192, 22);
			this->_menuViewConsole->Text = L"&Console";
			this->_menuViewConsole->Click += gcnew System::EventHandler(this, &MainForm::_menuViewConsole_Click);
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			this->toolStripMenuItem4->Size = System::Drawing::Size(189, 6);
			// 
			// _menuViewWireframe
			// 
			this->_menuViewWireframe->Name = L"_menuViewWireframe";
			this->_menuViewWireframe->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::W));
			this->_menuViewWireframe->Size = System::Drawing::Size(192, 22);
			this->_menuViewWireframe->Text = L"&Wireframe";
			this->_menuViewWireframe->Click += gcnew System::EventHandler(this, &MainForm::_menuViewWireframe_Click);
			// 
			// _menuViewStats
			// 
			this->_menuViewStats->Name = L"_menuViewStats";
			this->_menuViewStats->ShortcutKeys = System::Windows::Forms::Keys::F2;
			this->_menuViewStats->Size = System::Drawing::Size(192, 22);
			this->_menuViewStats->Text = L"&Statistics";
			this->_menuViewStats->Click += gcnew System::EventHandler(this, &MainForm::_menuViewStats_Click);
			// 
			// _menuViewTerrainNormals
			// 
			this->_menuViewTerrainNormals->Name = L"_menuViewTerrainNormals";
			this->_menuViewTerrainNormals->Size = System::Drawing::Size(192, 22);
			this->_menuViewTerrainNormals->Text = L"Terrain &Normals";
			this->_menuViewTerrainNormals->Click += gcnew System::EventHandler(this, &MainForm::_menuViewTerrainNormals_Click);
			// 
			// _menuViewEntityBBoxes
			// 
			this->_menuViewEntityBBoxes->Name = L"_menuViewEntityBBoxes";
			this->_menuViewEntityBBoxes->Size = System::Drawing::Size(192, 22);
			this->_menuViewEntityBBoxes->Text = L"Entity &Bounding Boxes";
			this->_menuViewEntityBBoxes->Click += gcnew System::EventHandler(this, &MainForm::_menuViewEntityBBoxes_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(189, 6);
			// 
			// _menuViewGameView
			// 
			this->_menuViewGameView->Name = L"_menuViewGameView";
			this->_menuViewGameView->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::G));
			this->_menuViewGameView->Size = System::Drawing::Size(192, 22);
			this->_menuViewGameView->Text = L"&Game View";
			this->_menuViewGameView->Click += gcnew System::EventHandler(this, &MainForm::_menuViewGameView_Click);
			// 
			// regionToolStripMenuItem
			// 
			this->regionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->_menuRegionNewTerrain, 
				this->_menuTerrainNewPatchAtEnd, this->_menuTerrainRemovePatch});
			this->regionToolStripMenuItem->Name = L"regionToolStripMenuItem";
			this->regionToolStripMenuItem->Size = System::Drawing::Size(53, 20);
			this->regionToolStripMenuItem->Text = L"&Terrain";
			// 
			// _menuRegionNewTerrain
			// 
			this->_menuRegionNewTerrain->Name = L"_menuRegionNewTerrain";
			this->_menuRegionNewTerrain->Size = System::Drawing::Size(171, 22);
			this->_menuRegionNewTerrain->Text = L"&New Patch";
			this->_menuRegionNewTerrain->Click += gcnew System::EventHandler(this, &MainForm::_menuRegionNewTerrainPatch_Click);
			// 
			// _menuTerrainNewPatchAtEnd
			// 
			this->_menuTerrainNewPatchAtEnd->Name = L"_menuTerrainNewPatchAtEnd";
			this->_menuTerrainNewPatchAtEnd->Size = System::Drawing::Size(171, 22);
			this->_menuTerrainNewPatchAtEnd->Text = L"New Patch At &End";
			this->_menuTerrainNewPatchAtEnd->Click += gcnew System::EventHandler(this, &MainForm::_menuTerrainNewPatchAtEnd_Click);
			// 
			// _menuTerrainRemovePatch
			// 
			this->_menuTerrainRemovePatch->Name = L"_menuTerrainRemovePatch";
			this->_menuTerrainRemovePatch->Size = System::Drawing::Size(171, 22);
			this->_menuTerrainRemovePatch->Text = L"&Remove Patch";
			this->_menuTerrainRemovePatch->Click += gcnew System::EventHandler(this, &MainForm::_menuTerrainRemovePatch_Click);
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->_menuToolsOptions});
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->toolsToolStripMenuItem->Text = L"&Tools";
			// 
			// _menuToolsOptions
			// 
			this->_menuToolsOptions->Name = L"_menuToolsOptions";
			this->_menuToolsOptions->Size = System::Drawing::Size(122, 22);
			this->_menuToolsOptions->Text = L"&Options";
			this->_menuToolsOptions->Click += gcnew System::EventHandler(this, &MainForm::_menuToolsOptions_Click);
			// 
			// _statusBar
			// 
			this->_statusBar->Location = System::Drawing::Point(0, 600);
			this->_statusBar->Name = L"_statusBar";
			this->_statusBar->Size = System::Drawing::Size(860, 22);
			this->_statusBar->TabIndex = 2;
			this->_statusBar->Text = L"statusStrip1";
			// 
			// _dockPanel
			// 
			this->_dockPanel->ActiveAutoHideContent = nullptr;
			this->_dockPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->_dockPanel->DocumentStyle = WeifenLuo::WinFormsUI::Docking::DocumentStyle::DockingWindow;
			this->_dockPanel->Location = System::Drawing::Point(0, 62);
			this->_dockPanel->Name = L"_dockPanel";
			this->_dockPanel->Size = System::Drawing::Size(860, 538);
			this->_dockPanel->TabIndex = 3;
			// 
			// _openFileDialog
			// 
			this->_openFileDialog->FileName = L"openFileDialog1";
			// 
			// _mainToolbar
			// 
			this->_mainToolbar->AutoSize = false;
			this->_mainToolbar->ImageScalingSize = System::Drawing::Size(32, 32);
			this->_mainToolbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->_toolBtnViewMode, 
				this->_toolBtnTerrainEdit, this->_toolBtnObjectPlacement, this->_toolBtnGrass, this->_toolBtnTriggers, this->_toolBtnParticleSystems, 
				this->_toolBtnLayers});
			this->_mainToolbar->Location = System::Drawing::Point(0, 24);
			this->_mainToolbar->Name = L"_mainToolbar";
			this->_mainToolbar->Size = System::Drawing::Size(860, 38);
			this->_mainToolbar->TabIndex = 4;
			this->_mainToolbar->Text = L"toolStrip1";
			// 
			// _toolBtnViewMode
			// 
			this->_toolBtnViewMode->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnViewMode->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnViewMode.Image")));
			this->_toolBtnViewMode->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnViewMode->Name = L"_toolBtnViewMode";
			this->_toolBtnViewMode->Size = System::Drawing::Size(36, 35);
			this->_toolBtnViewMode->Text = L"View Mode";
			this->_toolBtnViewMode->Click += gcnew System::EventHandler(this, &MainForm::_toolBtnViewMode_Click);
			// 
			// _toolBtnTerrainEdit
			// 
			this->_toolBtnTerrainEdit->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnTerrainEdit->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnTerrainEdit.Image")));
			this->_toolBtnTerrainEdit->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnTerrainEdit->Name = L"_toolBtnTerrainEdit";
			this->_toolBtnTerrainEdit->Size = System::Drawing::Size(36, 35);
			this->_toolBtnTerrainEdit->Text = L"Edit Terrain";
			this->_toolBtnTerrainEdit->Click += gcnew System::EventHandler(this, &MainForm::_toolBtnTerrainEdit_Click);
			// 
			// _toolBtnObjectPlacement
			// 
			this->_toolBtnObjectPlacement->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnObjectPlacement->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnObjectPlacement.Image")));
			this->_toolBtnObjectPlacement->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnObjectPlacement->Name = L"_toolBtnObjectPlacement";
			this->_toolBtnObjectPlacement->Size = System::Drawing::Size(36, 35);
			this->_toolBtnObjectPlacement->Text = L"Place Objects";
			this->_toolBtnObjectPlacement->Click += gcnew System::EventHandler(this, &MainForm::_toolBtnObjectPlacement_Click);
			// 
			// _toolBtnTriggers
			// 
			this->_toolBtnTriggers->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnTriggers->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnTriggers.Image")));
			this->_toolBtnTriggers->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnTriggers->Name = L"_toolBtnTriggers";
			this->_toolBtnTriggers->Size = System::Drawing::Size(36, 35);
			this->_toolBtnTriggers->Text = L"Triggers";
			this->_toolBtnTriggers->Click += gcnew System::EventHandler(this, &MainForm::_toolBtnTriggers_Click);
			// 
			// _toolBtnParticleSystems
			// 
			this->_toolBtnParticleSystems->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnParticleSystems->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnParticleSystems.Image")));
			this->_toolBtnParticleSystems->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnParticleSystems->Name = L"_toolBtnParticleSystems";
			this->_toolBtnParticleSystems->Size = System::Drawing::Size(36, 35);
			this->_toolBtnParticleSystems->Text = L"Particle Systems";
			this->_toolBtnParticleSystems->Click += gcnew System::EventHandler(this, &MainForm::_toolBtnParticleSystems_Click);
			// 
			// _toolBtnLayers
			// 
			this->_toolBtnLayers->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnLayers->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnLayers.Image")));
			this->_toolBtnLayers->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnLayers->Name = L"_toolBtnLayers";
			this->_toolBtnLayers->Size = System::Drawing::Size(36, 35);
			this->_toolBtnLayers->Text = L"Layers";
			this->_toolBtnLayers->Click += gcnew System::EventHandler(this, &MainForm::_toolBtnLayers_Click);
			// 
			// _toolBtnGrass
			// 
			this->_toolBtnGrass->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->_toolBtnGrass->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_toolBtnGrass.Image")));
			this->_toolBtnGrass->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->_toolBtnGrass->Name = L"_toolBtnGrass";
			this->_toolBtnGrass->Size = System::Drawing::Size(36, 35);
			this->_toolBtnGrass->Text = L"Grass";
			this->_toolBtnGrass->Click += gcnew System::EventHandler(this, &MainForm::_toolBtnGrass_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(860, 622);
			this->Controls->Add(this->_dockPanel);
			this->Controls->Add(this->_mainToolbar);
			this->Controls->Add(this->_statusBar);
			this->Controls->Add(this->_mainMenu);
			this->MainMenuStrip = this->_mainMenu;
			this->Name = L"MainForm";
			this->Text = L"Dark Dream Map Editor";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->_mainMenu->ResumeLayout(false);
			this->_mainMenu->PerformLayout();
			this->_mainToolbar->ResumeLayout(false);
			this->_mainToolbar->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
	private: System::Void _menuFileNewMap_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileOpenMap_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileSaveMap_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileSaveMapAs_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuFileExit_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuEditUndo_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuEditRedo_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuToolsOptions_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _toolBtnViewMode_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _toolBtnTerrainEdit_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _toolBtnObjectPlacement_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _toolBtnTriggers_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _toolBtnParticleSystems_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuRegionNewTerrainPatch_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewWireframe_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void OnIdle(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewStats_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewTerrainNormals_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewGameView_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuTerrainRemovePatch_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuTerrainNewPatchAtEnd_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewToolPanel_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewConsole_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuViewEntityBBoxes_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _toolBtnLayers_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _toolBtnGrass_Click(System::Object^  sender, System::EventArgs^  e);
};

}

