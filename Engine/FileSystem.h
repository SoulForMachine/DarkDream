
#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "BaseLib/Types.h"
#include "BaseLib/FileSys.h"
#include "Common.h"

#ifdef _WIN32

#define MAX_PATH	260

#endif

namespace Engine
{

	class ENGINE_API FileSystem
	{
	public:
		bool Init();
		void Deinit();
		void SetBaseDirPath(const tchar* path);
		const tchar* GetBaseDirPath();
		const tchar* GetBinDirPath();
		void MakeFullPath(tchar* dest, const tchar* src);
		FileSys::File* Open(const tchar* fname, const tchar* mode);

	private:
		tchar _baseDir[MAX_PATH];
		tchar _binDir[MAX_PATH];
	};

}


#endif // _FILE_SYSTEM_H_
