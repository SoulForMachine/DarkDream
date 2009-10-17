#include "StdAfx.h"
#include "Utility.h"
#include "LayersPanel.h"

using namespace math3d;
using namespace Engine;


namespace MapEditor
{

	ref class SpriteListBoxItem
	{
	public:
		SpriteListBoxItem(String^ text, BgLayer::Sprite* sprite)
		{
			_text = text;
			_sprite = sprite;
		}

		SpriteListBoxItem(SpriteListBoxItem^ item)
		{
			_text = item->_text;
			_sprite = item->_sprite;
		}

		virtual String^ ToString() override
			{ return _text; }
		BgLayer::Sprite* GetSprite()
			{ return _sprite; }

	private:
		String^ _text;
		BgLayer::Sprite* _sprite;
	};


	SpriteListBoxItem^ FindListItem(BgLayer::Sprite* sprite, ListBox^ list_box)
	{
		for each(SpriteListBoxItem^ item in list_box->Items)
		{
			if(item->GetSprite() == sprite)
				return item;
		}
		return nullptr;
	}

	LayersPanel::LayersPanel(EM_LayerEdit^ edit_mode)
	{
		InitializeComponent();

		_editMode = edit_mode;
		_btnLayer1->Checked = true;
		_checkTiledTex->Enabled = false;
		RefreshTextureList();
	}

	void LayersPanel::SelectSprite(BgLayer::Sprite* sprite)
	{
		_listTextures->SelectedIndexChanged -= gcnew System::EventHandler(this, &LayersPanel::_listTextures_SelectedIndexChanged);
		if(sprite)
		{
			SpriteListBoxItem^ item = FindListItem(sprite, _listTextures);
			_listTextures->SelectedItem = item;
			_checkTiledTex->Enabled = true;
			_checkTiledTex->Checked = (sprite->flags & BgLayer::Sprite::FLAG_TILED) != 0;
		}
		else
		{
			_listTextures->SelectedItem = nullptr;
			_checkTiledTex->Enabled = false;
		}
		_listTextures->SelectedIndexChanged += gcnew System::EventHandler(this, &LayersPanel::_listTextures_SelectedIndexChanged);
	}

