
#ifndef _BASELIB_HASH_MAP_H_
#define _BASELIB_HASH_MAP_H_

#include <cassert>
#include "Common.h"
#include "Hashing.h"
#include "Memory.h"


template <class _ValueType>
struct HashMapNode
{
	HashMapNode()
		{ endNode = 0; }
	HashMapNode(const HashMapNode& node):
		value(node.value)
	{
		hash = hash;
		balanceFactor = node.balanceFactor;
		endNode = 0;
	}

	HashMapNode* parent;
	HashMapNode* left;
	HashMapNode* right;
	ubyte endNode;
	char balanceFactor;
	uint hash;
	_ValueType value;
};

template <class _KeyType, class _ValueType>
class DefaultHashMapTraits
{
public:
	DefaultHashMapTraits(Memory::Allocator& pool, size_t size)
		: _pool(pool) {}
	HashMapNode<_ValueType>* New()
		{ return new(_pool) HashMapNode<_ValueType>; }
	void Delete(HashMapNode<_ValueType>* ptr)
		{ delete ptr; }
	uint GetHash(const _KeyType& key) const
		{ return ::GetHash(key); }

private:
	Memory::Allocator& _pool;
};


template <class _KeyType, class _ValueType, class _Traits = DefaultHashMapTraits<_KeyType, _ValueType> >
class HashMap
{
public:
	typedef HashMapNode<_ValueType> NodeType;

	class Iterator;
	friend class Iterator;
	class ConstIterator;
	friend class ConstIterator;

	explicit HashMap(Memory::Allocator& pool = Memory::mainPool, size_t max_size = -1);
	HashMap(const HashMap& hash_map, Memory::Allocator& pool = Memory::mainPool, size_t max_size = -1);
	~HashMap();

	HashMap& operator = (const HashMap& hash_map);
	Iterator Begin() { return Iterator(_headPtr->left); }
	ConstIterator Begin() const { return ConstIterator(_headPtr->left); }
	Iterator End() { return Iterator(_headPtr); }
	ConstIterator End() const { return ConstIterator(_headPtr); }
	_ValueType& operator [] (const _KeyType& key);
	size_t GetCount() const { return _count; }
	bool IsEmpty() const { return _count == 0; }
	ConstIterator Find(const _KeyType& key) const;
	Iterator Find(const _KeyType& key);
	void Remove(const _KeyType& key);
	void Remove(ConstIterator it);
	void Clear();

private:
	NodeType* _Find(uint hash, HashMapNode<_ValueType>* node) const;
	NodeType* _FindOrAdd(uint hash, HashMapNode<_ValueType>* node);
	NodeType* _AddNewLeft(NodeType* node, uint hash);
	NodeType* _AddNewRight(NodeType* node, uint hash);
	NodeType* _AddRoot(uint hash);
	void _DeleteTree(NodeType* node);
	static bool _IsEnd(NodeType* node);
	void _RotateLeft(NodeType* node);
	void _RotateRight(NodeType* node);
	void _RebalanceAfterAdd(NodeType* node);
	void _RebalanceAfterRemove(NodeType* node, bool left_removed);
	void _CopyTree(NodeType* source, NodeType*& dest);
	void _CopyNode(NodeType* source, NodeType*& dest);

	struct
	{
		NodeType* parent;
		NodeType* left;
		NodeType* right;
		ubyte endNode;
	}_head;

	NodeType* _headPtr;
	size_t _count;
	_Traits _traits;
};


//=======================================================
// iterators


template <class _KeyType, class _ValueType, class _Traits>
class HashMap<_KeyType, _ValueType, _Traits>::ConstIterator
{
public:
	friend class HashMap<_KeyType, _ValueType, _Traits>;

	explicit ConstIterator(HashMapNode<_ValueType>* node) { _node = node; }

	const _ValueType& operator * () { return _node->value; }
	const _ValueType* operator -> () { return &_node->value; }

	ConstIterator& operator ++ () 
	{
		_Inc();
		return *this;
	}
	ConstIterator operator ++ (int) 
	{
		ConstIterator oldval(_node);
		_Inc();
		return oldval; 
	}
	ConstIterator& operator -- () 
	{
		_Dec();
		return *this;
	}
	ConstIterator operator -- (int) 
	{
		ConstIterator oldval(_node);
		_Dec();
		return oldval;
	}

	bool operator != (const ConstIterator& it) const { return _node != it._node; }
	bool operator == (const ConstIterator& it) const { return _node == it._node; }

protected:
	void _Inc();
	void _Dec();

