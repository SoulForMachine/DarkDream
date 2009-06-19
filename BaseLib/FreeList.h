

#ifndef _FREELIST_H_
#define _FREELIST_H_

#include <cassert>
#include "FreePool.h"


// List container that uses FreePool allocator;
// multiple lists can share the same pool, which
// is not thread safe though


template <class _Type>
struct FreeListNode
{
	FreeListNode(const _Type& val): data(val) {}
	FreeListNode* next;
	FreeListNode* prev;
	_Type data;
};


template <class _Type>
class FreeList
{
public:
	typedef FreeListNode<_Type> NodeType;
	typedef FreePool<NodeType> FreePoolType;

	class Iterator;
	friend class Iterator;
	class ConstIterator;
	friend class ConstIterator;

	explicit FreeList(FreePoolType& pool);
	FreeList(const FreeList& list);
	~FreeList();

	FreeList& operator = (const FreeList& list);

	// operations
	void PushBack(const _Type& val);
	void PopBack();
	void PushFront(const _Type& val);
	void PopFront();
	void Insert(Iterator where, const _Type& val);
	void Remove(Iterator it);
	void Remove(int index);
	void Clear();
	void Reset();

	// access
	Iterator Begin() { return Iterator(_end->next); }
	ConstIterator Begin() const { return ConstIterator(_end->next); }
	Iterator End() { return Iterator(_end); }
	ConstIterator End() const { return ConstIterator(_end); }
	_Type& GetHead() { return _end->next->data; }
	_Type& GetTail() { return _end->prev->data; }
	_Type& GetByIndex(int index);
	int GetCount() const { return _count; }
	bool IsEmpty() const { return _count == 0; }

private:
	FreePoolType& _pool;
	// this node is for one element beyond the tail.
	// next points to head, prev points to tail
	struct
	{
		NodeType* next;
		NodeType* prev;
	} _endDummy;
	NodeType* _end;
	int _count;

	void _Insert(NodeType* where, const _Type& val);
	void _Remove(NodeType* node);
};

//=======================================================
// iterators


template <class _Type>
class FreeList<_Type>::ConstIterator
{
public:
	friend class FreeList<_Type>;

	explicit ConstIterator(FreeListNode<_Type>* node) { _node = node; }

	const _Type& operator * () { return _node->data; }
	const _Type* operator -> () { return &_node->data; }

	ConstIterator& operator ++ () 
	{
		_node = _node->next;
		return *this;
	}
	ConstIterator operator ++ (int) 
	{
		ConstIterator oldval(_node);
		_node = _node->next;
		return oldval; 
	}
	ConstIterator& operator -- () 
	{
		_node = _node->prev;
		return *this;
	}
	ConstIterator operator -- (int) 
	{
		ConstIterator oldval(_node);
		_node = _node->prev;
		return oldval; 
	}

	bool operator != (const ConstIterator& it) const { return _node != it._node; }
	bool operator == (const ConstIterator& it) const { return _node == it._node; }

protected:
	FreeListNode<_Type>* _node;
};


template <class _Type>
class FreeList<_Type>::Iterator: public FreeList<_Type>::ConstIterator
{
public:
	friend class FreeList<_Type>;

	explicit Iterator(FreeListNode<_Type>* node):
		ConstIterator(node)
	{ _node = node; }

	_Type& operator * () { return _node->data; }
	_Type* operator -> () { return &_node->data; }

	Iterator& operator ++ () 
	{
		_node = _node->next;
		return *this;
	}
	Iterator operator ++ (int) 
	{
		Iterator oldval(_node);
		_node = _node->next;
		return oldval; 
	}
	Iterator& operator -- () 
	{
		_node = _node->prev;
		return *this; 
	}
	Iterator operator -- (int) 
	{
		Iterator oldval(_node);
		_node = _node->prev;
		return oldval; 
	}
};


//=======================================================



template <class _Type>
inline
FreeList<_Type>::FreeList(FreePoolType& pool):
	_pool(pool),
	_end((NodeType*)&_endDummy),
	_count(0)
{
	_end->next = _end;
	_end->prev = _end;
}

template <class _Type>
inline
FreeList<_Type>::FreeList(const FreeList<_Type>& list):
	_pool(list._pool),
	_end((NodeType*)&_endDummy),
	_count(0)
{
	*this = list;
}

template <class _Type>
inline
FreeList<_Type>::~FreeList()
{
	Clear();
}


template <class _Type>
FreeList<_Type>& FreeList<_Type>::operator = (const FreeList& list)
{
	Clear();

	for(ConstIterator it = list.Begin(); it != list.End(); ++it)
	{
		PushBack(*it);
	}

	return *this;
}


// operations
template <class _Type>
inline
void FreeList<_Type>::PushBack(const _Type& val)
{
	_Insert(_end, val);
}

template <class _Type>
inline
void FreeList<_Type>::PopBack()
{
	_Remove(_end->prev);
}

template <class _Type>
inline
void FreeList<_Type>::PushFront(const _Type& val)
{
	_Insert(_end->next, val);
}

template <class _Type>
inline
void FreeList<_Type>::PopFront()
{
	_Remove(_end->next);
}

template <class _Type>
inline
void FreeList<_Type>::Insert(Iterator where, const _Type& val)
{
	_Insert(where._node, val);
}

template <class _Type>
inline
void FreeList<_Type>::Remove(Iterator it)
{
	_Remove(it._node);
}

template <class _Type>
void FreeList<_Type>::Remove(int index)
{
	for(Iterator it = Begin(); it != End(); ++it)
	{
		if(index-- == 0)
		{
			_Remove(it._node);
			break;
		}
	}
}

template <class _Type>
void FreeList<_Type>::Clear()
{
	for(Iterator it = Begin(); it != End(); ++it)
		_pool.Delete(it._node);

	_end->next = _end;
	_end->prev = _end;
	_count = 0;
}

template <class _Type>
void FreeList<_Type>::Reset()
{
	_pool.Reset();

	_end->next = _end;
	_end->prev = _end;
	_count = 0;
}

template <class _Type>
_Type& FreeList<_Type>::GetByIndex(int index)
{
	assert(index < _count);

	NodeType* node = _end->next;

	for(int i = 0; i < index; ++i)
		node = node->next;

	return node->data;
}

template <class _Type>
void FreeList<_Type>::_Insert(NodeType* where, const _Type& val)
{
	NodeType* new_node = _pool.New(val);
	assert(new_node);

	new_node->next = where;
	new_node->prev = where->prev;
	where->prev->next = new_node;
	where->prev = new_node;

	++_count;
}

template <class _Type>
void FreeList<_Type>::_Remove(NodeType* node)
{
	if(node != _end)
	{
		NodeType* next = node->next;
		NodeType* prev = node->prev;
		next->prev = prev;
		prev->next = next;

		_pool.Delete(node);

		--_count;
	}
}


#endif //_FREELIST_H_
