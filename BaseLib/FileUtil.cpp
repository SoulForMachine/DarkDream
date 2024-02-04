

#include <cassert>
#include "Console.h"
#include "FileUtil.h"


namespace FileUtil
{

	// file name helpers

	// includes trailing '\\'
	void ExtractDir(tchar* dest, const tchar* src)
	{
		const tchar* bslash = tstrrchr(src, _t('\\'));
		if(bslash)
		{
			size_t len = bslash - src + 1;
			tstrncpy(dest, src, len);
			dest[len] = _t('\0');
		}
		else
			tstrcpy(dest, src);
	}

	void ExtractFileName(tchar* dest, const tchar* src)
	{
		const tchar* bslash = tstrrchr(src, _t('\\'));
		if(bslash)
			tstrcpy(dest, bslash + 1);
		else
			tstrcpy(dest, src);
	}

	void ExtractFileNameNoExt(tchar* dest, const tchar* src)
	{
		const tchar* bslash = tstrrchr(src, _t('\\'));
		const tchar* dot = tstrrchr(src, _t('\\'));

		if(bslash)
		{
			if(dot)
			{
				size_t len = dot - bslash;
				tstrncpy(dest, bslash + 1, len);
				dest[len] = _t('\0');
			}
			else
				tstrcpy(dest, bslash + 1);
		}
		else
			tstrcpy(dest, src);
	}

	void ExtractExt(tchar* dest, const tchar* src)
	{
		const tchar* dot = tstrrchr(src, _t('.'));

		if(dot)
			tstrcpy(dest, dot + 1);
		else
			tstrcpy(dest, src);
	}


	// ----------- searching for files -----------------