	HashMapNode<_ValueType>* _node;
};

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::ConstIterator::_Inc()
{
	if(_IsEnd(_node))
	{
		assert(false);
	}
	else if(!_IsEnd(_node->right))
	{
		HashMapNode<_ValueType>* ptr = _node->right;
		while(!_IsEnd(ptr->left))
			ptr = ptr->left;
		_node = ptr;
	}
	else
	{
		HashMapNode<_ValueType>* ptr;
		while(!_IsEnd(ptr = _node->parent) && _node == ptr->right)
			_node = ptr;
		_node = ptr;
	}
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::ConstIterator::_Dec()
{
	if(_node == 0)
	{
		assert(false);
	}
	else if(_node->left)
	{
		HashMapNode<_ValueType>* ptr = _node->left;
		while(ptr->right)
			ptr = ptr->right;
		_node = ptr;
	}
	else
	{
		HashMapNode<_ValueType>* ptr;
		while(!_IsEnd(ptr = _node->parent) && _node == ptr->left)
			_node = ptr;
		_node = ptr;
	}
}


template <class _KeyType, class _ValueType, class _Traits>
class HashMap<_KeyType, _ValueType, _Traits>::Iterator: public HashMap<_KeyType, _ValueType, _Traits>::ConstIterator
{
public:
	friend class HashMap<_KeyType, _ValueType, _Traits>;

	explicit Iterator(HashMapNode<_ValueType>* node):
		ConstIterator(node)
	{ _node = node; }

	_ValueType& operator * () { return _node->value; }
	_ValueType* operator -> () { return &_node->value; }

	Iterator& operator ++ () 
	{
		_Inc();
		return *this;
	}
	Iterator operator ++ (int) 
	{
		Iterator oldval(_node);
		_Inc();
		return oldval; 
	}
	Iterator& operator -- () 
	{
		_Dec();
		return *this;
	}
	Iterator operator -- (int) 
	{
		Iterator oldval(_node);
		_Dec();
		return oldval; 
	}
};


//=======================================================


template <class _KeyType, class _ValueType, class _Traits>
inline
HashMap<_KeyType, _ValueType, _Traits>::HashMap(Memory::Allocator& pool, size_t max_size):
	_traits(pool, max_size),
	_headPtr((NodeType*)&_head)
{
	_count = 0;
	_headPtr->parent = _headPtr;
	_headPtr->left = _headPtr;
	_headPtr->right = _headPtr;
	_headPtr->endNode = 1;
}

template <class _KeyType, class _ValueType, class _Traits>
inline
HashMap<_KeyType, _ValueType, _Traits>::HashMap(const HashMap& hash_map, Memory::Allocator& pool, size_t max_size):
	_traits(pool, max_size),
	_headPtr((NodeType*)&_head)
{
	_count = 0;
	_headPtr->parent = _headPtr;
	_headPtr->left = _headPtr;
	_headPtr->right = _headPtr;
	_headPtr->endNode = 1;
	*this = hash_map;
}

template <class _KeyType, class _ValueType, class _Traits>
inline
HashMap<_KeyType, _ValueType, _Traits>::~HashMap()
{
	Clear();
}

template <class _KeyType, class _ValueType, class _Traits>
inline
HashMap<_KeyType, _ValueType, _Traits>&
HashMap<_KeyType, _ValueType, _Traits>::operator = (const HashMap<_KeyType, _ValueType, _Traits>& hash_map)
{
	Clear();
	_count = hash_map._count;
	_CopyTree(hash_map._headPtr->parent, _headPtr->parent);
	return *this;
}

template <class _KeyType, class _ValueType, class _Traits>
_ValueType& HashMap<_KeyType, _ValueType, _Traits>::operator [] (const _KeyType& key)
{
	uint hash = _traits.GetHash(key);
	if(_headPtr->parent != _headPtr)
	{
		NodeType* node = _FindOrAdd(hash, _headPtr->parent);
		return node->value;
	}
	else
	{
		NodeType* node = _AddRoot(hash);
		return node->value;
	}
}

template <class _KeyType, class _ValueType, class _Traits>
typename HashMap<_KeyType, _ValueType, _Traits>::ConstIterator
HashMap<_KeyType, _ValueType, _Traits>::Find(const _KeyType& key) const
{
	if(_headPtr->parent != _headPtr)
	{
		uint hash = _traits.GetHash(key);
		NodeType* node = _Find(hash, _headPtr->parent);
		return node? ConstIterator(node): ConstIterator(_headPtr);
	}
	else
	{
		return ConstIterator(_headPtr);
	}
}

template <class _KeyType, class _ValueType, class _Traits>
typename HashMap<_KeyType, _ValueType, _Traits>::Iterator
HashMap<_KeyType, _ValueType, _Traits>::Find(const _KeyType& key)
{
	if(_headPtr->parent != _headPtr)
	{
		uint hash = _traits.GetHash(key);
		NodeType* node = _Find(hash, _headPtr->parent);
		return node? Iterator(node): Iterator(_headPtr);
	}
	else
	{
		return Iterator(_headPtr);
	}
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::Remove(const _KeyType& key)
{
	assert(_headPtr->parent != _headPtr);

	uint hash = _traits.GetHash(key);
	NodeType* node = _Find(hash, _headPtr->parent);
	assert(node);
	if(node)
		Remove(ConstIterator(node));
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::Remove(ConstIterator it)
{
	assert(_headPtr->parent != _headPtr); // removing from empty tree

	bool left_removed;
	NodeType* node = it._node;
	NodeType* ptr = 0;
	NodeType* parent;
	if(!_IsEnd(node->left))
	{
		// find the rightmost node in left subtree
		ptr = node->left;
		while(!_IsEnd(ptr->right))
			ptr = ptr->right;
	}
	else if(!_IsEnd(node->right))
	{
		// find the leftmost node in right subtree
		ptr = node->right;
		while(!_IsEnd(ptr->left))
			ptr = ptr->left;
	}
	else
	{
		parent = node->parent;
		if(parent == _headPtr)
		{
			_headPtr->parent = _headPtr;
			_headPtr->left = _headPtr;
			_headPtr->right = _headPtr;
		}
		else
		{
			if(parent->left == node)
			{
				left_removed = true;
				parent->left = node->left;
				if(node->left == _headPtr)
					_headPtr->left = parent;
			}
			else
			{
				left_removed = false;
				parent->right = node->right;
				if(node->right == _headPtr)
					_headPtr->right = parent;
			}
		}
	}

	// if ptr is not 0, replace node with ptr
	if(ptr)
	{
		// unlink ptr
		parent = ptr->parent;
		if(parent->left == ptr)
		{
			left_removed = true;
			if(ptr->left == _headPtr)
			{
				parent->left = _headPtr;
				_headPtr->left = parent;
			}
			else if(ptr->left)
			{
				parent->left = ptr->left;
				ptr->left->parent = parent;
			}
			else
			{
				parent->left = ptr->right;
				if(ptr->right)
					ptr->right->parent = parent;
			}
		}
		else
		{
			left_removed = false;
			if(ptr->right == _headPtr)
			{
				parent->right = _headPtr;
				_headPtr->right = parent;
			}
			else if(ptr->right)
			{
				parent->right = ptr->right;
				ptr->right->parent = parent;
			}
			else
			{
				parent->right = ptr->left;
				if(ptr->left)
					ptr->left->parent = parent;
			}
		}

		if(ptr->parent == node)
			parent = ptr;

		// link that node in place of node that is to be removed
		ptr->parent = node->parent;
		if(node->parent == _headPtr)
		{
			_headPtr->parent = ptr;
		}
		else
		{
			if(node->parent->left == node)
				node->parent->left = ptr;
			else
				node->parent->right = ptr;
		}
		ptr->left = node->left;
		if(node->left)
		{
			if(node->left == _headPtr)
				_headPtr->left = ptr;
			else
				node->left->parent = ptr;
		}
		ptr->right = node->right;
		if(node->right)
		{
			if(node->right == _headPtr)
				_headPtr->right = ptr;
			else
				node->right->parent = ptr;
		}
		ptr->balanceFactor = node->balanceFactor;
	}

	// remove the node
	_traits.Delete(node);
	--_count;

	if(parent != _headPtr)
		_RebalanceAfterRemove(parent, left_removed);
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::Clear()
{
	if(_headPtr->parent != _headPtr)
	{
		_DeleteTree(_headPtr->parent);
		_headPtr->parent = _headPtr;
	}
	_headPtr->left = _headPtr;
	_headPtr->right = _headPtr;
	_count = 0;
}

template <class _KeyType, class _ValueType, class _Traits>
typename HashMap<_KeyType, _ValueType, _Traits>::NodeType*
HashMap<_KeyType, _ValueType, _Traits>::_Find(uint hash, HashMapNode<_ValueType>* node) const
{
	if(hash == node->hash)
		return node;
	else if(hash < node->hash)
		return _IsEnd(node->left)? 0: _Find(hash, node->left);
	else
		return _IsEnd(node->right)? 0: _Find(hash, node->right);
}

template <class _KeyType, class _ValueType, class _Traits>
typename HashMap<_KeyType, _ValueType, _Traits>::NodeType*
HashMap<_KeyType, _ValueType, _Traits>::_FindOrAdd(uint hash, HashMapNode<_ValueType>* node)
{
	if(hash == node->hash)
	{
		return node;
	}
	else if(hash < node->hash)
	{
		if(_IsEnd(node->left))
		{
			NodeType* new_node = _AddNewLeft(node, hash);
			return new_node;
		}
		else
		{
			return _FindOrAdd(hash, node->left);
		}
	}
	else
	{
		if(_IsEnd(node->right))
		{
			NodeType* new_node = _AddNewRight(node, hash);
			return new_node;
		}
		else
		{
			return _FindOrAdd(hash, node->right);
		}
	}
}

template <class _KeyType, class _ValueType, class _Traits>
typename HashMap<_KeyType, _ValueType, _Traits>::NodeType*
HashMap<_KeyType, _ValueType, _Traits>::_AddNewLeft(NodeType* node, uint hash)
{
	HashMapNode<_ValueType>* new_node = _traits.New();
	assert(new_node);
	node->left = new_node;
	new_node->parent = node;
	new_node->right = 0;
	new_node->hash = hash;
	new_node->balanceFactor = 0;
	if(node == _headPtr->left)
	{
		_headPtr->left = new_node;
		new_node->left = _headPtr;
	}
	else
	{
		new_node->left = 0;
	}
	_count++;
	_RebalanceAfterAdd(new_node);
	return new_node;
}

template <class _KeyType, class _ValueType, class _Traits>
typename HashMap<_KeyType, _ValueType, _Traits>::NodeType*
HashMap<_KeyType, _ValueType, _Traits>::_AddNewRight(NodeType* node, uint hash)
{
	HashMapNode<_ValueType>* new_node = _traits.New();
	assert(new_node);
	node->right = new_node;
	new_node->parent = node;
	new_node->left = 0;
	new_node->hash = hash;
	new_node->balanceFactor = 0;
	if(node == _headPtr->right)
	{
		_headPtr->right = new_node;
		new_node->right = _headPtr;
	}
	else
	{
		new_node->right = 0;
	}
	_count++;
	_RebalanceAfterAdd(new_node);
	return new_node;
}

template <class _KeyType, class _ValueType, class _Traits>
typename HashMap<_KeyType, _ValueType, _Traits>::NodeType*
HashMap<_KeyType, _ValueType, _Traits>::_AddRoot(uint hash)
{
	HashMapNode<_ValueType>* new_node = _traits.New();
	assert(new_node);
	_headPtr->parent = new_node;
	_headPtr->left = new_node;
	_headPtr->right = new_node;
	new_node->parent = _headPtr;
	new_node->left = _headPtr;
	new_node->right = _headPtr;
	new_node->hash = hash;
	new_node->balanceFactor = 0;
	_count++;
	return new_node;
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::_DeleteTree(NodeType* node)
{
	if(!_IsEnd(node->left))
		_DeleteTree(node->left);

	if(!_IsEnd(node->right))
		_DeleteTree(node->right);

	_traits.Delete(node);
}

template <class _KeyType, class _ValueType, class _Traits>
inline
bool HashMap<_KeyType, _ValueType, _Traits>::_IsEnd(NodeType* node)
{
	return (!node || node->endNode);
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::_RotateLeft(NodeType* node)
{
	NodeType* pivot = node->right;
	node->right = pivot->left;
	if(pivot->left)
		pivot->left->parent = node;
	pivot->left = node;
	pivot->parent = node->parent;
	if(node->parent != _headPtr)
	{
		if(node->parent->left == node)
			node->parent->left = pivot;
		else
			node->parent->right = pivot;
	}
	else
	{
		// node is a root node
		_headPtr->parent = pivot;
	}
	node->parent = pivot;

	// calculate new balance factors for root and pivot
	node->balanceFactor -= (1 + Max<char>(0, pivot->balanceFactor));
	pivot->balanceFactor -= (1 - Min<char>(0, node->balanceFactor));
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::_RotateRight(NodeType* node)
{
	NodeType* pivot = node->left;
	node->left = pivot->right;
	if(pivot->right)
		pivot->right->parent = node;
	pivot->right = node;
	pivot->parent = node->parent;
	if(node->parent != _headPtr)
	{
		if(node->parent->left == node)
			node->parent->left = pivot;
		else
			node->parent->right = pivot;
	}
	else
	{
		// node is a root node
		_headPtr->parent = pivot;
	}
	node->parent = pivot;

	// calculate new balance factors for root and pivot
	node->balanceFactor += (1 - Min<char>(0, pivot->balanceFactor));
	pivot->balanceFactor += (1 + Max<char>(0, node->balanceFactor));
}

/*
	Rebalance the tree after inserting a node.
*/
template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::_RebalanceAfterAdd(NodeType* node)
{
	NodeType* ptr = node;
	NodeType* parent = node->parent;
	while(parent != _headPtr)
	{
		if(ptr == parent->left)
		{
			// new node was added to the left sub-tree of this node
			--parent->balanceFactor;
			if(parent->balanceFactor == -2)
			{
				if(ptr->balanceFactor == 1)
				{
					ptr = ptr->right;
					_RotateLeft(ptr->parent);
				}
				_RotateRight(parent);
				parent = ptr;
			}
		}
		else
		{
			// new node was added to the right sub-tree of this node
			++parent->balanceFactor;
			if(parent->balanceFactor == 2)
			{
				if(ptr->balanceFactor == -1)
				{
					ptr = ptr->left;
					_RotateRight(ptr->parent);
				}
				_RotateLeft(parent);
				parent = ptr;
			}
		}

		if(parent->balanceFactor == 0)
			break;
		ptr = parent;
		parent = parent->parent;
	}
}

/*
	Rebalance the tree after removing a node. Parameter is removed node's parent.
*/
template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::_RebalanceAfterRemove(NodeType* node, bool left_removed)
{
	NodeType* parent = node;
	NodeType* ptr;

	if(left_removed)
	{
		// left node was removed
		++parent->balanceFactor;
		if(parent->balanceFactor == 2)
		{
			ptr = node->right;
			if(ptr->balanceFactor == -1)
			{
				ptr = ptr->left;
				_RotateRight(ptr->parent);
			}
			_RotateLeft(parent);
			parent = ptr;
		}
	}
	else
	{
		// right node was removed
		--parent->balanceFactor;
		if(parent->balanceFactor == -2)
		{
			ptr = node->left;
			if(ptr->balanceFactor == 1)
			{
				ptr = ptr->right;
				_RotateLeft(ptr->parent);
			}
			_RotateRight(parent);
			parent = ptr;
		}
	}

	if(parent->balanceFactor != 0)
		return;

	ptr = parent;
	parent = parent->parent;

	while(parent != _headPtr)
	{
		if(ptr == parent->left)
		{
			// node was removed from the left sub-tree of this node
			++parent->balanceFactor;
			if(parent->balanceFactor == 2)
			{
				ptr = parent->right;
				if(ptr->balanceFactor == -1)
				{
					ptr = ptr->left;
					_RotateRight(ptr->parent);
				}
				_RotateLeft(parent);
				parent = ptr;
			}
		}
		else
		{
			// node was removed form the right sub-tree of this node
			--parent->balanceFactor;
			if(parent->balanceFactor == -2)
			{
				ptr = parent->left;
				if(ptr->balanceFactor == 1)
				{
					ptr = ptr->right;
					_RotateLeft(ptr->parent);
				}
				_RotateRight(parent);
				parent = ptr;
			}
		}

		if(parent->balanceFactor != 0)
			break;
		ptr = parent;
		parent = parent->parent;
	}
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::_CopyTree(NodeType* source, NodeType*& dest)
{
	_CopyNode(source, dest);

	if(source->left)
	{
		if(source->left->endNode)
		{
			dest->left = _headPtr;
			_headPtr->left = dest;
		}
		else
		{
			_CopyTree(source->left, dest->left);
		}
	}
	if(source->right)
	{
		if(source->right->endNode)
		{
			dest->right = _headPtr;
			_headPtr->right = dest;
		}
		else
		{
			_CopyTree(source->right, dest->right);
		}
	}
}

template <class _KeyType, class _ValueType, class _Traits>
void HashMap<_KeyType, _ValueType, _Traits>::_CopyNode(NodeType* source, NodeType*& dest)
{
	dest = _traits.New();
	assert(dest);
	*dest = *source;
	dest->left = 0;
	dest->right = 0;
}


#endif // _BASELIB_HASH_MAP_H_
