
#include <cassert>
#include "GLBuffer.h"
#include "GLTexture.h"



namespace GL
{

#define STATE_MACHINE_HACK \
	if(_id != *_currentId) \
	{ \
		glBindTexture(_target, _id); \
		OPENGL_ERROR_CHECK \
		*_currentId = _id; \
		__gl_state->samplers[0] = _id; \
	}

#define PACK_BUFFER_STATE_MACHINE_HACK \
	if(buffer->_id != __gl_state->pixelPackBuf) \
	{ \
		glBindBufferARB(OBJ_PIXEL_PACK_BUFFER, buffer->_id); \
		OPENGL_ERROR_CHECK \
		__gl_state->pixelPackBuf = buffer->_id; \
	}

#define UNPACK_BUFFER_STATE_MACHINE_HACK \
	if(buffer->_id != __gl_state->pixelUnpackBuf) \
	{ \
		glBindBufferARB(OBJ_PIXEL_UNPACK_BUFFER, buffer->_id); \
		OPENGL_ERROR_CHECK \
		__gl_state->pixelUnpackBuf = buffer->_id; \
	}

	extern GLState* __gl_state;
	void __SetPixelPackState(const PixelStore* pixel_store);
	void __SetPixelUnpackState(const PixelStore* pixel_store);


	bool IsCompressedFormat(PixelFormat format)
	{
		PixelFormat cfmts[] =
		{
			PIXEL_FORMAT_COMPRESSED_R,
			PIXEL_FORMAT_COMPRESSED_RG,
			PIXEL_FORMAT_COMPRESSED_RGB,
			PIXEL_FORMAT_COMPRESSED_RGBA,
			PIXEL_FORMAT_COMPRESSED_SRGB,
			PIXEL_FORMAT_COMPRESSED_SRGB_A,
			PIXEL_FORMAT_COMPRESSED_R_RGTC1,
			PIXEL_FORMAT_COMPRESSED_SIGNED_R_RGTC1,
			PIXEL_FORMAT_COMPRESSED_RG_RGTC2,
			PIXEL_FORMAT_COMPRESSED_SIGNED_RG_RGTC2,
			PIXEL_FORMAT_COMPRESSED_RGB_DXT1,
			PIXEL_FORMAT_COMPRESSED_RGBA_DXT1,
			PIXEL_FORMAT_COMPRESSED_RGBA_DXT3,
			PIXEL_FORMAT_COMPRESSED_RGBA_DXT5,
			PIXEL_FORMAT_COMPRESSED_SRGB_DXT1,
			PIXEL_FORMAT_COMPRESSED_SRGB_A_DXT1,
			PIXEL_FORMAT_COMPRESSED_SRGB_A_DXT3,
			PIXEL_FORMAT_COMPRESSED_SRGB_A_DXT5,
		};

		for(int i = 0; i < sizeof(cfmts) / sizeof(cfmts); ++i)
			if(format == cfmts[i])
				return true;

		return false;
	}

	//==================== Texture ====================

	Texture::Texture()
	{
		_curSamplerState = 0;
		_currentId = 0;
	}

	void Texture::GenerateMipmap()
	{
		STATE_MACHINE_HACK
		glGenerateMipmapEXT(_target);
		OPENGL_ERROR_CHECK
	}

	int Texture::GetCompressedSize(int level) const
	{
		STATE_MACHINE_HACK
		GLint size;
		glGetTexLevelParameteriv(_target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
		OPENGL_ERROR_CHECK
		return size;
	}


	//==================== Texture1D ====================

	bool Texture1D::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_1D, _id);
		OPENGL_ERROR_CHECK
		__gl_state->tex1D = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_1D;
		_width = 0;
		_currentId = &__gl_state->tex1D;

		glTexParameteri(OBJ_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_1D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		OPENGL_ERROR_CHECK
		return true;
	}

