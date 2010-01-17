
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "BaseLib/Common.h"


#define RECORD_EXCEPTION_INFO(message) \
	RecordExceptionInfo(GetExceptionInformation(), message)

BASELIB_API int RecordExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs, LPCTSTR lpszMessage);


#endif // _EXCEPTION_H_