	System::Void LayersPanel::_btnLayer1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(0);
		RefreshTextureList();
	}

	System::Void LayersPanel::_btnLayer2_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(1);
		RefreshTextureList();
	}

	System::Void LayersPanel::_btnLayer3_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(2);
		RefreshTextureList();
	}

	System::Void LayersPanel::_btnLayer4_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(3);
		RefreshTextureList();
	}

	System::Void LayersPanel::_listTextures_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
			_editMode->SelectSprite(item->GetSprite());
			_checkTiledTex->Enabled = true;
			_checkTiledTex->Checked = (item->GetSprite()->flags & BgLayer::Sprite::FLAG_TILED) != 0;
		}
		else
		{
			_editMode->SelectSprite(0);
			_checkTiledTex->Enabled = false;
			_checkTiledTex->Checked = false;
		}
	}

	System::Void LayersPanel::_btnAddTexture_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_selectTextureDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
		if(_selectTextureDialog->ShowDialog(this) == Windows::Forms::DialogResult::OK)
		{
			tchar* file_name = GetRelativePath(_selectTextureDialog->FileName);
			const TextureRes* tex_res = engineAPI->textureManager->CreateTexture(file_name, true);
			delete[] file_name;
			if(tex_res)
			{
				int layer_i = _editMode->GetActiveLayer();
				BgLayer& layer = engineAPI->world->GetLayerManager().GetLayer(layer_i);
				int viewport[4];
				engineAPI->renderSystem->GetRenderer()->GetViewport(viewport);
				vec2f pos;
				layer.PickLayerPoint(viewport[2] / 2, viewport[3], pos);
				GL::Texture2D* tex = (GL::Texture2D*)tex_res->GetTexture();
				float width = tex->GetWidth() / 4.0f;
				float height = tex->GetHeight() / 4.0f;
				RectFloat rect;
				rect.x1 = pos.x - width * 0.5f;
				rect.y1 = pos.y - height,
				rect.x2 = rect.x1 + width;
				rect.y2 = rect.y1 + height;
				BgLayer::Sprite ts = { tex_res, rect, layer_i, 0 };
				BgLayer::Sprite& sprite = layer.AddSprite(ts);
				AddSpriteToList(&sprite);
			}
		}
	}

	System::Void LayersPanel::_btnRemoveTexture_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			SpriteListBoxItem^ item =  (SpriteListBoxItem^)_listTextures->SelectedItem;
			engineAPI->world->GetLayerManager().GetLayer(_editMode->GetActiveLayer()).RemoveSprite(*item->GetSprite());
			_listTextures->Items->Remove(_listTextures->SelectedItem);
		}
	}

	System::Void LayersPanel::_btnBringToFront_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr && _listTextures->SelectedIndex < _listTextures->Items->Count - 1)
		{
			SpriteListBoxItem^ item =  (SpriteListBoxItem^)_listTextures->SelectedItem;
			engineAPI->world->GetLayerManager().GetLayer(_editMode->GetActiveLayer()).MoveToFront(*item->GetSprite());
			int index = _listTextures->SelectedIndex + 1;
			item = gcnew SpriteListBoxItem(item);
			_listTextures->Items->Remove(_listTextures->SelectedItem);
			_listTextures->Items->Insert(index, item);
			_listTextures->SelectedIndex = index;
		}
	}

	System::Void LayersPanel::_btnSendToBack_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedIndex > 0)
		{
			SpriteListBoxItem^ item =  (SpriteListBoxItem^)_listTextures->SelectedItem;
			engineAPI->world->GetLayerManager().GetLayer(_editMode->GetActiveLayer()).MoveToBack(*item->GetSprite());
			int index = _listTextures->SelectedIndex - 1;
			item = gcnew SpriteListBoxItem(item);
			_listTextures->Items->Remove(_listTextures->SelectedItem);
			_listTextures->Items->Insert(index, item);
			_listTextures->SelectedIndex = index;
		}
	}

	System::Void LayersPanel::_checkTiledTex_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			SpriteListBoxItem^ item =  (SpriteListBoxItem^)_listTextures->SelectedItem;
			if(_checkTiledTex->Checked)
				item->GetSprite()->flags |= BgLayer::Sprite::FLAG_TILED;
			else
				item->GetSprite()->flags &= ~BgLayer::Sprite::FLAG_TILED;
		}
	}

	System::Void LayersPanel::_selectTextureDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
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

	System::Void LayersPanel::_trackScrollFactor_Scroll(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void LayersPanel::_textScrollFactor_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void LayersPanel::_trackScale_Scroll(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void LayersPanel::_textScale_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{

	}

	void LayersPanel::RefreshTextureList()
	{
		_listTextures->Items->Clear();
		BgLayer& layer = engineAPI->world->GetLayerManager().GetLayer(_editMode->GetActiveLayer());
		List<BgLayer::Sprite>& sprites = layer.GetSprites();
		_listTextures->BeginUpdate();
		for(List<BgLayer::Sprite>::Iterator it = sprites.Begin(); it != sprites.End(); ++it)
		{
			AddSpriteToList(&(*it));
		}
		_listTextures->EndUpdate();
	}

	void LayersPanel::AddSpriteToList(BgLayer::Sprite* sprite)
	{
		String^ text = gcnew String(sprite->texture->GetFileName());
		int dot = text->LastIndexOf('.');
		if(dot == -1)
			dot = 0;
		int sl = text->LastIndexOf('\\');
		if(sl == -1)
		{
			sl = text->LastIndexOf('/');
			if(sl == -1)
				sl = text->Length;
		}
		text = text->Substring(sl + 1, dot - sl - 1);
		String^ base_text = gcnew String(text);
		int n = 2;
		while(_listTextures->FindStringExact(text) != ListBox::NoMatches)
		{
			text = base_text + n.ToString();
			++n;
		}

		_listTextures->Items->Add(gcnew SpriteListBoxItem(text, sprite));
	}

}
