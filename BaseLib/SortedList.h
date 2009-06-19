
#ifndef _SORTED_LIST_H_
#define _SORTED_LIST_H_

#include "List.h"


template <class _Type>
class SortedList: private List<_Type>
{
public:
	explicit SortedList(Memory::Allocator& pool = Memory::mainPool)
		:List(pool) { }

	// operations
	void Insert(const _Type& val);
	using List::Remove;
	using List::Clear;

	// access
	using List::Begin;
	using List::End;
	using List::GetHead;
	using List::GetTail;
	using List::GetByIndex;
	using List::GetCount;
	using List::IsEmpty;

};

//=======================================================

// operations
template <class _Type>
void SortedList<_Type>::Insert(const _Type& val);
{
	
}

#endif // _SORTED_LIST_H_
