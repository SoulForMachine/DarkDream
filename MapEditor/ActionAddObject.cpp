
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
		_transform = new(tempPool) mat4f(point);
		_entity = 0;
	}

	ActionAddObject::~ActionAddObject()
	{
		delete _point;
		delete _transform;
	}

	bool ActionAddObject::BeginAction()
	{
		_transform->set_translation(*_point);
		return AddObject();
	}

	void ActionAddObject::EndAction()
	{
	}

	void ActionAddObject::Undo()
	{
		*_transform = _entity->GetWorldTransform(); // save current transform for redo
		engineAPI->world->RemoveEntity(_entity);
		delete _entity;
		_entity = 0;
	}

	void ActionAddObject::Redo()
	{
		AddObject();
	}

	ActionType ActionAddObject::GetActionType()
	{
		return ActionType::ADD_OBJECT;
	}

	bool ActionAddObject::AddObject()
	{
		tchar* path = ConvertString<tchar>(_path);
		ModelEntity* entity = engineAPI->modelEntityManager->CreateEntityObject(path);
		delete[] path;
		if(engineAPI->world->AddEntity(entity))
		{
			_entity = entity;
			_entity->SetWorldTransform(*_transform);
			engineAPI->modelEntityManager->ReleaseAll();
			return true;
		}
		else
		{
			_entity = 0;
			return false;
		}
	}

}
