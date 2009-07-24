
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class UndoManager;


	ref class EM_TerrainPaint: public EditMode
	{
	public:
		EM_TerrainPaint(UndoManager^ undo_manager);
		virtual System::Windows::Forms::UserControl^ GetPanel() override
			{ return _panel; }
		virtual EditModeEnum GetModeEnum() override
			{ return EditModeEnum::TERRAIN_PAINT; }

	private:
		System::Windows::Forms::UserControl^ _panel;
		UndoManager^ _undoManager;
	};

}
