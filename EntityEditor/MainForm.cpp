
#include "stdafx.h"
#include "Utility.h"
#include "ModelForm.h"
#include "EntityForm.h"
#include "PropertyForm.h"
#include "MainForm.h"

#define DOCK_PANEL_XML_FILE		"EntityEditorDocking.xml"

using namespace EditorCommon;


namespace EntityEditor
{

	MainForm::MainForm()
	{
		InitializeComponent();

		_deserializeDockContent = gcnew WeifenLuo::WinFormsUI::Docking::DeserializeDockContent(this, &MainForm::GetContentFromPersistString);
		_modelForm = gcnew ModelForm;
		_consoleForm = gcnew ConsoleForm(this);
		_entityForm = gcnew EntityForm(this);
		_propertyForm = gcnew PropertyForm(this);

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
		if(!_entityForm->IsHandleCreated)
			_entityForm->Show(_dockPanel);
		if(!_propertyForm->IsHandleCreated)
			_propertyForm->Show(_dockPanel);

		_entityForm->Activate();

		_modelForm->Show(_dockPanel);
		_modelForm->Focus();
		_dockPanel->ResumeLayout(true, true);

		_animate = true;
		_modelForm->Animate(true);
		_wireframe = false;
		_modelStats = true;
		_modelForm->ShowStats(true);
		_entity = nullptr;
		NewEntity();
		UpdateTitleBar();

		Application::Idle += gcnew EventHandler(this, &MainForm::OnIdle);
	}

	void MainForm::FormNotify(Form^ form, NotifyMessage msg)
	{
		if(form == _entityForm)
		{
			switch(msg)
			{
			case NotifyMessage::ModelChanged:
				_consoleForm->RedrawAsync();
				_modelForm->ModelChanged();
				_modelForm->RedrawAsync();
				_entity->SetModified(true);
				break;
			case NotifyMessage::AttachmentChanged:
				_consoleForm->RedrawAsync();
				_modelForm->ModelChanged();
				_modelForm->RedrawAsync();
				_entity->SetModified(true);
				break;
			}
		}
	}

