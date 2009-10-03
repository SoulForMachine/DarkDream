
#pragma once



namespace MapEditor
{

	ref class Action;

	public interface class UndoEventListener
	{
		enum class EventType
		{
			UNDO,
			REDO,
			CLEAR
		};

		void UndoEvent(EventType type, Action^ action);
	};


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
		void RegisterListener(UndoEventListener^ listener);
		void UnregisterListener(UndoEventListener^ listener);

	private:
		void ClearList(System::Collections::Generic::LinkedList<Action^>^ list);
		void NotifyListeners(UndoEventListener::EventType type, Action^ action);

		System::Collections::Generic::LinkedList<Action^>^ _undoList;
		System::Collections::Generic::LinkedList<Action^>^ _redoList;
		System::Collections::Generic::LinkedList<UndoEventListener^>^ _eventListenerList;
	};

}
