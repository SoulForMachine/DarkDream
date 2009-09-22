
#ifndef _FILESYS_H_
#define _FILESYS_H_


#include "Types.h"
#include "Common.h"
#include "Win32Common.h"
#include <cstdio>
#include <cstdarg>


namespace FileSys
{


	enum FILE_LOCATION
	{
		FLOC_FILESYSTEM,
		FLOC_ARCHIVE,
	};


	// file name helpers
	//=========================================================

	void ExtractDir(tchar* dest, const tchar* src);
	void ExtractFileName(tchar* dest, const tchar* src);
	void ExtractFileNameNoExt(tchar* dest, const tchar* src);
	void ExtractExt(tchar* dest, const tchar* src);

	// searching for files that match specified file name
	//=========================================================

	enum
	{
		FINDF_FILES = 1,
		FINDF_DIRECTORIES = 2,

		FINDF_SEARCH_FILESYS = 4,
		FINDF_SEARCH_ARCHIVES = 8,
	};

	class BASELIB_API FileFinder
	{
	public:
		FileFinder() { m_handle = 0; }
		~FileFinder() { EndSearch(); }

		bool		FindFirstFile(const tchar* path, uint flags = FINDF_FILES);
		const tchar*
					GetFileName() const { return m_fd.cFileName; }
		bool		IsDirectory() const { return (m_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0; }
		bool		FindNextFile();
		void		EndSearch();

	private:
		HANDLE m_handle;
		WIN32_FIND_DATA m_fd;
		uint m_flags;
	};


	class ArchiveDirectory
	{
	};


	// abstract base class for files
	//=========================================================

	class BASELIB_API File
	{
	public:
		virtual ~File() = 0 {}
		virtual bool Open(const tchar* filename, const tchar* mode) = 0;
		virtual void Close() = 0;

		virtual size_t Read(void* buff, size_t size) = 0;
		virtual char ReadChar() = 0;
		virtual unsigned char ReadUnsignedChar() = 0;
		virtual short ReadShort() = 0;
		virtual unsigned short ReadUnsignedShort() = 0;
		virtual int ReadInt() = 0;
		virtual unsigned int ReadUnsignedInt() = 0;
		virtual float ReadFloat() = 0;
		virtual double ReadDouble() = 0;
		virtual bool ReadBool() = 0;
		virtual void ReadVec2(float* vec) = 0;
		virtual void ReadVec3(float* vec) = 0;
		virtual void ReadVec4(float* vec) = 0;
		virtual void ReadMat3(float* mat) = 0;
		virtual void ReadMat4(float* mat) = 0;
		virtual void ReadString(char* strbuf, int len) = 0;
		virtual void Scanf(const char* format, ...) = 0;

		virtual size_t Write(const void* buff, size_t size) = 0;
		virtual void WriteChar(char value) = 0;
		virtual void WriteUnsignedChar(unsigned char value) = 0;
		virtual void WriteShort(short value) = 0;
		virtual void WriteUnsignedShort(unsigned short value) = 0;
		virtual void WriteInt(int value) = 0;
		virtual void WriteUnsignedInt(unsigned int value) = 0;
		virtual void WriteFloat(float value) = 0;
		virtual void WriteDouble(double value) = 0;
		virtual void WriteBool(bool value) = 0;
		virtual void WriteVec2(const float* vec) = 0;
		virtual void WriteVec3(const float* vec) = 0;
		virtual void WriteVec4(const float* vec) = 0;
		virtual void WriteMat3(const float* mat) = 0;
		virtual void WriteMat4(const float* mat) = 0;
		virtual void WriteString(const char* str) = 0;
		virtual void Printf(const char* format, ...) = 0;

		virtual void Flush() = 0;
		virtual int Seek(long offset, int origin) = 0;
		virtual long GetPos() = 0;
		virtual long GetSize() = 0;
		virtual FILE_LOCATION GetLocation() = 0;

		virtual int Eof() = 0;
		virtual int GetError() = 0;
		virtual void ClearError() = 0;

		virtual bool IsValid() = 0;
	};


	// ordinary file system file
	//=========================================================

	class BASELIB_API FsysFile: public File
	{
	public:
		FsysFile() { _file = 0; }
		~FsysFile() { Close(); }

		bool Open(const tchar* filename, const tchar* mode);
		void Close();

		size_t Read(void* buff, size_t size);
		char ReadChar();
		unsigned char ReadUnsignedChar();
		short ReadShort();
		unsigned short ReadUnsignedShort();
		int ReadInt();
		unsigned int ReadUnsignedInt();
		float ReadFloat();
		double ReadDouble();
		bool ReadBool();
		void ReadVec2(float* vec);
		void ReadVec3(float* vec);
		void ReadVec4(float* vec);
		void ReadMat3(float* mat);
		void ReadMat4(float* mat);
		void ReadString(char* strbuf, int len);
		void Scanf(const char* format, ...);

		size_t Write(const void* buff, size_t size);
		void WriteChar(char c);
		void WriteUnsignedChar(unsigned char value);
		void WriteShort(short value);
		void WriteUnsignedShort(unsigned short value);
		void WriteInt(int value);
		void WriteUnsignedInt(unsigned int value);
		void WriteFloat(float value);
		void WriteDouble(double value);
		void WriteBool(bool value);
		void WriteVec2(const float* vec);
		void WriteVec3(const float* vec);
		void WriteVec4(const float* vec);
		void WriteMat3(const float* mat);
		void WriteMat4(const float* mat);
		void WriteString(const char* str);
		void Printf(const char* format, ...);

		void Flush();
		int Seek(long offset, int origin);
		long GetPos();
		long GetSize();
		FILE_LOCATION GetLocation();

		int Eof();
		int GetError();
		void ClearError();

		bool IsValid();

	private:
		FILE* _file;
	};


	// file from archive
	//=========================================================

	class BASELIB_API ArchiveFile: public File
	{
	public:
		~ArchiveFile() { Close(); }

		bool Open(const tchar* filename, const tchar* mode);
		void Close();

		size_t Read(void* buff, size_t size);
		char ReadChar();
		unsigned char ReadUnsignedChar();
		short ReadShort();
		unsigned short ReadUnsignedShort();
		int ReadInt();
		unsigned int ReadUnsignedInt();
		float ReadFloat();
		double ReadDouble();
		bool ReadBool();
		void ReadVec2(float* vec);
		void ReadVec3(float* vec);
		void ReadVec4(float* vec);
		void ReadMat3(float* mat);
		void ReadMat4(float* mat);
		void ReadString(char* strbuf, int len);
		void Scanf(const char* format, ...);

		size_t Write(const void* buff, size_t size);
		void WriteChar(char c);
		void WriteUnsignedChar(unsigned char value);
		void WriteShort(short value);
		void WriteUnsignedShort(unsigned short value);
		void WriteInt(int value);
		void WriteUnsignedInt(unsigned int value);
		void WriteFloat(float value);
		void WriteDouble(double value);
		void WriteBool(bool value);
		void WriteVec2(const float* vec);
		void WriteVec3(const float* vec);
		void WriteVec4(const float* vec);
		void WriteMat3(const float* mat);
		void WriteMat4(const float* mat);
		void WriteString(const char* str);
		void Printf(const char* format, ...);

		void Flush();
		int Seek(long offset, int origin);
		long GetPos();
		long GetSize();
		FILE_LOCATION GetLocation();

		int Eof();
		int GetError();
		void ClearError();

		bool IsValid();
	};


} // namespace FileSys


#endif // _FILESYS_H_
