
#include <cstring>
#include "Memory.h"
#include "String.h"

using namespace Memory;


char* WideCharToChar(const wchar_t* str)
{
	char* p = NewArray<char>(stringPool, wcslen(str) + 1);
	char* pp = p;
	while(*pp++ = (char)*str++);
	return p;
}

wchar_t* CharToWideChar(const char* str)
{
	wchar_t* p = NewArray<wchar_t>(stringPool, strlen(str) + 1);
	wchar_t* pp = p;
	while(*pp++ = (wchar_t)*str++);
	return p;
}

char* StringDup(const char* str)
{
	size_t len = strlen(str) + 1;
	char* p = NewArray<char>(stringPool, len);
	strcpy(p, str);
	return p;
}

wchar_t* StringDup(const wchar_t* str)
{
	size_t len = wcslen(str) + 1;
	wchar_t* p = NewArray<wchar_t>(stringPool, len);
	wcscpy(p, str);
	return p;
}
