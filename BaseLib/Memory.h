

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <new>
#include "Common.h"
#include "Types.h"


#ifdef _DEBUG
	#define _DEBUG_MEM_ALLOC	1
#endif


namespace Memory
{
	class Allocator;

	extern size_t pageSize;
	extern size_t allocGranularity;
	extern BASELIB_API Allocator* const pools[];

	const uchar ALLOCATED_BIT = 0x80;
	const uchar ALLOCATOR_INDEX_MASK = 0x07;

#pragma pack(push, 1)

	struct MemoryHeader
	{
		uchar flags; // bits: 0,1,2 - allocator index; 7 - allocated bit
		size_t blockSize;
		size_t userSize;
		MemoryHeader* next;
		MemoryHeader* prev;

#if (_DEBUG_MEM_ALLOC)
		const char* file;
		int line;
		uint* guard;
#endif
	};

#pragma pack(pop)

	class BASELIB_API Allocator
	{
	public:
		Allocator(const char* pool_name, int index);

		bool	Init(size_t address_space, size_t initial_size, size_t pages_to_grow, size_t align);
		void	Deinit();

	#if (_DEBUG_MEM_ALLOC)
		void*	Alloc(size_t bytes, const char* file, int line);
	#else
		void*	Alloc(size_t bytes);
	#endif

		void	Free(void* ptr);
		bool	Clear();
		void	Compact();

		static void	DumpMemInfo(char* args);
	#if (_DEBUG_MEM_ALLOC)
		void	CheckMemoryBlocks();
	#endif

		size_t GetReservedTotal()
			{ return _reservedPages * pageSize; }
		size_t GetReservedFree()
			{ return (_reservedPages - _commitedPages) * pageSize; }
		size_t GetReservedTotalPages()
			{ return _reservedPages; }
		size_t GetReservedFreePages()
			{ return _reservedPages - _commitedPages; }
		size_t GetCommitedTotal()
			{ return _commitedPages * pageSize; }
		size_t GetCommitedFree()
			{ return GetCommitedTotal() - _allocatedMemory; }
		size_t GetCommitedUsed()
			{ return _allocatedMemory; }
		const char* GetPoolName()
			{ return _poolName; }
		size_t GetAllocatedBlockCount()
			{ return _allocBlockCount; }

	private:
		MemoryHeader* CommitPages(size_t pages);
	#if (_DEBUG_MEM_ALLOC)
		void* AllocateBlock(MemoryHeader* start_block, size_t bytes, size_t req_bytes, const char* file, int line);
	#else
		void* AllocateBlock(MemoryHeader* start_block, size_t bytes, size_t req_bytes);
	#endif

		MemoryHeader* _firstBlock;
		size_t _reservedPages; // total reserved adress space
		size_t _commitedPages; // pages of memory that have been commited to process; we allocate blocks from here
		size_t _allocatedMemory; // total allocated memory; includes header and padding bytes
		size_t _pagesToGrow; // minimal number of pages to grow when commiting more memory
		size_t _initialSize; // initial size of commited memory
		size_t _align; // all user pointers are aligned to specified boundary; can be 1, 2, 4, 8 or 16
		const char* _poolName;
		int _index;
		size_t _allocBlockCount; // number of allocated blocks
	};


	// allocation pools

	extern BASELIB_API Allocator mainPool;
	extern BASELIB_API Allocator mapPool;
	extern BASELIB_API Allocator stringPool;
	extern BASELIB_API Allocator glPool;
	extern BASELIB_API Allocator tempPool;

	bool BASELIB_API Init();
	void BASELIB_API Deinit();
	bool BASELIB_API IsInitialized();


} // namespace Memory


// overriden global new and delete

#if (_DEBUG_MEM_ALLOC)

	inline
	void* operator new(size_t size, Memory::Allocator& allocator, const char* file, int line)
	{
		return allocator.Alloc(size, file, line);
	}

	inline
	void* operator new[](size_t size, Memory::Allocator& allocator, const char* file, int line)
	{
		return allocator.Alloc(size, file, line);
	}

	inline
	void* operator new(size_t size)
	{
		return Memory::mainPool.Alloc(size, "", 0);
	}

	inline
	void* operator new[](size_t size)
	{
		return Memory::mainPool.Alloc(size, "", 0);
	}

	inline
	void operator delete(void* ptr, Memory::Allocator& allocator, const char* file, int line) throw()
	{
		if(ptr)
		{
			uchar* p = (uchar*)ptr;
			Memory::MemoryHeader* header = (Memory::MemoryHeader*)(p - *(p - 1));
			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->Free(ptr);
		}
	}

	inline
	void operator delete[](void* ptr, Memory::Allocator& allocator, const char* file, int line) throw()
	{
		if(ptr)
		{
			uchar* p = (uchar*)ptr;
			Memory::MemoryHeader* header = (Memory::MemoryHeader*)(p - *(p - 1));
			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->Free(ptr);
		}
	}

#else

	inline
	void* operator new(size_t size, Memory::Allocator& allocator)
	{
		return allocator.Alloc(size);
	}

	inline
	void* operator new[](size_t size, Memory::Allocator& allocator)
	{
		return allocator.Alloc(size);
	}

	inline
	void* operator new(size_t size)
	{
		return Memory::mainPool.Alloc(size);
	}

	inline
	void* operator new[](size_t size)
	{
		return Memory::mainPool.Alloc(size);
	}

	inline
	void operator delete(void* ptr, Memory::Allocator& allocator) throw()
	{
		if(ptr)
		{
			uchar* p = (uchar*)ptr;
			Memory::MemoryHeader* header = (Memory::MemoryHeader*)(p - *(p - 1));
			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->Free(ptr);
		}
	}

	inline
	void operator delete[](void* ptr, Memory::Allocator& allocator) throw()
	{
		if(ptr)
		{
			uchar* p = (uchar*)ptr;
			Memory::MemoryHeader* header = (Memory::MemoryHeader*)(p - *(p - 1));
			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->Free(ptr);
		}
	}

#endif

	inline
	void operator delete(void* ptr) throw()
	{
		if(ptr)
		{
			uchar* p = (uchar*)ptr;
			Memory::MemoryHeader* header = (Memory::MemoryHeader*)(p - *(p - 1));
			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->Free(ptr);
		}
	}

	inline
	void operator delete[](void* ptr) throw()
	{
		if(ptr)
		{
			uchar* p = (uchar*)ptr;
			Memory::MemoryHeader* header = (Memory::MemoryHeader*)(p - *(p - 1));
			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->Free(ptr);
		}
	}


#if (_DEBUG_MEM_ALLOC)

	#define new(allocator)	new((allocator), __FILE__, __LINE__)

#endif


#endif //_MEMORY_H_
