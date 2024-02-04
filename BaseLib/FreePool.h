
#ifndef _FREEPOOL_H_
#define _FREEPOOL_H_

#include "Memory.h"



// fix-sized pool for allocating single objects of same type;
// allocation and deallocation is allowed in any order


template <class _Type>
class FreePool
{
public:
	explicit FreePool(size_t size, Memory::Allocator& allocator = Memory::mainPool)
		: m_allocator(allocator)
	{
		Init(size);
	}
	~FreePool()
	{
		Memory::Delete(m_buffer);
		Memory::Delete(m_allocStack);
	}

	_Type* New()
	{
		if(m_top < m_size)
		{
			return new(m_allocStack[m_top++]) _Type;
		}
		else
			return 0;
	}

	_Type* New(const _Type& t)
	{
		if(m_top < m_size)
		{
			return new(m_allocStack[m_top++]) _Type(t);
		}
		else
			return 0;
	}

	void Delete(_Type* ptr)
	{
		ptr->~_Type();
		m_allocStack[--m_top] = ptr;
	}

	size_t GetFreeCount() { return m_size - m_top; }
	size_t GetAllocCount() { return m_top; }

	void Reset();

private:
	_Type* m_buffer;
	_Type** m_allocStack;
	size_t m_top;
	size_t m_size;
	Memory::Allocator& m_allocator;

	void Init(size_t size);
};


// WARNING: this function quickly reclaims all allocated objects;
// it should be used with objects with trivial destructors
// only, after making sure they are not in use any more
template <class _Type>
void FreePool<_Type>::Reset()
{
	if(m_top > 0)
	{
		m_top = 0;

		for(size_t i = 0; i < m_size; ++i)
			m_allocStack[i] = &m_buffer[i];
	}
}


template <class _Type>
void FreePool<_Type>::Init(size_t size)
{
	m_top = 0;
	m_size = size;

	m_buffer = reinterpret_cast<_Type*>(Memory::NewArray<ubyte>(m_allocator, size * sizeof(_Type)));
	m_allocStack = Memory::NewArray<_Type*>(m_allocator, size);

	for(size_t i = 0; i < size; ++i)
		m_allocStack[i] = &m_buffer[i];
}


#endif //_FREEPOOL_H_
