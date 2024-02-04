
#include "stdafx.h"
#include "ActionRemoveObjects.h"

using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	ActionRemoveObjects::ActionRemoveObjects(const List<Engine::RenderableEntity*>& obj_list)
	{
		_objList = New<List<Engine::RenderableEntity*>>(mainPool, obj_list);
		_ownObjects = false;
	}

	ActionRemoveObjects::~ActionRemoveObjects()
	{
		if(_ownObjects)
		{
			for(List<Engine::RenderableEntity*>::ConstIterator it = _objList->Begin(); it != _objList->End(); ++it)
				ObjectFactory::DeleteEntity(*it);
		}

		Delete(_objList);
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
		for(List<Engine::RenderableEntity*>::ConstIterator it = _objList->Begin(); it != _objList->End(); ++it)
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
		for(List<Engine::RenderableEntity*>::ConstIterator it = _objList->Begin(); it != _objList->End(); ++it)
			engineAPI->world->RemoveEntity(*it);
		_ownObjects = true;
	}

}
