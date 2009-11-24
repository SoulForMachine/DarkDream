
#pragma once

#include "Action.h"


namespace MapEditor
{

	public ref class ActionRemoveObjects: public Action
	{
	public:
		ActionRemoveObjects(const List<Engine::RenderableEntity*>& obj_list);
		~ActionRemoveObjects();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
		virtual ActionType GetActionType() override;

	private:
		void RemoveObjects();

		List<Engine::RenderableEntity*>* _objList;
		bool _ownObjects;
	};

}
