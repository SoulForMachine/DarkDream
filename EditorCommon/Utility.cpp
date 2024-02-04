
#include "StdAfx.h"
#include "UtilityTempl.h"
#include "Utility.h"


using namespace System;



namespace EditorCommon
{

	void EditorUtil::Init(Engine::EngineAPI* api)
	{
		_engineAPI = api;
	}

	/*
		Returns a path relative to game's base directory. If full_path is not
		within game's base directory, unmodified path is returned. Caller must
		delete returned buffer.
	*/
	tchar* EditorUtil::GetRelativePath(String^ full_path)
	{
		const tchar* base_dir = _engineAPI->fileSystem->GetBaseDirPath();
		int len = static_cast<int>(tstrlen(base_dir));
		int res = String::Compare(gcnew String(base_dir), 0, full_path, 0, len, true);
		if(res == 0)
			full_path = full_path->Remove(0, len);
		return ConvertString<tchar>(full_path);
	}

	/*
		Checks if given full file path is in game's base directory.
	*/
	bool EditorUtil::IsInGameBaseDir(String^ file_name)
	{
		const tchar* base_dir = _engineAPI->fileSystem->GetBaseDirPath();
		const tchar* p1 = base_dir;
		tchar* fn = ConvertString<tchar>(file_name);
		const tchar* p2 = fn;
		bool result = true;
		while(*p1)
		{
			if(tolower(*p1++) != tolower(*p2++))
			{
				result = false;
				break;
			}
		}
		Memory::Delete(fn);
		return result;
	}

}
