
#include "stdafx.h"
#include "ActionPlaceObjects.h"

using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	ActionPlaceObjects::ActionPlaceObjects(EM_PlaceObject::Parameters^ params)
	{
		_parameters = params;
		_undoData = New<StaticArray<ActionPlaceObjects_UndoData>>(mainPool);
	}

	ActionPlaceObjects::~ActionPlaceObjects()
	{
		Delete(_undoData);
	}

	bool ActionPlaceObjects::BeginAction()
	{
		List<RenderableEntity*>* ents = _parameters->selectedEntities;
		if(!ents || !ents->GetCount())
			return false;

		_undoData->SetCount(ents->GetCount());
		int i = 0;
		for(List<RenderableEntity*>::Iterator it = ents->Begin(); it != ents->End(); ++it)
		{
			(*_undoData)[i].entity = *it;
			(*_undoData)[i].transform = (*it)->GetWorldTransform();
			(*_undoData)[i].dropped = (*it)->IsDropped();
			i++;
		}

		if(_parameters->transformType == EM_PlaceObject::TransformType::DROP)
		{
			for(size_t i = 0; i < _undoData->GetCount(); ++i)
			{
				RenderableEntity* entity = (*_undoData)[i].entity;
				vec3f pos = entity->GetPosition();
				float y;
				if(engineAPI->world->GetTerrain().ElevationFromPoint(vec2f(pos.x, pos.z), y))
				{
					pos.y = y;
					entity->SetPosition(pos);
					entity->SetDropped(true);
				}
			}
		}

		return true;
	}

	void ActionPlaceObjects::EndAction()
	{
	}

	void ActionPlaceObjects::Update(float dt)
	{
		if(_parameters->transformType == EM_PlaceObject::TransformType::MOVE_XZ)
		{
			for(size_t i = 0; i < _undoData->GetCount(); ++i)
			{
				mat4f mat = (*_undoData)[i].transform;
				mat.translate(_parameters->translX, 0.0f, _parameters->translZ);
				if((*_undoData)[i].entity->IsDropped())
				{
					float y;
					if(engineAPI->world->GetTerrain().ElevationFromPoint(vec2f(mat.transl_vec.x, mat.transl_vec.z), y))
						mat.transl_vec.y = y;
				}
				(*_undoData)[i].entity->SetWorldTransform(mat);
			}
		}
		else if(_parameters->transformType == EM_PlaceObject::TransformType::MOVE_Y)
		{
			for(size_t i = 0; i < _undoData->GetCount(); ++i)
			{
				mat4f mat = (*_undoData)[i].transform;
				mat.translate(0.0f, _parameters->translY, 0.0f);
				(*_undoData)[i].entity->SetDropped(false);
				(*_undoData)[i].entity->SetWorldTransform(mat);
			}
		}
		else if(_parameters->transformType == EM_PlaceObject::TransformType::ROTATE)
		{
			for(size_t i = 0; i < _undoData->GetCount(); ++i)
			{
				mat4f rot, mat;
				rot.set_rotation_y(deg2rad(_parameters->rotateY));
				mul(mat, rot, (*_undoData)[i].transform);
				(*_undoData)[i].entity->SetWorldTransform(mat);
			}
		}
	}

	void ActionPlaceObjects::CancelAction()
	{
		for(size_t i = 0; i < _undoData->GetCount(); ++i)
		{
			(*_undoData)[i].entity->SetWorldTransform((*_undoData)[i].transform);
			(*_undoData)[i].entity->SetDropped((*_undoData)[i].dropped);
		}
	}

	void ActionPlaceObjects::Undo()
	{
		for(size_t i = 0; i < _undoData->GetCount(); ++i)
		{
			mat4f mat = (*_undoData)[i].entity->GetWorldTransform();
			bool dropped = (*_undoData)[i].entity->IsDropped();
			(*_undoData)[i].entity->SetWorldTransform((*_undoData)[i].transform);
			(*_undoData)[i].entity->SetDropped((*_undoData)[i].dropped);
			(*_undoData)[i].transform = mat;
			(*_undoData)[i].dropped = dropped;
		}
	}

	void ActionPlaceObjects::Redo()
	{
		Undo();
	}

	ActionType ActionPlaceObjects::GetActionType()
	{
		return ActionType::PLACE_OBJECTS;
	}

}
