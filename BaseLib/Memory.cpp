

#include <cassert>
#include "Win32Common.h"
#include "Console.h"
#include "FileUtil.h"
#include "SystemInfo.h"
#include "String.h"
#include "Memory.h"

#if (_DEBUG_MEM_ALLOC)
	#define PRINT_ERROR(text) Console::PrintError(text "\nFile: %s\nLine: %d", file, line)
	#define PRINT_ERROR1(text, a) Console::PrintError(text "\nFile: %s\nLine: %d", a, file, line)
	#define PRINT_ERROR2(text, a, b) Console::PrintError(text "\nFile: %s\nLine: %d", a, b, file, line)
#else
	#define PRINT_ERROR(text) Console::PrintError(text)
	#define PRINT_ERROR1(text, a) Console::PrintError(text, a)
	#define PRINT_ERROR2(text, a, b) Console::PrintError(text, a, b)
#endif

namespace Memory
{

	Allocator mainPool("main", 0);
	Allocator mapPool("map", 1);
	Allocator stringPool("string", 2);
	Allocator glPool("gl", 3);
	Allocator tempPool("temp", 4);

	Allocator* const pools[] =
	{
		&mainPool, &mapPool, &stringPool, &glPool, &tempPool
	};

	size_t pageSize;
	size_t allocGranularity;
	static bool g_initialized = false;

	bool Init()
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		pageSize = sysinfo.dwPageSize;
		allocGranularity = sysinfo.dwAllocationGranularity;

		if(!mainPool.Init(64 * 1024 * 1024, 0, 1, 16))
			return false;

		if(!mapPool.Init(64 * 1024 * 1024, 0, 1, 16))
			return false;

		if(!stringPool.Init(16 * 1024 * 1024, 0, 1, 1))
			return false;

		if(!glPool.Init(16 * 1024 * 1024, 0, 1, 16))
			return false;

		if(!tempPool.Init(64 * 1024 * 1024, 0, 1, 16))
			return false;

		g_initialized = true;

		// console commands
		static Console::Command mem_dump(
			"mem_dumpInfo",
			"Lists information for all or only allocated blocks of memory for specified pool.\n"
			"Syntax: mem_dumpInfo poolname [options] [file]\n"
			"Options:\n"
			"    -a    List all blocks. By default, lists only allocated blocks.\n"
			"    -g    Print pool global info only.",
			&Allocator::DumpMemInfo);

		return true;
	}

	void Deinit()
	{
#ifdef _DEBUG
		if(	mainPool.GetAllocatedBlockCount() ||
			mapPool.GetAllocatedBlockCount() ||
			stringPool.GetAllocatedBlockCount() ||
			glPool.GetAllocatedBlockCount() ||
			tempPool.GetAllocatedBlockCount() )
		{
			MessageBox(0, TEXT("Memory leaks detected! Check the log files."), TEXT("Daemonium"), MB_ICONWARNING | MB_OK);
		}
#endif
		mainPool.Deinit();
		mapPool.Deinit();
		stringPool.Deinit();
		glPool.Deinit();
		tempPool.Deinit();
		g_initialized = false;
	}

	bool IsInitialized()
	{
		return g_initialized;
	}


	const uint GUARD_MAGIC = 0xDEADBEEF;
	const ubyte CLEAR_PATTERN = 0xAB;
	const size_t BLOCK_SIZE_MINIMUM = 64;


	Allocator::Allocator(const char* pool_name, int index)
	{
		_firstBlock = 0;
		_poolName = pool_name;
		_index = index;
		_allocBlockCount = 0;
	}

	bool	Allocator::Init(size_t address_space, size_t initial_size, size_t pages_to_grow, size_t align)
	{
		address_space = RoundUp(address_space, pageSize);

		if(pages_to_grow * pageSize > address_space)
			return false;

		if(initial_size)
			initial_size = RoundUp(initial_size, pageSize);

		if(initial_size > address_space)
			return false;

		if(!IsPow2(align) || align > 16)
			return false;

		_firstBlock = (MemoryHeader*)VirtualAlloc(0, address_space, MEM_RESERVE, PAGE_READWRITE);

		if(_firstBlock)
		{
			_reservedPages = address_space / pageSize;
			_pagesToGrow = pages_to_grow;
			_initialSize = initial_size;
			_align = align;

			return Clear();
		}
		else
		{
			return false;
		}
	}

	void	Allocator::Deinit()
	{
		if(_firstBlock)
		{
#if (_DEBUG_MEM_ALLOC)
			char fname[MAX_PATH];
			sprintf(fname, "%s %s.txt", _poolName, _poolName);
			DumpMemInfo(fname);
#endif
			BOOL result = VirtualFree(_firstBlock, 0, MEM_RELEASE);
			assert(result);

			_firstBlock = 0;
			_allocatedMemory = 0;
			_commitedPages = 0;
			_reservedPages = 0;
			_pagesToGrow = 0;
			_initialSize = 0;
			_align = 0;
			_allocBlockCount = 0;
		}
	}

