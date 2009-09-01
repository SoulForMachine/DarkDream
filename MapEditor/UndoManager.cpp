
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
	}

	void UndoManager::Redo()
	{
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
