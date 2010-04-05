
#include "stdafx.h"
#include "ViewportForm.h"
#include "EmitterPanel.h"
#include "PropertiesPanel.h"
#include "EditorCommon/UtilityTempl.h"
#include "MainForm.h"

#define DOCK_PANEL_XML_FILE		"ParticleEditorDocking.xml"

using namespace EditorCommon;
using namespace Engine;
using namespace Memory;


namespace ParticleEditor
{

	MainForm::MainForm()
	{
		InitializeComponent();
		
		_deserializeDockContent = gcnew WeifenLuo::WinFormsUI::Docking::DeserializeDockContent(this, &MainForm::GetContentFromPersistString);
		_consoleForm = gcnew ConsoleForm(this);
		_consoleForm->DockAreas &= ~WeifenLuo::WinFormsUI::Docking::DockAreas::Document;
		_viewportForm = gcnew ViewportForm;
		_propertyPanel = gcnew PropertiesPanel(this);
		_emitterPanel = gcnew EmitterPanel(this, _propertyPanel);

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

		_particleSystem = 0;
		_fileName = nullptr;
		_modified = false;

		if(_viewportForm->IsRenderingInitialized())
		{
			Application::Idle += gcnew EventHandler(this, &MainForm::OnIdle);
			NewParticleSystem();
		}
		else
		{
			_emitterPanel->Enabled = false;
			_propertyPanel->Enabled = false;
		}
	}

	void MainForm::FormNotify(System::Windows::Forms::Form^ form, NotifyMessage msg, Object^ param)
	{
		if(form == _emitterPanel || form == _propertyPanel)
			_modified = true;
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
		if(PromptSave())
		{
			_viewportForm->SetParticleSystem(0);
			_emitterPanel->SetParticleSystem(0);
			_propertyPanel->SetParticleSystem(0);
			if(_particleSystem)
			{
				ObjectFactory::DeleteEntity(_particleSystem);
				_particleSystem = 0;
			}
			engineAPI->partSysManager->ReleaseAll();
			// save layout for dock panels to xml
			_dockPanel->SaveAsXml(Application::StartupPath + "\\" + DOCK_PANEL_XML_FILE);
		}
		else
		{
			e->Cancel = true;
		}
	}

	System::Void MainForm::_menuFileNew_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_viewportForm->IsRenderingInitialized())
			return;

		NewParticleSystem();
	}

	System::Void MainForm::_menuFileOpen_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_viewportForm->IsRenderingInitialized())
			return;

		LoadParticleSystem();
	}

	System::Void MainForm::_menuFileSave_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_viewportForm->IsRenderingInitialized())
			return;

		SaveParticleSystem();
	}

	System::Void MainForm::_menuFileSaveAs_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_viewportForm->IsRenderingInitialized())
			return;

		SaveParticleSystemAs();
	}

	System::Void MainForm::_menuFileExit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}

	System::Void MainForm::_menuViewConsole_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ToggleForm(_consoleForm);
	}

	System::Void MainForm::_menuViewStats_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_viewportForm->IsRenderingInitialized())
			return;

		_viewportForm->ShowStats(!_viewportForm->StatsVisible());
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
			_menuViewStats->Checked = _viewportForm->StatsVisible();
			_toolBtnViewConsole->Checked = _viewportForm->StatsVisible();
			_menuViewConsole->Checked = !_consoleForm->IsHidden;

			while(AppIsIdle())
			{
				_viewportForm->UpdateFrame();
				_viewportForm->Redraw();
			}
		}
	}

	void MainForm::NewParticleSystem()
	{
		if(PromptSave())
		{
			if(_particleSystem)
			{
				_particleSystem->Unload();
				_particleSystem->ParticleSystem::~ParticleSystem();
				::operator delete(_particleSystem);
			}

			_particleSystem = ObjectFactory::NewParticleSystem();
			_viewportForm->SetParticleSystem(_particleSystem);
			_emitterPanel->SetParticleSystem(_particleSystem);
			_propertyPanel->SetParticleSystem(_particleSystem);
			_consoleForm->RedrawAsync();
			UpdateTitleBar();
		}
	}

	bool MainForm::LoadParticleSystem()
	{
		if(PromptSave())
		{
			_openPartSysDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Particles";
			Forms::DialogResult dlg_result = _openPartSysDialog->ShowDialog(this);
			if(dlg_result == Forms::DialogResult::OK)
			{
				_particleSystem->Unload();
				tchar* fn = EditorUtil::GetRelativePath(_openPartSysDialog->FileName);
				::Console::PrintLn("Loading particle system: %ls", fn);
				bool result = _particleSystem->Load(fn);

				if(result)
				{
					engineAPI->textureManager->LoadAll();
					engineAPI->modelManager->LoadAll();
					_modified = false;
					_fileName = _openPartSysDialog->FileName;
					_viewportForm->SetParticleSystem(_particleSystem);
					_emitterPanel->SetParticleSystem(_particleSystem);
					_propertyPanel->SetParticleSystem(_particleSystem);
					UpdateTitleBar();
				}
				else
					::Console::PrintError("Failed to load particle system: %ls", fn);
				delete[] fn;

				_consoleForm->RedrawAsync();
				if(!result)
				{
					Forms::MessageBox::Show(
						this, "Failed to open " + _openPartSysDialog->FileName, GetAppName(),
						MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				return result;
			}
		}
		return false;
	}

	bool MainForm::SaveParticleSystem()
	{
		if(_fileName == nullptr)
			return SaveParticleSystemAs();
		else
			return SaveParticleSystem(_fileName);
	}

	bool MainForm::SaveParticleSystemAs()
	{
		_savePartSysDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Particles";
		Forms::DialogResult dlg_result = _savePartSysDialog->ShowDialog(this);
		if(dlg_result != Forms::DialogResult::OK)
			return false;

		return SaveParticleSystem(_savePartSysDialog->FileName);
	}

	bool MainForm::SaveParticleSystem(String^ file_name)
	{
		if(!_particleSystem)
			return false;

		tchar* fn = EditorUtil::GetRelativePath(file_name);
		::Console::PrintLn("Saving particle system: %ls", fn);
		bool result = _particleSystem->Save(fn);
		if(result)
		{
			_modified = false;
			_fileName = file_name;
			UpdateTitleBar();
		}
		else
		{
			::Console::PrintError("Failed to save particle system: %ls", fn);
		}
		delete[] fn;

		_consoleForm->RedrawAsync();
		if(!result)
		{
			Forms::MessageBox::Show(
				this, "Failed to save " + file_name, GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		return result;
	}

	// returns true to notify the caller to proceed with intended operation
	bool MainForm::PromptSave()
	{
		if(_particleSystem != nullptr && _modified)
		{
			Forms::DialogResult result = MessageBox::Show(
				this, "Particle system modified. Do you want to save changes?", GetAppName(),
				MessageBoxButtons::YesNoCancel, MessageBoxIcon::Warning);

			switch(result)
			{
			case Forms::DialogResult::Cancel:
				return false;
			case Forms::DialogResult::Yes:
				return SaveParticleSystem();
			default:
				return true;
			}
		}

		return true;
	}

	void MainForm::UpdateTitleBar()
	{
		String^ file_name = _fileName;
		if(file_name == nullptr)
			file_name = "<Untitled>";
		this->Text = GetAppName() + " - " + file_name;
	}

	void MainForm::ToggleForm(WeifenLuo::WinFormsUI::Docking::DockContent^ form)
	{
		if(form->IsHidden)
			form->Show(_dockPanel);
		else
			form->Hide();
	}

}
