#include "StdAfx.h"
#include "Utility.h"
#include "MaterialForm.h"


using namespace math3d;
using namespace Memory;
using namespace Engine;
using namespace System::Collections::Generic;
using namespace Salamander::Windows::Forms;
using namespace EditorCommon;


namespace EntityEditor
{

	MaterialForm::MaterialForm(FormDirector^ director)
	{
		InitializeComponent();
		_entity = 0;
		_material = 0;
		_director = director;
		_copiedMaterial = 0;
	}

	void MaterialForm::SetEntity(Engine::ModelEntity* entity)
	{
		_entity = entity;
		UpdateMaterialList();
	}

	void MaterialForm::SaveAllMaterials()
	{
		for(int i = 0; i < _listMaterials->Items->Count; ++i)
		{
			SaveMaterial(_listMaterials->Items[i]);
		}
	}

	bool MaterialForm::AnyModified()
	{
		for(int i = 0; i < _listMaterials->Items->Count; ++i)
		{
			if((bool)_listMaterials->Items[i]->Tag)
				return true;
		}
		return false;
	}

	void MaterialForm::DeleteCopiedMaterial()
	{
		delete _copiedMaterial;
	}

	System::Void MaterialForm::MaterialForm_Load(System::Object^  sender, System::EventArgs^  e)
	{
		_copiedMaterial = new(mainPool) Material;
		UpdateControls();
	}

	void MaterialForm::UpdateControls()
	{
		// get a pointer to selected material
		bool selected = (_listMaterials->SelectedItems->Count > 0);
		_material = 0;
		if(selected && _entity)
		{
			char* mat_name = ConvertString<char>(_listMaterials->SelectedItems[0]->Text);
			const ModelEntity::MaterialMap& materials = _entity->GetMaterials();
			ModelEntity::MaterialMap::ConstIterator it = materials.Find(mat_name);
			delete[] mat_name;
			if(it != materials.End())
			{
				_material = const_cast<Material*>(it->materialRes->GetMaterial());
			}
		}

		if(_material)
		{
			EnableControls(true);

			const TextureRes* tex = _material->GetEmissionTexture();
			_textEmissionTex->Text = gcnew String(tex? tex->GetFileName(): _t(""));

			_clrDiffuse->BackColor = Color::FromArgb(PackColor(_material->GetDiffuseColor()));
			tex = _material->GetDiffuseTexture();
			_textDiffuseTex->Text = gcnew String(tex? tex->GetFileName(): _t(""));

			tex = _material->GetNormalMap();
			_textNormalMap->Text = gcnew String(tex? tex->GetFileName(): _t(""));

			_checkTransparency->Checked = _material->UsesTransparency();
			int opacity = int(_material->GetOpacity() * 100.0f);
			_trackOpacity->Value = opacity;
			_textOpacity->Text = opacity.ToString();

			tex = _material->GetTransparencyTexture();
			_textTransparencyMap->Text = gcnew String(tex? tex->GetFileName(): _t(""));
		}
		else
		{
			EnableControls(false);

			// clear controls

			_textEmissionTex->Text = "";

			_clrDiffuse->BackColor = Color::White;
			_textDiffuseTex->Text = "";

			_textNormalMap->Text = "";

			_checkTransparency->Checked = false;
			_trackOpacity->Value = 100;
			_textOpacity->Text = "100";
			_textTransparencyMap->Text = "";
		}
	}

	void MaterialForm::UpdateMaterialList()
	{
		if(!_entity)
			return;

		const ModelRes* model = _entity->GetModelRes();

		_listMaterials->Items->Clear();
		if(model && model->GetModel())
		{
			LinkedList<String^>^ names = gcnew LinkedList<String^>;
			const StaticArray<Mesh>& meshes = model->GetModel()->GetMeshes();
			for(size_t i = 0; i < meshes.GetCount(); ++i)
			{
				String^ n = gcnew String(meshes[i].material);
				if(!names->Contains(n))
				{
					names->AddLast(n);
					ListViewItem^ item = gcnew ListViewItem(n);
					item->SubItems->Add("");
					item->Tag = true;
					_listMaterials->Items->Add(item);
				}
			}

			const ModelEntity::MaterialMap& materials = _entity->GetMaterials();
			for(ModelEntity::MaterialMap::ConstIterator it = materials.Begin(); it != materials.End(); ++it)
			{
				const ModelEntity::MaterialData& md = *it;
				ListViewItem^ item = FindListItem(_listMaterials, gcnew String(md.name));
				if(item != nullptr)
				{
					const tchar* file_name = md.materialRes->GetFileName();
					bool modified = (!file_name || !*file_name);
					item->Tag = modified;
					String^ text = modified? "<Unsaved>": gcnew String(file_name);
					item->SubItems[1]->Text = text;
				}
			}
		}

		UpdateControls();
	}

