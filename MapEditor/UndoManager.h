
#pragma once

using namespace System::Collections::Generic;


namespace MapEditor
{

	ref class Action;


	ref class UndoManager
	{
	public:
		UndoManager();
		~UndoManager();

		void Add(Action^ action);
		void Undo();
		void Redo();
		void Clear();

	private:
		void ClearList(LinkedList<Action^>^ list);

		LinkedList<Action^>^ _undoList;
		LinkedList<Action^>^ _redoList;
	};

}
