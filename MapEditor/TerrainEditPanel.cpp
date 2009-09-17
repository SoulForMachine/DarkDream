#include "StdAfx.h"
#include "TerrainEditPanel.h"


namespace MapEditor
{

	TerrainEditPanel::TerrainEditPanel(EM_TerrainEdit::Parameters^ params)
	{
		InitializeComponent();
		_parameters = params;

		UpdateControls();
	}

	void TerrainEditPanel::UpdateControls()
	{
		switch(_parameters->editType)
		{
		case EM_TerrainEdit::EditType::RAISE_LOWER:
			_btnRaiseLower->Checked = true;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
			break;
		case EM_TerrainEdit::EditType::SMOOTH:
			_btnSmooth->Checked = true;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
			break;
		case EM_TerrainEdit::EditType::NOISE:
			_btnNoise->Checked = true;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
			break;
		case EM_TerrainEdit::EditType::PLATEAU:
			_btnPlateau->Checked = true;
			_trackHeight->Enabled = true;
			_numHeight->Enabled = true;
			break;
		case EM_TerrainEdit::EditType::RELATIVE_PLATEAU:
			_btnRelativePlateau->Checked = true;
			_trackHeight->Enabled = true;
			_numHeight->Enabled = true;
			break;
		case EM_TerrainEdit::EditType::RAMP:
			_btnRamp->Checked = true;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
			break;
		}

		_trackRadius->Value = int(_parameters->radius * 2.0f);
		_numRadius->Text = _parameters->radius.ToString();

		_trackHardness->Value = int(_parameters->hardness * 100.0f);
		_numHardness->Text = _trackHardness->Value.ToString();

		_trackStrength->Value = int(_parameters->strength);
		_numStrength->Text = _parameters->strength.ToString();

		_trackHeight->Value = int(_parameters->height);
		_numHeight->Text = _parameters->height.ToString();
	}

	System::Void TerrainEditPanel::_btnRaiseLower_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_parameters->editType != EM_TerrainEdit::EditType::RAISE_LOWER)
		{
			_parameters->editType = EM_TerrainEdit::EditType::RAISE_LOWER;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
		}
	}

	System::Void TerrainEditPanel::_btnSmooth_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_parameters->editType != EM_TerrainEdit::EditType::SMOOTH)
		{
			_parameters->editType = EM_TerrainEdit::EditType::SMOOTH;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
		}
	}

	System::Void TerrainEditPanel::_btnNoise_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_parameters->editType != EM_TerrainEdit::EditType::NOISE)
		{
			_parameters->editType = EM_TerrainEdit::EditType::NOISE;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
		}
	}

	System::Void TerrainEditPanel::_btnPlateau_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_parameters->editType != EM_TerrainEdit::EditType::PLATEAU)
		{
			_parameters->editType = EM_TerrainEdit::EditType::PLATEAU;
			_trackHeight->Enabled = true;
			_numHeight->Enabled = true;
		}
	}

	System::Void TerrainEditPanel::_btnRelativePlateau_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_parameters->editType != EM_TerrainEdit::EditType::RELATIVE_PLATEAU)
		{
			_parameters->editType = EM_TerrainEdit::EditType::RELATIVE_PLATEAU;
			_trackHeight->Enabled = true;
			_numHeight->Enabled = true;
		}
	}

	System::Void TerrainEditPanel::_btnRamp_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if(_parameters->editType != EM_TerrainEdit::EditType::RAMP)
		{
			_parameters->editType = EM_TerrainEdit::EditType::RAMP;
			_trackHeight->Enabled = false;
			_numHeight->Enabled = false;
		}
	}

	System::Void TerrainEditPanel::_trackRadius_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = _trackRadius->Value / 2.0f;
		_numRadius->ValueChanged -= gcnew System::EventHandler(this, &TerrainEditPanel::_numRadius_ValueChanged);
		_numRadius->Text = val.ToString();
		_numRadius->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numRadius_ValueChanged);
		_parameters->radius = val;
	}

	System::Void TerrainEditPanel::_numRadius_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackRadius->Value = Decimal::ToInt32(_numRadius->Value) * 2;
		float val = Decimal::ToSingle(_numRadius->Value);
		_parameters->radius = val;
	}

	System::Void TerrainEditPanel::_trackHardness_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = (float)_trackHardness->Value;
		_numHardness->ValueChanged -= gcnew System::EventHandler(this, &TerrainEditPanel::_numHardness_ValueChanged);
		_numHardness->Text = val.ToString();
		_numHardness->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numHardness_ValueChanged);
		_parameters->hardness = val / 100.0f;
	}

	System::Void TerrainEditPanel::_numHardness_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackHardness->Value = Decimal::ToInt32(_numHardness->Value);
		float val = Decimal::ToSingle(_numHardness->Value);
		_parameters->hardness = val / 100.0f;
	}

	System::Void TerrainEditPanel::_trackStrength_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = (float)_trackStrength->Value;
		_numStrength->ValueChanged -= gcnew System::EventHandler(this, &TerrainEditPanel::_numStrength_ValueChanged);
		_numStrength->Text = val.ToString();
		_numStrength->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numStrength_ValueChanged);
		_parameters->strength = val;
	}

	System::Void TerrainEditPanel::_numStrength_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackStrength->Value = Decimal::ToInt32(_numStrength->Value);
		float val = Decimal::ToSingle(_numStrength->Value);
		_parameters->strength = val;
	}

	System::Void TerrainEditPanel::_trackHeight_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = (float)_trackHeight->Value;
		_numHeight->ValueChanged -= gcnew System::EventHandler(this, &TerrainEditPanel::_numHeight_ValueChanged);
		_numHeight->Text = val.ToString();
		_numHeight->ValueChanged += gcnew System::EventHandler(this, &TerrainEditPanel::_numHeight_ValueChanged);
		_parameters->height = val;
	}

	System::Void TerrainEditPanel::_numHeight_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackHeight->Value = Decimal::ToInt32(_numHeight->Value);
		float val = Decimal::ToSingle(_numHeight->Value);
		_parameters->height = val;
	}

}
