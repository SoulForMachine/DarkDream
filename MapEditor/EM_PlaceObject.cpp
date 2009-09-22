#include "stdafx.h"
#include "PlaceObjectPanel.h"
#include "EM_PlaceObject.h"


namespace MapEditor
{

	EM_PlaceObject::EM_PlaceObject(EditModeEventListener^ listener, bool persistent)
		: EditMode(listener, persistent)
	{
		_panel = gcnew PlaceObjectPanel;
	}

	EM_PlaceObject::~EM_PlaceObject()
	{
		delete _panel;
	}

	System::Windows::Forms::UserControl^ EM_PlaceObject::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_PlaceObject::GetModeEnum()
	{
		return EditModeEnum::PLACE_OBJECT;
	}

	void EM_PlaceObject::Activate()
	{
		
	}

	void EM_PlaceObject::MouseMove(int modifiers, int x, int y)
	{
		SetCursor(LoadCursor(0, IDC_ARROW));
	}

	void EM_PlaceObject::LeftButtonDown(int x, int y)
	{
	}

}
