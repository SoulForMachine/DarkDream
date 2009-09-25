
#pragma once

#include "Action.h"


namespace MapEditor
{

	public ref class ActionRemoveObjects: public Action
	{
	public:
		ActionRemoveObjects(const List<Engine::ModelEntity*>& obj_list);
		~ActionRemoveObjects();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
	};

}
