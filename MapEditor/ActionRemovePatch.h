
#pragma once

#include "Action.h"


namespace MapEditor
{

	public ref class ActionRemovePatch: public Action
	{
	public:
		ActionRemovePatch(int index);
		~ActionRemovePatch();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Undo() override;
		virtual void Redo() override;

	private:
		int _patchIndex;
	};

}
