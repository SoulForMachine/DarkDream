
#pragma once



namespace MapEditor
{

	ref class Action;


	public ref class UndoManager
	{
	public:
		UndoManager();
		~UndoManager();

		void Add(Action^ action);
		void Undo();
		void Redo();
		void Clear();
		bool HasUndo()
			{ return (_undoList->Count > 0); }
		bool HasRedo()
			{ return (_redoList->Count > 0); }

	private:
		void ClearList(System::Collections::Generic::LinkedList<Action^>^ list);

		System::Collections::Generic::LinkedList<Action^>^ _undoList;
		System::Collections::Generic::LinkedList<Action^>^ _redoList;
	};

}
