
#ifndef _BASELIB_STRING_H_
#define _BASELIB_STRING_H_

#include <cstring>
#include "Common.h"

//#define FORCE_ASCII

#if defined(_UNICODE) && !defined(FORCE_ASCII)

typedef wchar_t tchar;

#define _t(str)		L##str

#define tstrcpy		wcscpy
#define tstrncpy	wcsncpy
#define tstrcat		wcscat
#define tstrncat	wcsncat
#define tstrchr		wcschr
#define tstrrchr	wcsrchr
#define tstrlen		wcslen
#define tstrcmp		wcscmp
#define tstrncmp	wcsncmp
#define tstricmp	wcsicmp
#define tstrnicmp	wcsnicmp
#define ttoupper	towupper
#define ttolower	towlower

#else

typedef char tchar;

#define _t(str)		str

#define tstrcpy		strcpy
#define tstrncpy	strncpy
#define tstrcat		strcat
#define tstrncat	strncat
#define tstrchr		strchr
#define tstrrchr	strrchr
#define tstrlen		strlen
#define tstrcmp		strcmp
#define tstrncmp	strncmp
#define tstricmp	stricmp
#define tstrnicmp	strnicmp
#define ttoupper	toupper
#define ttolower	tolower

#endif

BASELIB_API char* WideCharToChar(const wchar_t* str);
BASELIB_API wchar_t* CharToWideChar(const char* str);
BASELIB_API char* StringDup(const char* str);
BASELIB_API wchar_t* StringDup(const wchar_t* str);


#endif // _BASELIB_STRING_H_
