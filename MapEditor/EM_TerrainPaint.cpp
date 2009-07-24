
#include "stdafx.h"
#include "TerrainPaintPanel.h"
#include "UndoManager.h"
#include "EM_TerrainPaint.h"


namespace MapEditor
{

	EM_TerrainPaint::EM_TerrainPaint(UndoManager^ undo_manager)
	{
		_panel = gcnew TerrainPaintPanel;
		_undoManager = undo_manager;
	}

}
