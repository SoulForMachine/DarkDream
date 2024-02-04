

#include "BaseLib/SmartPtr.h"
#include "BaseLib/Console.h"
#include "BaseLib/Memory.h"
#include "BaseLib/Image.h"
#include "FileSystem.h"
#include "EngineInternal.h"
#include "RenderSystem.h"
#include "Model.h"
#include "Material.h"
#include "Animation.h"
#include "ParticleSystem.h"
#include "ModelEntity.h"
#include "StaticEntity.h"
#include "Resource.h"


using namespace Memory;
using namespace GL;


namespace Engine
{

	static
	GL::PixelFormat GetGLInternalFormat(ImagePixelFormat image_format)
	{
		switch(image_format)
		{
		case IMG_LUMINANCE:
		case IMG_ALPHA:
			return PIXEL_FORMAT_R8;
		case IMG_RGB:
		case IMG_BGR:
			return PIXEL_FORMAT_RGB8;
		case IMG_RGBA:
		case IMG_BGRA:
			return PIXEL_FORMAT_RGBA8;
		default:
			assert(!"image_format not valid");
			return PIXEL_FORMAT_RGBA8;
		}
	}

	static
	GL::ImageFormat GetGLFormat(ImagePixelFormat image_format)
	{
		switch(image_format)
		{
		case IMG_LUMINANCE:
			return IMAGE_FORMAT_LUMINANCE;
		case IMG_ALPHA:
			return IMAGE_FORMAT_ALPHA;
		case IMG_RGB:
			return IMAGE_FORMAT_RGB;
		case IMG_RGBA:
			return IMAGE_FORMAT_RGBA;
		case IMG_BGR:
			return IMAGE_FORMAT_BGR;
		case IMG_BGRA:
			return IMAGE_FORMAT_BGRA;
		default:
			assert(!"image_format not valid");
			return IMAGE_FORMAT_RGBA;
		}
	}

	static
	GL::PixelFormat GetGLCompressedFormat(ImagePixelFormat image_format)
	{
		switch(image_format)
		{
		case IMG_DXT1:
			return PIXEL_FORMAT_COMPRESSED_RGBA_DXT1;
		case IMG_DXT3:
			return PIXEL_FORMAT_COMPRESSED_RGBA_DXT3;
		case IMG_DXT5:
			return PIXEL_FORMAT_COMPRESSED_RGBA_DXT5;
		default:
			assert(!"image_format not valid");
			return PIXEL_FORMAT_COMPRESSED_RGBA_DXT5;
		}
	}


	// ============ Resource =================

	template <class _Res, int _ResSubType>
	Resource<_Res, _ResSubType>::Resource(const tchar* file_name)
	{
		_resource = _null;
		_fileName = StringDup(file_name);
		_refCount = 1;
	}

	template <class _Res, int _ResSubType>
	Resource<_Res, _ResSubType>::~Resource()
	{
		Memory::Delete(_fileName);
	}

	template <class _Res, int _ResSubType>
	bool Resource<_Res, _ResSubType>::IsLoaded() const
	{
		return (_resource != _null);
	}

	// ============ Texture2DRes =================

	template <>
	GL::Texture2D* Resource<GL::Texture2D, 0>::_null{};

	Texture2DRes::Texture2DRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	Texture2DRes::~Texture2DRes()
	{
		Unload();
	}

	bool Texture2DRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading 2D texture: %ls", _fileName);

		// load the texture from file
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(_fileName, _t("rb"));	
		if(!file)
		{
			Console::PrintError("Failed to open texture file: %ls", _fileName);
			return false;
		}

