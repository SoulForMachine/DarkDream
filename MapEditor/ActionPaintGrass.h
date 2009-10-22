
#pragma once

#include "Action.h"
#include "EM_PaintGrass.h"


namespace MapEditor
{

	ref class ActionPaintGrass: public Action
	{
	public:
		ActionPaintGrass(EM_PaintGrass::Parameters^ params);
		~ActionPaintGrass();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Update(float dt) override;
		virtual void CancelAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
		virtual ActionType GetActionType() override;

	private:
		EM_PaintGrass::Parameters^ _parameters;
	};

}
