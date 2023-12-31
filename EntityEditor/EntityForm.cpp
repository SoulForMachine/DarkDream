#include "StdAfx.h"
#include "Entity.h"
#include "AddAnimForm.h"
#include "EditorCommon/UtilityTempl.h"
#include "EntityForm.h"


using namespace Engine;
using namespace EditorCommon;


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
		_btnTPose->Enabled = false;
		_btnAddSound->Enabled = false;
		_btnRemoveSound->Enabled = false;
		_btnPlaySound->Enabled = false;
	}

	void EntityForm::UpdateControls()
	{
		// model
		String^ model_file = "";
		ModelResPtr model = ModelResPtr::null;

		if(_entity)
		{
			model = _entity->GetModelRes();
			if(model)
			{
				const tchar* file_name = model.GetRes()->GetFileName();
				if(file_name)
					model_file = gcnew String(file_name);
			}
		}
		_textModel->Text = model_file;

		// skeleton
		_treeSkelet->BeginUpdate();
		_treeSkelet->Nodes->Clear();
		bool model_loaded = _entity && model.IsValid();
		if(model_loaded && model->GetRootJoint())
		{
			InsertJoint(_treeSkelet->Nodes, model->GetRootJoint());
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
		_treeSkelet->EndUpdate();

		_textJointAttachment->Text = "";
		_btnBrowseAttachment->Enabled = false;
		_btnRemoveAttachment->Enabled = false;

		// animations
		bool has_skelet = model_loaded && model->HasSkelet();
		_btnAddAnim->Enabled = has_skelet;
		_btnRemoveAnim->Enabled = false;
		_btnPlayAnim->Enabled = false;
		_btnTPose->Enabled = has_skelet;
		_listAnimations->Items->Clear();
		if(has_skelet)
		{
			const ModelEntity::AnimMap& anims = _entity->GetAnimations();
			for(ModelEntity::AnimMap::ConstIterator it = anims.Begin(); it != anims.End(); ++it)
			{
				const ModelEntity::AnimData& ad = *it;
				ListViewItem^ item = gcnew ListViewItem(gcnew String(ad.name));
				item->SubItems->Add(gcnew String(ad.animation.GetRes()->GetFileName()));
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
				item->SubItems->Add(gcnew String(sd.sound.GetRes()->GetFileName()));
				_listSounds->Items->Add(item);
			}
		}
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

		if(joint->sibling)
			InsertJoint(nodes, joint->sibling);
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
			tchar* file_name = EditorUtil::GetRelativePath(_openFileDialog->FileName);
			if(_entity->SetModel(file_name))
			{
				UpdateControls();
				_director->FormNotify(this, NotifyMessage::ModelChanged, nullptr);
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
				tchar* file_name = EditorUtil::GetRelativePath(_openFileDialog->FileName);
				char* name = ConvertString<char>(node->Text);
				if(_entity->SetJointAttachment(name, file_name))
				{
					// load resources for attached entity
					engineAPI->materialManager->LoadAll();
					engineAPI->textureManager->LoadAll();
					engineAPI->modelManager->LoadAll();
					engineAPI->animationManager->LoadAll();

					// initialize the data for model entity attachment
					ModelEntity::JointAttachMap::ConstIterator att_ent = _entity->GetJointAttachments().Find(name);
					if(att_ent != _entity->GetJointAttachments().End())
					{
						if(att_ent->type == ModelEntity::JOINT_ATTACH_MODEL)
						{
							ModelEntity* ent = (ModelEntity*) ((ModelEntityRes*)att_ent->attachment)->GetResource();
							ent->SetupModelData();
						}
					}

					_textJointAttachment->Text = gcnew String(file_name);
					node->ImageIndex = JOINT_ATT_IMAGE_INDEX;
					node->SelectedImageIndex = JOINT_ATT_IMAGE_INDEX;
					_btnRemoveAttachment->Enabled = true;
					_director->FormNotify(this, NotifyMessage::AttachmentChanged, nullptr);
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
				_director->FormNotify(this, NotifyMessage::AttachmentChanged, nullptr);
			}
			delete[] name;
		}
	}

	System::Void EntityForm::_btnPlayAnim_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_entity->IsAnimationPlaying())
		{
			// pause animation
			_entity->PauseAnimation();
			_btnPlayAnim->ImageIndex = 2;
		}
		else
		{
			// play selected
			if(!_listAnimations->SelectedItems->Count)
				return;

			ListViewItem^ item = _listAnimations->SelectedItems[0];
			char* name = ConvertString<char>(item->Text);
			const ModelEntity::AnimData* anim_data = _entity->GetCurrentAnimation();
			if(anim_data && !strcmp(name, anim_data->name))
			{
				_entity->PlayAnimation();
				_btnPlayAnim->ImageIndex = 3;
			}
			else
			{
				if(_entity->SetActiveAnimation(name))
					_btnPlayAnim->ImageIndex = 3;
			}
			delete[] name;
		}
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

				_director->FormNotify(this, NotifyMessage::AnimationChanged, nullptr);
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
		array<ListViewItem^>^ selected = gcnew array<ListViewItem^>(_listAnimations->SelectedItems->Count);
		_listAnimations->SelectedItems->CopyTo(selected, 0);
		for each(ListViewItem^ item in selected)
		{
			char* name = ConvertString<char>(item->Text);
			const ModelEntity::AnimData* anim_data = _entity->GetCurrentAnimation();
			if(anim_data && !strcmp(name, anim_data->name))
				_btnPlayAnim->ImageIndex = 2;
			_entity->RemoveAnimation(name);
			delete[] name;
			_listAnimations->Items->Remove(item);
		}
		_btnRemoveAnim->Enabled = false;
		_director->FormNotify(this, NotifyMessage::AnimationChanged, nullptr);
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

	System::Void EntityForm::_openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		// the file must be within game's base directory
		if(!EditorUtil::IsInGameBaseDir(_openFileDialog->FileName))
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
			assert(_entity && _entity->GetModelRes().IsValid());
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

	System::Void EntityForm::_btnTPose_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_entity->SetActiveAnimation("");
		_btnPlayAnim->ImageIndex = 2;
	}

}
