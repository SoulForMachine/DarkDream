
#include "stdafx.h"
#include "ActionRemoveObjects.h"

using namespace Memory;


namespace MapEditor
{

	ActionRemoveObjects::ActionRemoveObjects(const List<Engine::ModelEntity*>& obj_list)
	{
		_objList = new(mainPool) List<Engine::ModelEntity*>(obj_list);
		_ownObjects = false;
	}

	ActionRemoveObjects::~ActionRemoveObjects()
	{
		if(_ownObjects)
		{
			for(List<Engine::ModelEntity*>::ConstIterator it = _objList->Begin(); it != _objList->End(); ++it)
				delete *it;
		}

		delete _objList;
	}

	bool ActionRemoveObjects::BeginAction()
	{
		RemoveObjects();
		return true;
	}

	void ActionRemoveObjects::EndAction()
	{
	}

	void ActionRemoveObjects::Undo()
	{
		for(List<Engine::ModelEntity*>::ConstIterator it = _objList->Begin(); it != _objList->End(); ++it)
			engineAPI->world->AddEntity(*it);
		_ownObjects = false;
	}

	void ActionRemoveObjects::Redo()
	{
		RemoveObjects();
	}

	ActionType ActionRemoveObjects::GetActionType()
	{
		return ActionType::REMOVE_OBJECTS;
	}

	void ActionRemoveObjects::RemoveObjects()
	{
		for(List<Engine::ModelEntity*>::ConstIterator it = _objList->Begin(); it != _objList->End(); ++it)
			engineAPI->world->RemoveEntity(*it);
		_ownObjects = true;
	}

}