	void Texture1D::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->tex1D)
			{
				glBindTexture(OBJ_TEXTURE_1D, 0);
				__gl_state->tex1D = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	bool Texture1D::TexImage(int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexImage1D(OBJ_TEXTURE_1D, level, internal_format, width, 0, format, type, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool Texture1D::TexImage(int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexImage(level, internal_format, width, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture1D::CompressedTexImage(int level, PixelFormat internal_format, int width, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexImage1DARB(OBJ_TEXTURE_1D, level, internal_format, width, 0, size, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool Texture1D::TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexSubImage1D(OBJ_TEXTURE_1D, level, xoffset, width, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1D::TexSubImage(int level, int xoffset, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexSubImage(level, xoffset, width, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture1D::CompressedTexSubImage(int level, int xoffset, int width, ImageFormat format, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexSubImage1DARB(OBJ_TEXTURE_1D, level, xoffset, width, format, size, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1D::CopyTexImage(int level, PixelFormat internal_format, int x, int y, int width)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexImage1D(OBJ_TEXTURE_1D, level, internal_format, x, y, width, 0);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool Texture1D::CopyTexSubImage(int level, int xoffset, int x, int y, int width)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexSubImage1D(OBJ_TEXTURE_1D, level, xoffset, x, y, width);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelPackState(pixel_store);
		glGetTexImage(OBJ_TEXTURE_1D, level, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture1D::GetCompressedTexImage(int level, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		glGetCompressedTexImageARB(OBJ_TEXTURE_1D, level, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1D::GetCompressedTexImage(int level, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
	}


	//==================== Texture2D ====================

	bool Texture2D::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_2D, _id);
		OPENGL_ERROR_CHECK
		__gl_state->tex2D = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_2D;
		_width = 0;
		_height = 0;
		_currentId = &__gl_state->tex2D;

		glTexParameteri(OBJ_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(OBJ_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		OPENGL_ERROR_CHECK
		return true;
	}

	void Texture2D::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->tex2D)
			{
				glBindTexture(OBJ_TEXTURE_2D, 0);
				__gl_state->tex2D = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	bool Texture2D::TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexImage2D(OBJ_TEXTURE_2D, level, internal_format, width, height, 0, format, type, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
		}

		return true;
	}

	bool Texture2D::TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexImage(level, internal_format, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture2D::CompressedTexImage(int level, PixelFormat internal_format, int width, int height, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexImage2DARB(OBJ_TEXTURE_2D, level, internal_format, width, height, 0, size, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
		}

		return true;
	}

	bool Texture2D::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexSubImage2D(OBJ_TEXTURE_2D, level, xoffset, yoffset, width, height, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2D::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexSubImage(level, xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture2D::CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexSubImage2DARB(OBJ_TEXTURE_2D, level, xoffset, yoffset, width, height, format, size, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2D::CopyTexImage(int level, PixelFormat internal_format, int x, int y, int width, int height)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexImage2D(OBJ_TEXTURE_2D, level, internal_format, x, y, width, height, 0);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
		}

		return true;
	}

	bool Texture2D::CopyTexSubImage(int level, int xoffset, int yoffset, int x, int y, int width, int height)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexSubImage2D(OBJ_TEXTURE_2D, level, xoffset, yoffset, x, y, width, height);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelPackState(pixel_store);
		glGetTexImage(OBJ_TEXTURE_2D, level, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture2D::GetCompressedTexImage(int level, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		glGetCompressedTexImageARB(OBJ_TEXTURE_2D, level, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2D::GetCompressedTexImage(int level, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
	}

	//==================== Texture3D ====================

	bool Texture3D::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_3D, _id);
		OPENGL_ERROR_CHECK
		__gl_state->tex3D = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_3D;
		_width = 0;
		_height = 0;
		_depth = 0;
		_currentId = &__gl_state->tex3D;

		glTexParameteri(OBJ_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(OBJ_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(OBJ_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		OPENGL_ERROR_CHECK
		return true;
	}

	void Texture3D::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->tex3D)
			{
				glBindTexture(OBJ_TEXTURE_3D, 0);
				__gl_state->tex3D = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	bool Texture3D::TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexImage3DEXT(OBJ_TEXTURE_3D, level, internal_format, width, height, depth, 0, format, type, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
			_depth = depth;
		}

		return true;
	}

	bool Texture3D::TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexImage(level, internal_format, width, height, depth, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture3D::CompressedTexImage(int level, PixelFormat internal_format, int width, int height, int depth, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexImage3DARB(OBJ_TEXTURE_3D, level, internal_format, width, height, depth, 0, size, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
			_depth = depth;
		}

		return true;
	}
	
	bool Texture3D::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexSubImage3DEXT(OBJ_TEXTURE_3D, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture3D::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexSubImage(level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture3D::CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexSubImage3DARB(OBJ_TEXTURE_3D, level, xoffset, yoffset, zoffset, width, height, depth, format, size, pixels);

		return (glGetError() == GL_NO_ERROR);
	}
	
	bool Texture3D::CopyTexSubImage(int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height)
	{
		assert(false && "do not call this function for now, no GL_EXT_copy_texture extension although glCopyTexSubImage3DEXT entry exists");

		assert(_id);
		STATE_MACHINE_HACK

	//	glCopyTexSubImage3DEXT(OBJ_TEXTURE_3D, level, xoffset, yoffset, zoffset, x, y, width, height);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture3D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelPackState(pixel_store);
		glGetTexImage(OBJ_TEXTURE_3D, level, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture3D::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture3D::GetCompressedTexImage(int level, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		glGetCompressedTexImageARB(OBJ_TEXTURE_3D, level, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture3D::GetCompressedTexImage(int level, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
	}

	//==================== TextureCube ====================

	bool TextureCube::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_CUBE, _id);
		OPENGL_ERROR_CHECK
		__gl_state->texCube = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_CUBE;
		_width = 0;
		_currentId = &__gl_state->texCube;

		glTexParameteri(OBJ_TEXTURE_CUBE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_CUBE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_CUBE, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(OBJ_TEXTURE_CUBE, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(OBJ_TEXTURE_CUBE, GL_TEXTURE_WRAP_T, GL_REPEAT);
		OPENGL_ERROR_CHECK
		return true;
	}

	void TextureCube::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->texCube)
			{
				glBindTexture(OBJ_TEXTURE_CUBE, 0);
				__gl_state->texCube = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	bool TextureCube::TexImage(int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* const* pixel_store, void** pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		for(int i = 0; i < 6; ++i)
		{
			__SetPixelUnpackState(pixel_store? pixel_store[i]: &GLState::defaultPixelStore);
			glTexImage2D(CUBE_FACE_POSITIVE_X + i, level, internal_format, width, width, 0, format, type, pixels[i]);

			if(glGetError() != GL_NO_ERROR)
				return false;
		}

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool TextureCube::TexImage(CubeFace face, int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexImage2D(face, level, internal_format, width, width, 0, format, type, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool TextureCube::TexImage(CubeFace face, int level, PixelFormat internal_format, int width, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexImage(face, level, internal_format, width, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool TextureCube::CompressedTexImage(int level, PixelFormat internal_format, int width, int size, void** pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		for(int i = 0; i < 6; ++i)
		{
			glCompressedTexImage2DARB(CUBE_FACE_POSITIVE_X + i, level, internal_format, width, width, 0, size, pixels[i]);

			if(glGetError() != GL_NO_ERROR)
				return false;
		}

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool TextureCube::CompressedTexImage(CubeFace face, int level, PixelFormat internal_format, int width, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexImage2DARB(face, level, internal_format, width, width, 0, size, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool TextureCube::TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexSubImage2D(face, level, xoffset, yoffset, width, height, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureCube::TexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexSubImage(face, level, xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool TextureCube::CompressedTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int width, int height, ImageFormat format, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glTexSubImage2D(face, level, xoffset, yoffset, width, height, format, size, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureCube::CopyTexImage(CubeFace face, int level, PixelFormat internal_format, int x, int y, int width)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexImage2D(face, level, internal_format, x, y, width, width, 0);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
		}

		return true;
	}

	bool TextureCube::CopyTexSubImage(CubeFace face, int level, int xoffset, int yoffset, int x, int y, int width, int height)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexSubImage2D(face, level, xoffset, yoffset, x, y, width, height);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureCube::GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelPackState(pixel_store);
		glGetTexImage(face, level, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureCube::GetTexImage(CubeFace face, int level, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetTexImage(face, level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool TextureCube::GetCompressedTexImage(CubeFace face, int level, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		glGetCompressedTexImageARB(face, level, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureCube::GetCompressedTexImage(CubeFace face, int level, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetCompressedTexImage(face, level, BUFFER_OFFSET(buffer_offset));
	}

	//==================== Texture1DArray ====================

	bool Texture1DArray::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_1D_ARRAY, _id);
		OPENGL_ERROR_CHECK
		__gl_state->tex1DArray = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_1D_ARRAY;
		_width = 0;
		_height = 0;
		_currentId = &__gl_state->tex1DArray;

		glTexParameteri(OBJ_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
		OPENGL_ERROR_CHECK
		return true;
	}

	void Texture1DArray::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->tex1DArray)
			{
				glBindTexture(OBJ_TEXTURE_1D_ARRAY, 0);
				__gl_state->tex1DArray = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	bool Texture1DArray::TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexImage2D(OBJ_TEXTURE_1D_ARRAY, level, internal_format, width, height, 0, format, type, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
		}

		return true;
	}

	bool Texture1DArray::TexImage(int level, PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexImage(level, internal_format, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture1DArray::CompressedTexImage(int level, PixelFormat internal_format, int width, int height, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexImage2DARB(OBJ_TEXTURE_1D_ARRAY, level, internal_format, width, height, 0, size, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
		}

		return true;
	}

	bool Texture1DArray::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexSubImage2D(OBJ_TEXTURE_1D_ARRAY, level, xoffset, yoffset, width, height, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1DArray::TexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexSubImage(level, xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture1DArray::CompressedTexSubImage(int level, int xoffset, int yoffset, int width, int height, ImageFormat format, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glTexSubImage2D(OBJ_TEXTURE_1D_ARRAY, level, xoffset, yoffset, width, height, format, size, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1DArray::CopyTexImage(int level, PixelFormat internal_format, int x, int y, int width, int height)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexImage2D(OBJ_TEXTURE_1D_ARRAY, level, internal_format, x, y, width, height, 0);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
		}

		return true;
	}

	bool Texture1DArray::CopyTexSubImage(int level, int xoffset, int yoffset, int x, int y, int width, int height)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexSubImage2D(OBJ_TEXTURE_1D_ARRAY, level, xoffset, yoffset, x, y, width, height);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelPackState(pixel_store);
		glGetTexImage(OBJ_TEXTURE_1D_ARRAY, level, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture1DArray::GetCompressedTexImage(int level, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		glGetCompressedTexImageARB(OBJ_TEXTURE_1D_ARRAY, level, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture1DArray::GetCompressedTexImage(int level, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
	}

	//==================== Texture2DArray ====================

	bool Texture2DArray::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_2D_ARRAY, _id);
		OPENGL_ERROR_CHECK
		__gl_state->tex2DArray = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_2D_ARRAY;
		_width = 0;
		_height = 0;
		_depth = 0;
		_currentId = &__gl_state->tex2DArray;

		glTexParameteri(OBJ_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(OBJ_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		OPENGL_ERROR_CHECK
		return true;
	}

	void Texture2DArray::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->tex2DArray)
			{
				glBindTexture(OBJ_TEXTURE_2D_ARRAY, 0);
				__gl_state->tex2DArray = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	bool Texture2DArray::TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexImage3DEXT(OBJ_TEXTURE_2D_ARRAY, level, internal_format, width, height, depth, 0, format, type, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
			_depth = depth;
		}

		return true;
	}

	bool Texture2DArray::TexImage(int level, PixelFormat internal_format, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexImage(level, internal_format, width, height, depth, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture2DArray::CompressedTexImage(int level, PixelFormat internal_format, int width, int height, int depth, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCompressedTexImage3DARB(OBJ_TEXTURE_2D_ARRAY, level, internal_format, width, height, depth, 0, size, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		if(level == 0)
		{
			_format = internal_format;
			_width = width;
			_height = height;
			_depth = depth;
		}

		return true;
	}

	bool Texture2DArray::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexSubImage3DEXT(OBJ_TEXTURE_2D_ARRAY, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2DArray::TexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexSubImage(level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture2DArray::CompressedTexSubImage(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, ImageFormat format, int size, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glTexSubImage3DEXT(OBJ_TEXTURE_2D_ARRAY, level, xoffset, yoffset, zoffset, width, height, depth, format, size, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2DArray::CopyTexSubImage(int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height)
	{
		assert(false && "do not call this function for now, no GL_EXT_copy_texture extension although glCopyTexSubImage3DEXT entry exists");

		assert(_id);
		STATE_MACHINE_HACK

	//	glCopyTexSubImage3DEXT(OBJ_TEXTURE_2D_ARRAY, level, xoffset, yoffset, zoffset, x, y, width, height);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelPackState(pixel_store);
		glGetTexImage(OBJ_TEXTURE_2D_ARRAY, level, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2DArray::GetTexImage(int level, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetTexImage(level, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool Texture2DArray::GetCompressedTexImage(int level, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		glGetCompressedTexImageARB(OBJ_TEXTURE_2D_ARRAY, level, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool Texture2DArray::GetCompressedTexImage(int level, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetCompressedTexImage(level, BUFFER_OFFSET(buffer_offset));
	}

	//==================== TextureBuffer ====================

	bool TextureBuffer::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_BUFFER, _id);
		OPENGL_ERROR_CHECK
		__gl_state->texBuffer = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_BUFFER;
		_size = 0;
		_currentId = &__gl_state->texBuffer;

		return true;
	}

	void TextureBuffer::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->texBuffer)
			{
				glBindTexture(OBJ_TEXTURE_BUFFER, 0);
				__gl_state->texBuffer = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	void TextureBuffer::TexBuffer(PixelFormat internal_format, const Buffer* buffer)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glTexBufferARB(OBJ_TEXTURE_BUFFER, internal_format, buffer->_id);

		OPENGL_ERROR_CHECK

		_format = internal_format;
		_size = buffer->_size;
	}

	//==================== TextureRectangle ====================
	// - REPEAT AND MIRRORED_REPEAT not alowed for texture wrap mode
	// - minification filter must be NEAREST or LINEAR
	// - compressed formats not allowed
	// - mipmaps not allowed

	bool TextureRectangle::Create()
	{
		if(_id)
			return false;
		glGenTextures(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindTexture(OBJ_TEXTURE_RECTANGLE, _id);
		OPENGL_ERROR_CHECK
		__gl_state->texRect = _id;
		__gl_state->samplers[0] = _id;
		_target = OBJ_TEXTURE_RECTANGLE;
		_width = 0;
		_height = 0;
		_currentId = &__gl_state->texRect;

		glTexParameteri(OBJ_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(OBJ_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexParameteri(OBJ_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
		OPENGL_ERROR_CHECK
		return true;
	}

	void TextureRectangle::Destroy()
	{
		if(_id != 0)
		{
			if(_id == __gl_state->texRect)
			{
				glBindTexture(OBJ_TEXTURE_RECTANGLE, 0);
				__gl_state->texRect = 0;
				__gl_state->samplers[0] = 0;
			}
			glDeleteTextures(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	bool TextureRectangle::TexImage(PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexImage2D(OBJ_TEXTURE_RECTANGLE, 0, internal_format, width, height, 0, format, type, pixels);

		if(glGetError() != GL_NO_ERROR)
			return false;

		_format = internal_format;
		_width = width;
		_height = height;

		return true;
	}

	bool TextureRectangle::TexImage(PixelFormat internal_format, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexImage(internal_format, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool TextureRectangle::TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels)
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelUnpackState(pixel_store);
		glTexSubImage2D(OBJ_TEXTURE_RECTANGLE, 0, xoffset, yoffset, width, height, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureRectangle::TexSubImage(int xoffset, int yoffset, int width, int height, ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset)
	{
		UNPACK_BUFFER_STATE_MACHINE_HACK

		return TexSubImage(xoffset, yoffset, width, height, format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

	bool TextureRectangle::CopyTexImage(PixelFormat internal_format, int x, int y, int width, int height)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexImage2D(OBJ_TEXTURE_RECTANGLE, 0, internal_format, x, y, width, height, 0);

		if(glGetError() != GL_NO_ERROR)
			return false;

		_format = internal_format;
		_width = width;
		_height = height;

		return true;
	}

	bool TextureRectangle::CopyTexSubImage(int xoffset, int yoffset, int x, int y, int width, int height)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glCopyTexSubImage2D(OBJ_TEXTURE_RECTANGLE, 0, xoffset, yoffset, x, y, width, height);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureRectangle::GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, void* pixels) const
	{
		assert(_id);
		STATE_MACHINE_HACK

		__SetPixelPackState(pixel_store);
		glGetTexImage(OBJ_TEXTURE_RECTANGLE, 0, format, type, pixels);

		return (glGetError() == GL_NO_ERROR);
	}

	bool TextureRectangle::GetTexImage(ImageFormat format, DataType type, const PixelStore* pixel_store, const Buffer* buffer, size_t buffer_offset) const
	{
		PACK_BUFFER_STATE_MACHINE_HACK

		return GetTexImage(format, type, pixel_store, BUFFER_OFFSET(buffer_offset));
	}

}
