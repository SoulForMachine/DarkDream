
#include "stdafx.h"
#include "Action.h"
#include "UndoManager.h"


namespace MapEditor
{

	UndoManager::UndoManager()
	{
		_undoList = gcnew LinkedList<Action^>;
		_redoList = gcnew LinkedList<Action^>;
	}

	UndoManager::~UndoManager()
	{
		Clear();
		delete _undoList;
		delete _redoList;
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
		}
	}

	void UndoManager::Clear()
	{
		ClearList(_undoList);
		ClearList(_redoList);
	}

	void UndoManager::ClearList(LinkedList<Action^>^ list)
	{
		for each(Action^ action in list)
			delete action;
		list->Clear();
	}

}
