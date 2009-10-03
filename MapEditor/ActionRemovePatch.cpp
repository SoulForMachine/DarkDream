
#include "stdafx.h"
#include "ActionRemovePatch.h"


namespace MapEditor
{

	ActionRemovePatch::ActionRemovePatch(int index)
	{
		_patchIndex = index;
	}

	ActionRemovePatch::~ActionRemovePatch()
	{
		
	}

	bool ActionRemovePatch::BeginAction()
	{
		engineAPI->world->GetTerrain().RemovePatch(_patchIndex);
		return true;
	}

	void ActionRemovePatch::EndAction()
	{
	}

	void ActionRemovePatch::Undo()
	{
		engineAPI->world->GetTerrain().AddPatch(_patchIndex);
	}

	void ActionRemovePatch::Redo()
	{
		engineAPI->world->GetTerrain().RemovePatch(_patchIndex);
	}

	ActionType ActionRemovePatch::GetActionType()
	{
		return ActionType::REMOVE_PATCH;
	}

}