#if (_DEBUG_MEM_ALLOC)
	void*	Allocator::Alloc(size_t bytes, const char* file, int line)
#else
	void*	Allocator::Alloc(size_t bytes)
#endif
	{
		if(!g_initialized)
		{
			assert(0);
			PRINT_ERROR("Memory request while memory manager not initialized.");
			return 0;
		}

		if(!_firstBlock)
		{
			PRINT_ERROR1("Memory request in %s pool while pool not initialized.", _poolName);
			return 0;
		}

		if(!bytes)
			return 0;

		size_t req_bytes = bytes;

		bytes += sizeof(MemoryHeader) + 1; // add space for header and header offset byte
	#if (_DEBUG_MEM_ALLOC)
		bytes += 4; // add space for 4 guard bytes
	#endif
		bytes = RoundUp(bytes, 4); // round up to 4 byte boundary

		// number of pages needed
		size_t pages = Max(RoundUp(bytes + _align, pageSize) / pageSize, _pagesToGrow);

		MemoryHeader* block;
		if(bytes > GetCommitedFree())
		{
			block = CommitPages(pages);
			if(!block)
			{
				PRINT_ERROR2("Not enough memory in %s pool. Requested %u bytes.", _poolName, req_bytes);
				return 0;
			}
		}
		else
			block = _firstBlock;

		void* ptr;

#if (_DEBUG_MEM_ALLOC)
		ptr = AllocateBlock(block, bytes, req_bytes, file, line);
#else
		ptr = AllocateBlock(block, bytes, req_bytes);
#endif

		// if no single block large enough found, try to commit more memory
		// and try again
		if(!ptr)
		{
			block = CommitPages(pages);
			if(!block)
			{
				PRINT_ERROR2("Not enough memory in %s pool. Requested %u bytes.", _poolName, req_bytes);
				return 0;
			}

		#if (_DEBUG_MEM_ALLOC)
			ptr = AllocateBlock(block, bytes, req_bytes, file, line);
		#else
			ptr = AllocateBlock(block, bytes, req_bytes);
		#endif
		}

		return ptr;
	}

	void	Allocator::Free(void* ptr)
	{
		if(!g_initialized)
		{
			Console::PrintError("Dealocating memory while memory manager not initialized.");
			return;
		}

		if(!ptr)
			return;

		uchar* p = (uchar*)ptr;
		MemoryHeader* hdr = (MemoryHeader*)(p - *(p - 1));
		MemoryHeader* next_block = hdr->next;
		MemoryHeader* prev_block = hdr->prev;
		assert(hdr->flags & ALLOCATED_BIT);
		_allocatedMemory -= hdr->blockSize;

		#if (_DEBUG_MEM_ALLOC)
			memset(ptr, CLEAR_PATTERN, hdr->userSize);
		#endif

		// if previous block is free, merge them
		if(hdr != _firstBlock && !(prev_block->flags & ALLOCATED_BIT))
		{
			prev_block->blockSize += hdr->blockSize;
			hdr = prev_block;
		}

		// if next block is free, merge them
		if(next_block != _firstBlock && !(next_block->flags & ALLOCATED_BIT))
		{
			hdr->blockSize += next_block->blockSize;
			hdr->next = next_block->next;
			next_block->next->prev = hdr;
		}
		else
		{
			hdr->next = next_block;
			next_block->prev = hdr;
		}

		hdr->flags = 0; // mark as free
		hdr->userSize = 0;
		--_allocBlockCount;
	}

	MemoryHeader* Allocator::CommitPages(size_t pages)
	{
		if(pages > GetReservedFreePages())
			return 0;

		void* reserved = (void*)(((uchar*)_firstBlock) + GetCommitedTotal());
		size_t size = pages * pageSize;
		MemoryHeader* block = (MemoryHeader*)VirtualAlloc(reserved, size, MEM_COMMIT, PAGE_READWRITE);

		if(block)
		{
			_commitedPages += pages;

			// if this is the first commited block
			if(block == _firstBlock)
			{
				block->flags = 0;
				block->next = block;
				block->prev = block;
			}

			// if previous block is not allocated, merge it with new block
			MemoryHeader* prev_block = _firstBlock->prev;
			if((prev_block->flags & ALLOCATED_BIT) == 0)
			{
				prev_block->blockSize += size;

				return prev_block;
			}
			else
			{
				// make new block and link it in
				block->blockSize = size;
				block->userSize = 0;
				block->flags = 0;
				block->next = _firstBlock;
				block->prev = prev_block;
				prev_block->next = block;
				_firstBlock->prev = block;

				return block;
			}
		}
		else
		{
			Console::PrintError("Commit failed in %s pool.", _poolName);
			return 0;
		}
	}

