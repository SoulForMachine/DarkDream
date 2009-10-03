
#pragma once

#include "Action.h"
#include "EM_PlaceObject.h"


namespace MapEditor
{

	struct ActionPlaceObjects_UndoData
	{
		Engine::ModelEntity* entity;
		math3d::mat4f transform;
		bool dropped;
	};


	public ref class ActionPlaceObjects: public Action
	{
	public:
		ActionPlaceObjects(EM_PlaceObject::Parameters^ params);
		~ActionPlaceObjects();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Update(float dt) override;
		virtual void CancelAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
		virtual ActionType GetActionType() override;

	private:
		EM_PlaceObject::Parameters^ _parameters;
		StaticArray<ActionPlaceObjects_UndoData>* _undoData;
	};

}
