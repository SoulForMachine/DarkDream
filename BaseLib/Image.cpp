
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <windows.h>
#include <ddraw.h>

#include "Memory.h"
#include "FileUtil.h"
#include "Image.h"


#define DDS_MAGIC			0x20534444
#define DDS_DDSURFACEDESC2_HEADER_SIZE		124
#define DDS_DDPIXELFORMAT_HEADER_SIZE		32
#define DDS_REQUIRED_FLAGS	(DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT)

#define DDS_ATI2			MAKEFOURCC('A', 'T', 'I', '2') // ATI's Normal compression format
#define DDS_A16B16G16R16	0x00000024 	// A16B16G16R16
#define DDS_Q16W16V16U16	0x0000006e 	// Q16W16V16U16
#define DDS_R16F			0x0000006f 	// R16F
#define DDS_G16R16F			0x00000070 	// G16R16F
#define DDS_A16B16G16R16F	0x00000071 	// A16B16G16R16F
#define DDS_R32F			0x00000072 	// R32F
#define DDS_G32R32F			0x00000073 	// G32R32F
#define DDS_A32B32G32R32F	0x00000074 	// A32B32G32R32F
#define DDS_CxV8U8			0x00000075 	// CxV8U8
#define DDS_Q8W8V8U8		0x0000003f 	// Q8W8V8U8


#pragma pack(push, 1)

struct TgaHeader
{
	char id_length;
	char colormap_type;
	char image_type;

	short first_entry;
	short colormap_len;
	char clrmap_entry_size;

	short x_orig;
	short y_orig;
	short img_width;
	short img_height;
	char pixel_depth;
	char img_desc;
};

#pragma pack(pop)


using namespace Memory;
using namespace FileUtil;

static void DDSRead2DTex(File& file, int width, int height, int bytes_per_pixel, int mipmap_count, MipLevelData* images);
static void DDSRead2DTexCompressed(File& file, int width, int height, ImagePixelFormat format, int mipmap_count, MipLevelData* images);



Image::Image()
{
	memset(&_data, 0, sizeof(_data));
	_loaded = false;
}

Image::~Image()
{
	Unload();
}

bool Image::Load(const tchar* fname)
{
	FsysFile file;
	if(!file.Open(fname, _t("rb")))
		return false;
	return Load(file);
}

bool Image::Load(File& file)
{
	ImageFileFormat fformat = DetermineFileFormat(file);
	return Load(file, fformat);
}

bool Image::Load(const tchar* fname, ImageFileFormat fformat)
{
	FsysFile file;
	if(!file.Open(fname, _t("rb")))
		return false;
	return Load(file, fformat);
}

bool Image::Load(File& file, ImageFileFormat fformat)
{
	Unload();

	switch(fformat)
	{
	case IMG_WINBMP:
		return LoadBmp(file);
	case IMG_TARGA:
		return LoadTga(file);
	case IMG_DDS:
		return LoadDDS(file);
	}

	return false;
}

bool Image::Save(const tchar* fname, ImageFileFormat fformat)
{
	return false;
}

void Image::Unload()
{
	if(_loaded)
	{
		if(_data.images)
		{
			for(int i = 0; i < _data.mipmapCount; ++i)
				delete[] _data.images[i].pixels;
			delete[] _data.images;
		}

		if(_data.cubeNegX)
		{
			for(int i = 0; i < _data.mipmapCount; ++i)
				delete[] _data.cubeNegX[i].pixels;
			delete[] _data.cubeNegX;
		}

		if(_data.cubePosY)
		{
			for(int i = 0; i < _data.mipmapCount; ++i)
				delete[] _data.cubePosY[i].pixels;
			delete[] _data.cubePosY;
		}

		if(_data.cubeNegY)
		{
			for(int i = 0; i < _data.mipmapCount; ++i)
				delete[] _data.cubeNegY[i].pixels;
			delete[] _data.cubeNegY;
		}

		if(_data.cubePosZ)
		{
			for(int i = 0; i < _data.mipmapCount; ++i)
				delete[] _data.cubePosZ[i].pixels;
			delete[] _data.cubePosZ;
		}

		if(_data.cubeNegZ)
		{
			for(int i = 0; i < _data.mipmapCount; ++i)
				delete[] _data.cubeNegZ[i].pixels;
			delete[] _data.cubeNegZ;
		}

		memset(&_data, 0, sizeof(_data));
		_loaded = false;
	}
}



