#include "StdAfx.h"
#include "EditorCommon/UtilityTempl.h"
#include "GrassPanel.h"

using namespace Memory;
using namespace Engine;
using namespace EditorCommon;


namespace MapEditor
{

	GrassPanel::GrassPanel(EM_PaintGrass::Parameters^ params)
	{
		InitializeComponent();

		_textureList = gcnew EditorCommon::TextureListView;
		_textureList->Location = _panelDummy->Location;
		_textureList->Size = _panelDummy->Size;
		_textureList->TextureSize = Terrain::GRASS_TEX_SIZE;
		_textureList->TextureIndexChanged += gcnew EventHandler(this, &GrassPanel::_textureList_TextureIndexChanged);
		Controls->Remove(_panelDummy);
		delete _panelDummy;
		Controls->Add(_textureList);

		_parameters = params;
		UpdateControls();
	}

	void GrassPanel::UpdateControls()
	{
		int val = int(Math::Round(_parameters->radius * 2.0f));
		math3d::clamp(val, _trackRadius->Minimum, _trackRadius->Maximum);
		_trackRadius->Value = val;
		_numRadius->Text = _parameters->radius.ToString();

		val = int(Math::Round(_parameters->hardness * 100.0f));
		math3d::clamp(val, _trackHardness->Minimum, _trackHardness->Maximum);
		_trackHardness->Value = val;
		_numHardness->Text = _trackHardness->Value.ToString();

		val = int(Math::Round(_parameters->strength));
		math3d::clamp(val, _trackStrength->Minimum, _trackStrength->Maximum);
		_trackStrength->Value = val;
		_numStrength->Text = _parameters->strength.ToString();

		val = int(Math::Round(_parameters->scale * 10.0f));
		math3d::clamp(val, _trackScale->Minimum, _trackScale->Maximum);
		_trackScale->Value = val;
		_numScale->Text = _parameters->scale.ToString();
	}

