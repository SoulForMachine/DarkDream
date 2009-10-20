
#ifndef _SYSTEM_INFO_
#define _SYSTEM_INFO_

#include "Types.h"


class BASELIB_API SystemInfo
{
public:
	struct CPUInfo
	{
		int SSEVersionMajor;
		int SSEVersionMinor;
	};

	static void Init();
	static bool GetApplicationDirectory(tchar* dir);
	static const CPUInfo& GetCPUInfo()
		{ return _cpuInfo; }

private:
	static CPUInfo _cpuInfo;
};


#endif // _SYSTEM_INFO_
