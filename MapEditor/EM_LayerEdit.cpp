
#include "stdafx.h"
#include "LayersPanel.h"
#include "EM_LayerEdit.h"

using namespace Engine;


namespace MapEditor
{

	EM_LayerEdit::EM_LayerEdit(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager)
		: EditMode(listener, persistent)
	{
		_panel = gcnew LayersPanel;
		_undoManager = undo_manager;
	}

	EM_LayerEdit::~EM_LayerEdit()
	{
	}

	System::Windows::Forms::UserControl^ EM_LayerEdit::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_LayerEdit::GetModeEnum()
	{
		return EditMode::EditModeEnum::LAYER_EDIT;
	}

	void EM_LayerEdit::MouseMove(int modifiers, int x, int y)
	{
	}

	void EM_LayerEdit::LeftButtonDown(int x, int y)
	{
	}

	void EM_LayerEdit::LeftButtonUp(int x, int y)
	{
	}

	void EM_LayerEdit::KeyDown(int key)
	{
	}

	void EM_LayerEdit::Update(float dt)
	{
	}

	void EM_LayerEdit::Render()
	{
	}

}