	System::Void MaterialForm::_listMaterials_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		UpdateControls();
	}

	System::Void MaterialForm::_mnuFileNew_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_material || !PromptSave())
			return;

		// get file name from user and create a new resource
		_saveFileDialog->Title = "Create New Material";
		_saveFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Materials";
		if(_saveFileDialog->ShowDialog(this) == Forms::DialogResult::OK)
		{
			char* mat_name = ConvertString<char>(_listMaterials->SelectedItems[0]->Text);
			tchar* file_name = GetRelativePath(_saveFileDialog->FileName);
			Material mat;
			if(mat.Save(file_name))
			{
				// if material with this file name is already loaded, force it's reload
				const MaterialRes* matres = engineAPI->materialManager->FindMaterial(file_name);
				if(matres)
				{
					const_cast<Material*>(matres->GetMaterial())->Load(file_name);
				}

				if(_entity->SetMaterial(mat_name, file_name))
				{
					_listMaterials->SelectedItems[0]->SubItems[1]->Text = gcnew String(file_name);
					UpdateControls();
					MaterialChangedNotify(false);
				}
				else
				{
					Forms::MessageBox::Show(
						this, "Failed to set material named '\'" + _listMaterials->SelectedItems[0]->Text + "\'" +
						"\' to file \'" + _saveFileDialog->FileName + "\'.", GetAppName(),
						MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else
			{
				Forms::MessageBox::Show(
					this, "Failed to save material \'" + _saveFileDialog->FileName + "\'.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			delete[] mat_name;
			delete[] file_name;
		}
	}

	System::Void MaterialForm::_mnuFileOpen_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_material || !PromptSave())
			return;

		// get material file name from user and open it
		_openFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Materials";
		if(_openFileDialog->ShowDialog(this) == Forms::DialogResult::OK)
		{
			char* mat_name = ConvertString<char>(_listMaterials->SelectedItems[0]->Text);
			tchar* file_name = GetRelativePath(_openFileDialog->FileName);

			// if material with this file name is already loaded, force it's reload
			const MaterialRes* matres = engineAPI->materialManager->FindMaterial(file_name);
			if(matres)
			{
				const_cast<Material*>(matres->GetMaterial())->Load(file_name);
			}

			if(_entity->SetMaterial(mat_name, file_name))
			{
				engineAPI->textureManager->LoadAll(); // force this material's textures to load
				_listMaterials->SelectedItems[0]->SubItems[1]->Text = gcnew String(file_name);
				UpdateControls();
				MaterialChangedNotify(false);
			}
			else
			{
				Forms::MessageBox::Show(
					this, "Failed to set material named '\'" + _listMaterials->SelectedItems[0]->Text + "\'" +
					"\' to file \'" + _saveFileDialog->FileName + "\'.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			delete[] mat_name;
			delete[] file_name;
		}
	}

	System::Void MaterialForm::_mnuFileSave_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_material)
			return;

		SaveMaterial(_listMaterials->SelectedItems[0]);
	}

	System::Void MaterialForm::_mnuFileSaveAs_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(!_material)
			return;

		// save material to a different file
		_saveFileDialog->Title = "Save Material";
		_saveFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Materials";
		if(_saveFileDialog->ShowDialog(this) == Forms::DialogResult::OK)
		{
			char* mat_name = ConvertString<char>(_listMaterials->SelectedItems[0]->Text);
			tchar* file_name = GetRelativePath(_saveFileDialog->FileName);
			if(_material->Save(file_name))
			{
				if(_entity->SetMaterial(mat_name, file_name))
				{
					_listMaterials->SelectedItems[0]->SubItems[1]->Text = gcnew String(file_name);
					_listMaterials->SelectedItems[0]->Tag = false;
					UpdateControls();
				}
				else
				{
					Forms::MessageBox::Show(
						this, "Failed to set material named '\'" + _listMaterials->SelectedItems[0]->Text + "\'" +
						"\' to file \'" + _saveFileDialog->FileName + "\'.", GetAppName(),
						MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else
			{
				Forms::MessageBox::Show(
					this, "Failed to save material \'" + _saveFileDialog->FileName + "\'.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			delete[] file_name;
			delete[] mat_name;
		}
	}

	bool MaterialForm::SaveMaterial(ListViewItem^ item)
	{
		char* mat_name = ConvertString<char>(item->Text);
		const ModelEntity::MaterialMap& materials = _entity->GetMaterials();
		ModelEntity::MaterialMap::ConstIterator it = materials.Find(mat_name);
		delete[] mat_name;
		bool result = false;
		if(it != materials.End())
		{
			// save material to file
			const Material* mat = it->materialRes->GetMaterial();
			const tchar* file_name = it->materialRes->GetFileName();
			if(file_name && *file_name)
			{
				if(mat->Save(file_name))
				{
					item->Tag = false;
					result = true;
				}
				else
				{
					Forms::MessageBox::Show(
						this, "Failed to save material \'" + gcnew String(file_name) + "\'.", GetAppName(),
						MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
			}
			else
			{
				_saveFileDialog->Title = "Save Material";
				_saveFileDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Materials";
				if(_saveFileDialog->ShowDialog(this) == Forms::DialogResult::OK)
				{
					tchar* file_name = GetRelativePath(_saveFileDialog->FileName);
					if(_material->Save(file_name))
					{
						result = true;
						if(_entity->SetMaterial(it->name, file_name))
						{
							item->SubItems[1]->Text = gcnew String(file_name);
							item->Tag = false;
							UpdateControls();
						}
						else
						{
							Forms::MessageBox::Show(
								this, "Failed to set material named '\'" + _listMaterials->SelectedItems[0]->Text + "\'" +
								"\' to file \'" + _saveFileDialog->FileName + "\'.", GetAppName(),
								MessageBoxButtons::OK, MessageBoxIcon::Error);
						}
					}
					else
					{
						Forms::MessageBox::Show(
							this, "Failed to save material \'" + _saveFileDialog->FileName + "\'.", GetAppName(),
							MessageBoxButtons::OK, MessageBoxIcon::Error);
					}
					delete[] file_name;
				}
			}
		}

		return result;
	}

	bool MaterialForm::GetColorFromUser(vec3f& color)
	{
		_colorDialog->Color = Color::FromArgb(PackColor(color));
		if(_colorDialog->ShowDialog(this) == Forms::DialogResult::OK)
		{
			UnpackColor(_colorDialog->Color.ToArgb(), color);
			return true;
		}
		return false;
	}

	/*
		Notifies the main window that a material has been changed,
		and sets modified flag for current material
	*/
	void MaterialForm::MaterialChangedNotify(bool modified)
	{
		_director->FormNotify(this, NotifyMessage::MaterialChanged);
		_listMaterials->SelectedItems[0]->Tag = modified;
		if(modified)
			_entity->MaterialChanged(_material);
	}

	ListViewItem^ MaterialForm::FindListItem(ListView^ list_view, System::String^ text)
	{
		for(int i = 0; i < _listMaterials->Items->Count; ++i)
		{
			if(list_view->Items[i]->Text->Equals(text))
				return _listMaterials->Items[i];
		}

		return nullptr;
	}

	/*
		Prompts the user to save current material;
		retrns true if its' safe to proceed with intended action
	*/
	bool MaterialForm::PromptSave()
	{
		if(!_listMaterials->SelectedItems->Count ||
			!(bool)_listMaterials->SelectedItems[0]->Tag)
		{
			return true;
		}

		Forms::DialogResult result = Forms::MessageBox::Show(
			this, "Material \'" + _listMaterials->SelectedItems[0]->Text + "\' modified. Do you want to save changes?",
			GetAppName(), MessageBoxButtons::YesNoCancel, MessageBoxIcon::Question);

		switch(result)
		{
		case Forms::DialogResult::Cancel:
			return false;
		case Forms::DialogResult::Yes:
			return SaveMaterial(_listMaterials->SelectedItems[0]);
		default:
			return true;
		}
	}

	void MaterialForm::EnableControls(bool enable)
	{
		_mnuFileNew->Enabled = enable;
		_mnuFileOpen->Enabled = enable;
		_mnuFileSave->Enabled = enable;
		_mnuFileSaveAs->Enabled = enable;
		_mnuEditCopy->Enabled = enable;
		_mnuEditPaste->Enabled = enable;
		_toolBtnNew->Enabled = enable;
		_toolBtnOpen->Enabled = enable;
		_toolBtnSave->Enabled = enable;
		_toolBtnCopy->Enabled = enable;
		_toolBtnPaste->Enabled = enable;

		for each(Control^ c in _panelScroll->Controls)
			c->Enabled = enable;
	}

	System::Void MaterialForm::_mnuEditCopy_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
			*_copiedMaterial = *_material;
	}

	System::Void MaterialForm::_mnuEditPaste_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			*_material = *_copiedMaterial;
			UpdateControls();
			MaterialChangedNotify(true);
		}
	}

	System::Void MaterialForm::_clrDiffuse_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			vec3f color = _material->GetDiffuseColor();
			if(GetColorFromUser(color))
			{
				_material->SetDiffuseColor(color);
				_clrDiffuse->BackColor = Color::FromArgb(PackColor(color));
				MaterialChangedNotify(true);
			}
		}
	}

	System::Void MaterialForm::_btnBrowseDiffuseTex_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_selectTextureDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
			if(_selectTextureDialog->ShowDialog(this) == Forms::DialogResult::OK)
			{
				tchar* file_name = GetRelativePath(_selectTextureDialog->FileName);
				const TextureRes* tex = engineAPI->textureManager->CreateTexture(file_name, true);
				delete[] file_name;
				if(tex)
				{
					_material->SetDiffuseTexture(tex);
					_textDiffuseTex->Text = gcnew String(tex->GetFileName());
					MaterialChangedNotify(true);
				}
			}
		}
	}

	System::Void MaterialForm::_btnBrowseNormalMap_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_selectTextureDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
			if(_selectTextureDialog->ShowDialog(this) == Forms::DialogResult::OK)
			{
				tchar* file_name = GetRelativePath(_selectTextureDialog->FileName);
				const TextureRes* tex = engineAPI->textureManager->CreateTexture(file_name, true);
				delete[] file_name;
				if(tex)
				{
					_material->SetNormalMap(tex);
					_textNormalMap->Text = gcnew String(tex->GetFileName());
					MaterialChangedNotify(true);
				}
			}
		}
	}

	System::Void MaterialForm::_trackOpacity_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_textOpacity->ValueChanged -= gcnew System::EventHandler(this, &MaterialForm::_textOpacity_ValueChanged);
			_textOpacity->Text = _trackOpacity->Value.ToString();
			_textOpacity->ValueChanged += gcnew System::EventHandler(this, &MaterialForm::_textOpacity_ValueChanged);
			float val = float(_trackOpacity->Value) / 100.0f;
			_material->SetOpacity(val);
			MaterialChangedNotify(true);
		}
	}

	System::Void MaterialForm::_textOpacity_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_trackOpacity->Value = int(_textOpacity->Value);
			float val = Decimal::ToSingle(_textOpacity->Value) / 100.0f;
			_material->SetOpacity(val);
			MaterialChangedNotify(true);
		}
	}

	System::Void MaterialForm::fileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
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

	System::Void MaterialForm::_btnRemoveDiffuseTex_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_material->SetDiffuseTexture(0);
			_textDiffuseTex->Text = "";
			MaterialChangedNotify(true);
		}
	}

	System::Void MaterialForm::_btnRemoveNormalMap_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_material->SetNormalMap(0);
			_textNormalMap->Text = "";
			MaterialChangedNotify(true);
		}
	}

	System::Void MaterialForm::_btnBrowseEmissionTex_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_selectTextureDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
			if(_selectTextureDialog->ShowDialog(this) == Forms::DialogResult::OK)
			{
				tchar* file_name = GetRelativePath(_selectTextureDialog->FileName);
				const TextureRes* tex = engineAPI->textureManager->CreateTexture(file_name, true);
				delete[] file_name;
				if(tex)
				{
					_material->SetEmissionTexture(tex);
					_textEmissionTex->Text = gcnew String(tex->GetFileName());
					MaterialChangedNotify(true);
				}
			}
		}
	}

	System::Void MaterialForm::_btnRemoveEmissionTex_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_material->SetEmissionTexture(0);
			_textEmissionTex->Text = "";
			MaterialChangedNotify(true);
		}
	}

	System::Void MaterialForm::_checkTransparency_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_material->UseTransparency(_checkTransparency->Checked);
			MaterialChangedNotify(true);
		}
	}

	System::Void MaterialForm::_btnBrowseTransparencyMap_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_selectTextureDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
			if(_selectTextureDialog->ShowDialog(this) == Forms::DialogResult::OK)
			{
				tchar* file_name = GetRelativePath(_selectTextureDialog->FileName);
				const TextureRes* tex = engineAPI->textureManager->CreateTexture(file_name, true);
				delete[] file_name;
				if(tex)
				{
					_material->SetTransparencyTexture(tex);
					_textTransparencyMap->Text = gcnew String(tex->GetFileName());
					MaterialChangedNotify(true);
				}
			}
		}
	}

	System::Void MaterialForm::_btnRemoveTransparencyMap_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_material)
		{
			_material->SetTransparencyTexture(0);
			_textTransparencyMap->Text = "";
			MaterialChangedNotify(true);
		}
	}

}
