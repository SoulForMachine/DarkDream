
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
		_movingCam = false;

		Application::Idle += gcnew EventHandler(this, &MainForm::OnIdle);

		engineAPI->world->GetTerrain().AddPatch();

		System::Reflection::MethodInfo^ method = Control::typeid->GetMethod(
               "SetStyle",
			   Reflection::BindingFlags::NonPublic | 
			   Reflection::BindingFlags::Instance);
		array<Object^, 1>^ params = gcnew array<Object^, 1>(2);
		params[0] = ControlStyles::AllPaintingInWmPaint | ControlStyles::OptimizedDoubleBuffer;
		params[1] = true;
		method->Invoke(_panelCamPosBar, params);
		_panelCamPosBar->Hide();
	}

	void MainForm::FormNotify(Form^ form, EditorCommon::NotifyMessage msg)
	{
		if(form == _mapForm)
		{
			_panelCamPosBar->Refresh();
		}
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
			_panelCamPosBar->Refresh();
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
		_panelCamPosBar->Show();
		UpdateToolbarButtons();
	}

	System::Void MainForm::_toolBtnGrass_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_mapForm->SetCurrentEditMode(EditMode::EditModeEnum::PAINT_GRASS);
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
		_toolBtnGrass->Checked = false;

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
		case EditMode::EditModeEnum::PAINT_GRASS:
			_toolBtnGrass->Checked = true;
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
		{
			_mapForm->SetViewMode(MapRenderWindow::ViewMode::GAME);
			_panelCamPosBar->Show();
		}
		else
		{
			_mapForm->SetViewMode(MapRenderWindow::ViewMode::EDITOR);
			_panelCamPosBar->Hide();
		}
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

	System::Void MainForm::_panelCamPosBar_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		e->Graphics->Clear(Color::Gray);
		Drawing::Rectangle cl_rect = _panelCamPosBar->ClientRectangle;
		cl_rect.Width -= 1;
		cl_rect.Height -= 1;
		e->Graphics->DrawRectangle(Pens::Black, cl_rect);

		int count = engineAPI->world->GetTerrain().GetPatchCount();
		float y1 = (float)_panelCamPosBar->ClientRectangle.Bottom;
		float y2 = (float)_panelCamPosBar->ClientRectangle.Height / 2;
		float step = _panelCamPosBar->ClientRectangle.Width / float(count);
		float x = step;
		Drawing::Font^ font = gcnew Drawing::Font("Verdana", 11);
		for(int i = 0; i < count; ++i)
		{
			e->Graphics->DrawString((i + 1).ToString(), font, Brushes::Black, x - step / 2.0f, (y1 - font->Height) / 2.0f);

			if(i < count - 1)
			{
				e->Graphics->DrawLine(Pens::Black, x, y1, x, y2);
			}
			x += step;
		}

		float cam_x = _mapForm->GetCamX() / (engineAPI->world->GetTerrain().GetPatchCount() * Engine::Terrain::PATCH_WIDTH) *  _panelCamPosBar->ClientRectangle.Width;
		Drawing::Rectangle rect((int)cam_x - 5, 1, 10, (int)y1 - 3);
		SolidBrush^ brush = gcnew SolidBrush(Color::FromArgb(50, 0, 0, 0));
		e->Graphics->FillRectangle(brush, rect);
		e->Graphics->DrawRectangle(Pens::Black, rect);
	}

	System::Void MainForm::_panelCamPosBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_mapForm->GetViewMode() == MapRenderWindow::ViewMode::GAME)
		{
			_panelCamPosBar->Capture = true;
			_movingCam = true;
			float x = float(e->X) / _panelCamPosBar->ClientRectangle.Width * engineAPI->world->GetTerrain().GetPatchCount() * Engine::Terrain::PATCH_WIDTH;
			_mapForm->SetCamX(x);
			_panelCamPosBar->Refresh();
		}
	}

	System::Void MainForm::_panelCamPosBar_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_panelCamPosBar->Capture = false;
		_movingCam = false;
	}

	System::Void MainForm::_panelCamPosBar_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_movingCam)
		{
			float terr_w = (float)engineAPI->world->GetTerrain().GetPatchCount() * Engine::Terrain::PATCH_WIDTH;
			float x = float(e->X) / _panelCamPosBar->ClientRectangle.Width * terr_w;
			math3d::clamp(x, 0.0f, terr_w);
			_mapForm->SetCamX(x);
			_panelCamPosBar->Refresh();
		}
	}

	System::Void MainForm::_panelCamPosBar_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		_panelCamPosBar->Invalidate();
	}

}
