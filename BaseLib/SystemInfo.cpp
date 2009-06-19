
#include "FileSys.h"
#include "SystemInfo.h"


bool SystemInfo::GetApplicationDirectory(tchar* dir)
{
	tchar fname[MAX_PATH];
	DWORD result = GetModuleFileName(NULL, fname, MAX_PATH);

	if(!result)
		return false;

	FileSys::ExtractDir(dir, fname);
	return true;
}
