
#pragma once

namespace MapEditor
{

	ref class Action abstract
	{
	public:
		virtual void BeginAction() = 0;
		virtual void EndAction() = 0;
		virtual void CancelAction() = 0;
		virtual void Undo() = 0;
		virtual void Redo() = 0;
	};

}
