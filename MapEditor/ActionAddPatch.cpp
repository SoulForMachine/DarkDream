
#include "stdafx.h"
#include "ActionAddPatch.h"


namespace MapEditor
{

	ActionAddPatch::ActionAddPatch(int index)
	{
		_patchIndex = index;
	}

	ActionAddPatch::~ActionAddPatch()
	{
		
	}

	bool ActionAddPatch::BeginAction()
	{
		return (engineAPI->world->GetTerrain().AddPatch(_patchIndex) != -1);
	}

	void ActionAddPatch::EndAction()
	{
	}

	void ActionAddPatch::Undo()
	{
		engineAPI->world->GetTerrain().RemovePatch(_patchIndex);
	}

	void ActionAddPatch::Redo()
	{
		engineAPI->world->GetTerrain().AddPatch(_patchIndex);
	}

	ActionType ActionAddPatch::GetActionType()
	{
		return ActionType::ADD_PATCH;
	}

}