bool Image::LoadBmp(File& file)
{
	BITMAPFILEHEADER bfh;

	// read bitmap file header
	file.Read(&bfh, sizeof(bfh));
	if(file.GetError())
	{
		return false;
	}

	// check if we have a bitmap
	if(bfh.bfType != 'MB')
	{
		return false;
	}

	// read bitmap info header
	BITMAPINFOHEADER bih;
	file.Read(&bih, sizeof(bih));
	if(file.GetError())
	{
		return false;
	}

	// check if format is supported
	if(bih.biBitCount < 24)
	{
		return false;
	}

	_data.images = new(tempPool) MipLevelData[1];
	_data.bits = bih.biBitCount;
	_data.format = IMG_BGR;
	_data.mipmapCount = 1;
	_data.type = IMG_2D;

	_data.images[0].width = bih.biWidth;
	_data.images[0].height = bih.biHeight;
	_data.images[0].depth = 1;

	int px_bytes = _data.bits / 8;
	int line_bytes = _data.images[0].width * px_bytes;
	line_bytes = RoundUp(line_bytes, 4);
	int data_size = line_bytes * _data.images[0].height;
	_data.images[0].bytesPerScanline = line_bytes;
	_data.images[0].size = data_size;
	_data.images[0].pixels = new(tempPool) ubyte[data_size];

	ubyte* dest = ((ubyte*)_data.images[0].pixels) + (_data.images[0].height - 1) * line_bytes;

	for(int line_i = 0; line_i < _data.images[0].height; ++line_i)
	{
		file.Read(dest, line_bytes);
		dest -= line_bytes;
	}

	_loaded = true;

	return true;
}

bool Image::LoadTga(File& file)
{
	// read the header
	TgaHeader header;
	file.Read(&header, sizeof(header));
	if(file.GetError())
	{
		return false;
	}

	// don't support colormap images
	if(header.colormap_type)
	{
		return false;
	}

	// check if supported format
	if(header.image_type != 2 &&
		header.image_type != 10 &&
		header.image_type != 3 &&
		header.image_type != 11)
	{
		return false;
	}

	// skip image id if any
	if(header.id_length)
		file.Seek(header.id_length, SEEK_CUR);

	// read image data
	_data.bits = header.pixel_depth;
	_data.mipmapCount = 1;
	_data.type = IMG_2D;

	if(_data.bits == 8)
		_data.format = IMG_LUMINANCE;
	else if(_data.bits == 24)
		_data.format = IMG_RGB;
	else if(_data.bits == 32)
		_data.format = IMG_RGBA;
	else
		return false;

	int px_bytes = _data.bits / 8;
	int num_px = header.img_width * header.img_height;
	int data_size = num_px * px_bytes;

	_data.images = new(tempPool) MipLevelData[1];
	_data.images[0].width = header.img_width;
	_data.images[0].height = header.img_height;
	_data.images[0].depth = 1;
	_data.images[0].bytesPerScanline = px_bytes * header.img_width;
	_data.images[0].size = data_size;

	ubyte* temp = new(tempPool) ubyte[data_size];

	file.Read(temp, data_size);

	_data.images[0].pixels = new(tempPool) ubyte[data_size];

	ubyte* src = temp;
	ubyte* dest = (ubyte*)_data.images[0].pixels;
	if(header.image_type == 2)
	{ // uncompressed rgb(a)
		for(int px = 0; px < num_px; ++px)
		{
			dest[0] = src[2];
			dest[1] = src[1];
			dest[2] = src[0];

			if(px_bytes == 4)
				dest[3] = src[3];
			
			src += px_bytes;
			dest += px_bytes;
		}
	}
	else if(header.image_type == 10)
	{ // RLE compressed rgb(a)
		for(int px = 0; px < num_px;)
		{
			ubyte packet_hdr = *src++;
			int packet_count = (packet_hdr & 0x7f) + 1;

			if(packet_hdr & 0x80)
			{ // run-length packet
				for(int i = 0; i < packet_count; ++i)
				{
					dest[0] = src[2];
					dest[1] = src[1];
					dest[2] = src[0];

					if(px_bytes == 4)
						dest[3] = src[3];

					dest += px_bytes;
					++px;
				}

				src += px_bytes;
			}
			else
			{ // raw packet
				for(int i = 0; i < packet_count; ++i)
				{
					dest[0] = src[2];
					dest[1] = src[1];
					dest[2] = src[0];

					if(px_bytes == 4)
						dest[3] = src[3];

					src += px_bytes;
					dest += px_bytes;
					++px;
				}
			}
		}
	}
	else if(header.image_type == 3)
	{ // uncompressed grayscale
		memcpy(dest, src, data_size);
	}
	else if(header.image_type == 11)
	{ // RLE compressed grayscale
		for(int px = 0; px < num_px; )
		{
			ubyte packet_hdr = *src++;
			int packet_count = (packet_hdr & 0x7f) + 1;

			if(packet_hdr & 0x80)
			{ // run-length packet
				for(int i = 0; i < packet_count; ++i)
				{
					*dest++ = *src;
					++px;
				}
			}
			else
			{ // raw packet
				for(int i = 0; i < packet_count; ++i)
				{
					*dest++ = *src++;
					++px;
				}
			}
		}
	}

	// flip the image verticaly
	int scanline_bytes = _data.images[0].bytesPerScanline;
	ubyte* ptr1 = ((ubyte*)_data.images[0].pixels) + (_data.images[0].height - 1) * scanline_bytes;
	ubyte* ptr2 = (ubyte*)_data.images[0].pixels;
	int count = _data.images[0].height / 2;
	for(int row_i = 0; row_i < count; ++row_i)
	{
		memcpy(temp, ptr1, scanline_bytes);
		memcpy(ptr1, ptr2, scanline_bytes);
		memcpy(ptr2, temp, scanline_bytes);

		ptr1 -= scanline_bytes;
		ptr2 += scanline_bytes;
	}

	delete[] temp;
	_loaded = true;

	return true;
}

