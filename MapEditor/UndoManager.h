
#pragma once


namespace MapEditor
{

	ref class Action;


	ref class UndoManager
	{
	public:
		void Add(Action^ action);
		void Undo();
		void Redo();

	private:
		System::Collections::Generic::LinkedList<Action^>^ _undoList;
		System::Collections::Generic::LinkedList<Action^>^ _redoList;
	};

}
