
#include "stdafx.h"
#include "ActionPaintGrass.h"

using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	ActionPaintGrass::ActionPaintGrass(EM_PaintGrass::Parameters^ params)
	{
		_parameters = params;
	}

	ActionPaintGrass::~ActionPaintGrass()
	{
	}

	bool ActionPaintGrass::BeginAction()
	{

		return true;
	}

	void ActionPaintGrass::EndAction()
	{
	}

	void ActionPaintGrass::CancelAction()
	{
	}

	void ActionPaintGrass::Update(float dt)
	{
	}

	void ActionPaintGrass::Undo()
	{
	}

	void ActionPaintGrass::Redo()
	{
	}

	ActionType ActionPaintGrass::GetActionType()
	{
		return ActionType::PAINT_GRASS;
	}

}
