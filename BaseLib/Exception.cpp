

#include <Windows.h>
#include <tchar.h>
#include <ctime>
#include <cstdio>
#include <dbghelp.h>
#include "Exception.h"

#pragma comment(lib, "dbghelp.lib")


#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

///////////////////////////////////////////////////////////////////////////////
// lstrrchr (avoid the C Runtime )
static TCHAR * lstrrchr(LPCTSTR string, int ch)
{
	TCHAR *start = (TCHAR *)string;

	while (*string++)                       /* find end of string */
		;
											/* search towards front */
	while (--string != start && *string != (TCHAR) ch)
		;

	if (*string == (TCHAR) ch)                /* char found ? */
		return (TCHAR *)string;

	return NULL;
}



//#ifdef XCRASHREPORT_WRITE_MINIDUMP

///////////////////////////////////////////////////////////////////////////////
// DumpMiniDump
static void DumpMiniDump(HANDLE hFile, PEXCEPTION_POINTERS excpInfo)
{
	if (excpInfo == NULL) 
	{
		// Generate exception to get proper context in dump
		__try 
		{
			OutputDebugString(_T("raising exception\r\n"));
			RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
		} 
		__except(DumpMiniDump(hFile, GetExceptionInformation()),
				 EXCEPTION_CONTINUE_EXECUTION) 
		{
		}
	} 
	else
	{
		MINIDUMP_EXCEPTION_INFORMATION eInfo;

		OutputDebugString(_T("writing minidump\r\n"));
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = excpInfo;
		eInfo.ClientPointers = FALSE;

		// note:  MiniDumpWithIndirectlyReferencedMemory does not work on Win98
		MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			MiniDumpNormal,
			excpInfo ? &eInfo : NULL,
			NULL,
			NULL);
	}
}

//#endif	// XCRASHREPORT_WRITE_MINIDUMP

///////////////////////////////////////////////////////////////////////////////
// GetFilePart
static TCHAR * GetFilePart(LPCTSTR source)
{
	TCHAR *result = lstrrchr(source, _T('\\'));
	if (result)
		result++;
	else
		result = (TCHAR *)source;
	return result;
}

int RecordExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs, 
								LPCTSTR lpszMessage)
{
	static BOOL bFirstTime = TRUE;
	TCHAR szModuleName[MAX_PATH];
	TCHAR *pszFilePart;
	HANDLE hMiniDumpFile;

	if (!bFirstTime)	// Going recursive! That must mean this routine crashed!
		return EXCEPTION_CONTINUE_SEARCH;
	bFirstTime = FALSE;

	ZeroMemory(szModuleName, sizeof(szModuleName));
	if (GetModuleFileName(0, szModuleName, MAX_PATH) <= 0)
		lstrcpy(szModuleName, _T("Unknown"));

	pszFilePart = GetFilePart(szModuleName);

	// Replace the filename with our minidump file name
	time_t t;
	time(&t);
	tm* lt = localtime(&t);
#ifdef UNICODE
	swprintf(pszFilePart, MAX_PATH, L"crash-%02d-%02d-%02d-%02d-%d.dmp", lt->tm_hour, lt->tm_min, lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
#else
	sprintf(pszFilePart, "crash-%2d-%2d-%d.dmp", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
#endif

	// Create the file
	hMiniDumpFile = CreateFile(
		szModuleName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);

	// Write the minidump to the file
	if (hMiniDumpFile != INVALID_HANDLE_VALUE)
	{
		DumpMiniDump(hMiniDumpFile, pExceptPtrs);

		// Close file
		CloseHandle(hMiniDumpFile);
	}

//#endif	// XCRASHREPORT_WRITE_MINIDUMP

	// return the magic value which tells Win32 that this handler didn't
	// actually handle the exception - so that things will proceed as per
	// normal.
	return EXCEPTION_CONTINUE_SEARCH;
}
