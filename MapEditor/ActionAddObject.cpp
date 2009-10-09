
#include "stdafx.h"
#include "Utility.h"
#include "ActionAddObject.h"

using namespace System;
using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	ActionAddObject::ActionAddObject(String^ path, const vec3f& point)
	{
		_path = path;
		_point = new(tempPool) vec3f(point);
		_entity = 0;
		_ownObject = false;
	}

	ActionAddObject::~ActionAddObject()
	{
		delete _point;
		if(_ownObject)
			delete _entity;
	}

	bool ActionAddObject::BeginAction()
	{
		tchar* path = ConvertString<tchar>(_path);
		_entity = engineAPI->modelEntityManager->CreateEntityObject(path);
		engineAPI->modelEntityManager->ReleaseAll();
		delete[] path;
		if(engineAPI->world->AddEntity(_entity))
		{
			_entity->SetPosition(*_point);
			_entity->SetActiveAnimation("Idle");
			return true;
		}
		else
		{
			delete _entity;
			_entity = 0;
			return false;
		}
	}

	void ActionAddObject::EndAction()
	{
	}

	void ActionAddObject::Undo()
	{
		if(_entity)
		{
			engineAPI->world->RemoveEntity(_entity);
			_ownObject = true;
		}
	}

	void ActionAddObject::Redo()
	{
		if(_entity)
		{
			engineAPI->world->AddEntity(_entity);
			_ownObject = false;
		}
	}

	ActionType ActionAddObject::GetActionType()
	{
		return ActionType::ADD_OBJECT;
	}

}
