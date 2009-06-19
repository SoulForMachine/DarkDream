
#ifndef _FREESTACKPOOL_H_
#define _FREESTACKPOOL_H_

#include "Memory.h"



// fix-sized pool for allocating single objects of same type;
// allocates objects by poping them from the stack, and deallocation
// must happen in reverse order; typical usage would dealocate all objects at once


template <class _Type>
class FreeStackPool
{
public:
	explicit FreeStackPool(size_t size, Memory::Allocator& allocator = Memory::mainPool)
		: m_allocator(allocator)
	{
		Init(size);
	}
	~FreeStackPool()
	{
		delete[] m_stack;
	}

	_Type* New()
	{
		if(m_top < m_size)
		{
		#if (_DEBUG_MEM_ALLOC)
			#undef new
		#endif

			return new(&m_stack[m_top++]) _Type;

		#if (_DEBUG_MEM_ALLOC)
			#define new(allocator)	new((allocator), __FILE__, __LINE__)
		#endif
		}
		else
			return 0;
	}

	_Type* New(const _Type& t)
	{
		if(m_top < m_size)
		{
		#if (_DEBUG_MEM_ALLOC)
			#undef new
		#endif

			return new(&m_stack[m_top++]) _Type(t);

		#if (_DEBUG_MEM_ALLOC)
			#define new(allocator)	new((allocator), __FILE__, __LINE__)
		#endif
		}
		else
			return 0;
	}

	// delete object from top of stack
	void Delete()
	{
		if(m_top > 0)
			m_stack[m_top--]->~_Type();
	}

	size_t GetFreeCount() { return m_size - m_top; }
	size_t GetAllocCount() { return m_top; }

	void Reset();

private:
	_Type* m_stack;
	size_t m_top;
	size_t m_size;
	Memory::Allocator& m_allocator;

	void Init(size_t size);
};


// WARNING: this function quickly reclaims all allocated objects;
// it should be used with objects with trivial destructors
// only, after making sure they are not in use any more
template <class _Type>
inline
void FreeStackPool<_Type>::Reset()
{
	m_top = 0;
}


template <class _Type>
void FreeStackPool<_Type>::Init(size_t size)
{
	m_top = 0;
	m_size = size;

	m_stack = (_Type*)new(m_allocator) ubyte[sizeof(_Type) * size];
}


#endif //_FREESTACKPOOL_H_
