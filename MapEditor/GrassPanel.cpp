#include "StdAfx.h"
#include "Utility.h"
#include "GrassPanel.h"

using namespace EditorCommon;
using namespace Engine;


namespace MapEditor
{

	GrassPanel::GrassPanel(EM_PaintGrass::Parameters^ params)
	{
		InitializeComponent();

		_textureList = gcnew EditorCommon::TextureListView;
		_textureList->Location = _panelDummy->Location;
		_textureList->Size = _panelDummy->Size;
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

		val = int(Math::Round(_parameters->scale));
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
			tchar* file_name = GetRelativePath(_selectTextureDialog->FileName);
			_textTexture->Text = gcnew String(file_name);
			if(FillTextureList(_selectTextureDialog->FileName))
			{
				//const TextureRes* tex = engineAPI->textureManager->CreateTexture(file_name, true);
				//engineAPI->world->GetTerrain()
			}
			delete[] file_name;
		}
	}

	System::Void GrassPanel::_selectTextureDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
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

	bool GrassPanel::FillTextureList(String^ file_name)
	{
		tchar* fn = ConvertString<tchar>(file_name);
		::Image img;
		if(!img.Load(fn))
		{
			delete fn;
			return false;
		}
		delete fn;
		const ImageData& img_data = img.GetImageData();
		if(img_data.format != IMG_BGRA && img_data.format != IMG_RGBA)
			return false;
		const uchar* src = (uchar*)img_data.images[0].pixels;

		int xcount = img_data.images[0].width / 64;
		int ycount = img_data.images[0].height / 64;

		_textureList->Clear();

		for(int y = 0; y < img_data.images[0].height; y += 64)
		{
			for(int x = 0; x < img_data.images[0].width; x += 64)
			{
				Bitmap^ b = gcnew Bitmap(64, 64, PixelFormat::Format32bppArgb);
				BitmapData^ dest_data = b->LockBits(Drawing::Rectangle(0, 0, 64, 64), ImageLockMode::WriteOnly, PixelFormat::Format32bppArgb);
				uchar* dest = (uchar*)dest_data->Scan0.ToPointer();
				const uchar* psrc = src + img_data.images[0].bytesPerScanline * y;

				for(int j = 0; j < 64; ++j)
				{
					for(int i = 0; i < 64; ++i)
					{
						dest[i * 4] = psrc[(i + x) * 4];
						dest[i * 4 + 1] = psrc[(i + x) * 4 + 1];
						dest[i * 4 + 2] = psrc[(i + x) * 4 + 2];
						dest[i * 4 + 3] = psrc[(i + x) * 4 + 3];
					}
					dest += dest_data->Stride;
					psrc += img_data.images[0].bytesPerScanline;
				}

				b->UnlockBits(dest_data);
				_textureList->Add(gcnew TextureListViewItem(b));
			}
		}

		return true;
	}

}
