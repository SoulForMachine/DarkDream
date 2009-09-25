
#include "stdafx.h"
#include "ActionRemoveObjects.h"


namespace MapEditor
{

	ActionRemoveObjects::ActionRemoveObjects(const List<Engine::ModelEntity*>& obj_list)
	{
	}

	ActionRemoveObjects::~ActionRemoveObjects()
	{
	}

	bool ActionRemoveObjects::BeginAction()
	{
		return true;
	}

	void ActionRemoveObjects::EndAction()
	{
	}

	void ActionRemoveObjects::Undo()
	{
	}

	void ActionRemoveObjects::Redo()
	{
	}

}
