
#pragma once

namespace MapEditor
{

	public ref class Action abstract
	{
	public:
		virtual bool BeginAction() = 0;
		virtual void EndAction() = 0;
		virtual void Update(float dt) = 0;
		virtual void CancelAction() = 0;
		virtual void Undo() = 0;
		virtual void Redo() = 0;
	};

}
