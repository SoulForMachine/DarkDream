
#pragma once

#include "Action.h"


namespace MapEditor
{

	public ref class ActionPlaceObjects: public Action
	{
	public:
		virtual bool BeginAction() override;
		virtual void EndAction() override;
		virtual void Update(float dt) override;
		virtual void CancelAction() override;
		virtual void Undo() override;
		virtual void Redo() override;
	};

}