bool Image::LoadDDS(File& file)
{
	// check magic value
	DWORD magic;
	file.Read(&magic, sizeof(DWORD));
	if(magic != DDS_MAGIC)
		return false;

	// read the header
	DDSURFACEDESC2 ddsd;
	file.Read(&ddsd, sizeof(ddsd));

	// check DDSURFACEDESC2 header size
	if(ddsd.dwSize != DDS_DDSURFACEDESC2_HEADER_SIZE)
		return false;

	// check required flags
	if((ddsd.dwFlags & DDS_REQUIRED_FLAGS) != DDS_REQUIRED_FLAGS)
		return false;

	// check DDPIXELFORMAT
	if(ddsd.ddpfPixelFormat.dwSize != DDS_DDPIXELFORMAT_HEADER_SIZE)
		return false;


	// get mipmap level count
	if(ddsd.ddsCaps.dwCaps & DDSCAPS_MIPMAP)
	{
		if(ddsd.dwFlags & DDSD_MIPMAPCOUNT)
			_data.mipmapCount = ddsd.dwMipMapCount;
		else
			return false;
	}
	else
	{
		_data.mipmapCount = 1;
	}

	if(_data.mipmapCount <= 0)
		return false;

	// get image format
	_data.compressed = false;
	if(ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHA)
	{
		_data.format = IMG_ALPHA;
		_data.bits = 8;
	}
	if(ddsd.ddpfPixelFormat.dwFlags & DDPF_LUMINANCE)
	{
		_data.format = IMG_LUMINANCE;
		_data.bits = 8;
	}
	else if(ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB)
	{
		if(	ddsd.ddpfPixelFormat.dwBBitMask == 0xFF &&
			ddsd.ddpfPixelFormat.dwGBitMask == 0xFF00 &&
			ddsd.ddpfPixelFormat.dwRBitMask == 0xFF0000 )
		{
			_data.format = (ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)? IMG_BGRA: IMG_BGR;
		}
		else if(ddsd.ddpfPixelFormat.dwRBitMask == 0xFF &&
				ddsd.ddpfPixelFormat.dwGBitMask == 0xFF00 &&
				ddsd.ddpfPixelFormat.dwBBitMask == 0xFF0000 )
		{
			_data.format = (ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)? IMG_RGBA: IMG_RGB;
		}

		_data.bits = (ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)? 32: 24;
	}
	else if(ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC)
	{
		switch(ddsd.ddpfPixelFormat.dwFourCC)
		{
		case MAKEFOURCC('D', 'X', 'T', '1'):
			_data.format = IMG_DXT1;
			break;
		case MAKEFOURCC('D', 'X', 'T', '2'):
			_data.format = IMG_DXT2;
			break;
		case MAKEFOURCC('D', 'X', 'T', '3'):
			_data.format = IMG_DXT3;
			break;
		case MAKEFOURCC('D', 'X', 'T', '4'):
			_data.format = IMG_DXT4;
			break;
		case MAKEFOURCC('D', 'X', 'T', '5'):
			_data.format = IMG_DXT5;
			break;
		default:
			// format not supported
			return false;
		}

		_data.compressed = true;
	}
	else
	{
		// format not supported
		return false;
	}

	// get image type
	if(ddsd.ddsCaps.dwCaps2 & DDSCAPS2_VOLUME)
		_data.type = IMG_3D;
	else if(ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP)
		_data.type = IMG_CUBEMAP;
	else
		_data.type = IMG_2D;

	if(_data.type == IMG_CUBEMAP)
	{
		// if this is a cubemap, require all faces to be present
		if((ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_ALLFACES) != DDSCAPS2_CUBEMAP_ALLFACES)
			return false;

		_data.cubePosX = new(tempPool) MipLevelData[_data.mipmapCount];
		_data.cubeNegX = new(tempPool) MipLevelData[_data.mipmapCount];
		_data.cubePosY = new(tempPool) MipLevelData[_data.mipmapCount];
		_data.cubeNegY = new(tempPool) MipLevelData[_data.mipmapCount];
		_data.cubePosZ = new(tempPool) MipLevelData[_data.mipmapCount];
		_data.cubeNegZ = new(tempPool) MipLevelData[_data.mipmapCount];
	}
	else
	{
		_data.images = new(tempPool) MipLevelData[_data.mipmapCount];
	}

	if(_data.type == IMG_2D)
	{
		if(_data.compressed)
		{
			DDSRead2DTexCompressed(file, ddsd.dwWidth, ddsd.dwHeight, _data.format, _data.mipmapCount, _data.images);
		}
		else
		{
			DDSRead2DTex(file, ddsd.dwWidth, ddsd.dwHeight, _data.bits / 8, _data.mipmapCount, _data.images);
		}
	}
	else if(_data.type == IMG_3D)
	{
		int width = ddsd.dwWidth;
		int height = ddsd.dwHeight;
		int depth = ddsd.dwDepth;

		for(int i = 0; i < _data.mipmapCount; ++i)
		{
			int pitch = RoundUp(width * (_data.bits / 8), 4);
			int size = pitch * height * depth;

			_data.images[i].width = width;
			_data.images[i].height = height;
			_data.images[i].depth = depth;
			_data.images[i].bytesPerScanline = pitch;
			_data.images[i].size = size;
			_data.images[i].pixels = new(tempPool) ubyte[size];
			file.Read(_data.images[i].pixels, (size_t)size);

			width = max(1, width / 2);
			height = max(1, height / 2);
			depth = max(1, depth / 2);
		}
	}
	else if(_data.type == IMG_CUBEMAP)
	{
		if(_data.compressed)
		{
			DDSRead2DTexCompressed(file, ddsd.dwWidth, ddsd.dwHeight, _data.format, _data.mipmapCount, _data.cubePosX);
			DDSRead2DTexCompressed(file, ddsd.dwWidth, ddsd.dwHeight, _data.format, _data.mipmapCount, _data.cubeNegX);
			DDSRead2DTexCompressed(file, ddsd.dwWidth, ddsd.dwHeight, _data.format, _data.mipmapCount, _data.cubePosY);
			DDSRead2DTexCompressed(file, ddsd.dwWidth, ddsd.dwHeight, _data.format, _data.mipmapCount, _data.cubeNegY);
			DDSRead2DTexCompressed(file, ddsd.dwWidth, ddsd.dwHeight, _data.format, _data.mipmapCount, _data.cubePosZ);
			DDSRead2DTexCompressed(file, ddsd.dwWidth, ddsd.dwHeight, _data.format, _data.mipmapCount, _data.cubeNegZ);
		}
		else
		{
			DDSRead2DTex(file, ddsd.dwWidth, ddsd.dwHeight, _data.bits / 8, _data.mipmapCount, _data.cubePosX);
			DDSRead2DTex(file, ddsd.dwWidth, ddsd.dwHeight, _data.bits / 8, _data.mipmapCount, _data.cubeNegX);
			DDSRead2DTex(file, ddsd.dwWidth, ddsd.dwHeight, _data.bits / 8, _data.mipmapCount, _data.cubePosY);
			DDSRead2DTex(file, ddsd.dwWidth, ddsd.dwHeight, _data.bits / 8, _data.mipmapCount, _data.cubeNegY);
			DDSRead2DTex(file, ddsd.dwWidth, ddsd.dwHeight, _data.bits / 8, _data.mipmapCount, _data.cubePosZ);
			DDSRead2DTex(file, ddsd.dwWidth, ddsd.dwHeight, _data.bits / 8, _data.mipmapCount, _data.cubeNegZ);
		}
	}

	_loaded = true;
	return true;
}

