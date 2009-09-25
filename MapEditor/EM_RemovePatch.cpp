
#include "stdafx.h"
#include "UndoManager.h"
#include "ActionRemovePatch.h"
#include "EM_RemovePatch.h"

using namespace Engine;
using namespace math3d;
using namespace System::Windows::Forms;


namespace MapEditor
{

	EM_RemovePatch::EM_RemovePatch(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager)
			: EditMode(listener, persistent)
	{
		_isExecuting = false;
		_undoManager = undo_manager;
	}

	void EM_RemovePatch::Activate()
	{
		_patchIndex = -1;
	}

	void EM_RemovePatch::MouseMove(int modifiers, int x, int y)
	{
		SetCursor(LoadCursor(0, IDC_ARROW));

		int vp_x, vp_y, vp_width, vp_height;
		engineAPI->renderSystem->GetRenderer()->GetViewport(vp_x, vp_y, vp_width, vp_height);
		vec3f point;
		_patchIndex = engineAPI->world->GetTerrain().PickTerrainPoint(x, vp_height - y, point);
		engineAPI->world->GetTerrain().HighlightPatch(_patchIndex);
	}

	void EM_RemovePatch::LeftButtonDown(int x, int y)
	{
		if(_patchIndex != -1)
		{
			if(engineAPI->world->GetTerrain().GetPatchCount() == 1)
			{
				MessageBox::Show(
					"Cannot remove the only terrain patch.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);

				_evListener->EditModeEvent(EditModeEventListener::EMEvent::EDIT_ERROR);
				engineAPI->world->GetTerrain().HighlightPatch(-1);

				return;
			}

			ActionRemovePatch^ action = gcnew ActionRemovePatch(_patchIndex);
			action->BeginAction();
			action->EndAction();
			_undoManager->Add(action);

			// if shift is not being held we exit this edit mode
			if(!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				_evListener->EditModeEvent(EditModeEventListener::EMEvent::EDIT_COMPLETE);
				engineAPI->world->GetTerrain().HighlightPatch(-1);
			}
		}
	}

	void EM_RemovePatch::KeyDown(int key)
	{
		if(key == VK_ESCAPE)
		{
			_evListener->EditModeEvent(EditModeEventListener::EMEvent::EDIT_CANCELED);
			engineAPI->world->GetTerrain().HighlightPatch(-1);
		}
	}

}
