
#pragma once

#include "Action.h"


namespace MapEditor
{

	public ref class ActionAddPatch: public Action
	{
	public:
		ActionAddPatch(int index);
		~ActionAddPatch();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
		virtual ActionType GetActionType() override;

	private:
		int _patchIndex;
	};

}
