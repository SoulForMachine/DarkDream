
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	public ref class EM_RemovePatch: public EditMode
	{
	public:
		EM_RemovePatch(EditModeEventListener^ listener, bool persistent);

		virtual EditModeEnum GetModeEnum() override
			{ return EditModeEnum::REMOVE_PATCH; }

		virtual void Activate() override;
		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void KeyDown(int key) override;
		
	private:
		int _patchIndex;
	};

}
