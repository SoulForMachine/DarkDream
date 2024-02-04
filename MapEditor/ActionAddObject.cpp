
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
		_point = New<vec3f>(tempPool, point);
		_entity = 0;
		_ownObject = false;
	}

	ActionAddObject::~ActionAddObject()
	{
		Delete(_point);
		if(_ownObject)
			ObjectFactory::DeleteEntity(_entity);
	}

	bool ActionAddObject::BeginAction()
	{
		tchar* path = ConvertString<tchar>(_path);
		ModelEntity::JointAttachType type = ModelEntity::GetJointAttachTypeByExt(path);
		switch(type)
		{
		case ModelEntity::JOINT_ATTACH_MODEL:
			_entity = engineAPI->modelEntityManager->CreateEntityObject(path);
			((ModelEntity*)_entity)->SetActiveAnimation("Idle");
			break;
		case ModelEntity::JOINT_ATTACH_PARTICLE_SYSTEM:
			_entity = engineAPI->partSysManager->CreateParticleSystemObject(path);
			break;
		default:
			Memory::Delete(path);
			return false;
		}

		Memory::Delete(path);
		_entity->SetPosition(*_point);

		if(engineAPI->world->AddEntity(_entity))
		{
			return true;
		}
		else
		{
			ObjectFactory::DeleteEntity(_entity);
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
