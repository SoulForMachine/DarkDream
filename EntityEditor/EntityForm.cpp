#include "StdAfx.h"
#include "Utility.h"
#include "Entity.h"
#include "AddAnimForm.h"
#include "EntityForm.h"


using namespace EditorCommon;
using namespace Engine;


namespace EntityEditor
{

	EntityForm::EntityForm(FormDirector^ director)
	{
		InitializeComponent();
		_entity = 0;
		_director = director;
		_btnAddAnim->Enabled = false;
		_btnRemoveAnim->Enabled = false;
		_btnPlayAnim->Enabled = false;
		_btnAddSound->Enabled = false;
		_btnRemoveSound->Enabled = false;
		_btnPlaySound->Enabled = false;
	}

	void EntityForm::UpdateControls()
	{
		// model
		String^ model_file = "";
		const ModelRes* model = 0;

		if(_entity)
		{
			model = _entity->GetModelRes();
			if(model)
			{
				const tchar* file_name = model->GetFileName();
				if(file_name)
					model_file = gcnew String(file_name);
			}
		}
		_textModel->Text = model_file;

		// skeleton
		_treeSkelet->Nodes->Clear();
		bool model_loaded = _entity && model && model->GetModel();
		if(model_loaded && model->GetModel()->GetRootJoint())
		{
			InsertJoint(_treeSkelet->Nodes, model->GetModel()->GetRootJoint());
			if(_treeSkelet->Nodes->Count)
			{
				_treeSkelet->ExpandAll();
				_treeSkelet->Nodes[0]->EnsureVisible();
			}
		}
		else
		{
			TreeNode^ node = gcnew TreeNode("< No skelet >");
			node->ImageIndex = JOINT_NONE_IMAGE_INDEX;
			node->SelectedImageIndex = JOINT_NONE_IMAGE_INDEX;
			_treeSkelet->Nodes->Add(node);
		}

		_textJointAttachment->Text = "";
		_btnBrowseAttachment->Enabled = false;
		_btnRemoveAttachment->Enabled = false;

		// animations
		bool has_skelet = model_loaded && model->GetModel()->HasSkelet();
		_btnAddAnim->Enabled = has_skelet;
		_btnRemoveAnim->Enabled = false;
		_btnPlayAnim->Enabled = false;
		_listAnimations->Items->Clear();
		if(has_skelet)
		{
			const ModelEntity::AnimMap& anims = _entity->GetAnimations();
			for(ModelEntity::AnimMap::ConstIterator it = anims.Begin(); it != anims.End(); ++it)
			{
				const ModelEntity::AnimData& ad = *it;
				ListViewItem^ item = gcnew ListViewItem(gcnew String(ad.name));
				item->SubItems->Add(gcnew String(ad.animation->GetFileName()));
				_listAnimations->Items->Add(item);
			}
		}

		// sounds
		_listSounds->Items->Clear();
		if(_entity)
		{
			const ModelEntity::SoundMap& sounds = _entity->GetSounds();
			for(ModelEntity::SoundMap::ConstIterator it = sounds.Begin(); it != sounds.End(); ++it)
			{
				const ModelEntity::SoundData& sd = *it;
				ListViewItem^ item = gcnew ListViewItem(gcnew String(sd.name));
				item->SubItems->Add(gcnew String(sd.sound->GetFileName()));
				_listSounds->Items->Add(item);
			}
		}

		// ai script
		String^ ai_path = "";
		if(_entity)
		{
			const AIScriptRes* script = _entity->GetAIScriptRes();
			if(script)
				ai_path = gcnew String(script->GetFileName());
		}
		_textAIScript->Text = ai_path;
	}

