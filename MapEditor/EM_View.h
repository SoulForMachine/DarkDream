
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class ViewPanel;


	public ref class EM_View: public EditMode
	{
	public:
		ref struct Parameters
		{
			float camHeight;
			float camDistance;
			float camFOV;
		};

		EM_View(EditModeEventListener^ listener, bool persistent);

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		Parameters^ GetParameters()
			{ return _parameters; }

	private:
		ViewPanel^ _panel;
		Parameters^ _parameters;
	};

}
