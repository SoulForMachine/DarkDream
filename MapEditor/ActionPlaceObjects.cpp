
#include "stdafx.h"
#include "ActionPlaceObjects.h"


namespace MapEditor
{

	ActionPlaceObjects::ActionPlaceObjects(EM_PlaceObject::Parameters^ params)
	{
		_parameters = params;
	}

	ActionPlaceObjects::~ActionPlaceObjects()
	{
	}

	bool ActionPlaceObjects::BeginAction()
	{
		return true;
	}

	void ActionPlaceObjects::EndAction()
	{
	}

	void ActionPlaceObjects::Update(float dt)
	{
	}

	void ActionPlaceObjects::CancelAction()
	{
	}

	void ActionPlaceObjects::Undo()
	{
	}

	void ActionPlaceObjects::Redo()
	{
	}

}
