
#ifndef IMAGE_H
#define IMAGE_H

#include "Common.h"
#include "String.h"


namespace FileSys
{
	class File;
}

enum ImageFileFormat
{
	IMG_UNKNOWN_FORMAT,
	IMG_WINBMP,
	IMG_TARGA,
	IMG_DDS,
};


enum ImagePixelFormat
{
	IMG_LUMINANCE,
	IMG_ALPHA,
	IMG_RGB,
	IMG_RGBA,
	IMG_BGR,
	IMG_BGRA,
	IMG_DXT1,
	IMG_DXT2,
	IMG_DXT3,
	IMG_DXT4,
	IMG_DXT5,
};

enum ImageType
{
	IMG_2D,
	IMG_3D,
	IMG_CUBEMAP,
};

struct MipLevelData
{
	int width;
	int height;
	int depth;
	int bytesPerScanline; // this is same as size for compressed textures
	int size;
	void* pixels;
};

struct ImageData
{
	ImagePixelFormat format;
	ImageType type;
	int bits;
	int mipmapCount;
	bool compressed;

	union
	{
		MipLevelData* images;
		MipLevelData* cubePosX;
	};
	MipLevelData* cubeNegX;
	MipLevelData* cubePosY;
	MipLevelData* cubeNegY;
	MipLevelData* cubePosZ;
	MipLevelData* cubeNegZ;
};


class BASELIB_API Image
{
public:
	Image();
	~Image();

	bool Load(const tchar* fname);
	bool Load(FileSys::File& file);
	bool Load(const tchar* fname, ImageFileFormat fformat);
	bool Load(FileSys::File& file, ImageFileFormat fformat);
	bool Save(const tchar* fname, ImageFileFormat fformat);
	void Unload();

	const ImageData& GetImageData() const
		{ return _data; }

private:
	bool LoadBmp(FileSys::File& file);
	bool LoadTga(FileSys::File& file);
	bool LoadDDS(FileSys::File& file);
	ImageFileFormat DetermineFileFormat(FileSys::File& file);

	ImageData _data;
	bool _loaded;
};


#endif // IMAGE_H
