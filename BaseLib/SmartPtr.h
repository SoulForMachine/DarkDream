
#ifndef _BASELIB_SMART_POINTER_H_
#define _BASELIB_SMART_POINTER_H_

#include <cassert>
#include "Memory.h"


template <class _Type> class SmartPtr;

template <class _Type>
bool operator == (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2);
template <class _Type>
bool operator != (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2);
template <class _Type>
size_t operator - (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2);


/*
	Smart pointer class
*/
template <class _Type>
class SmartPtr
{
public:
	SmartPtr(_Type* ptr = 0)
		{ _ptr = ptr; }
	SmartPtr(const SmartPtr& other)
	{
		_ptr = 0;
		*this = other;
	}
	~SmartPtr()
	{
		Memory::Delete(_ptr);
	}
	SmartPtr& operator = (const SmartPtr& other)
	{
		*this = other._ptr;
		other._ptr = 0;
		return *this;
	}

	SmartPtr& operator = (_Type* ptr)
	{
		Memory::Delete(_ptr);
		_ptr = ptr;
		return *this;
	}

	_Type& operator * ()
	{
		assert(_ptr);
		return *_ptr;
	}
	operator _Type* ()
		{ return _ptr; }
	_Type* operator -> ()
	{
		assert(_ptr);
		return _ptr;
	}
	bool operator ! () const
		{ return !_ptr; }

	_Type* Release()
	{
		_Type* p = _ptr;
		_ptr = 0;
		return p;
	}

private:
	friend bool operator == <_Type> (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2);
	friend bool operator != <_Type> (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2);
	friend size_t operator - <_Type> (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2);

	mutable _Type* _ptr;
};


template <class _Type>
bool operator == (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2)
{
	return (ptr1._ptr == ptr2._ptr);
}

template <class _Type>
bool operator != (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2)
{
	return (ptr1._ptr != ptr2._ptr);
}

template <class _Type>
size_t operator - (const SmartPtr<_Type>& ptr1, const SmartPtr<_Type>& ptr2)
{
	return (ptr1._ptr - ptr2._ptr);
}



#endif //_BASELIB_SMART_POINTER_H_