ImageFileFormat Image::DetermineFileFormat(FileUtil::File& file)
{
	long oldpos = file.GetPos();

	// check if BMP
	BITMAPFILEHEADER bfh;
	file.Read(&bfh, sizeof(bfh));
	if(!file.GetError())
	{
		if(bfh.bfType == 'MB')
		{
			file.Seek(oldpos, SEEK_SET);
			return IMG_WINBMP;
		}
	}

	// check if TGA
	char buf[26];
	file.Seek(-26, SEEK_END);
	file.Read(buf, 26);
	if(!file.GetError())
	{
		if(!strncmp(&buf[8], "TRUEVISION-XFILE", 16))
		{
			file.Seek(oldpos, SEEK_SET);
			return IMG_TARGA;
		}
	}

	// check if DDS
	file.Seek(oldpos, SEEK_SET);
	DWORD magic;
	file.Read(&magic, sizeof(DWORD));
	if(magic == DDS_MAGIC)
	{
		file.Seek(oldpos, SEEK_SET);
		return IMG_DDS;
	}

	file.Seek(oldpos, SEEK_SET);
	return IMG_UNKNOWN_FORMAT;
}

void DDSRead2DTex(File& file, int width, int height, int bytes_per_pixel, int mipmap_count, MipLevelData* images)
{
	for(int i = 0; i < mipmap_count; ++i)
	{
		int pitch = RoundUp(width * bytes_per_pixel, 4);
		int size = pitch * height;

		images[i].width = width;
		images[i].height = height;
		images[i].depth = 1;
		images[i].bytesPerScanline = pitch;
		images[i].size = size;
		images[i].pixels = new(tempPool) ubyte[size];
		file.Read(images[i].pixels, (size_t)size);

		width = max(1, width / 2);
		height = max(1, height / 2);
	}
}

