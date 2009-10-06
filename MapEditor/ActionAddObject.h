
#pragma once

#include "Action.h"


namespace MapEditor
{

	public ref class ActionAddObject: public Action
	{
	public:
		ActionAddObject(System::String^ path, const math3d::vec3f& point);
		~ActionAddObject();

		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
		virtual ActionType GetActionType() override;

	private:
		System::String^ _path;
		const math3d::vec3f* _point;
		Engine::ModelEntity* _entity;
		bool _ownObject;
	};

}
