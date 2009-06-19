
#include "BaseLib/Memory.h"
#include "BaseLib/Console.h"
#include "BaseLib/SystemInfo.h"
#include "FileSystem.h"

using namespace Memory;
using namespace FileSys;


namespace Engine
{

	/*
		The game has base directory, which is it's file system's root.
		All paths are relative to this directory. Ordinary directories
		are searched first, then archives.
	*/

	bool FileSystem::Init()
	{
		Console::PrintLn("----- Initializing file system -----");

		// get game binaries directory
		bool result = SystemInfo::GetApplicationDirectory(_binDir);

		if(!result)
		{
			Console::PrintError("Failed to get application directory.");
			return false;
		}

		//! temporary; solve this!
		tstrrchr(_binDir, _t('\\'))[0] = _t('\0');
		tstrrchr(_binDir, _t('\\'))[1] = _t('\0');

		// set default basedir
		tstrcpy(_baseDir, _binDir);
		tstrcat(_baseDir, _t("base\\"));

		// search for archives in basedir and load their directories
		FileFinder ff;
		tchar path[MAX_PATH];
		tstrcpy(path, _baseDir);
		size_t basedir_end = tstrlen(_baseDir);

		tstrcat(path, _t("*.*"));

		bool found = ff.FindFirstFile(path);

		while(found)
		{
			const tchar* file_name = ff.GetFileName();
			tstrcpy(path + basedir_end, file_name);
			found = ff.FindNextFile();
		}

		ff.EndSearch();

		Console::PrintLn("Game base directory: %ls", _baseDir);

		return true;
	}

	void FileSystem::Deinit()
	{
		Console::PrintLn("----- Deinitializing file system -----");
	}

	void FileSystem::SetBaseDirPath(const tchar* path)
	{
		tstrcpy(_baseDir, path);
	}

	const tchar* FileSystem::GetBaseDirPath()
	{
		return _baseDir;
	}

	const tchar* FileSystem::GetBinDirPath()
	{
		return _binDir;
	}

	void FileSystem::MakeFullPath(tchar* dest, const tchar* src)
	{
		tstrcpy(dest, _baseDir);
		tstrncat(dest, src, MAX_PATH);
	}

	FileSys::File* FileSystem::Open(const tchar* fname, const tchar* mode)
	{
		tchar path[MAX_PATH];
		MakeFullPath(path, fname);
		File* file = new(tempPool) FsysFile;
		if(file->Open(path, mode))
		{
			return file;
		}
		else
		{
			delete file;
			return 0;
		}
	}

}
