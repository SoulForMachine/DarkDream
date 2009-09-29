
#include "stdafx.h"
#include "Action.h"
#include "UndoManager.h"

using namespace System::Collections::Generic;


namespace MapEditor
{

	UndoManager::UndoManager()
	{
		_undoList = gcnew LinkedList<Action^>;
		_redoList = gcnew LinkedList<Action^>;
		_eventListenerList = gcnew LinkedList<UndoEventListener^>;
	}

	UndoManager::~UndoManager()
	{
		Clear();
		delete _undoList;
		delete _redoList;
		delete _eventListenerList;
	}

	void UndoManager::Add(Action^ action)
	{
		ClearList(_redoList);
		_undoList->AddLast(action);
	}

	void UndoManager::Undo()
	{
		if(_undoList->Count > 0)
		{
			Action^ action = _undoList->Last->Value;
			action->Undo();
			_undoList->RemoveLast();
			_redoList->AddLast(action);
			NotifyListeners(UndoEventListener::EventType::UNDO);
		}
	}

	void UndoManager::Redo()
	{
		if(_redoList->Count > 0)
		{
			Action^ action = _redoList->Last->Value;
			action->Redo();
			_redoList->RemoveLast();
			_undoList->AddLast(action);
			NotifyListeners(UndoEventListener::EventType::REDO);
		}
	}

	void UndoManager::Clear()
	{
		ClearList(_undoList);
		ClearList(_redoList);
		NotifyListeners(UndoEventListener::EventType::CLEAR);
	}

	void UndoManager::ClearList(LinkedList<Action^>^ list)
	{
		for each(Action^ action in list)
			delete action;
		list->Clear();
	}

	void UndoManager::RegisterListener(UndoEventListener^ listener)
	{
		_eventListenerList->AddLast(listener);
	}

	void UndoManager::UnregisterListener(UndoEventListener^ listener)
	{
		_eventListenerList->Remove(listener);
	}

	void UndoManager::NotifyListeners(UndoEventListener::EventType type)
	{
		for each(UndoEventListener^ listener in _eventListenerList)
		{
			listener->UndoEvent(type);
		}
	}

}
