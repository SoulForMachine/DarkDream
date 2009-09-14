
#include "stdafx.h"
#include "MapForm.h"
#include "ToolPanel.h"
#include "TerrainEditPanel.h"
#include "Utility.h"
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
		UpdateToolbarButtons();

		_dockPanel->ResumeLayout(true, true);

		_wireframe = false;
		_viewStats = true;

		Application::Idle += gcnew EventHandler(this, &MainForm::OnIdle);

		AddTerrainPatch();
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

	System::Void MainForm::_menuRegionNewTerrainPatch_Click(System::Object^  sender, System::EventArgs^  e)
	{
		AddTerrainPatch();
	}

	System::Void MainForm::_menuToolsOptions_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_toolBtnViewMode_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::VIEW);
		_toolPanel->SetPanel(nullptr);
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

		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnTriggers_Click(System::Object^  sender, System::EventArgs^  e)
	{

		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnParticleSystems_Click(System::Object^  sender, System::EventArgs^  e)
	{

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

		switch(_mapForm->GetCurrentEditMode()->GetModeEnum())
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
			_menuViewEditorView->Checked = (_mapForm->GetViewMode() == MapRenderWindow::ViewMode::EDITOR);
			_menuViewGameView->Checked = (_mapForm->GetViewMode() == MapRenderWindow::ViewMode::GAME);

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

	void MainForm::AddTerrainPatch()
	{
		if(!engineAPI->world->GetTerrain().AddPatch())
		{
			if(engineAPI->world->GetTerrain().GetPatchCount() == Engine::Terrain::MAX_PATCHES)
			{
				Windows::Forms::MessageBox::Show(
					this, "Failed to add terrain patch - maximum of 32 patches reached.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			else
			{
				Windows::Forms::MessageBox::Show(
					this, "Failed to add terrain patch.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
		_mapForm->Redraw();
	}

	System::Void MainForm::_menuViewTerrainNormals_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_menuViewTerrainNormals->Checked = !_menuViewTerrainNormals->Checked;
		char buf[128];
		sprintf(buf, "r_drawTerrainNormals %d", _menuViewTerrainNormals->Checked? 1: 0);
		::Console::ExecuteStatement(buf);
	}

	System::Void MainForm::_menuViewEditorView_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_mapForm->GetViewMode() != MapRenderWindow::ViewMode::EDITOR)
			_mapForm->SetViewMode(MapRenderWindow::ViewMode::EDITOR);
	}

	System::Void MainForm::_menuViewGameView_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_mapForm->GetViewMode() != MapRenderWindow::ViewMode::GAME)
			_mapForm->SetViewMode(MapRenderWindow::ViewMode::GAME);
	}

}
