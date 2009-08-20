
#ifndef _LIST_H_
#define _LIST_H_

#include <cassert>
#include "Memory.h"

// double-linked list


template <class _Type>
struct ListNode
{
	ListNode(const _Type& val): data(val) {}
	ListNode* next;
	ListNode* prev;
	_Type data;
};


template <class _Type>
class List
{
public:
	typedef ListNode<_Type> NodeType;

	class Iterator;
	friend class Iterator;
	class ConstIterator;
	friend class ConstIterator;

	explicit List(Memory::Allocator& pool = Memory::mainPool);
	List(const List& list);
	~List();

	List& operator = (const List& list);

	// operations
	void PushBack(const _Type& val);
	void PopBack();
	void PushFront(const _Type& val);
	void PopFront();
	void Insert(Iterator where, const _Type& val);
	void Remove(Iterator it);
	void Remove(size_t index);
	void Clear();

	// access
	Iterator Begin() { return Iterator(_end->next); }
	ConstIterator Begin() const { return ConstIterator(_end->next); }
	Iterator End() { return Iterator(_end); }
	ConstIterator End() const { return ConstIterator(_end); }
	_Type& GetHead() { return _end->next->data; }
	_Type& GetTail() { return _end->prev->data; }
	_Type& GetByIndex(size_t index);
	size_t GetCount() const { return _count; }
	bool IsEmpty() const { return _count == 0; }

protected:
	Memory::Allocator& _pool;
	// this node is for one element beyond the tail.
	// next points to head, prev points to tail
	struct
	{
		NodeType* next;
		NodeType* prev;
	} _endDummy;
	NodeType* _end;
	size_t _count;

private:
	void _Insert(NodeType* where, const _Type& val);
	void _Remove(NodeType* node);
};

//=======================================================
// iterators


template <class _Type>
class List<_Type>::ConstIterator
{
public:
	friend class List<_Type>;

	explicit ConstIterator(ListNode<_Type>* node) { _node = node; }

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
	ListNode<_Type>* _node;
};


template <class _Type>
class List<_Type>::Iterator: public List<_Type>::ConstIterator
{
public:
	friend class List<_Type>;

	explicit Iterator(ListNode<_Type>* node):
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
List<_Type>::List(Memory::Allocator& pool):
	_pool(pool),
	_end((NodeType*)&_endDummy),
	_count(0)
{
	_end->next = _end;
	_end->prev = _end;
}

template <class _Type>
inline
List<_Type>::List(const List<_Type>& list):
	_pool(list._pool),
	_end((NodeType*)&_endDummy),
	_count(0)
{
	*this = list;
}

template <class _Type>
inline
List<_Type>::~List()
{
	Clear();
}


template <class _Type>
List<_Type>& List<_Type>::operator = (const List& list)
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
void List<_Type>::PushBack(const _Type& val)
{
	_Insert(_end, val);
}

template <class _Type>
inline
void List<_Type>::PopBack()
{
	_Remove(_end->prev);
}

template <class _Type>
inline
void List<_Type>::PushFront(const _Type& val)
{
	_Insert(_end->next, val);
}

template <class _Type>
inline
void List<_Type>::PopFront()
{
	_Remove(_end->next);
}

template <class _Type>
inline
void List<_Type>::Insert(Iterator where, const _Type& val)
{
	_Insert(where._node, val);
}

template <class _Type>
inline
void List<_Type>::Remove(Iterator it)
{
	_Remove(it._node);
}

template <class _Type>
void List<_Type>::Remove(size_t index)
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
void List<_Type>::Clear()
{
	for(Iterator it = Begin(); it != End(); ++it)
		delete it._node;

	_end->next = _end;
	_end->prev = _end;
	_count = 0;
}

template <class _Type>
_Type& List<_Type>::GetByIndex(size_t index)
{
	assert(index < _count);

	NodeType* node = _end->next;

	for(size_t i = 0; i < index; ++i)
		node = node->next;

	return node->data;
}

template <class _Type>
void List<_Type>::_Insert(NodeType* where, const _Type& val)
{
	NodeType* new_node = new(_pool) NodeType(val);
	assert(new_node);

	new_node->next = where;
	new_node->prev = where->prev;
	where->prev->next = new_node;
	where->prev = new_node;

	++_count;
}

template <class _Type>
void List<_Type>::_Remove(NodeType* node)
{
	if(node != _end)
	{
		NodeType* next = node->next;
		NodeType* prev = node->prev;
		next->prev = prev;
		prev->next = next;

		delete node;

		--_count;
	}
}


#endif //_LIST_H_