	bool FileFinder::FindFirstFile(const tchar* path, uint flags)
	{
		HANDLE handle = ::FindFirstFile(path, &m_fd);

		if(handle != INVALID_HANDLE_VALUE)
		{
			m_handle = handle;
			m_flags = flags;

			bool is_dir = (m_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			bool match_flags = ((flags & FINDF_DIRECTORIES) != 0) == is_dir ||
				((flags & FINDF_FILES) != 0) != is_dir;

			return match_flags || FindNextFile();
		}
		else
		{
			return false;
		}
	}

	bool FileFinder::FindNextFile()
	{
		bool match_flags;

		do
		{
			bool result = ::FindNextFile(m_handle, &m_fd) != 0;

			if(!result)
			{
				EndSearch();
				return false;
			}

			bool is_dir = (m_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			match_flags = ((m_flags & FINDF_DIRECTORIES) != 0) == is_dir ||
				((m_flags & FINDF_FILES) != 0) != is_dir;
		}
		while(!match_flags);

		return true;
	}

	void FileFinder::EndSearch()
	{
		if(m_handle)
		{
			FindClose(m_handle);
			m_handle = 0;
		}
	}

	// --------- normal file system file -------------

	bool FsysFile::Open(const tchar* filename, const tchar* mode)
	{
		_file = _wfopen(filename, mode);
		return _file != 0;
	}

	void FsysFile::Close()
	{
		if(_file)
		{
			fclose(_file);
			_file = 0;
		}
	}

	size_t FsysFile::Read(void* buff, size_t size)
	{
		return fread(buff, size, 1, _file);
	}

	size_t FsysFile::Write(const void* buff, size_t size)
	{
		return fwrite(buff, size, 1, _file);
	}

	int FsysFile::Seek(long offset, int origin)
	{
		return fseek(_file, offset, origin);
	}

	long FsysFile::GetPos()
	{
		return ftell(_file);
	}

	long FsysFile::GetSize()
	{
		long oldpos = ftell(_file);
		fseek(_file, 0, SEEK_END);
		long size = ftell(_file);
		fseek(_file, oldpos, SEEK_SET);

		return size;
	}

	FILE_LOCATION FsysFile::GetLocation()
	{
		return FLOC_FILESYSTEM; 
	}

	char FsysFile::ReadChar()
	{
		char val;
		fread(&val, sizeof(char), 1, _file);
		return val;
	}

	unsigned char FsysFile::ReadUnsignedChar()
	{
		unsigned char val;
		fread(&val, sizeof(unsigned char), 1, _file);
		return val;
	}

	short FsysFile::ReadShort()
	{
		short val;
		fread(&val, sizeof(short), 1, _file);
		return val;
	}

	unsigned short FsysFile::ReadUnsignedShort()
	{
		unsigned short val;
		fread(&val, sizeof(unsigned short), 1, _file);
		return val;
	}

	int FsysFile::ReadInt()
	{
		int val;
		fread(&val, sizeof(int), 1, _file);
		return val;
	}

	unsigned int FsysFile::ReadUnsignedInt()
	{
		unsigned int val;
		fread(&val, sizeof(unsigned int), 1, _file);
		return val;
	}

	float FsysFile::ReadFloat()
	{
		float val;
		fread(&val, sizeof(float), 1, _file);
		return val;
	}

	double FsysFile::ReadDouble()
	{
		double val;
		fread(&val, sizeof(double), 1, _file);
		return val;
	}

	bool FsysFile::ReadBool()
	{
		bool val;
		fread(&val, sizeof(bool), 1, _file);
		return val;
	}

	void FsysFile::ReadVec2(float* vec)
	{
		fread(vec, sizeof(float), 2, _file);
	}

	void FsysFile::ReadVec3(float* vec)
	{
		fread(vec, sizeof(float), 3, _file);
	}

	void FsysFile::ReadVec4(float* vec)
	{
		fread(vec, sizeof(float), 4, _file);
	}

	void FsysFile::ReadMat3(float* mat)
	{
		fread(mat, sizeof(float), 9, _file);
	}

	void FsysFile::ReadMat4(float* mat)
	{
		fread(mat, sizeof(float), 16, _file);
	}

	void FsysFile::ReadString(char* strbuf, int len)
	{
		fread(strbuf, sizeof(char), len - 1, _file);
		strbuf[len - 1] = '\0';
	}

	void FsysFile::Scanf(const char* format, ...)
	{
		assert(0 && "nema vfscanf()");
		va_list args;
		va_start(args, format);
	//	vfscanf(_file, format, args);
		va_end(args);
	}

	void FsysFile::WriteChar(char value)
	{
		fwrite(&value, sizeof(char), 1, _file);
	}

	void FsysFile::WriteUnsignedChar(unsigned char value)
	{
		fwrite(&value, sizeof(unsigned char), 1, _file);
	}

	void FsysFile::WriteShort(short value)
	{
		fwrite(&value, sizeof(short), 1, _file);
	}

	void FsysFile::WriteUnsignedShort(unsigned short value)
	{
		fwrite(&value, sizeof(unsigned short), 1, _file);
	}

	void FsysFile::WriteInt(int value)
	{
		fwrite(&value, sizeof(int), 1, _file);
	}

	void FsysFile::WriteUnsignedInt(unsigned int value)
	{
		fwrite(&value, sizeof(unsigned int), 1, _file);
	}

	void FsysFile::WriteFloat(float value)
	{
		fwrite(&value, sizeof(float), 1, _file);
	}

	void FsysFile::WriteDouble(double value)
	{
		fwrite(&value, sizeof(double), 1, _file);
	}

	void FsysFile::WriteBool(bool value)
	{
		fwrite(&value, sizeof(bool), 1, _file);
	}

	void FsysFile::WriteVec2(const float* vec)
	{
		fwrite(&vec, sizeof(float), 2, _file);
	}

	void FsysFile::WriteVec3(const float* vec)
	{
		fwrite(&vec, sizeof(float), 3, _file);
	}

	void FsysFile::WriteVec4(const float* vec)
	{
		fwrite(&vec, sizeof(float), 4, _file);
	}

	void FsysFile::WriteMat3(const float* mat)
	{
		fwrite(&mat, sizeof(float), 9, _file);
	}

	void FsysFile::WriteMat4(const float* mat)
	{
		fwrite(&mat, sizeof(float), 16, _file);
	}

	void FsysFile::WriteString(const char* str)
	{
		size_t len = strlen(str);
		fwrite(str, sizeof(char), len, _file);
	}

	void FsysFile::Printf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		vfprintf(_file, format, args);
		va_end(args);
	}


	void FsysFile::Flush()
	{
		fflush(_file);
	}

	int FsysFile::Eof()
	{
		return feof(_file);
	}

	int FsysFile::GetError()
	{
		return ferror(_file);
	}

	void FsysFile::ClearError()
	{
		clearerr(_file);
	}

	bool FsysFile::IsValid()
	{
		return _file != 0;
	}



	// ---------- archive file --------------

	bool ArchiveFile::Open(const tchar* filename, const tchar* mode)
	{
		return true;
	}

	void ArchiveFile::Close()
	{

	}

	size_t ArchiveFile::Read(void* buff, size_t size)
	{
		return 0;
	}

	size_t ArchiveFile::Write(const void* buff, size_t size)
	{
		return 0;
	}


	int ArchiveFile::Seek(long offset, int origin)
	{
		return 0;
	}

	long ArchiveFile::GetPos()
	{
		return 0;
	}

	long ArchiveFile::GetSize()
	{
		return 0;
	}

	FILE_LOCATION ArchiveFile::GetLocation()
	{
		return FLOC_ARCHIVE; 
	}

	char ArchiveFile::ReadChar()
	{
		return 0;
	}

	unsigned char ArchiveFile::ReadUnsignedChar()
	{
		return 0;
	}

	short ArchiveFile::ReadShort()
	{
		return 0;
	}

	unsigned short ArchiveFile::ReadUnsignedShort()
	{
		return 0;
	}

	int ArchiveFile::ReadInt()
	{
		return 0;
	}

	unsigned int ArchiveFile::ReadUnsignedInt()
	{
		return 0;
	}

	float ArchiveFile::ReadFloat()
	{
		return 0;
	}

	double ArchiveFile::ReadDouble()
	{
		return 0;
	}

	bool ArchiveFile::ReadBool()
	{
		return 0;
	}

	void ArchiveFile::ReadVec2(float* vec)
	{
		
	}

	void ArchiveFile::ReadVec3(float* vec)
	{
		
	}

	void ArchiveFile::ReadVec4(float* vec)
	{
		
	}

	void ArchiveFile::ReadMat3(float* mat)
	{
		
	}

	void ArchiveFile::ReadMat4(float* mat)
	{
		
	}

	void ArchiveFile::ReadString(char* strbuf, int len)
	{
		
	}

	void ArchiveFile::Scanf(const char* format, ...)
	{

	}


	void ArchiveFile::WriteChar(char c)
	{

	}

	void ArchiveFile::WriteUnsignedChar(unsigned char value)
	{
		
	}

	void ArchiveFile::WriteShort(short value)
	{
		
	}

	void ArchiveFile::WriteUnsignedShort(unsigned short value)
	{
		
	}

	void ArchiveFile::WriteInt(int value)
	{
		
	}

	void ArchiveFile::WriteUnsignedInt(unsigned int value)
	{
		
	}

	void ArchiveFile::WriteFloat(float value)
	{
		
	}

	void ArchiveFile::WriteDouble(double value)
	{
		
	}

	void ArchiveFile::WriteBool(bool value)
	{
		
	}

	void ArchiveFile::WriteVec2(const float* vec)
	{
		
	}

	void ArchiveFile::WriteVec3(const float* vec)
	{
		
	}

	void ArchiveFile::WriteVec4(const float* vec)
	{
		
	}

	void ArchiveFile::WriteMat3(const float* mat)
	{
		
	}

	void ArchiveFile::WriteMat4(const float* mat)
	{
		
	}

	void ArchiveFile::WriteString(const char* str)
	{

	}

	void ArchiveFile::Printf(const char* format, ...)
	{

	}


	void ArchiveFile::Flush()
	{

	}


	int ArchiveFile::Eof()
	{
		return 0;
	}

	int ArchiveFile::GetError()
	{
		return 0;
	}

	void ArchiveFile::ClearError()
	{

	}

	bool ArchiveFile::IsValid()
	{
		return false;
	}

} // namespace FileUtil
