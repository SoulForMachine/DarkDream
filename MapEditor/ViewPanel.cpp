#include "StdAfx.h"
#include "ViewPanel.h"


namespace MapEditor
{

	ViewPanel::ViewPanel(EM_View::Parameters^ params)
	{
		InitializeComponent();

		_parameters = params;

		_numHeight->Text = _parameters->camHeight.ToString();
		_numDistance->Text = _parameters->camDistance.ToString();
		_numFOV->Text = _parameters->camFOV.ToString();
	}

	System::Void ViewPanel::_trackHeight_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = _trackHeight->Value / 10.0f;
		_numHeight->ValueChanged -= gcnew System::EventHandler(this, &ViewPanel::_numHeight_ValueChanged);
		_numHeight->Text = val.ToString();
		_numHeight->ValueChanged += gcnew System::EventHandler(this, &ViewPanel::_numHeight_ValueChanged);
		_parameters->camHeight = val;
	}

	System::Void ViewPanel::_numHeight_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackHeight->Value = Decimal::ToInt32(Decimal::Round(_numHeight->Value * 10));
		float val = Decimal::ToSingle(_numHeight->Value);
		_parameters->camHeight = val;
	}

	System::Void ViewPanel::_trackDistance_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = _trackDistance->Value / 10.0f;
		_numDistance->ValueChanged -= gcnew System::EventHandler(this, &ViewPanel::_numDistance_ValueChanged);
		_numDistance->Text = val.ToString();
		_numDistance->ValueChanged += gcnew System::EventHandler(this, &ViewPanel::_numDistance_ValueChanged);
		_parameters->camDistance = val;
	}

	System::Void ViewPanel::_numDistance_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackDistance->Value = Decimal::ToInt32(Decimal::Round(_numDistance->Value * 10));
		float val = Decimal::ToSingle(_numDistance->Value);
		_parameters->camDistance = val;
	}

	System::Void ViewPanel::_trackFOV_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		float val = (float)_trackFOV->Value;
		_numFOV->ValueChanged -= gcnew System::EventHandler(this, &ViewPanel::_numFOV_ValueChanged);
		_numFOV->Text = val.ToString();
		_numFOV->ValueChanged += gcnew System::EventHandler(this, &ViewPanel::_numFOV_ValueChanged);
		_parameters->camFOV = val;
	}

	System::Void ViewPanel::_numFOV_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		_trackFOV->Value = Decimal::ToInt32(Decimal::Round(_numFOV->Value));
		float val = Decimal::ToSingle(_numFOV->Value);
		_parameters->camFOV = val;
	}

}