	void EntityForm::InsertJoint(TreeNodeCollection^ nodes, const Engine::Joint* joint)
	{
		const ModelEntity::JointAttachMap& att = _entity->GetJointAttachments();
		int img_index;
		TreeNode^ n = gcnew TreeNode(gcnew String(joint->name));
		img_index = (att.Find(joint->name) != att.End())? JOINT_ATT_IMAGE_INDEX: JOINT_IMAGE_INDEX;
		n->ImageIndex = img_index;
		n->SelectedImageIndex = img_index;
		nodes->Add(n);
		if(joint->child)
			InsertJoint(n->Nodes, joint->child);
		Joint* ptr = joint->sibling;
		while(ptr)
		{
			TreeNode^ n = gcnew TreeNode(gcnew String(ptr->name));
			img_index = (att.Find(joint->name) != att.End())? JOINT_ATT_IMAGE_INDEX: JOINT_IMAGE_INDEX;
			n->ImageIndex = img_index;
			n->SelectedImageIndex = img_index;
			nodes->Add(n);
			if(ptr->child)
				InsertJoint(n->Nodes, ptr->child);
			ptr = ptr->sibling;
		}
	}

	System::Void EntityForm::_btnBrowseModel_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_openFileDialog->Reset();
		_openFileDialog->Title = "Open Model";
		_openFileDialog->DefaultExt = "model";
		_openFileDialog->Filter = "Model Files (*.model)|*.model|All Files (*.*)|*.*";
		_openFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Models";
		if(_openFileDialog->ShowDialog(this) == Forms::DialogResult::OK)
		{
			tchar* file_name = GetRelativePath(_openFileDialog->FileName);
			if(_entity->SetModel(file_name))
			{
				UpdateControls();
				_director->FormNotify(this, NotifyMessage::ModelChanged);
			}
			else
			{
				Forms::MessageBox::Show(
					this, "Failed to load " + _openFileDialog->FileName, GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			delete[] file_name;
		}
	}

	System::Void EntityForm::_btnBrowseAttachment_Click(System::Object^  sender, System::EventArgs^  e)
	{
		TreeNode^ node = _treeSkelet->SelectedNode;
		if(node != nullptr)
		{
			_openFileDialog->Reset();
			_openFileDialog->Title = "Select Attachment (Entity or Particle System)";
			_openFileDialog->DefaultExt = "";
			_openFileDialog->Filter = "Entities and particle systems (*.entity;*.part)|*.entity;*.part|All Files (*.*)|*.*";
			_openFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath());
			if(_openFileDialog->ShowDialog(this) == Forms::DialogResult::OK)
			{
				tchar* file_name = GetRelativePath(_openFileDialog->FileName);
				char* name = ConvertString<char>(node->Text);
				if(_entity->SetJointAttachment(name, file_name))
				{
					_textJointAttachment->Text = gcnew String(file_name);
					node->ImageIndex = JOINT_ATT_IMAGE_INDEX;
					node->SelectedImageIndex = JOINT_ATT_IMAGE_INDEX;
					_btnRemoveAttachment->Enabled = true;
					_director->FormNotify(this, NotifyMessage::AttachmentChanged);
				}
				else
				{
					Forms::MessageBox::Show(
						this, "Failed to load " + _openFileDialog->FileName, GetAppName(),
						MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				delete[] file_name;
				delete[] name;
			}
		}
	}

	System::Void EntityForm::_btnRemoveAttachment_Click(System::Object^  sender, System::EventArgs^  e)
	{
		TreeNode^ node = _treeSkelet->SelectedNode;
		if(node != nullptr)
		{
			char* name = ConvertString<char>(node->Text);
			if(_entity->RemoveJointAttachment(name))
			{
				_textJointAttachment->Text = "";
				node->ImageIndex = JOINT_IMAGE_INDEX;
				node->SelectedImageIndex = JOINT_IMAGE_INDEX;
				_btnRemoveAttachment->Enabled = false;
				_director->FormNotify(this, NotifyMessage::AttachmentChanged);
			}
			delete[] name;
		}
	}

	System::Void EntityForm::_btnPlayAnim_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_listAnimations->SelectedItems->Count)
			return;

		ListViewItem^ item = _listAnimations->SelectedItems[0];
		char* name = ConvertString<char>(item->Text);
		_entity->ActiveAnimation(name);
		delete[] name;
	}

