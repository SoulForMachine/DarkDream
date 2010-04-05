
#include "stdafx.h"
#include "EditorCommon/UtilityTempl.h"
#include "ActionAddObject.h"

using namespace System;
using namespace math3d;
using namespace Memory;
using namespace Engine;
using namespace EditorCommon;


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
		ModelEntity::JointAttachType type = ModelEntity::GetJointAttachTypeByExt(path);
		switch(type)
		{
		case ModelEntity::JOINT_ATTACH_MODEL:
			_entity = engineAPI->modelEntityManager->CreateEntityObject(path);
			engineAPI->modelEntityManager->ReleaseAll(); //!
			((ModelEntity*)_entity)->SetActiveAnimation("Idle");
			break;
		case ModelEntity::JOINT_ATTACH_PARTICLE_SYSTEM:
			_entity = engineAPI->partSysManager->CreateParticleSystemObject(path);
			engineAPI->partSysManager->ReleaseAll(); //!
			break;
		default:
			delete[] path;
			return false;
		}

		delete[] path;
		if(engineAPI->world->AddEntity(_entity))
		{
			_entity->SetPosition(*_point);
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
