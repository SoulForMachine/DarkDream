
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
		virtual ActionType GetActionType() override;

	private:
		void RemovePatch();

		int _patchIndex;
		float* _elevation;
		List<Engine::Entity*>* _entities;
	};

}
