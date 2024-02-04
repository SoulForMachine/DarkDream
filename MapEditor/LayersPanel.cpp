#include "StdAfx.h"
#include "EditorCommon/UtilityTempl.h"
#include "LayersPanel.h"

using namespace math3d;
using namespace Engine;
using namespace EditorCommon;


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
		RefreshTextureList();
		UpdateScrollFactorControls();
		UpdateScaleControls();
		UpdateControlState();
	}

	void LayersPanel::SelectSprite(BgLayer::Sprite* sprite)
	{
		_listTextures->SelectedIndexChanged -= gcnew System::EventHandler(this, &LayersPanel::_listTextures_SelectedIndexChanged);
		if(sprite)
		{
			SpriteListBoxItem^ item = FindListItem(sprite, _listTextures);
			_listTextures->SelectedItem = item;
		}
		else
		{
			_listTextures->SelectedItem = nullptr;
		}
		_listTextures->SelectedIndexChanged += gcnew System::EventHandler(this, &LayersPanel::_listTextures_SelectedIndexChanged);
		UpdateScaleControls();
		UpdateControlState();
	}

	System::Void LayersPanel::_btnLayer1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(0);
		RefreshTextureList();
		UpdateScrollFactorControls();
		UpdateControlState();
	}

	System::Void LayersPanel::_btnLayer2_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(1);
		RefreshTextureList();
		UpdateScrollFactorControls();
		UpdateControlState();
	}

	System::Void LayersPanel::_btnLayer3_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(2);
		RefreshTextureList();
		UpdateScrollFactorControls();
		UpdateControlState();
	}

	System::Void LayersPanel::_btnLayer4_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		_editMode->SetActiveLayer(3);
		RefreshTextureList();
		UpdateScrollFactorControls();
		UpdateControlState();
	}

	System::Void LayersPanel::_listTextures_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
			_editMode->SelectSprite(item->GetSprite());
		}
		else
		{
			_editMode->SelectSprite(0);
		}

		UpdateScaleControls();
		UpdateControlState();
	}

	System::Void LayersPanel::_btnAddTexture_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_selectTextureDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
		if(_selectTextureDialog->ShowDialog(this) == Windows::Forms::DialogResult::OK)
		{
			tchar* file_name = EditorUtil::GetRelativePath(_selectTextureDialog->FileName);
			Texture2DResPtr tex_res = engineAPI->textureManager->CreateTexture2D(file_name, true);
			Memory::Delete(file_name);
			if(tex_res)
			{
				int layer_i = _editMode->GetActiveLayer();
				BgLayer& layer = engineAPI->world->GetLayerManager().GetLayer(layer_i);
				
				BgLayer::Sprite ts;
				ts.texture = tex_res;
				ts.layerIndex = layer_i;
				ts.flags = 0;
				ts.uvScale.set(1.0f, 1.0f);
				SetInitialSpriteRect(&ts);
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

	System::Void LayersPanel::_checkTileTexU_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			SpriteListBoxItem^ item =  (SpriteListBoxItem^)_listTextures->SelectedItem;
			if(_checkTileTexU->Checked)
				item->GetSprite()->flags |= BgLayer::Sprite::FLAG_TILE_U;
			else
				item->GetSprite()->flags &= ~BgLayer::Sprite::FLAG_TILE_U;
			SetInitialSpriteRect(item->GetSprite());

			if(	(item->GetSprite()->flags & BgLayer::Sprite::FLAG_TILE_U) &&
				(item->GetSprite()->flags & BgLayer::Sprite::FLAG_TILE_V))
			{
				_textTile->Enabled = true;
				_trackTile->Enabled = true;
				_textScale->Enabled = false;
				_trackScale->Enabled = false;
				SetSpriteScale(item->GetSprite(), Decimal::ToSingle(_textTile->Value));
			}
			else
			{
				_textTile->Enabled = false;
				_trackTile->Enabled = false;
				_textScale->Enabled = true;
				_trackScale->Enabled = true;
				SetSpriteScale(item->GetSprite(), Decimal::ToSingle(_textScale->Value));
			}
		}
	}

	System::Void LayersPanel::_checkTileTexV_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			SpriteListBoxItem^ item =  (SpriteListBoxItem^)_listTextures->SelectedItem;
			if(_checkTileTexV->Checked)
				item->GetSprite()->flags |= BgLayer::Sprite::FLAG_TILE_V;
			else
				item->GetSprite()->flags &= ~BgLayer::Sprite::FLAG_TILE_V;
			SetInitialSpriteRect(item->GetSprite());

			if(	(item->GetSprite()->flags & BgLayer::Sprite::FLAG_TILE_U) &&
				(item->GetSprite()->flags & BgLayer::Sprite::FLAG_TILE_V))
			{
				_textTile->Enabled = true;
				_trackTile->Enabled = true;
				_textScale->Enabled = false;
				_trackScale->Enabled = false;
				SetSpriteScale(item->GetSprite(), Decimal::ToSingle(_textTile->Value));
			}
			else
			{
				_textTile->Enabled = false;
				_trackTile->Enabled = false;
				_textScale->Enabled = true;
				_trackScale->Enabled = true;
				SetSpriteScale(item->GetSprite(), Decimal::ToSingle(_textScale->Value));
			}
		}
	}

	System::Void LayersPanel::_selectTextureDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		// the file must be within game's base directory
		FileDialog^ dlg = (FileDialog^)sender;
		if(!EditorUtil::IsInGameBaseDir(dlg->FileName))
		{
			MessageBox::Show(
				this, "File must be within game's base directory.", GetAppName(),
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			e->Cancel = true;
		}
	}

	System::Void LayersPanel::_trackScrollFactor_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = _trackScrollFactor->Value / 100.0f;
		_textScrollFactor->ValueChanged -= gcnew System::EventHandler(this, &LayersPanel::_textScrollFactor_ValueChanged);
		_textScrollFactor->Text = val.ToString();
		_textScrollFactor->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textScrollFactor_ValueChanged);
		engineAPI->world->GetLayerManager().GetLayer(_editMode->GetActiveLayer()).SetScrollFactor(val);
	}

	System::Void LayersPanel::_textScrollFactor_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackScrollFactor->Value = Decimal::ToInt32(Decimal::Round(_textScrollFactor->Value * 100));
		float val = Decimal::ToSingle(_textScrollFactor->Value);
		engineAPI->world->GetLayerManager().GetLayer(_editMode->GetActiveLayer()).SetScrollFactor(val);
	}

	System::Void LayersPanel::_trackScale_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			float val = _trackScale->Value / 100.0f;
			_textScale->ValueChanged -= gcnew System::EventHandler(this, &LayersPanel::_textScale_ValueChanged);
			_textScale->Text = val.ToString();
			_textScale->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textScale_ValueChanged);

			SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
			SetSpriteScale(item->GetSprite(), val);
		}
	}

	System::Void LayersPanel::_textScale_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			_trackScale->Value = Decimal::ToInt32(Decimal::Round(_textScale->Value * 100));
			float val = Decimal::ToSingle(_textScale->Value);

			SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
			SetSpriteScale(item->GetSprite(), val);
		}
	}

	System::Void LayersPanel::_trackTile_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			float val = _trackTile->Value / 100.0f;
			_textTile->ValueChanged -= gcnew System::EventHandler(this, &LayersPanel::_textTile_ValueChanged);
			_textTile->Text = val.ToString();
			_textTile->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textTile_ValueChanged);

			SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
			SetSpriteScale(item->GetSprite(), val);
		}
	}

	System::Void LayersPanel::_textTile_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if(_listTextures->SelectedItem != nullptr)
		{
			_trackTile->Value = Decimal::ToInt32(Decimal::Round(_textTile->Value * 100));
			float val = Decimal::ToSingle(_textTile->Value);

			SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
			SetSpriteScale(item->GetSprite(), val);
		}
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
		String^ text = gcnew String(sprite->texture.GetRes()->GetFileName());
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
			text = base_text + "_" + n.ToString();
			++n;
		}

		_listTextures->Items->Add(gcnew SpriteListBoxItem(text, sprite));
	}

	void LayersPanel::UpdateScrollFactorControls()
	{
		float factor = engineAPI->world->GetLayerManager().GetLayer(_editMode->GetActiveLayer()).GetScrollFactor();

		int val = int(Math::Round(factor * 100.0f));
		math3d::clamp(val, _trackScrollFactor->Minimum, _trackScrollFactor->Maximum);
		_trackScrollFactor->Value = val;
		_textScrollFactor->Text = factor.ToString();
	}

	void LayersPanel::UpdateScaleControls()
	{
		if( _listTextures->SelectedItem != nullptr)
		{
			SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
			BgLayer::Sprite* sprite = item->GetSprite();

			bool tile = false;
			const GL::Texture2D* tex = sprite->texture;
			float scale;
			if(	(sprite->flags & BgLayer::Sprite::FLAG_TILE_U) &&
				(sprite->flags & BgLayer::Sprite::FLAG_TILE_V) )
			{
				scale = sprite->uvScale.x;
				tile = true;
			}
			else if(sprite->flags & BgLayer::Sprite::FLAG_TILE_U)
			{
				scale = (sprite->rect.y2 - sprite->rect.y1) / tex->GetHeight();
			}
			else if(sprite->flags & BgLayer::Sprite::FLAG_TILE_V)
			{
				scale = (sprite->rect.x2 - sprite->rect.x1) / tex->GetWidth();
			}
			else
			{
				scale = (sprite->rect.x2 - sprite->rect.x1) / tex->GetWidth();
			}

			if(tile)
			{
				int val = int(Math::Round(scale * 100.0f));
				math3d::clamp(val, _trackTile->Minimum, _trackTile->Maximum);
				_trackTile->Value = val;
				_textTile->ValueChanged -= gcnew System::EventHandler(this, &LayersPanel::_textTile_ValueChanged);
				_textTile->Text = scale.ToString();
				_textTile->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textTile_ValueChanged);
			}
			else
			{
				int val = int(Math::Round(scale * 100.0f));
				math3d::clamp(val, _trackScale->Minimum, _trackScale->Maximum);
				_trackScale->Value = val;
				_textScale->ValueChanged -= gcnew System::EventHandler(this, &LayersPanel::_textScale_ValueChanged);
				_textScale->Text = scale.ToString();
				_textScale->ValueChanged += gcnew System::EventHandler(this, &LayersPanel::_textScale_ValueChanged);
			}
		}
	}

	void LayersPanel::UpdateControlState()
	{
		SpriteListBoxItem^ item = (SpriteListBoxItem^)_listTextures->SelectedItem;
		BgLayer::Sprite* sprite = item? item->GetSprite(): 0;
		bool enable = (sprite != 0);
		bool en_tile = sprite? 
			((sprite->flags & BgLayer::Sprite::FLAG_TILE_U) && (sprite->flags & BgLayer::Sprite::FLAG_TILE_V)) : false;

		_checkTileTexU->Enabled = enable;
		_checkTileTexU->Checked = sprite && (sprite->flags & BgLayer::Sprite::FLAG_TILE_U) != 0;
		_checkTileTexV->Enabled = enable;
		_checkTileTexV->Checked = sprite && (sprite->flags & BgLayer::Sprite::FLAG_TILE_V) != 0;;
		_btnRemoveTexture->Enabled = enable;
		_btnBringToFront->Enabled = enable;
		_btnSendToBack->Enabled = enable;
		_trackScale->Enabled = enable && !en_tile;
		_textScale->Enabled = enable && !en_tile;
		_trackTile->Enabled = enable && en_tile;
		_textTile->Enabled = enable && en_tile;
	}

	void LayersPanel::SetInitialSpriteRect(BgLayer::Sprite* sprite)
	{
		int layer_i = _editMode->GetActiveLayer();
		BgLayer& layer = engineAPI->world->GetLayerManager().GetLayer(layer_i);

		if((sprite->flags & BgLayer::Sprite::FLAG_TILE_U) && (sprite->flags & BgLayer::Sprite::FLAG_TILE_V))
		{
			sprite->rect.x1 = - layer.GetScreenWidth();
			sprite->rect.x2 = layer.GetScreenWidth();
			sprite->rect.y1 = -1000.0f;
			sprite->rect.y2 = 1000.0f;
		}
		else if(sprite->flags & BgLayer::Sprite::FLAG_TILE_U)
		{
			int viewport[4];
			engineAPI->renderSystem->GetRenderer()->GetViewport(viewport);
			vec2f pos;
			layer.PickLayerPoint(viewport[2] / 2, viewport[3], pos);
			float height = sprite->texture->GetHeight() / 4.0f;

			sprite->rect.x1 = - layer.GetScreenWidth();
			sprite->rect.x2 = layer.GetScreenWidth();
			sprite->rect.y1 = pos.y - height;
			sprite->rect.y2 = pos.y;
		}
		else if(sprite->flags & BgLayer::Sprite::FLAG_TILE_V)
		{
			int viewport[4];
			engineAPI->renderSystem->GetRenderer()->GetViewport(viewport);
			vec2f pos;
			layer.PickLayerPoint(viewport[2] / 2, viewport[3], pos);
			float width = sprite->texture->GetWidth() / 4.0f;

			sprite->rect.x1 = pos.x - width * 0.5f;
			sprite->rect.x2 = sprite->rect.x1 + width;
			sprite->rect.y1 = -1000.0f;
			sprite->rect.y2 = 1000.0f;
		}
		else
		{
			int viewport[4];
			engineAPI->renderSystem->GetRenderer()->GetViewport(viewport);
			vec2f pos;
			layer.PickLayerPoint(viewport[2] / 2, viewport[3], pos);
			float width = sprite->texture->GetWidth() / 4.0f;
			float height = sprite->texture->GetHeight() / 4.0f;
			sprite->rect.x1 = pos.x - width * 0.5f;
			sprite->rect.y1 = pos.y - height;
			sprite->rect.x2 = sprite->rect.x1 + width;
			sprite->rect.y2 = pos.y;
		}
	}

	void LayersPanel::SetSpriteScale(BgLayer::Sprite* sprite, float scale)
	{
		if((sprite->flags & BgLayer::Sprite::FLAG_TILE_U) && (sprite->flags & BgLayer::Sprite::FLAG_TILE_V))
		{
			float width = sprite->rect.x2 - sprite->rect.x1;
			float height = sprite->rect.y2 - sprite->rect.y1;
			float vscale = (height * sprite->texture->GetWidth()) / (width * sprite->texture->GetHeight()) * scale;
			sprite->uvScale.set(scale, vscale);
		}
		else if(sprite->flags & BgLayer::Sprite::FLAG_TILE_U)
		{
			sprite->rect.y2 = sprite->rect.y1 + sprite->texture->GetHeight() * scale;
			float width = sprite->rect.x2 - sprite->rect.x1;
			float height = sprite->rect.y2 - sprite->rect.y1;
			float uscale = (width * sprite->texture->GetHeight()) / (height * sprite->texture->GetWidth());
			sprite->uvScale.set(uscale, 1.0f);
		}
		else if(sprite->flags & BgLayer::Sprite::FLAG_TILE_V)
		{
			sprite->rect.x2 = sprite->rect.x1 + sprite->texture->GetWidth() * scale;
			float width = sprite->rect.x2 - sprite->rect.x1;
			float height = sprite->rect.y2 - sprite->rect.y1;
			float vscale = (height * sprite->texture->GetWidth()) / (width * sprite->texture->GetHeight());
			sprite->uvScale.set(1.0f, vscale);
		}
		else
		{
			sprite->rect.x2 = sprite->rect.x1 + sprite->texture->GetWidth() * scale;
			sprite->rect.y2 = sprite->rect.y1 + sprite->texture->GetHeight() * scale;
			sprite->uvScale.set(1.0f, 1.0f);
		}
	}

}