	System::Void GrassPanel::_trackRadius_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = _trackRadius->Value / 2.0f;
		_numRadius->ValueChanged -= gcnew System::EventHandler(this, &GrassPanel::_numRadius_ValueChanged);
		_numRadius->Text = val.ToString();
		_numRadius->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numRadius_ValueChanged);
		_parameters->radius = val;
	}

	System::Void GrassPanel::_numRadius_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackRadius->Value = Decimal::ToInt32(Decimal::Round(_numRadius->Value * 2));
		float val = Decimal::ToSingle(_numRadius->Value);
		_parameters->radius = val;
	}

	System::Void GrassPanel::_trackHardness_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = (float)_trackHardness->Value;
		_numHardness->ValueChanged -= gcnew System::EventHandler(this, &GrassPanel::_numHardness_ValueChanged);
		_numHardness->Text = val.ToString();
		_numHardness->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numHardness_ValueChanged);
		_parameters->hardness = val / 100.0f;
	}

	System::Void GrassPanel::_numHardness_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackHardness->Value = Decimal::ToInt32(Decimal::Round(_numHardness->Value));
		float val = Decimal::ToSingle(_numHardness->Value);
		_parameters->hardness = val / 100.0f;
	}

	System::Void GrassPanel::_trackStrength_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = (float)_trackStrength->Value;
		_numStrength->ValueChanged -= gcnew System::EventHandler(this, &GrassPanel::_numStrength_ValueChanged);
		_numStrength->Text = val.ToString();
		_numStrength->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numStrength_ValueChanged);
		_parameters->strength = val;
	}

	System::Void GrassPanel::_numStrength_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackStrength->Value = Decimal::ToInt32(Decimal::Round(_numStrength->Value));
		float val = Decimal::ToSingle(_numStrength->Value);
		_parameters->strength = val;
	}

	System::Void GrassPanel::_trackScale_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = (float)_trackScale->Value / 10.0f;
		_numScale->ValueChanged -= gcnew System::EventHandler(this, &GrassPanel::_numScale_ValueChanged);
		_numScale->Text = val.ToString();
		_numScale->ValueChanged += gcnew System::EventHandler(this, &GrassPanel::_numScale_ValueChanged);
		_parameters->scale = val;
	}

	System::Void GrassPanel::_numScale_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackScale->Value = Decimal::ToInt32(Decimal::Round(_numScale->Value * 10));
		float val = Decimal::ToSingle(_numScale->Value);
		_parameters->scale = val;
	}

	System::Void GrassPanel::_buttonBrowse_Click(System::Object^  sender, System::EventArgs^  e)
	{
		_selectTextureDialog->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + "Textures";
		if(_selectTextureDialog->ShowDialog() == DialogResult::OK)
		{
			tchar* file_name = EditorUtil::GetRelativePath(_selectTextureDialog->FileName);
			_textTexture->Text = gcnew String(file_name);
			Texture2DResPtr tex = engineAPI->textureManager->CreateTexture2D(file_name, true);
			if(FillTextureList(tex))
			{
				engineAPI->world->GetTerrain().SetGrassTexture(tex);
			}
			delete[] file_name;
		}
	}

	System::Void GrassPanel::_selectTextureDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
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

	System::Void GrassPanel::_textureList_TextureIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_parameters->texIndex = _textureList->SelectedIndex;
	}

	System::Void GrassPanel::_checkRandomTex_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_parameters->randomTex = _checkRandomTex->Checked;
	}

	bool GrassPanel::FillTextureList(const GL::Texture* texture)
	{
		if(!texture || texture->GetType() != GL::OBJ_TEXTURE_2D)
			return false;

		GL::Texture2D* tex2d = (GL::Texture2D*)texture;
		int width = tex2d->GetWidth();
		int height = tex2d->GetHeight();

		int size;
		if(tex2d->IsCompressed())
			size = tex2d->GetCompressedSize(0);
		else
			size = width * height * 4;

		uchar* src = new(tempPool) uchar[size];
		GL::PixelStore pixel_store = GL::GLState::defaultPixelStore;
		pixel_store.alignment = 1;
		if(!tex2d->GetTexImage(0, GL::IMAGE_FORMAT_BGRA, GL::TYPE_UNSIGNED_BYTE, &pixel_store, src))
		{
			delete[] src;
			return false;
		}

		int xcount = width / Terrain::GRASS_TEX_SIZE;
		int ycount = height / Terrain::GRASS_TEX_SIZE;

		_textureList->Clear();

		for(int y = 0; y < height; y += Terrain::GRASS_TEX_SIZE)
		{
			for(int x = 0; x < width; x += Terrain::GRASS_TEX_SIZE)
			{
				Bitmap^ b = gcnew Bitmap(Terrain::GRASS_TEX_SIZE, Terrain::GRASS_TEX_SIZE, PixelFormat::Format32bppArgb);
				BitmapData^ dest_data = b->LockBits(Drawing::Rectangle(0, 0, Terrain::GRASS_TEX_SIZE, Terrain::GRASS_TEX_SIZE), ImageLockMode::WriteOnly, PixelFormat::Format32bppArgb);
				uchar* dest = (uchar*)dest_data->Scan0.ToPointer();
				const uchar* psrc = src + width * 4 * y;

				for(int j = 0; j < Terrain::GRASS_TEX_SIZE; ++j)
				{
					for(int i = 0; i < Terrain::GRASS_TEX_SIZE; ++i)
					{
						dest[i * 4] = psrc[(i + x) * 4];
						dest[i * 4 + 1] = psrc[(i + x) * 4 + 1];
						dest[i * 4 + 2] = psrc[(i + x) * 4 + 2];
						dest[i * 4 + 3] = psrc[(i + x) * 4 + 3];
					}
					dest += dest_data->Stride;
					psrc += width * 4;
				}

				b->UnlockBits(dest_data);
				_textureList->Add(gcnew TextureListViewItem(b));
			}
		}

		_parameters->texCount = _textureList->TextureCount;

		delete[] src;
		return true;
	}

}
