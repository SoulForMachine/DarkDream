

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <new>
#include <source_location>
#include "Common.h"
#include "Types.h"


#ifdef _DEBUG
	#define _DEBUG_MEM_ALLOC	1
	#define _CRT_MEM_ALLOC		0
#endif


namespace Memory
{
	class Allocator;

	extern size_t pageSize;
	extern size_t allocGranularity;
	extern BASELIB_API Allocator* const pools[];

	const uchar ALLOCATED_BIT = 0x80;
	const uchar ALLOCATOR_INDEX_MASK = 0x07;

	struct MemoryHeader
	{
		uchar flags; // bits: 0,1,2 - allocator index; 7 - allocated bit
		size_t blockSize;
		size_t userSize;
		size_t numElements;
		MemoryHeader* next;
		MemoryHeader* prev;

#if (_DEBUG_MEM_ALLOC)
		const char* file;
		int line;
		uint* guard;
#endif
	};


	class BASELIB_API Allocator
	{
	public:
		static constexpr size_t DefaultAlignment = 8;

		Allocator(const char* pool_name, int index);

		bool	Init(size_t address_space, size_t initial_size, size_t pages_to_grow, size_t align);
		void	Deinit();

	#if (_DEBUG_MEM_ALLOC)
		void* Alloc(size_t bytes, const char* file, int line);
		void* AllocArray(size_t bytes, size_t numElements, const char* file, int line);
	#else
		void* Alloc(size_t bytes);
		void* AllocArray(size_t bytes, size_t numElements);
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
		void* AllocateBlock(MemoryHeader* start_block, size_t bytes, size_t req_bytes, size_t numElements, const char* file, int line);
	#else
		void* AllocateBlock(MemoryHeader* start_block, size_t bytes, size_t req_bytes, size_t numElements);
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

	// Pool allocation functions.

#if (_CRT_MEM_ALLOC)
	template<typename _Type, typename... _ArgTs>
	_Type* New(Allocator& alloc, _ArgTs&&... args)
	{
		return new _Type(std::forward<_ArgTs>(args)...);
	}

	template<typename _Type, typename... _ArgTs>
	_Type* NewArray(Allocator& alloc, size_t numElements)
	{
		if (numElements == 0)
			return nullptr;

		return new _Type[numElements];
	}

	template<typename _Type>
	void Delete(_Type* ptr) noexcept
	{
		delete ptr;
	}

	template<typename _Type>
	void Delete(const _Type* ptr) noexcept
	{
		Delete(const_cast<_Type*>(ptr));
	}
#else
#if (_DEBUG_MEM_ALLOC)
	struct AllocDbgWrapper
	{
		AllocDbgWrapper(Allocator& alloc, const std::source_location srcLoc = std::source_location::current())
			: allocator(alloc), srcLocation(srcLoc)
		{ }

		Allocator& allocator;
		const std::source_location srcLocation;
	};

	template<typename _Type, typename... _ArgTs>
	_Type* New(AllocDbgWrapper alloc, _ArgTs&&... args)
	{
		void* ptr = alloc.allocator.Alloc(sizeof(_Type), alloc.srcLocation.file_name(), alloc.srcLocation.line());
		if (ptr == nullptr)
			throw std::bad_alloc();

		_Type* typedPtr = reinterpret_cast<_Type*>(ptr);
		new(typedPtr) _Type(std::forward<_ArgTs>(args)...);
		return typedPtr;
	}

	template<typename _Type, typename... _ArgTs>
	_Type* NewArray(AllocDbgWrapper alloc, size_t numElements)
	{
		if (numElements == 0)
			return nullptr;

		void* ptr = alloc.allocator.AllocArray(sizeof(_Type), numElements, alloc.srcLocation.file_name(), alloc.srcLocation.line());
		if (ptr == nullptr)
			throw std::bad_alloc();

		_Type* typedPtr = static_cast<_Type*>(ptr);
		for (size_t i = 0; i < numElements; ++i)
			new(&typedPtr[i]) _Type();

		return typedPtr;
	}
#else
	template<typename _Type, typename... _ArgTs>
	_Type* New(Allocator& alloc, _ArgTs&&... args)
	{
		void* ptr = alloc.Alloc(sizeof(_Type));
		if (ptr == nullptr)
			throw std::bad_alloc();

		_Type* typedPtr = static_cast<_Type*>(ptr);
		new(typedPtr) _Type(std::forward<_ArgTs>(args)...);
		return typedPtr;
	}

	template<typename _Type, typename... _ArgTs>
	_Type* NewArray(Allocator& alloc, size_t numElements)
	{
		if (numElements == 0)
			return nullptr;

		void* ptr = alloc.AllocArray(sizeof(_Type), numElements);
		if (ptr == nullptr)
			throw std::bad_alloc();

		_Type* typedPtr = static_cast<_Type*>(ptr);
		for (size_t i = 0; i < numElements; ++i)
			new(&typedPtr[i]) _Type();

		return typedPtr;
	}
#endif
	
	template<typename _Type>
	void Delete(_Type* ptr) noexcept
	{
		if (ptr != nullptr)
		{
			auto charPtr = reinterpret_cast<uchar*>(ptr);
			Memory::MemoryHeader* header = (Memory::MemoryHeader*)(charPtr - *(charPtr - 1));
			size_t numElements = header->numElements;

			if constexpr (std::is_destructible_v<_Type>)
			{
				for (size_t i = 0; i < numElements; ++i)
					ptr[i].~_Type();
			}

			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->CheckMemoryBlocks(); //! remove this
			Memory::pools[header->flags & Memory::ALLOCATOR_INDEX_MASK]->Free(ptr);
		}
	}

	template<typename _Type>
	void Delete(const _Type* ptr) noexcept
	{
		Delete(const_cast<_Type*>(ptr));
	}
#endif

} // namespace Memory

#endif //_MEMORY_H_
