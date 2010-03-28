
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <Windows.h>
#include "BaseLib/Common.h"


class BASELIB_API Exception
{
public:
	Exception(const char* desc);

	const char* GetDesc() const
		{ return _desc; }

protected:
	Exception()
		{ _desc[0] = '\0'; }

	void SetDesc(const char* fmt, ...);

private:
	static const int MAX_DESC_LEN = 1024;
	char _desc[MAX_DESC_LEN];
};


#define RECORD_EXCEPTION_INFO(message) \
	RecordExceptionInfo(GetExceptionInformation(), message)

BASELIB_API int RecordExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs, LPCTSTR lpszMessage);


#endif // _EXCEPTION_H_
