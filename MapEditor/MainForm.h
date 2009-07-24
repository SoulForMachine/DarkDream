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

		WeifenLuo::WinFormsUI::Docking::DeserializeDockContent^ _deserializeDockContent;
		EditorCommon::ConsoleForm^ _consoleForm;
		MapForm^ _mapForm;
		ToolPanel^ _toolPanel;

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
	private: System::Windows::Forms::ToolStripMenuItem^  _menuRegionNewGrid;

	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  _menuToolsOptions;
	private: System::Windows::Forms::OpenFileDialog^  _openFileDialog;
	private: System::Windows::Forms::SaveFileDialog^  _saveFileDialog;
















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
			this->regionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuRegionNewTerrain = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuRegionNewGrid = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_menuToolsOptions = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->_statusBar = (gcnew System::Windows::Forms::StatusStrip());
			this->_dockPanel = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanel());
			this->_openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->_saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->_mainMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// _mainMenu
			// 
			this->_mainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->fileToolStripMenuItem, 
				this->editToolStripMenuItem, this->regionToolStripMenuItem, this->toolsToolStripMenuItem});
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
			this->_menuEditUndo->Size = System::Drawing::Size(110, 22);
			this->_menuEditUndo->Text = L"&Undo";
			this->_menuEditUndo->Click += gcnew System::EventHandler(this, &MainForm::_menuEditUndo_Click);
			// 
			// _menuEditRedo
			// 
			this->_menuEditRedo->Name = L"_menuEditRedo";
			this->_menuEditRedo->Size = System::Drawing::Size(110, 22);
			this->_menuEditRedo->Text = L"&Redo";
			this->_menuEditRedo->Click += gcnew System::EventHandler(this, &MainForm::_menuEditRedo_Click);
			// 
			// regionToolStripMenuItem
			// 
			this->regionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_menuRegionNewTerrain, 
				this->_menuRegionNewGrid});
			this->regionToolStripMenuItem->Name = L"regionToolStripMenuItem";
			this->regionToolStripMenuItem->Size = System::Drawing::Size(52, 20);
			this->regionToolStripMenuItem->Text = L"&Region";
			// 
			// _menuRegionNewTerrain
			// 
			this->_menuRegionNewTerrain->Name = L"_menuRegionNewTerrain";
			this->_menuRegionNewTerrain->Size = System::Drawing::Size(143, 22);
			this->_menuRegionNewTerrain->Text = L"New &Terrain";
			this->_menuRegionNewTerrain->Click += gcnew System::EventHandler(this, &MainForm::_menuRegionNewTerrain_Click);
			// 
			// _menuRegionNewGrid
			// 
			this->_menuRegionNewGrid->Name = L"_menuRegionNewGrid";
			this->_menuRegionNewGrid->Size = System::Drawing::Size(143, 22);
			this->_menuRegionNewGrid->Text = L"New &Grid";
			this->_menuRegionNewGrid->Click += gcnew System::EventHandler(this, &MainForm::_menuRegionNewGrid_Click);
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
			this->_menuToolsOptions->Size = System::Drawing::Size(152, 22);
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
			this->_dockPanel->Location = System::Drawing::Point(0, 24);
			this->_dockPanel->Name = L"_dockPanel";
			this->_dockPanel->Size = System::Drawing::Size(860, 576);
			this->_dockPanel->TabIndex = 3;
			// 
			// _openFileDialog
			// 
			this->_openFileDialog->FileName = L"openFileDialog1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(860, 622);
			this->Controls->Add(this->_dockPanel);
			this->Controls->Add(this->_statusBar);
			this->Controls->Add(this->_mainMenu);
			this->MainMenuStrip = this->_mainMenu;
			this->Name = L"MainForm";
			this->Text = L"Dark Dream Map Editor";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->_mainMenu->ResumeLayout(false);
			this->_mainMenu->PerformLayout();
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
	private: System::Void _menuRegionNewTerrain_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuRegionNewGrid_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void _menuToolsOptions_Click(System::Object^  sender, System::EventArgs^  e);
};
}