		Image img;
		if(img.Load(*file))
		{
			if(img.GetImageData().type == IMG_2D)
			{
				_resource = LoadTexture2D(img);

				if(_resource != _null)
				{
					if(img.GetImageData().mipmapCount == 1)
						_resource->GenerateMipmap();
					return true;
				}
				else
				{
					Console::PrintError("Failed to create texture object for file: %ls", _fileName);
					return false;
				}
			}
			else
			{
				Console::PrintError("File %ls is not a 2D texture", _fileName);
				return false;
			}
		}
		else
		{
			Console::PrintError("Failed to load texture file: %ls", _fileName);
			return false;
		}
	}

	bool Texture2DRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void Texture2DRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(_resource);
			_resource = _null;
		}
	}

	GL::Texture2D* Texture2DRes::CreateDefault()
	{
		Texture2D* texture = engineAPI.renderSystem->GetRenderer()->CreateTexture2D();
		if(!texture)
			return 0;

		const int width = 32;
		const int height = 32;
		ubyte* pixels = NewArray<ubyte>(tempPool, width * height * 3);
		ubyte* ptr = pixels;
		for(int i = 0; i < width; ++i)
		{
			for(int j = 0; j < height; ++j)
			{
				int m = (i < width / 2)? 1: 0;
				int n = (j < height / 2)? 1: 0;
				if(m ^ n)
				{
					ptr[0] = 255;
					ptr[1] = 255;
					ptr[2] = 255;
				}
				else
				{
					ptr[0] = 255;
					ptr[1] = 0;
					ptr[2] = 255;
				}
				ptr += 3;
			}
		}

		PixelStore ps = GLState::defaultPixelStore;
		ps.alignment = 1;
		bool result = texture->TexImage(0, PIXEL_FORMAT_RGB8, width, height, IMAGE_FORMAT_RGB, TYPE_UNSIGNED_BYTE, &ps, pixels);
		Memory::Delete(pixels);
		if(result)
		{
			texture->GenerateMipmap();
		}
		else
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
		}

		return result? texture: 0;
	}

	bool Texture2DRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void Texture2DRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(_null);
			_null = 0;
		}
	}

	Texture2D* Texture2DRes::LoadTexture2D(const Image& image)
	{
		const ImageData& data = image.GetImageData();
		PixelFormat internal_format;
		ImageFormat format;
		Texture2D* texture = engineAPI.renderSystem->GetRenderer()->CreateTexture2D();
		if(!texture)
			return _null;

		if(data.compressed)
		{
			internal_format = GetGLCompressedFormat(data.format);

			for(int i = 0; i < data.mipmapCount; ++i)
			{
				bool result = texture->CompressedTexImage(
					i, internal_format, data.images[i].width, data.images[i].height,
					data.images[i].size, data.images[i].pixels);
				if(!result)
				{
					engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
					return _null;
				}
			}
		}
		else
		{
			format = GetGLFormat(data.format);
			internal_format = GetGLInternalFormat(data.format);

			PixelStore ps = GLState::defaultPixelStore;
			ps.alignment = 1;
			for(int i = 0; i < data.mipmapCount; ++i)
			{
				int row_length = data.images[i].bytesPerScanline / (data.bits / 8);
				ps.rowLength = row_length;
				bool result = texture->TexImage(
					i, internal_format, data.images[i].width, data.images[i].height,
					format, TYPE_UNSIGNED_BYTE, &ps, data.images[i].pixels);
				if(!result)
				{
					engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
					return _null;
				}
			}
		}

		return texture;
	}

	// ============ Texture3DRes =================

	template <>
	GL::Texture3D* Resource<GL::Texture3D, 0>::_null{};


	Texture3DRes::Texture3DRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	Texture3DRes::~Texture3DRes()
	{
		Unload();
	}

	bool Texture3DRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading 3D texture: %ls", _fileName);

		// load the texture from file
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(_fileName, _t("rb"));
		if(!file)
		{
			Console::PrintError("Failed to open texture file: %ls", _fileName);
			return false;
		}

		Image img;
		if(img.Load(*file))
		{
			if(img.GetImageData().type == IMG_3D)
			{
				_resource = LoadTexture3D(img);

				if(_resource != _null)
				{
					if(img.GetImageData().mipmapCount == 1)
						_resource->GenerateMipmap();
					return true;
				}
				else
				{
					Console::PrintError("Failed to create texture object for file: %ls", _fileName);
					return false;
				}
			}
			else
			{
				Console::PrintError("File %ls is not a 3D texture", _fileName);
				return false;
			}
		}
		else
		{
			Console::PrintError("Failed to load texture file: %ls", _fileName);
			return false;
		}
	}

	bool Texture3DRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void Texture3DRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(_resource);
			_resource = _null;
		}
	}

	GL::Texture3D* Texture3DRes::CreateDefault()
	{
		Texture3D* texture = engineAPI.renderSystem->GetRenderer()->CreateTexture3D();
		if(!texture)
			return 0;

		const int width = 32;
		const int height = 32;
		const int depth = 32;
		ubyte* pixels = NewArray<ubyte>(tempPool, width * height * depth * 3);
		ubyte* ptr = pixels;
		for(int i = 0; i < width; ++i)
		{
			for(int j = 0; j < height; ++j)
			{
				for(int k = 0; k < depth; ++k)
				{
					int m = (i < width / 2)? 1: 0;
					int n = (j < height / 2)? 1: 0;
					int o = (k < depth / 2)? 1: 0;
					if(m ^ n ^ o)
					{
						ptr[0] = 255;
						ptr[1] = 255;
						ptr[2] = 255;
					}
					else
					{
						ptr[0] = 255;
						ptr[1] = 0;
						ptr[2] = 255;
					}
					ptr += 3;
				}
			}
		}

		PixelStore ps = GLState::defaultPixelStore;
		ps.alignment = 1;
		bool result = texture->TexImage(0, PIXEL_FORMAT_RGB8, width, height, depth, IMAGE_FORMAT_RGB, TYPE_UNSIGNED_BYTE, &ps, pixels);
		Memory::Delete(pixels);
		if(result)
		{
			texture->GenerateMipmap();
		}
		else
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
		}
		return result? texture: 0;
	}

	bool Texture3DRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void Texture3DRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(_null);
			_null = 0;
		}
	}

	Texture3D* Texture3DRes::LoadTexture3D(const Image& image)
	{
		const ImageData& data = image.GetImageData();
		PixelFormat internal_format;
		ImageFormat format;
		Texture3D* texture = engineAPI.renderSystem->GetRenderer()->CreateTexture3D();
		if(!texture)
			return _null;

		if(data.compressed)
		{
			internal_format = GetGLCompressedFormat(data.format);

			for(int i = 0; i < data.mipmapCount; ++i)
			{
				bool result = texture->CompressedTexImage(
					i, internal_format, data.images[i].width, data.images[i].height,
					data.images[i].depth, data.images[i].size, data.images[i].pixels);
				if(!result)
				{
					engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
					return _null;
				}
			}
		}
		else
		{
			format = GetGLFormat(data.format);
			internal_format = GetGLInternalFormat(data.format);

			PixelStore ps = GLState::defaultPixelStore;
			ps.alignment = 1;
			for(int i = 0; i < data.mipmapCount; ++i)
			{
				int row_length = data.images[i].bytesPerScanline / (data.bits / 8);
				ps.rowLength = row_length;
				bool result = texture->TexImage(
					i, internal_format, data.images[i].width, data.images[i].height,
					data.images[i].depth, format, TYPE_UNSIGNED_BYTE, &ps, data.images[i].pixels);
				if(!result)
				{
					engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
					return _null;
				}
			}
		}

		return texture;
	}

	// ============ TextureCubeRes =================

	template <>
	GL::TextureCube* Resource<GL::TextureCube, 0>::_null{};


	TextureCubeRes::TextureCubeRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	TextureCubeRes::~TextureCubeRes()
	{
		Unload();
	}

	bool TextureCubeRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading cube texture: %ls", _fileName);

		// load the texture from file
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(_fileName, _t("rb"));
		if(!file)
		{
			Console::PrintError("Failed to open texture file: %ls", _fileName);
			return false;
		}

		Image img;
		if(img.Load(*file))
		{
			if(img.GetImageData().type == IMG_CUBEMAP)
			{			
				_resource = LoadTextureCube(img);

				if(_resource != _null)
				{
					if(img.GetImageData().mipmapCount == 1)
						_resource->GenerateMipmap();
					return true;
				}
				else
				{
					Console::PrintError("Failed to create texture object for file: %ls", _fileName);
					return false;
				}
			}
			else
			{
				Console::PrintError("File %ls is not a cube texture", _fileName);
				return false;
			}
		}
		else
		{
			Console::PrintError("Failed to load texture file: %ls", _fileName);
			return false;
		}
	}

	bool TextureCubeRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void TextureCubeRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(_resource);
			_resource = _null;
		}
	}

	GL::TextureCube* TextureCubeRes::CreateDefault()
	{
		TextureCube* texture = engineAPI.renderSystem->GetRenderer()->CreateTextureCube();
		if(!texture)
			return 0;

		const int width = 32;
		ubyte* pixels[6] = {
			NewArray<ubyte>(tempPool, width * width * 3),
			NewArray<ubyte>(tempPool, width * width * 3),
			NewArray<ubyte>(tempPool, width * width * 3),
			NewArray<ubyte>(tempPool, width * width * 3),
			NewArray<ubyte>(tempPool, width * width * 3),
			NewArray<ubyte>(tempPool, width * width * 3)
		};
		for(int face_i = 0; face_i < 6; ++face_i)
		{
			ubyte* ptr = pixels[face_i];
			for(int i = 0; i < width; ++i)
			{
				for(int j = 0; j < width; ++j)
				{
					int m = (i < width / 2)? 1: 0;
					int n = (j < width / 2)? 1: 0;
					if(m ^ n)
					{
						ptr[0] = 255;
						ptr[1] = 255;
						ptr[2] = 255;
					}
					else
					{
						ptr[0] = 255;
						ptr[1] = 0;
						ptr[2] = 255;
					}
					ptr += 3;
				}
			}
		}

		PixelStore ps = GLState::defaultPixelStore;
		ps.alignment = 1;
		PixelStore* pss[6] = { &ps, &ps, &ps, &ps, &ps, &ps };
		bool result = texture->TexImage(0, PIXEL_FORMAT_RGB8, width, IMAGE_FORMAT_RGB, TYPE_UNSIGNED_BYTE, pss, (void**)pixels);
		for(int face_i = 0; face_i < 6; ++face_i)
			Memory::Delete(pixels[face_i]);
		if(result)
		{
			texture->GenerateMipmap();
		}
		else
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
		}
		return result? texture: 0;
	}

	bool TextureCubeRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void TextureCubeRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyTexture(_null);
			_null = 0;
		}
	}

	TextureCube* TextureCubeRes::LoadTextureCube(const Image& image)
	{
		const ImageData& data = image.GetImageData();
		PixelFormat internal_format;
		ImageFormat format;
		TextureCube* texture = engineAPI.renderSystem->GetRenderer()->CreateTextureCube();
		if(!texture)
			return _null;

		if(data.compressed)
		{
			internal_format = GetGLCompressedFormat(data.format);

			for(int i = 0; i < data.mipmapCount; ++i)
			{
				void* pixels[] =
				{
					data.cubePosX[i].pixels,
					data.cubeNegX[i].pixels,
					data.cubePosY[i].pixels,
					data.cubeNegY[i].pixels,
					data.cubePosZ[i].pixels,
					data.cubeNegZ[i].pixels
				};
				bool result = texture->CompressedTexImage(i, internal_format, data.images[i].width, data.images[i].size, pixels);
				if(!result)
				{
					engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
					return _null;
				}
			}
		}
		else
		{
			format = GetGLFormat(data.format);
			internal_format = GetGLInternalFormat(data.format);

			PixelStore ps = GLState::defaultPixelStore;
			ps.alignment = 1;
			PixelStore* pixel_store[] = { &ps, &ps, &ps, &ps, &ps, &ps };
			for(int i = 0; i < data.mipmapCount; ++i)
			{
				int row_length = data.images[i].bytesPerScanline / (data.bits / 8);
				ps.rowLength = row_length;
				void* pixels[] =
				{
					data.cubePosX[i].pixels,
					data.cubeNegX[i].pixels,
					data.cubePosY[i].pixels,
					data.cubeNegY[i].pixels,
					data.cubePosZ[i].pixels,
					data.cubeNegZ[i].pixels
				};
				bool result = texture->TexImage(i, internal_format, data.images[i].width, format, TYPE_UNSIGNED_BYTE, pixel_store, pixels);
				if(!result)
				{
					engineAPI.renderSystem->GetRenderer()->DestroyTexture(texture);
					return _null;
				}
			}
		}

		return texture;
	}


	// ============ ShaderRes =================


	template <GL::ObjectType _ShaderType>
	ShaderRes<_ShaderType>::ShaderRes(const tchar* file_name, const char* macros):
		Resource<GL::GLSLShader, _ShaderType>(file_name)
	{
		_macros = StringDup(macros);
	}

	template <GL::ObjectType _ShaderType>
	ShaderRes<_ShaderType>::~ShaderRes()
	{
		Memory::Delete(_macros);
	}

	template <GL::ObjectType _ShaderType>
	bool ShaderRes<_ShaderType>::Load()
	{
		if(this->IsLoaded())
			return true;
		if(!this->_fileName || !*this->_fileName)
			return false;

		// load the shader from file
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(this->_fileName, _t("rb"));
		if(!file)
		{
			Console::PrintError("Failed to open shader file: %ls", this->_fileName);
			return false;
		}

		long size = file->GetSize();
		char* source = NewArray<char>(tempPool, size + 1);
		file->Read(source, size);
		source[size] = '\0';

		file->Close();

		// parse macros, make a string with macro defines
		const char* mptr = _macros;
		int mc = 0;
		if(*mptr)
		{
			mc = 1;
			while(*mptr)
			{
				if(*mptr == ' ')
					++mc;
				++mptr;
			}
		}

		char* mbuf = 0;
		const char* strings[2];
		size_t count;
		if(mc)
		{
			size_t mlen = strlen(_macros) + 1;
			mlen += mc * (strlen("#define") + 2) + strlen("#line 0\n");
			mbuf = NewArray<char>(tempPool, mlen);
			mptr = _macros;
			char* ptr = mbuf;
			while(*mptr)
			{
				strcpy(ptr, "#define ");
				ptr += strlen("#define ");
				while(*mptr && *mptr != ' ')
					*ptr++ = *mptr++;
				if(*mptr == ' ')
					++mptr;
				*ptr++ = '\n';
			}
			strcpy(ptr, "#line 0\n");
			ptr += strlen("#line 0\n");
			*ptr = '\0';
			assert(mlen > strlen(mbuf));

			strings[0] = mbuf;
			strings[1] = source;
			count = 2;
		}
		else
		{
			strings[0] = source;
			count = 1;
		}

		// create the shader
		const char* err_str;
		this->_resource = engineAPI.renderSystem->GetRenderer()->CreateShader(_ShaderType, static_cast<int>(count), strings, err_str);
		bool result = (this->_resource != 0);
		Memory::Delete(source);
		Memory::Delete(mbuf);

		if(err_str)
		{
			if(result)
				Console::Print(ESC_YELLOW "Shader compile warnings" ESC_DEFAULT ":\nIn file: %ls, macros: %s\n%s", this->_fileName, _macros, err_str);
			else
				Console::Print(ESC_RED "Shader compile errors" ESC_DEFAULT ":\nIn file: %ls, macros: %s\n%s", this->_fileName, _macros, err_str);

			Memory::Delete(err_str);
		}

		if(!result)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyShader(this->_resource);
			this->_resource = this->_null;
			return false;
		}

		return true;
	}

	// ============ VertexShaderRes =================

	template <>
	GL::GLSLShader* Resource<GL::GLSLShader, GL::OBJ_GLSL_VERTEX_SHADER>::_null{};


	VertexShaderRes::VertexShaderRes(const tchar* file_name, const char* macros)
		: ShaderRes<GL::OBJ_GLSL_VERTEX_SHADER>(file_name, macros)
	{
	}

	VertexShaderRes::~VertexShaderRes()
	{
		Unload();
	}

	bool VertexShaderRes::Load()
	{
		return ShaderRes<GL::OBJ_GLSL_VERTEX_SHADER>::Load();
	}

	bool VertexShaderRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void VertexShaderRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyShader(_resource);
			_resource = _null;
		}
	}

	GL::GLSLShader* VertexShaderRes::CreateDefault()
	{
		const char* source = 
				"#version 130\n"
				"void main(){\n"
				"gl_Position = vec4(0.0, 0.0, 0.0, 1.0);}\n";

		// create the shader
		const char* err_str;
		GL::GLSLShader* shader = engineAPI.renderSystem->GetRenderer()->CreateShader(GL::OBJ_GLSL_VERTEX_SHADER, 1, (const char**)&source, err_str);
		if(err_str)
			Memory::Delete(err_str);
		return shader;
	}

	bool VertexShaderRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void VertexShaderRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyShader(_null);
			_null = 0;
		}
	}

	// ============ FragmentShaderRes =================

	template <>
	GL::GLSLShader* Resource<GL::GLSLShader, GL::OBJ_GLSL_FRAGMENT_SHADER>::_null{};


	FragmentShaderRes::FragmentShaderRes(const tchar* file_name, const char* macros)
		: ShaderRes<GL::OBJ_GLSL_FRAGMENT_SHADER>(file_name, macros)
	{
	}

	FragmentShaderRes::~FragmentShaderRes()
	{
		Unload();
	}

	bool FragmentShaderRes::Load()
	{
		return ShaderRes<GL::OBJ_GLSL_FRAGMENT_SHADER>::Load();
	}

	bool FragmentShaderRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void FragmentShaderRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyShader(_resource);
			_resource = _null;
		}
	}

	GL::GLSLShader* FragmentShaderRes::CreateDefault()
	{
		const char* source = 
				"#version 130\n"
				"out vec4 fragColor;\n"
				"void main(){\n"
				"fragColor = vec4(1.0, 0.0, 1.0, 1.0);}\n";

		// create the shader
		const char* err_str;
		GL::GLSLShader* shader = engineAPI.renderSystem->GetRenderer()->CreateShader(GL::OBJ_GLSL_FRAGMENT_SHADER, 1, (const char**)&source, err_str);
		if(err_str)
			Memory::Delete(err_str);
		return shader;
	}

	bool FragmentShaderRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void FragmentShaderRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyShader(_null);
			_null = 0;
		}
	}

	// ============ GeometryShaderRes =================

	template <>
	GL::GLSLShader* Resource<GL::GLSLShader, GL::OBJ_GLSL_GEOMETRY_SHADER>::_null{};


	GeometryShaderRes::GeometryShaderRes(const tchar* file_name, const char* macros)
		: ShaderRes<GL::OBJ_GLSL_GEOMETRY_SHADER>(file_name, macros)
	{
	}

	GeometryShaderRes::~GeometryShaderRes()
	{
		Unload();
	}

	bool GeometryShaderRes::Load()
	{
		return ShaderRes<GL::OBJ_GLSL_GEOMETRY_SHADER>::Load();
	}

	bool GeometryShaderRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void GeometryShaderRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyShader(_resource);
			_resource = _null;
		}
	}

	GL::GLSLShader* GeometryShaderRes::CreateDefault()
	{
		return 0;
	}

	bool GeometryShaderRes::CreateNull()
	{
		_null = CreateDefault();
		return true;
	}

	void GeometryShaderRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyShader(_null);
			_null = 0;
		}
	}

	// ============ ASMProgRes =================


	template <GL::ObjectType _ShaderType>
	ASMProgRes<_ShaderType>::ASMProgRes(const tchar* file_name):
		Resource<GL::ASMProgram, _ShaderType>(file_name)
	{
	}

	template <GL::ObjectType _ShaderType>
	ASMProgRes<_ShaderType>::~ASMProgRes()
	{
	}

	template <GL::ObjectType _ShaderType>
	bool ASMProgRes<_ShaderType>::Load()
	{
		if(this->IsLoaded())
			return true;
		if(!this->_fileName || !*this->_fileName)
			return false;

		// load the shader from file
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(this->_fileName, _t("rb"));
		if(!file)
		{
			Console::PrintError("Failed to open program file: %ls", this->_fileName);
			return false;
		}

		long size = file->GetSize();
		char* source = NewArray<char>(tempPool, size + 1);
		file->Read(source, size);
		source[size] = '\0';

		file->Close();

		const char* err_str = 0;
		this->_resource = engineAPI.renderSystem->GetRenderer()->CreateASMProgram(_ShaderType, source, err_str);
		bool result = (this->_resource != 0);
		Memory::Delete(source);

		if(err_str)
		{
			if(result)
				Console::Print(ESC_YELLOW "ASM program compile warnings" ESC_DEFAULT ":\nIn file: %ls\n%s", this->_fileName, err_str);
			else
				Console::Print(ESC_RED "ASM program compile errors" ESC_DEFAULT ":\nIn file: %ls\n%s", this->_fileName, err_str);

			Memory::Delete(err_str);
		}

		if(!result)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyASMProgram(this->_resource);
			this->_resource = this->_null;
			return false;
		}

		return true;
	}

	// ============ VertexASMProgRes =================

	template <>
	GL::ASMProgram* Resource<GL::ASMProgram, GL::OBJ_ASM_VERTEX_PROGRAM>::_null{};


	VertexASMProgRes::VertexASMProgRes(const tchar* file_name):
		ASMProgRes<GL::OBJ_ASM_VERTEX_PROGRAM>(file_name)
	{
	}

	VertexASMProgRes::~VertexASMProgRes()
	{
		Unload();
	}

	bool VertexASMProgRes::Load()
	{
		return ASMProgRes<GL::OBJ_ASM_VERTEX_PROGRAM>::Load();
	}

	bool VertexASMProgRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void VertexASMProgRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyASMProgram(_resource);
			_resource = _null;
		}
	}

	GL::ASMProgram* VertexASMProgRes::CreateDefault()
	{
		const char* source = 
				"!!ARBvp1.0\n"
				"ATTRIB vertPos = vertex.attrib[0];\n"
				"MOV	result.position, vertPos;\n"
				"END\n";

		// create the program
		const char* err_str;
		GL::ASMProgram* prog = engineAPI.renderSystem->GetRenderer()->CreateASMProgram(GL::OBJ_ASM_VERTEX_PROGRAM, source, err_str);
		if(err_str)
			Memory::Delete(err_str);
		return prog;
	}

	bool VertexASMProgRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void VertexASMProgRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyASMProgram(_null);
			_null = 0;
		}
	}

	// ============ FragmentASMProgRes =================

	template <>
	GL::ASMProgram* Resource<GL::ASMProgram, GL::OBJ_ASM_FRAGMENT_PROGRAM>::_null{};


	FragmentASMProgRes::FragmentASMProgRes(const tchar* file_name):
		ASMProgRes<GL::OBJ_ASM_FRAGMENT_PROGRAM>(file_name)
	{
	}

	FragmentASMProgRes::~FragmentASMProgRes()
	{
		Unload();
	}

	bool FragmentASMProgRes::Load()
	{
		return ASMProgRes<GL::OBJ_ASM_FRAGMENT_PROGRAM>::Load();
	}

	bool FragmentASMProgRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void FragmentASMProgRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyASMProgram(_resource);
			_resource = _null;
		}
	}

	GL::ASMProgram* FragmentASMProgRes::CreateDefault()
	{
		const char* source = 
				"!!ARBfp1.0\n"
				"PARAM	color = { 1.0, 0.0, 1.0, 1.0 };\n"
				"MOV	result.color, color;\n"
				"END\n";

		// create the program
		const char* err_str;
		GL::ASMProgram* prog = engineAPI.renderSystem->GetRenderer()->CreateASMProgram(GL::OBJ_ASM_FRAGMENT_PROGRAM, source, err_str);
		if(err_str)
			Memory::Delete(err_str);
		return prog;
	}

	bool FragmentASMProgRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void FragmentASMProgRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyASMProgram(_null);
			_null = 0;
		}
	}

	// ============ GeometryASMProgRes =================

	template <>
	GL::ASMProgram* Resource<GL::ASMProgram, GL::OBJ_ASM_GEOMETRY_PROGRAM>::_null{};


	GeometryASMProgRes::GeometryASMProgRes(const tchar* file_name):
		ASMProgRes<GL::OBJ_ASM_GEOMETRY_PROGRAM>(file_name)
	{
	}

	GeometryASMProgRes::~GeometryASMProgRes()
	{
		Unload();
	}

	bool GeometryASMProgRes::Load()
	{
		return ASMProgRes<GL::OBJ_ASM_GEOMETRY_PROGRAM>::Load();
	}

	bool GeometryASMProgRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void GeometryASMProgRes::Unload()
	{
		if(IsLoaded())
		{
			engineAPI.renderSystem->GetRenderer()->DestroyASMProgram(_resource);
			_resource = _null;
		}
	}

	GL::ASMProgram* GeometryASMProgRes::CreateDefault()
	{
		return 0;
	}

	bool GeometryASMProgRes::CreateNull()
	{
		_null = CreateDefault();
		return true;
	}

	void GeometryASMProgRes::DestroyNull()
	{
		if(_null)
		{
			engineAPI.renderSystem->GetRenderer()->DestroyASMProgram(_null);
			_null = 0;
		}
	}

	// ============ ModelRes =================

	template <>
	Model* Resource<Model, 0>::_null{};


	ModelRes::ModelRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	ModelRes::~ModelRes()
	{
		Unload();
	}

	bool ModelRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading model: %ls", _fileName);
		_resource = New<Model>(mapPool);
		bool result = _resource->Load(_fileName);
		if(!result)
		{
			Delete(_resource);
			_resource = _null;
			Console::PrintError("Failed to load model: %ls", _fileName);
		}
		return result;
	}

	bool ModelRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void ModelRes::Unload()
	{
		if(IsLoaded())
		{
			_resource->Unload();
			Delete(_resource);
			_resource = _null;
		}
	}

	Model* ModelRes::CreateDefault()
	{
		// even if it fails to load null model from file, return an empty object
		Model* model = New<Model>(mapPool);
		model->Load(_t("Models/null.model"));
		return model;
	}

	bool ModelRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void ModelRes::DestroyNull()
	{
		if(_null)
		{
			_null->Unload();
			Delete(_null);
			_null = 0;
		}
	}

	// ============ MaterialRes =================

	template <>
	Material* Resource<Material, 0>::_null{};

	bool MaterialRes::_loadDefaultOnFail = false;


	MaterialRes::MaterialRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	MaterialRes::~MaterialRes()
	{
		Unload();
	}

	bool MaterialRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return _loadDefaultOnFail? LoadDefault(): false;

		Console::PrintLn("Loading material: %ls", _fileName);
		_resource = New<Material>(mapPool);
		bool result = _resource->Load(_fileName);
		if(!result)
		{
			Console::PrintError("Failed to load material: %ls", _fileName);
			Delete(_resource);
			if(_loadDefaultOnFail)
				result = LoadDefault();
			else
				_resource = _null;
		}
		return result;
	}

	bool MaterialRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void MaterialRes::Unload()
	{
		if(IsLoaded())
		{
			_resource->Unload();
			Delete(_resource);
			_resource = _null;
		}
	}

	Material* MaterialRes::CreateDefault()
	{
		return New<Material>(mapPool);
	}

	bool MaterialRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void MaterialRes::DestroyNull()
	{
		if(_null)
		{
			Delete(_null);
			_null = 0;
		}
	}

	// ============ ModelEntityRes =================

	template <>
	ModelEntity* Resource<ModelEntity, 0>::_null{};


	ModelEntityRes::ModelEntityRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	ModelEntityRes::ModelEntityRes(const ModelEntityRes& res):
		Resource(res._fileName)
	{
		if(res.IsLoaded())
			_resource = res._resource->CreateCopy();
	}

	ModelEntityRes::~ModelEntityRes()
	{
		Unload();
	}

	bool ModelEntityRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading entity: %ls", _fileName);
		_resource = ModelEntity::CreateFromFile(_fileName);
		if(!_resource)
		{
			_resource = _null;
			Console::PrintError("Failed to load entity: %ls", _fileName);
		}

		return IsLoaded();
	}

	bool ModelEntityRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void ModelEntityRes::Unload()
	{
		if(IsLoaded())
		{
			_resource->Unload();
			Delete(_resource);
			_resource = _null;
		}
	}

	ModelEntity* ModelEntityRes::CreateDefault()
	{
		// even if it fails to load null entity from file, return an empty object
		ModelEntity* entity = ModelEntity::CreateFromFile(_t("Entities/null.entity"));
		return entity? entity: New<StaticEntity>(mapPool);
	}

	bool ModelEntityRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void ModelEntityRes::DestroyNull()
	{
		if(_null)
		{
			_null->Unload();
			Delete(_null);
			_null = 0;
		}
	}

	/*
		Checks if given entity is referenced by this entity. If it's the same entity, returns true.
	*/
	bool ModelEntityRes::IsEntityReferenced(const tchar* file_name)
	{
		if(!_fileName || !_resource)
			return false;

		if(!tstrcmp(_fileName, file_name))
			return true;

		const ModelEntity::JointAttachMap& att = _resource->GetJointAttachments();
		for(ModelEntity::JointAttachMap::ConstIterator it = att.Begin(); it != att.End(); ++it)
		{
			if(it->type == ModelEntity::JOINT_ATTACH_MODEL)
			{
				if(!tstrcmp(it->attachment->GetFileName(), file_name))
					return true;
				((ModelEntityRes*)it->attachment)->IsEntityReferenced(file_name);
			}
		}

		return false;
	}

	// ============ AnimationRes =================

	template <>
	Animation* Resource<Animation, 0>::_null{};


	AnimationRes::AnimationRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	AnimationRes::~AnimationRes()
	{
		Unload();
	}

	bool AnimationRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading animation: %ls", _fileName);
		_resource = New<Animation>(mapPool);
		bool result = _resource->Load(_fileName);
		if(!result)
		{
			Delete(_resource);
			_resource = _null;
			Console::PrintError("Failed to load animation: %ls", _fileName);
		}
		return result;
	}

	bool AnimationRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void AnimationRes::Unload()
	{
		if(IsLoaded())
		{
			_resource->Unload();
			Delete(_resource);
			_resource = _null;
		}
	}

	Animation* AnimationRes::CreateDefault()
	{
		return New<Animation>(mapPool);
	}

	bool AnimationRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void AnimationRes::DestroyNull()
	{
		if(_null)
		{
			Delete(_null);
			_null = 0;
		}
	}

	// ============ SoundRes =================

	/*template <>
	Sound* Resource<Sound, 0>::_null{};*/


	SoundRes::SoundRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	SoundRes::~SoundRes()
	{
		Unload();
	}

	bool SoundRes::Load()
	{
		return true;
	}

	bool SoundRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void SoundRes::Unload()
	{
	}

	int* SoundRes::CreateDefault()
	{
		return 0;
	}

	bool SoundRes::CreateNull()
	{
		return true;
	}

	void SoundRes::DestroyNull()
	{
	}


	// ============ AIScriptRes =================

	template <>
	int* Resource<int, 0>::_null{};


	AIScriptRes::AIScriptRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	AIScriptRes::~AIScriptRes()
	{
		Unload();
	}

	bool AIScriptRes::Load()
	{
		return true;
	}

	bool AIScriptRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void AIScriptRes::Unload()
	{
	}

	int* AIScriptRes::CreateDefault()
	{
		return 0;
	}

	bool AIScriptRes::CreateNull()
	{
		return true;
	}

	void AIScriptRes::DestroyNull()
	{
	}

	// ============ PartSysRes =================

	template <>
	ParticleSystem* Resource<ParticleSystem, 0>::_null{};


	PartSysRes::PartSysRes(const tchar* file_name):
		Resource(file_name)
	{
	}

	PartSysRes::PartSysRes(const PartSysRes& res):
		Resource(res._fileName)
	{
		if(res.IsLoaded())
			_resource = New<ParticleSystem>(mapPool, *res._resource);
	}

	PartSysRes::~PartSysRes()
	{
		Unload();
	}

	bool PartSysRes::Load()
	{
		if(IsLoaded())
			return true;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading particle system: %ls", _fileName);
		_resource = New<ParticleSystem>(mapPool);
		bool result = _resource->Load(_fileName);
		if(!result)
		{
			Delete(_resource);
			_resource = _null;
			Console::PrintError("Failed to load particle system: %ls", _fileName);
		}
		return result;
	}

	bool PartSysRes::LoadDefault()
	{
		_resource = CreateDefault();
		if(!_resource)
			_resource = _null;
		return IsLoaded();
	}

	void PartSysRes::Unload()
	{
		if(IsLoaded())
		{
			_resource->Unload();
			Delete(_resource);
			_resource = _null;
		}
	}

	ParticleSystem* PartSysRes::CreateDefault()
	{
		return New<ParticleSystem>(mapPool);
	}

	bool PartSysRes::CreateNull()
	{
		if(_null)
			return false;

		_null = CreateDefault();
		return (_null != 0);
	}

	void PartSysRes::DestroyNull()
	{
		if(_null)
		{
			Delete(_null);
			_null = 0;
		}
	}

}
