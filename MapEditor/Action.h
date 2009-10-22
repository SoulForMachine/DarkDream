
#pragma once

namespace MapEditor
{

	enum class ActionType
	{
		ADD_OBJECT,
		REMOVE_OBJECTS,
		PLACE_OBJECTS,
		ADD_PATCH,
		REMOVE_PATCH,
		TERRAIN_EDIT,
		PAINT_GRASS,
	};

	public ref class Action abstract
	{
	public:
		virtual bool BeginAction() = 0;
		virtual void EndAction() = 0;
		virtual void Update(float dt) {}
		virtual void CancelAction() {}
		virtual void Undo() = 0;
		virtual void Redo() = 0;
		virtual ActionType GetActionType() = 0;
	};

}
