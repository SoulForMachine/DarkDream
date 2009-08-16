
#pragma once

/*
	Converts System::String to tchar*. Caller must delete returned buffer.
*/
template <class _CharType>
_CharType* ConvertString(System::String^ str)
{
	msclr::interop::marshal_context^ context = gcnew msclr::interop::marshal_context;
	const _CharType* cs = context->marshal_as<const _CharType*>(str);
	int len = 0;
	const _CharType* p = cs;
	while(*p++) len++;
	_CharType* ret = new(Memory::tempPool) _CharType[len + 1];
	_CharType* dest = ret;
	while(*dest++ = *cs++);
	delete context;
	return ret;
}


tchar* GetRelativePath(System::String^ full_path);
bool IsInGameBaseDir(System::String^ file_name);
int PackColor(const math3d::vec4f& color);
int PackColor(const math3d::vec3f& color);
void UnpackColor(int color, math3d::vec4f& unpacked);
void UnpackColor(int color, math3d::vec3f& unpacked);
