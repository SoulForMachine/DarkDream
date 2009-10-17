
#include "stdafx.h"
#include "MapForm.h"
#include "ToolPanel.h"
#include "TerrainEditPanel.h"
#include "Utility.h"
#include "UndoManager.h"
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
		_mapForm = gcnew MapForm(this);

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

		_mapForm->Show();
		_toolPanel->SetPanel(_mapForm->GetCurrentEditMode()->GetPanel());
		UpdateToolbarButtons();

		_dockPanel->ResumeLayout(true, true);

		_wireframe = false;
		_viewStats = true;

		Application::Idle += gcnew EventHandler(this, &MainForm::OnIdle);

		engineAPI->world->GetTerrain().AddPatch();
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
		_mapForm->GetUndoManager()->Undo();
	}

	System::Void MainForm::_menuEditRedo_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->GetUndoManager()->Redo();
	}

	System::Void MainForm::_menuRegionNewTerrainPatch_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::ADD_PATCH);
	}

	System::Void MainForm::_menuTerrainNewPatchAtEnd_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(engineAPI->world->GetTerrain().GetPatchCount() == Engine::Terrain::MAX_PATCHES)
		{
			MessageBox::Show(
				"Failed to add terrain patch - maximum of 32 patches reached.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		if(engineAPI->world->GetTerrain().AddPatch() == -1)
		{
			MessageBox::Show(
				"Failed to add terrain patch.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		else
		{
			_mapForm->GetUndoManager()->Clear();
		}
	}

	System::Void MainForm::_menuTerrainRemovePatch_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::REMOVE_PATCH);
	}

	System::Void MainForm::_menuToolsOptions_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_toolBtnViewMode_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::VIEW);
		_toolPanel->SetPanel(_mapForm->GetCurrentEditMode()->GetPanel());
		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnTerrainEdit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::TERRAIN_EDIT);
		_toolPanel->SetPanel(_mapForm->GetCurrentEditMode()->GetPanel());
		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnObjectPlacement_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::PLACE_OBJECT);
		_toolPanel->SetPanel(_mapForm->GetCurrentEditMode()->GetPanel());
		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnTriggers_Click(System::Object^  sender, System::EventArgs^  e)
	{

		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnParticleSystems_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_toolBtnLayers_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::LAYER_EDIT);
		_mapForm->SetViewMode(MapRenderWindow::ViewMode::GAME);
		_toolPanel->SetPanel(_mapForm->GetCurrentEditMode()->GetPanel());
		UpdateToolbarButtons();
	}

	System::Void MainForm::_menuViewWireframe_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_wireframe = !_wireframe;
		_mapForm->Wireframe(_wireframe);
		_mapForm->Redraw();
	}

	void MainForm::UpdateToolbarButtons()
	{
		// reset toolbar button states
		_toolBtnViewMode->Checked = false;
		_toolBtnTerrainEdit->Checked = false;
		_toolBtnObjectPlacement->Checked = false;
		_toolBtnTriggers->Checked = false;
		_toolBtnParticleSystems->Checked = false;
		_toolBtnLayers->Checked = false;

		switch(_mapForm->GetPersistentEditMode()->GetModeEnum())
		{
		case EditMode::EditModeEnum::VIEW:
			_toolBtnViewMode->Checked = true;
			break;
		case EditMode::EditModeEnum::TERRAIN_EDIT:
			_toolBtnTerrainEdit->Checked = true;
			break;
		case EditMode::EditModeEnum::PLACE_OBJECT:
			_toolBtnObjectPlacement->Checked = true;
			break;
		case EditMode::EditModeEnum::LAYER_EDIT:
			_toolBtnLayers->Checked = true;
			break;
		default:
			assert(0);
		}
	}

	bool AppIsIdle()
	{
		MSG msg;
		return !PeekMessage(&msg, 0, 0, 0, 0);
	}


	System::Void MainForm::OnIdle(System::Object^  sender, System::EventArgs^  e)
	{
		if(GetForegroundWindow() == (HWND)Handle.ToPointer())
		{
			_menuViewWireframe->Checked = _wireframe;
			_menuViewStats->Checked = _viewStats;
			_menuViewGameView->Checked = (_mapForm->GetViewMode() == MapRenderWindow::ViewMode::GAME);
			_menuViewGameView->Enabled = (_mapForm->GetCurrentEditMode()->GetModeEnum() != EditMode::EditModeEnum::LAYER_EDIT);
			_menuViewToolPanel->Checked = !_toolPanel->IsHidden;
			_menuViewConsole->Checked = !_consoleForm->IsHidden;
			_menuEditUndo->Enabled = _mapForm->GetUndoManager()->HasUndo();
			_menuEditRedo->Enabled = _mapForm->GetUndoManager()->HasRedo();

			while(AppIsIdle())
			{
				_mapForm->UpdateFrame();
				_mapForm->Redraw();
			}
		}
	}

	System::Void MainForm::_menuViewStats_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_viewStats = !_viewStats;
		_mapForm->ShowStats(_viewStats);
	}

	System::Void MainForm::_menuViewTerrainNormals_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_menuViewTerrainNormals->Checked = !_menuViewTerrainNormals->Checked;
		char buf[128];
		sprintf(buf, "r_drawTerrainNormals %d", _menuViewTerrainNormals->Checked? 1: 0);
		::Console::ExecuteStatement(buf);
	}

	System::Void MainForm::_menuViewEntityBBoxes_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_menuViewEntityBBoxes->Checked = !_menuViewEntityBBoxes->Checked;
		char buf[128];
		sprintf(buf, "r_drawEntityBBoxes %d", _menuViewEntityBBoxes->Checked? 1: 0);
		::Console::ExecuteStatement(buf);
	}

	System::Void MainForm::_menuViewGameView_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_mapForm->GetViewMode() == MapRenderWindow::ViewMode::EDITOR)
			_mapForm->SetViewMode(MapRenderWindow::ViewMode::GAME);
		else
			_mapForm->SetViewMode(MapRenderWindow::ViewMode::EDITOR);
	}

	System::Void MainForm::_menuViewToolPanel_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ToggleForm(_toolPanel);
	}

	System::Void MainForm::_menuViewConsole_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ToggleForm(_consoleForm);
	}

	void MainForm::ToggleForm(WeifenLuo::WinFormsUI::Docking::DockContent^ form)
	{
		if(form->IsHidden)
			form->Show(_dockPanel);
		else
			form->Hide();
	}

}