	System::Void MainForm::MainForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
		/*HashMapTest^ test = gcnew HashMapTest;
		test->Show(this);
		test->Activate();*/
	}

	System::Void MainForm::MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
	{
		if(PromptSave())
		{
			_entity->Unload();
			delete _entity;
			// save layout for dock panels to xml
			_dockPanel->SaveAsXml(Application::StartupPath + "\\" + DOCK_PANEL_XML_FILE);
		}
		else
		{
			e->Cancel = true;
		}
	}

	WeifenLuo::WinFormsUI::Docking::IDockContent^ MainForm::GetContentFromPersistString(String^ persistString)
	{
		if(persistString == _consoleForm->GetType()->ToString())
		{
			return _consoleForm;
		}
		else if(persistString == _entityForm->GetType()->ToString())
		{
			return _entityForm;
		}
		else if(persistString == _propertyForm->GetType()->ToString())
		{
			return _propertyForm;
		}
		else
		{
			return nullptr;
		}
	}

	void MainForm::ToggleForm(WeifenLuo::WinFormsUI::Docking::DockContent^ form)
	{
		if(form->IsHidden)
			form->Show(_dockPanel);
		else
			form->Hide();
	}

	System::Void MainForm::_mnuViewConsole_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ToggleForm(_consoleForm);
	}

	System::Void MainForm::_mnuModelStats_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_modelStats = !_modelStats;
		_modelForm->ShowStats(_modelStats);
	}

	System::Void MainForm::_mnuViewEntityPanel_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ToggleForm(_entityForm);
	}

	System::Void MainForm::_mnuViewPropertyPanel_Click(System::Object^  sender, System::EventArgs^  e)
	{
		ToggleForm(_propertyForm);
	}

	System::Void MainForm::_mnuView_DropDownOpening(System::Object^  sender, System::EventArgs^  e)
	{
		
	}

	System::Void MainForm::_mnuAnimate_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_animate = !_animate;
		_modelForm->Animate(_animate);
	}

	System::Void MainForm::_mnuWireframe_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_wireframe = !_wireframe;
		_modelForm->Wireframe(_wireframe);
	}

	System::Void MainForm::_mnuEntityNew_Click(System::Object^  sender, System::EventArgs^  e)
	{
		NewEntity();
		UpdateTitleBar();
	}

	System::Void MainForm::_mnuEntityOpen_Click(System::Object^  sender, System::EventArgs^  e)
	{
		LoadEntity();
		UpdateTitleBar();
	}

	System::Void MainForm::_mnuEntitySave_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SaveEntity();
		UpdateTitleBar();
	}

	System::Void MainForm::_mnuEntitySaveAs_Click(System::Object^  sender, System::EventArgs^  e)
	{
		SaveEntityAs();
		UpdateTitleBar();
	}

	System::Void MainForm::_mnuEntityExit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		Application::Exit();
	}

	void MainForm::NewEntity()
	{
		if(PromptSave())
		{
			if(_entity != nullptr)
			{
				_entity->Unload();
				delete _entity;
			}
			_entity = gcnew Entity;
			_entityForm->SetEntity(_entity->GetEntity());
			_propertyForm->SetProperties(_entity->GetProperties());
			_modelForm->SetEntity(_entity->GetEntity());
			_consoleForm->RedrawAsync();
		}
	}

	bool MainForm::LoadEntity()
	{
		if(PromptSave())
		{
			_openEntityDlg->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Entities";
			Forms::DialogResult dlg_result = _openEntityDlg->ShowDialog(this);
			if(dlg_result == Forms::DialogResult::OK)
			{
				_entity->Unload();
				bool result = _entity->Load(_openEntityDlg->FileName);
				_entityForm->SetEntity(_entity->GetEntity());
				_propertyForm->SetProperties(_entity->GetProperties());
				_modelForm->SetEntity(_entity->GetEntity());
				_consoleForm->RedrawAsync();
				if(!result)
				{
					Forms::MessageBox::Show(
						this, "Failed to open " + _openEntityDlg->FileName, GetAppName(),
						MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				return result;
			}
		}
		return false;
	}

	bool MainForm::SaveEntity()
	{
		if(_entity->IsUntitled())
			return SaveEntityAs();
		else
			return SaveEntity(_entity->GetFileName());
	}

	bool MainForm::SaveEntityAs()
	{
		_saveEntityDlg->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Entities";
		Forms::DialogResult dlg_result = _saveEntityDlg->ShowDialog(this);
		if(dlg_result != Forms::DialogResult::OK)
			return false;

		return SaveEntity(_saveEntityDlg->FileName);
	}

	bool MainForm::SaveEntity(String^ file_name)
	{
		bool result = _entity->Save(file_name);
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
		if(_entity != nullptr && _entity->IsModified())
		{
			Forms::DialogResult result = MessageBox::Show(
				this, "Entity modified. Do you want to save changes?", GetAppName(),
				MessageBoxButtons::YesNoCancel, MessageBoxIcon::Warning);

			switch(result)
			{
			case Forms::DialogResult::Cancel:
				return false;
			case Forms::DialogResult::Yes:
				return SaveEntity();
			default:
				return true;
			}
		}

		return true;
	}

	void MainForm::UpdateTitleBar()
	{
		String^ file_name = _entity->GetFileName();
		if(file_name == nullptr)
			file_name = "<Untitled>";
		this->Text = GetAppName() + " - " + file_name;
	}

	void MainForm::OnIdle(System::Object^  sender, System::EventArgs^  e)
	{
		_mnuViewConsole->Checked = !_consoleForm->IsHidden;
		_mnuViewEntityPanel->Checked = !_entityForm->IsHidden;
		_mnuViewPropertyPanel->Checked = !_propertyForm->IsHidden;
		_mnuAnimate->Checked = _animate;
		_mnuWireframe->Checked = _wireframe;
		_mnuModelStats->Checked = _modelStats;
		_toolBtnConsole->Checked = !_consoleForm->IsHidden;

		if(_animate)
			_modelForm->RedrawAsync();
	}

	System::Void MainForm::fileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		// the file must be within game's base directory
		FileDialog^ dlg = (FileDialog^)sender;
		if(!IsInGameBaseDir(dlg->FileName))
		{
			MessageBox::Show(
				this, "File must be within game's base directory.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			e->Cancel = true;
		}
	}

}