
#include "stdafx.h"
#include "ViewportForm.h"
#include "EmitterPanel.h"
#include "PropertiesPanel.h"
#include "MainForm.h"

#define DOCK_PANEL_XML_FILE		"ParticleEditorDocking.xml"

using namespace EditorCommon;


namespace ParticleEditor
{

	MainForm::MainForm()
	{
		InitializeComponent();
		
		_deserializeDockContent = gcnew WeifenLuo::WinFormsUI::Docking::DeserializeDockContent(this, &MainForm::GetContentFromPersistString);
		_consoleForm = gcnew ConsoleForm(this);
		_consoleForm->DockAreas &= ~WeifenLuo::WinFormsUI::Docking::DockAreas::Document;
		_viewportForm = gcnew ViewportForm;
		_emitterPanel = gcnew EmitterPanel;
		_propertyPanel = gcnew PropertiesPanel;

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
		if(!_viewportForm->IsHandleCreated)
			_viewportForm->Show(_dockPanel);
		if(!_emitterPanel->IsHandleCreated)
			_emitterPanel->Show(_viewportForm->Pane, WeifenLuo::WinFormsUI::Docking::DockAlignment::Right, 0.25);
		if(!_propertyPanel->IsHandleCreated)
			_propertyPanel->Show(_viewportForm->Pane, WeifenLuo::WinFormsUI::Docking::DockAlignment::Bottom, 0.25);

		_viewportForm->Show();

		_dockPanel->ResumeLayout(true, true);

		Application::Idle += gcnew EventHandler(this, &MainForm::OnIdle);

		_viewStats = true;
	}

	void MainForm::FormNotify(System::Windows::Forms::Form^ form, NotifyMessage msg)
	{
	}

	WeifenLuo::WinFormsUI::Docking::IDockContent^ MainForm::GetContentFromPersistString(String^ persistString)
	{
		if(persistString == _consoleForm->GetType()->ToString())
		{
			return _consoleForm;
		}
		else if(persistString == _viewportForm->GetType()->ToString())
		{
			return _viewportForm;
		}
		else if(persistString == _emitterPanel->GetType()->ToString())
		{
			return _emitterPanel;
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

	System::Void MainForm::_menuFileNew_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileOpen_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileSave_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileSaveAs_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuFileExit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}

	System::Void MainForm::_menuViewConsole_Click(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void MainForm::_menuViewStats_Click(System::Object^  sender, System::EventArgs^  e)
	{

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
			_menuViewStats->Checked = _viewStats;
			_menuViewConsole->Checked = !_consoleForm->IsHidden;

			while(AppIsIdle())
			{
				_viewportForm->UpdateFrame();
				_viewportForm->Redraw();
			}
		}
	}

}
