
#pragma once

namespace MapEditor
{

	public ref class Action abstract
	{
	public:
		virtual bool BeginAction() = 0;
		virtual void EndAction() = 0;
		virtual void Update(float dt) {}
		virtual void CancelAction() {}
		virtual void Undo() = 0;
		virtual void Redo() = 0;
	};

}
