
#ifndef _BASELIB_HASHING_H_
#define _BASELIB_HASHING_H_

#include "Common.h"
#include "Types.h"


uint BASELIB_API GetHash(const char* str);
uint BASELIB_API GetHash(const wchar_t* str);
uint BASELIB_API GetStringiHash(const char* str);
uint BASELIB_API GetStringiHash(const wchar_t* str);
uint BASELIB_API GetHash(char val);
uint BASELIB_API GetHash(unsigned char val);
uint BASELIB_API GetHash(short val);
uint BASELIB_API GetHash(unsigned short val);
uint BASELIB_API GetHash(int val);
uint BASELIB_API GetHash(unsigned int val);
uint BASELIB_API GetHash(long val);
uint BASELIB_API GetHash(unsigned long val);
unsigned long BASELIB_API GetCRC32(const void* data, int length);


#endif // _BASELIB_HASHING_H_
