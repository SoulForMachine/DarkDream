
#ifndef _GLTEXTURE_H_
#define _GLTEXTURE_H_

#include "GLObject.h"


namespace GL
{

	class Buffer;
	class SamplerState;
	class Renderer;


	bool BASELIB_API IsCompressedFormat(PixelFormat format);


	class BASELIB_API Texture: public Object
	{
	public:
		Texture();
		~Texture() {}

		void GenerateMipmap();
		PixelFormat GetFormat() const
			{ return _format; }
		bool IsCompressed() const
			{ return IsCompressedFormat(_format); }

	protected:
		virtual bool Create() = 0;
		virtual void Destroy() = 0;

		PixelFormat _format;	// that's internal format
		GLuint* _currentId;
		mutable const SamplerState* _curSamplerState;

	private:
		friend class Renderer;

		Texture(const Texture&);
		Texture& operator = (const Texture&);
	};

	class BASELIB_API Texture1D: public Texture
	{
	public:
		bool TexImage(int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexImage(int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexImage(int level, PixelFormat internal_format, int width, size_t size, void* pixels);
		bool TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexSubImage(int level, int xoffset, int width, ImageFormat format, size_t size, void* pixels);
		bool CopyTexImage(int level, PixelFormat internal_format, int x, int y, int width);
		bool CopyTexSubImage(int level, int xoffset, int x, int y, int width);

		int GetWidth() const
			{ return _width; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		Texture1D() {}
		~Texture1D() {}

		int _width;
	};

	class BASELIB_API Texture2D: public Texture
	{
	public:
		bool TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexImage(int level, PixelFormat internal_format, int width, int height, size_t size, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, size_t size, void* pixels);
		bool CopyTexImage(int level, PixelFormat internal_format, int x, int y, int width, int height);
		bool CopyTexSubImage(int level, int xoffset, int yoffset, int x, int y, int width, int height);

		int GetWidth() const
			{ return _width; }
		int GetHeight() const
			{ return _height; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		Texture2D() {}
		~Texture2D() {}

		int _width;
		int _height;
	};

	class BASELIB_API Texture3D: public Texture
	{
	public:
		bool TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexImage(int level, PixelFormat internal_format, int width, int height, int depth, size_t size, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, size_t size, void* pixels);
		bool CopyTexSubImage(int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height);

		int GetWidth() const
			{ return _width; }
		int GetHeight() const
			{ return _height; }
		int GetDepth() const
			{ return _depth; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		Texture3D() {}
		~Texture3D() {}

		int _width;
		int _height;
		int _depth;
	};

	class BASELIB_API TextureCube: public Texture
	{
	public:
		bool TexImage(int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* const* pixel_store, void** pixels);
		bool TexImage(CubeFace face, int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexImage(CubeFace face, int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexImage(int level, PixelFormat internal_format, int width, size_t size, void** pixels);
		bool CompressedTexImage(CubeFace face, int level, PixelFormat internal_format, int width, size_t size, void* pixels);
		bool TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, size_t size, void* pixels);
		bool CopyTexImage(CubeFace face, int level, PixelFormat internal_format, int x, int y, int width);
		bool CopyTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int x, int y, int width, int height);

		int GetWidth() const
			{ return _width; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		TextureCube() {}
		~TextureCube() {}

		int _width;
	};

	class BASELIB_API Texture1DArray: public Texture
	{
	public:
		bool TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexImage(int level, PixelFormat internal_format, int width, int height, size_t size, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, size_t size, void* pixels);
		bool CopyTexImage(int level, PixelFormat internal_format, int x, int y, int width, int height);
		bool CopyTexSubImage(int level, int xoffset, int yoffset, int x, int y, int width, int height);

		int GetWidth() const
			{ return _width; }
		int GetHeight() const
			{ return _height; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		Texture1DArray() {}
		~Texture1DArray() {}

		int _width;
		int _height;
	};

	class BASELIB_API Texture2DArray: public Texture
	{
	public:
		bool TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexImage(int level, PixelFormat internal_format, int width, int height, int depth, size_t size, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, size_t size, void* pixels);
		bool CopyTexSubImage(int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height);

		int GetWidth() const
			{ return _width; }
		int GetHeight() const
			{ return _height; }
		int GetDepth() const
			{ return _depth; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		Texture2DArray() {}
		~Texture2DArray() {}

		int _width;
		int _height;
		int _depth;
	};

	class BASELIB_API TextureBuffer: public Texture
	{
	public:
		void TexBuffer(PixelFormat internal_format, const Buffer* buffer);

		size_t GetSize() const
			{ return _size; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		// these are not applicable to buffer textures
		using Texture::GenerateMipmap;
		using Texture::IsCompressed;

		TextureBuffer() {}
		~TextureBuffer() {}

		size_t _size;
	};

	class BASELIB_API TextureRectangle: public Texture
	{
	public:
		bool TexImage(PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexImage(PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels);
		bool TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset);
		bool CopyTexImage(PixelFormat internal_format, int x, int y, int width, int height);
		bool CopyTexSubImage(int xoffset, int yoffset, int x, int y, int width, int height);

		int GetWidth() const
			{ return _width; }
		int GetHeight() const
			{ return _height; }

	protected:
		bool Create();
		void Destroy();

	private:
		friend class Renderer;

		TextureRectangle() {}
		~TextureRectangle() {}

		int _width;
		int _height;
	};

}

#endif //_GLTEXTURE_H_
