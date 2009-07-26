
#include "stdafx.h"
#include "MapForm.h"
#include "ToolPanel.h"
#include "TerrainEditPanel.h"
#include "MainForm.h"

#define DOCK_PANEL_XML_FILE		"MapEditorDocking.xml"

using namespace EditorCommon;


namespace MapEditor
{

	MainForm::MainForm()
	{
		InitializeComponent();
		
		_deserializeDockContent = gcnew WeifenLuo::WinFormsUI::Docking::DeserializeDockContent(this, &MainForm::GetContentFromPersistString);
		_consoleForm = gcnew ConsoleForm(this);
		_toolPanel = gcnew ToolPanel(this);
		_mapForm = gcnew MapForm(this, _toolPanel);

		// load layout for dock panels from an xml
		_dockPanel->SuspendLayout(true);
		try
		{
			_dockPanel->LoadFromXml(Application::StartupPath + "\\" + DOCK_PANEL_XML_FILE, _deserializeDockContent);
		}
		catch(...)
		{
			
		}

		if(!_consoleForm->IsHandleCreated)
			_consoleForm->Show(_dockPanel);
		if(!_mapForm->IsHandleCreated)
			_mapForm->Show(_dockPanel);
		if(!_toolPanel->IsHandleCreated)
			_toolPanel->Show(_dockPanel);

		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::VIEW);
		_mapForm->Show();
		UpdateToolbarButtons();

		_dockPanel->ResumeLayout(true, true);
	}

	void MainForm::FormNotify(Form^ form, EditorCommon::NotifyMessage msg)
	{
	}

	WeifenLuo::WinFormsUI::Docking::IDockContent^ MainForm::GetContentFromPersistString(String^ persistString)
	{
		if(persistString == _consoleForm->GetType()->ToString())
		{
			return _consoleForm;
		}
		else if(persistString == _mapForm->GetType()->ToString())
		{
			return _mapForm;
		}
		else if(persistString == _toolPanel->GetType()->ToString())
		{
			return _toolPanel;
		}
		else
		{
			return nullptr;
		}
	}

	System::Void MainForm::MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
	{
		// save layout for dock panels to xml
		_dockPanel->SaveAsXml(Application::StartupPath + "\\" + DOCK_PANEL_XML_FILE);
	}

	System::Void MainForm::_menuFileNewMap_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileOpenMap_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileSaveMap_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileSaveMapAs_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileExit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}

	System::Void MainForm::_menuEditUndo_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuEditRedo_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuRegionNewTerrain_Click(System::Object^  sender, System::EventArgs^  e)
	{
		
	}

	System::Void MainForm::_menuRegionNewGrid_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuToolsOptions_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_toolBtnViewMode_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::VIEW);
		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnTerrainEdit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::TERRAIN_EDIT);
		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnObjectPlacement_Click(System::Object^  sender, System::EventArgs^  e)
	{

		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnTriggers_Click(System::Object^  sender, System::EventArgs^  e)
	{

		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnParticleSystems_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	void MainForm::UpdateToolbarButtons()
	{
		// reset toolbar button states
		_toolBtnViewMode->Checked = false;
		_toolBtnTerrainEdit->Checked = false;
		_toolBtnObjectPlacement->Checked = false;
		_toolBtnTriggers->Checked = false;
		_toolBtnParticleSystems->Checked = false;

		switch(_mapForm->GetCurrentEditMode())
		{
		case EditMode::EditModeEnum::VIEW:
			_toolBtnViewMode->Checked = true;
			break;
		case EditMode::EditModeEnum::TERRAIN_EDIT:
			_toolBtnTerrainEdit->Checked = true;
			break;
		default:
			assert(0);
		}
	}
}
