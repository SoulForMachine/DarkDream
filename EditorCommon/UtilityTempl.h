#pragma once


namespace EditorCommon
{

	/*
		Converts System::String to tchar*. Caller must delete returned buffer.
	*/
	template <typename _CharType>
	_CharType* ConvertString(System::String^ str)
	{
		msclr::interop::marshal_context^ context = gcnew msclr::interop::marshal_context;
		const _CharType* cs = context->marshal_as<const _CharType*>(str);
		int len = 0;
		const _CharType* p = cs;
		while(*p++) len++;
		_CharType* ret = Memory::NewArray<_CharType>(Memory::tempPool, len + 1);
		_CharType* dest = ret;
		while(*dest++ = *cs++);
		delete context;
		return ret;
	}

	inline
	int PackColor(const math3d::vec4f& color)
	{
		return (
			(int(color.a * 255) << 24) |
			(int(color.r * 255) << 16) |
			(int(color.g * 255) << 8) |
			(int(color.b * 255)) );
	}

	inline
	int PackColor(const math3d::vec3f& color)
	{
		return (
			(255 << 24) |
			(int(color.r * 255) << 16) |
			(int(color.g * 255) << 8) |
			(int(color.b * 255)) );
	}

	inline
	void UnpackColor(int color, math3d::vec4f& unpacked)
	{
		unpacked.b = float(color & 0xFF) / 255.0f;
		unpacked.g = float(unsigned(color & 0xFF00) >> 8) / 255.0f;
		unpacked.r = float(unsigned(color & 0xFF0000) >> 16) / 255.0f;
		unpacked.a = float(unsigned(color & 0xFF000000) >> 24) / 255.0f;
	}

	inline
	void UnpackColor(int color, math3d::vec3f& unpacked)
	{
		unpacked.b = float(color & 0xFF) / 255.0f;
		unpacked.g = float(unsigned(color & 0xFF00) >> 8) / 255.0f;
		unpacked.r = float(unsigned(color & 0xFF0000) >> 16) / 255.0f;
	}

}
