
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class EM_View: public EditMode
	{
	public:
		virtual System::Windows::Forms::UserControl^ GetPanel() override
			{ return nullptr; }

		virtual EditModeEnum GetModeEnum() override
			{ return EditModeEnum::VIEW; }
	};

}
