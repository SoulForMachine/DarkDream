
#include <cstring>
#include "Memory.h"
#include "String.h"

using namespace Memory;


char* WideCharToChar(const wchar_t* str)
{
	char* p = new(stringPool) char[wcslen(str) + 1];
	char* pp = p;
	while(*pp++ = (char)*str++);
	return p;
}

wchar_t* CharToWideChar(const char* str)
{
	wchar_t* p = new(stringPool) wchar_t[strlen(str) + 1];
	wchar_t* pp = p;
	while(*pp++ = (wchar_t)*str++);
	return p;
}

char* StringDup(const char* str)
{
	int len = strlen(str) + 1;
	char* p = new(stringPool) char[len];
	strcpy(p, str);
	return p;
}

wchar_t* StringDup(const wchar_t* str)
{
	int len = wcslen(str) + 1;
	wchar_t* p = new(stringPool) wchar_t[len];
	wcscpy(p, str);
	return p;
}
