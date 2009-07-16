
#include "StdAfx.h"
#include "Utility.h"

#define APP_NAME	"Daemonium Entity Editor"

using namespace System;


/*
	Returns a path relative to game's base directory. If full_path is not
	within game's base directory, unmodified path is returned. Caller must
	delete returned buffer.
*/
tchar* GetRelativePath(String^ full_path)
{
	const tchar* base_dir = engineAPI->fileSystem->GetBaseDirPath();
	int len = tstrlen(base_dir);
	int res = String::Compare(gcnew String(base_dir), 0, full_path, 0, len, true);
	if(res == 0)
		full_path = full_path->Remove(0, len);
	return ConvertString<tchar>(full_path);
}

System::String^ GetAppName()
{
	return APP_NAME;
}

/*
	Checks if given full file path is in game's base directory.
*/
bool IsInGameBaseDir(String^ file_name)
{
	const tchar* base_dir = engineAPI->fileSystem->GetBaseDirPath();
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
	delete[] fn;
	return result;
}

int PackColor(const math3d::vec4f& color)
{
	return (
		(int(color.a * 255) << 24) |
		(int(color.r * 255) << 16) |
		(int(color.g * 255) << 8) |
		(int(color.b * 255)) );
}

int PackColor(const math3d::vec3f& color)
{
	return (
		(255 << 24) |
		(int(color.r * 255) << 16) |
		(int(color.g * 255) << 8) |
		(int(color.b * 255)) );
}

void UnpackColor(int color, math3d::vec4f& unpacked)
{
	unpacked.b = float(color & 0xFF) / 255.0f;
	unpacked.g = float(unsigned(color & 0xFF00) >> 8) / 255.0f;
	unpacked.r = float(unsigned(color & 0xFF0000) >> 16) / 255.0f;
	unpacked.a = float(unsigned(color & 0xFF000000) >> 24) / 255.0f;
}

void UnpackColor(int color, math3d::vec3f& unpacked)
{
	unpacked.b = float(color & 0xFF) / 255.0f;
	unpacked.g = float(unsigned(color & 0xFF00) >> 8) / 255.0f;
	unpacked.r = float(unsigned(color & 0xFF0000) >> 16) / 255.0f;
}