#if (_DEBUG_MEM_ALLOC)
	void* Allocator::AllocateBlock(MemoryHeader* start_block, size_t bytes, size_t req_bytes, const char* file, int line)
#else
	void* Allocator::AllocateBlock(MemoryHeader* start_block, size_t bytes, size_t req_bytes)
#endif
	{
		MemoryHeader* block = start_block;

		// search for an empty block large enough
		do
		{
			if(!(block->flags & ALLOCATED_BIT) && bytes <= block->blockSize)
			{
				uchar* ptr = (uchar*)(block + 1);
				// add one byte for header offset and round up to requested boundary
				uchar* user_ptr = RoundUp(ptr + 1, _align);
				*(user_ptr - 1) = uchar(user_ptr - (uchar*)block); // write header offset
				size_t align_bytes = user_ptr - ptr - 1;

				if(bytes + align_bytes <= block->blockSize)
				{
					bytes += align_bytes;
					size_t block_me_left = block->blockSize - bytes;

					// if not enough memory left for an empty block,
					// allocate the whole block, otherwise allocate
					// part of the free block and make the rest of memory
					// a new free block
					if(block_me_left >= BLOCK_SIZE_MINIMUM)
					{
						block->blockSize = bytes;

						MemoryHeader* next_block;
						next_block = (MemoryHeader*)(((uchar*)block) + bytes);
						next_block->blockSize = block_me_left;
						next_block->userSize = 0;
						next_block->flags = 0;
						next_block->next = block->next;
						next_block->prev = block;
						block->next->prev = next_block;
						block->next = next_block;
					}

					block->userSize = req_bytes;
					block->flags = (ALLOCATED_BIT | (_index & ALLOCATOR_INDEX_MASK));

		#if (_DEBUG_MEM_ALLOC)
					block->file = file;
					block->line = line;

					// add 4 guard bytes after the end of user memory
					block->guard = (uint*)(user_ptr + req_bytes);
					*block->guard = GUARD_MAGIC;
		#endif

					_allocatedMemory += block->blockSize;
					++_allocBlockCount;

					return user_ptr;
				}
			}

			block = block->next;

		} while(block != start_block);

		return 0; // end of list, no free block large enough found
	}

	bool	Allocator::Clear()
	{
		// decommit all pages
		BOOL result = VirtualFree(_firstBlock, 0, MEM_DECOMMIT);
		if(!result)
		{
			Console::PrintError("Decommit failed in %s pool.", _poolName);
			return false;
		}

		_commitedPages = 0;
		_allocatedMemory = 0;
		_allocBlockCount = 0;

		if(_initialSize)
		{
			MemoryHeader* block = (MemoryHeader*)VirtualAlloc(_firstBlock, _initialSize, MEM_COMMIT, PAGE_READWRITE);

			if(block)
			{
				assert(block == _firstBlock);

				_commitedPages = _initialSize / pageSize;
				block->flags = 0;
				block->blockSize = _initialSize;
				block->userSize = 0;
				block->next = block;
				block->prev = block;
			}
			else
			{
				Console::PrintError("Commit failed in %s pool.", _poolName);
				return false;
			}
		}

		return true;
	}

	void	Allocator::Compact()
	{
		
	}

	void	Allocator::DumpMemInfo(char* args)
	{
		if(!args || !*args)
		{
			Console::PrintLn("Pool name required.");
			return;
		}

		// find requested pool
		const char* pool_name = strtok(args, " \n\t");

		if(!pool_name || !*pool_name)
		{
			Console::PrintLn("Pool name required.");
			return;
		}

		Allocator* pool = 0;
		for(int i = 0; i < COUNTOF(pools); ++i)
		{
			if(!strcmp(pool_name, pools[i]->GetPoolName()))
			{
				pool = pools[i];
				break;
			}
		}

		if(!pool)
		{
			Console::PrintLn("Pool \'%s\' does not exist.", pool_name);
			return;
		}

		if(!pool->_firstBlock)
		{
			Console::PrintLn("Pool \'%s\' is not initialized.", pool_name);
			return;
		}

		if(!pool->_commitedPages)
		{
			Console::PrintLn("Pool \'%s\' has no commited pages.", pool_name);
			return;
		}

		// options
		bool print_all = false;
		bool only_gi = false;
		const char* file_name = 0;
		const char* ptr = strtok(0, " \n\t");
		while(ptr)
		{
			if(*ptr == '-' && tolower(*(ptr + 1)) == 'a')
				print_all = true;
			else if(*ptr == '-' && tolower(*(ptr + 1)) == 'g')
				only_gi = true;
			else if(!file_name)
				file_name = ptr;
			else
				break;

			ptr = strtok(0, " \n\t");
		}

		if(file_name && *file_name)
		{
			// dump to a file
			FileUtil::FsysFile file;
			tchar fn[MAX_PATH];
			_stprintf(fn, _t("%hs"), file_name);
			// if we do not have full path, create file in application directory
			if(!tstrrchr(fn, _t('\\')))
			{
				tchar app_dir[MAX_PATH];
				SystemInfo::GetApplicationDirectory(app_dir);
				tstrncat(app_dir, fn, MAX_PATH);
				tstrcpy(fn, app_dir);
			}
			if(!file.Open(fn, _t("wt")))
			{
				Console::PrintError("Failed to create output file: %s", file_name);
				return;
			}

			file.Printf("-------------------------------\n");
			file.Printf("Pool: %s\n", pool->GetPoolName());
			file.Printf("Reserved pages: %u\n", pool->_reservedPages);
			file.Printf("Commited pages: %u\n", pool->_commitedPages);
			file.Printf("Pages to grow: %u\n", pool->_pagesToGrow);
			file.Printf("Initial commited: %u\n", pool->_initialSize);
			file.Printf("Allocated memory: %u\n", pool->_allocatedMemory);
			file.Printf("Allocation alignment: %u\n", pool->_align);
			file.Printf("Allocated block count: %u\n", pool->_allocBlockCount);
			file.Printf("-------------------------------\n");

			if(!only_gi)
			{
				MemoryHeader* block = pool->_firstBlock;

				do
				{
					if(print_all || (block->flags & ALLOCATED_BIT))
					{
						if(block->flags & ALLOCATED_BIT)
						{
							file.Printf("--- Block at %p ---\n", block);
							file.Printf("Size: %u, user size: %u\n", block->blockSize, block->userSize);
						#if (_DEBUG_MEM_ALLOC)
							file.Printf("File: %s\n", block->file);
							file.Printf("Line: %d\n", block->line);
						#endif
						}
						else
						{
							file.Printf("--- Unalocated block at %p ---\n", block);
							file.Printf("Size: %u\n", block->blockSize);
						}
					}

					block = block->next;
				}
				while(block != pool->_firstBlock);
			}

			file.Close();
		}
		else
		{
			// dump to console
			Console::PrintLn("-------------------------------");
			Console::PrintLn("Pool: %s", pool->GetPoolName());
			Console::PrintLn("Reserved pages: %u", pool->_reservedPages);
			Console::PrintLn("Commited pages: %u", pool->_commitedPages);
			Console::PrintLn("Pages to grow: %u", pool->_pagesToGrow);
			Console::PrintLn("Initial commited: %u", pool->_initialSize);
			Console::PrintLn("Allocated memory: %u", pool->_allocatedMemory);
			Console::PrintLn("Allocation alignment: %u", pool->_align);
			Console::PrintLn("Allocated block count: %u", pool->_allocBlockCount);
			Console::PrintLn("-------------------------------");

			if(!only_gi)
			{
				MemoryHeader* block = pool->_firstBlock;

				do
				{
					if(print_all || (block->flags & ALLOCATED_BIT))
					{
						if(block->flags & ALLOCATED_BIT)
						{
							Console::PrintLn("--- Block at %p ---", block);
							Console::PrintLn("Size: %u, user size: %u", block->blockSize, block->userSize);
						#if (_DEBUG_MEM_ALLOC)
							Console::PrintLn("File: %s", block->file);
							Console::PrintLn("Line: %d", block->line);
						#endif
						}
						else
						{
							Console::PrintLn("--- Unalocated block at %p ---", block);
							Console::PrintLn("Size: %u", block->blockSize);
						}
					}

					block = block->next;
				}
				while(block != pool->_firstBlock);
			}
		}
	}

#if (_DEBUG_MEM_ALLOC)
	void	Allocator::CheckMemoryBlocks()
	{
		MemoryHeader* hdr = _firstBlock;

		do
		{
			if(hdr->flags & ALLOCATED_BIT)
			{
				if(*hdr->guard != GUARD_MAGIC)
				{
					// written past allocated memory block
					const char* file = hdr->file;
					int line = hdr->line;
					PRINT_ERROR2("Buffer overrun - block at %p, size: %u bytes.", hdr, hdr->blockSize);
				}
			}

			hdr = hdr->next;
		}
		while(hdr != _firstBlock);
	}
#endif

} // namespace Memory