void DDSRead2DTexCompressed(File& file, int width, int height, ImagePixelFormat format, int mipmap_count, MipLevelData* images)
{
	int min_size = (format == IMG_DXT1)? 8: 16;

	for(int i = 0; i < mipmap_count; ++i)
	{
		int size = max(1, width / 4) * max(1, height / 4) * min_size;

		images[i].width = width;
		images[i].height = height;
		images[i].depth = 1;
		images[i].bytesPerScanline = size;
		images[i].size = size;
		images[i].pixels = new(tempPool) ubyte[size];
		file.Read(images[i].pixels, (size_t)size);

		width = max(1, width / 2);
		height = max(1, height / 2);	
	}
}


/* =============================================
	Code taken from OpenSG
============================================= */

struct DXTColBlock {
  unsigned short col0;
  unsigned short col1;

  unsigned char row[4];
};

struct DXT3AlphaBlock {
  unsigned short row[4];
};

struct DXT5AlphaBlock
{
  unsigned char alpha0;
  unsigned char alpha1;

  unsigned char row[6];
};



///////////////////////////////////////////////////////////////////////////////
// swap to sections of memory
void swap(void *byte1, void *byte2, int size)
{
    unsigned char *tmp = new unsigned char[size];

    memcpy(tmp, byte1, size);
    memcpy(byte1, byte2, size);
    memcpy(byte2, tmp, size);

    delete [] tmp;
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT1 color block
void flip_blocks_dxtc1(DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;

    for (int i = 0; i < numBlocks; i++)
    {
        swap(&curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swap(&curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT3 color block
void flip_blocks_dxtc3(DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;
    DXT3AlphaBlock *alphablock;

    for (int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT3AlphaBlock*)curblock;

        swap(&alphablock->row[0], &alphablock->row[3], sizeof(unsigned short));
        swap(&alphablock->row[1], &alphablock->row[2], sizeof(unsigned short));

        curblock++;

        swap(&curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swap(&curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT5 alpha block
void flip_dxt5_alpha(DXT5AlphaBlock *block)
{
    unsigned char gBits[4][4];

    const unsigned long mask = 0x00000007;          // bits = 00 00 01 11
    unsigned long bits = 0;
    memcpy(&bits, &block->row[0], sizeof(unsigned char) * 3);

    gBits[0][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[0][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[0][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[0][3] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[1][3] = (unsigned char)(bits & mask);

    bits = 0;
    memcpy(&bits, &block->row[3], sizeof(unsigned char) * 3);

    gBits[2][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[2][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[2][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[2][3] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][0] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][1] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][2] = (unsigned char)(bits & mask);
    bits >>= 3;
    gBits[3][3] = (unsigned char)(bits & mask);

    unsigned long *pBits = ((unsigned long*) &(block->row[0]));

    *pBits = *pBits | (gBits[3][0] << 0);
    *pBits = *pBits | (gBits[3][1] << 3);
    *pBits = *pBits | (gBits[3][2] << 6);
    *pBits = *pBits | (gBits[3][3] << 9);

    *pBits = *pBits | (gBits[2][0] << 12);
    *pBits = *pBits | (gBits[2][1] << 15);
    *pBits = *pBits | (gBits[2][2] << 18);
    *pBits = *pBits | (gBits[2][3] << 21);

    pBits = ((unsigned long*) &(block->row[3]));

#ifdef MACOS
    *pBits &= 0x000000ff;
#else
    *pBits &= 0xff000000;
#endif

    *pBits = *pBits | (gBits[1][0] << 0);
    *pBits = *pBits | (gBits[1][1] << 3);
    *pBits = *pBits | (gBits[1][2] << 6);
    *pBits = *pBits | (gBits[1][3] << 9);

    *pBits = *pBits | (gBits[0][0] << 12);
    *pBits = *pBits | (gBits[0][1] << 15);
    *pBits = *pBits | (gBits[0][2] << 18);
    *pBits = *pBits | (gBits[0][3] << 21);
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT5 color block
void flip_blocks_dxtc5(DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;
    DXT5AlphaBlock *alphablock;

    for (int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT5AlphaBlock*)curblock;

        flip_dxt5_alpha(alphablock);

        curblock++;

        swap(&curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swap(&curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// flip image around X axis
void flip(char *image, int width, int height, int depth, int size, bool compressed, ImagePixelFormat format)
{
    int linesize;
    int offset;

    if (!compressed)
    {
        assert(depth > 0);

        int imagesize = size/depth;
        linesize = imagesize / height;

        for (int n = 0; n < depth; n++)
        {
            offset = imagesize*n;
            char *top = image + offset;
            char *bottom = top + (imagesize-linesize);

            for (int i = 0; i < (height >> 1); i++)
            {
                swap(bottom, top, linesize);

                top += linesize;
                bottom -= linesize;
            }
        }
    }
    else
    {
        void (*flipblocks)(DXTColBlock*, int);
        int xblocks = width / 4;
        int yblocks = height / 4;
        int blocksize;

        switch (format)
        {
            case IMG_DXT1:
                blocksize = 8;
                flipblocks = &flip_blocks_dxtc1;
                break;
            case IMG_DXT3:
                blocksize = 16;
                flipblocks = &flip_blocks_dxtc3;
                break;
            case IMG_DXT5:
                blocksize = 16;
                flipblocks = &flip_blocks_dxtc5;
                break;
            default:
                return;
        }

        linesize = xblocks * blocksize;

        DXTColBlock *top;
        DXTColBlock *bottom;

        for (int j = 0; j < (yblocks >> 1); j++)
        {
            top = (DXTColBlock*)(image + j * linesize);
            bottom = (DXTColBlock*)(image + (((yblocks-j)-1) * linesize));

            flipblocks(top, xblocks);
            flipblocks(bottom, xblocks);

            swap(bottom, top, linesize);
        }
    }
}
