
#include "FileUtil.h"
#include "SystemInfo.h"


SystemInfo::CPUInfo SystemInfo::_cpuInfo;


void SystemInfo::Init()
{
}

bool SystemInfo::GetApplicationDirectory(tchar* dir)
{
	tchar fname[MAX_PATH];
	DWORD result = GetModuleFileName(NULL, fname, MAX_PATH);

	if(!result)
		return false;

	FileUtil::ExtractDir(dir, fname);
	return true;
}