	System::Void EntityForm::_btnAddAnim_Click(System::Object^  sender, System::EventArgs^  e)
	{
		AddAnimForm^ form = gcnew AddAnimForm;
		if(form->ShowDialog(this) == Forms::DialogResult::OK)
		{
			char* name = ConvertString<char>(form->AnimName);
			tchar* file_name = ConvertString<tchar>(form->FileName);
			if(_entity->AddAnimation(name, file_name))
			{
				ListViewItem^ item = nullptr;
				for each(ListViewItem^ it in _listAnimations->Items)
				{
					if(it->Text == form->AnimName)
					{
						item = it;
						break;
					}
				}

				if(item != nullptr)
					item->SubItems[1]->Text = form->FileName;
				else
				{
					item = gcnew ListViewItem(form->AnimName);
					item->SubItems->Add(form->FileName);
					_listAnimations->Items->Add(item);
				}
			}
			else
			{
				Forms::MessageBox::Show(
					this, "Failed to add animation.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			delete[] name;
			delete[] file_name;
		}
	}

	System::Void EntityForm::_btnRemoveAnim_Click(System::Object^  sender, System::EventArgs^  e)
	{
		for each(ListViewItem^ item in  _listAnimations->SelectedItems)
		{
			char* name = ConvertString<char>(item->Text);
			_entity->RemoveAnimation(name);
			delete[] name;
			
			_listAnimations->Items->Remove(item);
		}
		_btnRemoveAnim->Enabled = false;
	}

	System::Void EntityForm::_btnPlaySound_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}

	System::Void EntityForm::_btnAddSound_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}

	System::Void EntityForm::_btnRemoveSound_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}

	System::Void EntityForm::_btnBrowseAIScript_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}

	System::Void EntityForm::_openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		// the file must be within game's base directory
		if(!IsInGameBaseDir(_openFileDialog->FileName))
		{
			MessageBox::Show(
				this, "File must be within game's base directory.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			e->Cancel = true;
		}
	}

	System::Void EntityForm::_treeSkelet_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
	{
		if(e->Node != nullptr && e->Node->Text != "< No skelet >")
		{
			assert(_entity && _entity->GetModelRes() && _entity->GetModelRes()->GetModel());
			char* name = ConvertString<char>(e->Node->Text);
			const ModelEntity::JointAttachMap& att = _entity->GetJointAttachments();
			ModelEntity::JointAttachMap::ConstIterator it = att.Find(name);
			_textJointAttachment->Text = (it != att.End())? gcnew String(it->attachment->GetFileName()): "";
			delete[] name;
			_btnBrowseAttachment->Enabled = true;
			_btnRemoveAttachment->Enabled = (_textJointAttachment->Text != "");
		}
		else
		{
			_btnBrowseAttachment->Enabled = false;
			_btnRemoveAttachment->Enabled = false;
			_textJointAttachment->Text = "";
		}
	}

	System::Void EntityForm::_listAnimations_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_btnRemoveAnim->Enabled = (_listAnimations->SelectedItems->Count > 0);
		_btnPlayAnim->Enabled = (_listAnimations->SelectedItems->Count == 1);
	}

	System::Void EntityForm::_listSounds_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_btnRemoveSound->Enabled = (_listSounds->SelectedItems->Count > 0);
		_btnPlaySound->Enabled = (_listSounds->SelectedItems->Count == 1);
	}

	System::Void EntityForm::_mnuViewExpandAll_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_panelEntity->ExpandAll();
	}

	System::Void EntityForm::_mnuViewCollapseAll_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_panelEntity->CollapseAll();
	}

	System::Void EntityForm::_btnExpandAllSkelet_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_treeSkelet->ExpandAll();
	}

	System::Void EntityForm::_btnCollapseAllSkelet_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_treeSkelet->CollapseAll();
	}

}
