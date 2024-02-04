
#include <cstring>
#include <cstdlib>
#include "Algorithm.h"


void QSortStrings(const char** base, size_t count)
{
	struct local
	{
		static int cmp_func(const void* s1, const void* s2)
		{
			return strcmp((const char*)s1, (const char*)s2);
		}
	};

	qsort((void*)base, count, sizeof(char*), local::cmp_func);
}

intptr_t BinSearch(const char* search_str, const char** start, size_t count)
{
	if(count == 0)
		return -1;
	if(count == 1)
		return (strcmp(start[0], search_str) == 0)? 0: -1;

	size_t middle = count / 2;
	size_t upper = count, lower = 0;
	for(;;)
	{
		int cmp = strcmp(start[middle], search_str);
		if(cmp == 0)
			return middle;
		else if(cmp < 0) // word is on the right side
			lower = middle + 1;
		else
			upper = middle;

		switch(upper - lower)
		{
		case 0:
			return -1;
		case 1:
			return (strcmp(start[lower], search_str) == 0)? lower: -1;
		default:
			middle = (lower + upper) / 2;
		}
	}

}
