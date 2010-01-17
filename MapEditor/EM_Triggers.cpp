#include "stdafx.h"
#include "resource.h"
#include "TriggerPanel.h"
#include "EM_Triggers.h"

#define CIRCLE_VERTEX_COUNT		64
#define GIZMO_INNER_SCALE		1.0f
#define GIZMO_OUTER_SCALE		2.0f


using namespace System;
using namespace System::Windows;
using namespace System::Drawing;
using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	EM_Triggers::EM_Triggers(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager)
		: EditMode(listener, persistent)
	{
		_panel = gcnew TriggerPanel;
		_undoManager = undo_manager;
		_undoManager->RegisterListener(this);
	}

	EM_Triggers::~EM_Triggers()
	{
		delete _panel;
	}

	System::Windows::Forms::UserControl^ EM_Triggers::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_Triggers::GetModeEnum()
	{
		return EditModeEnum::EDIT_TRIGGERS;
	}

	void EM_Triggers::Activate()
	{
		
	}

	void EM_Triggers::MouseMove(int modifiers, int x, int y)
	{
	}

	void EM_Triggers::LeftButtonDown(int x, int y)
	{
	}

	void EM_Triggers::LeftButtonUp(int x, int y)
	{
		
	}

	void EM_Triggers::KeyDown(int key)
	{
		
	}

	void EM_Triggers::Update(float dt)
	{
	}

	void EM_Triggers::Render()
	{
		
	}

	void EM_Triggers::UndoEvent(UndoEventListener::EventType type, Action^ action)
	{
		
	}

	void EM_Triggers::ClearSelection()
	{
	}

	void EM_Triggers::DropSelected()
	{
	}

}
