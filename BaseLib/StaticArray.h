
#ifndef _STATIC_ARRAY_H_
#define _STATIC_ARRAY_H_

#include <cassert>
#include "Memory.h"



template <class _T>
class StaticArray
{
public:
	explicit StaticArray(Memory::Allocator& allocator = Memory::mainPool);
	explicit StaticArray(size_t count, Memory::Allocator& allocator = Memory::mainPool);
	StaticArray(const StaticArray& other);
	~StaticArray();
	StaticArray& operator = (const StaticArray& other);

	void SetCount(size_t count);
	void Clear();

	_T& operator [] (size_t index);
	const _T& operator [] (size_t index) const;
	_T* GetData();
	const _T* GetData() const;
	size_t GetCount() const;

private:
	_T* _data;
	size_t _count;
	Memory::Allocator& _allocator;
};




template <class _T>
StaticArray<_T>::StaticArray(Memory::Allocator& allocator = Memory::mainPool)
	: _allocator(allocator)
{
	_count = 0;
	_data = 0;
}

template <class _T>
StaticArray<_T>::StaticArray(size_t count, Memory::Allocator& allocator)
	: _allocator(allocator)
{
	assert(count > 0);
	_count = 0;
	_data = 0;
	SetCount(count);
}

template <class _T>
StaticArray<_T>::StaticArray(const StaticArray& other)
	:_allocator(other._allocator)
{
	*this = other;
}

template <class _T>
inline
StaticArray<_T>::~StaticArray()
{
	Clear();
}

template <class _T>
StaticArray<_T>& StaticArray<_T>::operator = (const StaticArray& other)
{
	Clear();
	_count = other._count;
	_data = (_T*)new(_allocator) ubyte[_count * sizeof(_T)];
	for(size_t i = 0; i < _count; ++i)
		_data[i] = other._data[i];
	return *this;
}

template <class _T>
void StaticArray<_T>::SetCount(size_t count)
{
	if(_count != count)
	{
		Clear();
		_count = count;
		if(count)
			_data = new(_allocator) _T[count];
	}
}

template <class _T>
void StaticArray<_T>::Clear()
{
	delete[] _data;
	_count = 0;
	_data = 0;
}

template <class _T>
inline
_T& StaticArray<_T>::operator [] (size_t index)
{
	assert(index >= 0 && index < _count);
	return _data[index];
}

template <class _T>
inline
const _T& StaticArray<_T>::operator [] (size_t index) const
{
	assert(index >= 0 && index < _count);
	return _data[index];
}

template <class _T>
inline
_T* StaticArray<_T>::GetData()
{
	return _data;
}

template <class _T>
inline
const _T* StaticArray<_T>::GetData() const
{
	return _data;
}

template <class _T>
inline
size_t StaticArray<_T>::GetCount() const
{
	return _count;
}


#endif // _STATIC_ARRAY_H_
