
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class EM_View: public EditMode
	{
	public:
		EM_View(EditModeEventListener^ listener, bool persistent)
			: EditMode(listener, persistent) {}

		virtual EditModeEnum GetModeEnum() override
			{ return EditModeEnum::VIEW; }
	};

}
