

#include "BaseLib/SmartPtr.h"
#include "BaseLib/Console.h"
#include "BaseLib/GL/GLRenderer.h"
#include "Engine/FileSystem.h"
#include "Engine/EngineInternal.h"
#include "FileResource.h"
#include "BaseLib/Memory.h"
#include "Renderer/Model.h"
#include "Renderer/Material.h"
#include "Renderer/Animation.h"
#include "Renderer/ParticleSystem.h"
#include "Renderer/RenderSystem.h"
#include "ModelEntity.h"


using namespace Memory;
using namespace GL;


namespace Engine
{

	FileResource::FileResource(const tchar* file_name)
	{
		_fileName = StringDup(file_name);
		_refCount = 1;
	}

	FileResource::~FileResource()
	{
		delete[] _fileName;
	}

	// ============ TextureRes =================


	TextureRes::TextureRes(const tchar* file_name, GL::Renderer* renderer):
		FileResource(file_name)
	{
		_texture = 0;
		_renderer = renderer;
	}

	TextureRes::~TextureRes()
	{
		_renderer->DestroyTexture(_texture);
	}

	bool TextureRes::Load()
	{
		if(!_renderer)
			return false;
		if(_texture)
			return false;
		if(!_fileName || !*_fileName)
			return false;

		Console::PrintLn("Loading texture: %ls", _fileName);

		// load the texture from file
		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(_fileName, _t("rb"));
		if(!file)
		{
			Console::PrintError("Failed to open texture file: %ls", _fileName);
			return false;
		}

		Image img;
		if(img.Load(*file))
		{
			switch(img.GetImageData().type)
			{
			case IMG_2D:
				_texture = LoadTexture2D(img);
				break;
			case IMG_3D:
				_texture = LoadTexture3D(img);
				break;
			case IMG_CUBEMAP:
				_texture = LoadTextureCube(img);
				break;
			}

			if(_texture)
			{
				if(img.GetImageData().mipmapCount == 1)
					_texture->GenerateMipmap();
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
			Console::PrintError("Failed to load texture file: %ls", _fileName);
			return false;
		}
	}

	bool TextureRes::LoadDefault()
	{
		if(_texture)
			return false;

		Texture2D* texture = _renderer->CreateTexture2D();
		if(!texture)
			return false;

		const int width = 32;
		const int height = 32;
		ubyte* pixels = new(tempPool) ubyte[width * height * 3];
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
		delete[] pixels;
		if(result)
		{
			_texture = texture;
			_texture->GenerateMipmap();
		}
		return result;
	}

	void TextureRes::Unload()
	{
		if(_texture)
		{
			_renderer->DestroyTexture(_texture);
			_texture = 0;
		}
	}

	Texture2D* TextureRes::LoadTexture2D(const Image& image)
	{
		const ImageData& data = image.GetImageData();
		PixelFormat internal_format;
		ImageFormat format;
		Texture2D* texture = _renderer->CreateTexture2D();
		if(!texture)
			return 0;

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
					_renderer->DestroyTexture(texture);
					return 0;
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
					_renderer->DestroyTexture(texture);
					return 0;
				}
			}
		}

		return texture;
	}

	Texture3D* TextureRes::LoadTexture3D(const Image& image)
	{
		const ImageData& data = image.GetImageData();
		PixelFormat internal_format;
		ImageFormat format;
		Texture3D* texture = _renderer->CreateTexture3D();
		if(!texture)
			return 0;

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
					_renderer->DestroyTexture(texture);
					return 0;
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
					_renderer->DestroyTexture(texture);
					return 0;
				}
			}
		}

		return texture;
	}

	TextureCube* TextureRes::LoadTextureCube(const Image& image)
	{
		const ImageData& data = image.GetImageData();
		PixelFormat internal_format;
		ImageFormat format;
		TextureCube* texture = _renderer->CreateTextureCube();
		if(!texture)
			return 0;

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
					_renderer->DestroyTexture(texture);
					return 0;
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
					_renderer->DestroyTexture(texture);
					return 0;
				}
			}
		}

		return texture;
	}

	GL::PixelFormat TextureRes::GetGLInternalFormat(ImagePixelFormat image_format)
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

	GL::ImageFormat TextureRes::GetGLFormat(ImagePixelFormat image_format)
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

	GL::PixelFormat TextureRes::GetGLCompressedFormat(ImagePixelFormat image_format)
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

	// ============ ShaderRes =================

	ShaderRes::ShaderRes(const tchar* file_name, const char* macros, GL::Renderer* renderer):
		FileResource(file_name)
	{
		_shader = 0;
		_macros = StringDup(macros);
		_renderer = renderer;
	}

	ShaderRes::~ShaderRes()
	{
		_renderer->DestroyShader(_shader);
		delete[] _macros;
	}

	bool ShaderRes::Load()
	{
		if(!_renderer)
			return false;
		if(_shader)
			return false;
		if(!_fileName || !*_fileName)
			return false;

		ObjectType shader_type = GetShaderType(_fileName);

		// load the shader from file
		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(_fileName, _t("rb"));
		if(!file)
		{
			Console::PrintError("Failed to open shader file: %ls", _fileName);
			return false;
		}

		long size = file->GetSize();
		char* source = new(tempPool) char[size + 1];
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
			mbuf = new(tempPool) char[mlen];
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
		_shader = _renderer->CreateShader(shader_type, count, strings, err_str);
		bool result = (_shader != 0);
		delete[] source;
		delete[] mbuf;

		if(err_str)
		{
			if(result)
				Console::Print(ESC_YELLOW "Shader compile warnings" ESC_DEFAULT ":\nIn file: %ls, macros: %s\n%s", _fileName, _macros, err_str);
			else
				Console::Print(ESC_RED "Shader compile errors" ESC_DEFAULT ":\nIn file: %ls, macros: %s\n%s", _fileName, _macros, err_str);

			delete[] err_str;
		}

		if(!result)
		{
			_renderer->DestroyShader(_shader);
			_shader = 0;
			return false;
		}

		return true;
	}

	bool ShaderRes::LoadDefault()
	{
		ObjectType shader_type = GetShaderType(_fileName);
		const char* shader;

		if(shader_type == OBJ_GLSL_VERTEX_SHADER)
		{
			shader = 
				"#version 130\n"
				"in vec4 vertPos;\n"
				"void main(){\n"
				"gl_Position = vertPos;}\n";
		}
		else if(shader_type == OBJ_GLSL_GEOMETRY_SHADER)
		{
			return false;
		}
		else if(shader_type == OBJ_GLSL_FRAGMENT_SHADER)
		{
			shader = 
				"#version 130\n"
				"out vec4 fragColor;\n"
				"void main(){\n"
				"fragColor = vec4(1.0, 0.0, 1.0, 1.0);}\n";
		}

		// create the shader
		const char* err_str;
		_shader = _renderer->CreateShader(shader_type, 1, (const char**)&shader, err_str);
		if(err_str)
			delete[] err_str;
		return (_shader != 0);
	}

	void ShaderRes::Unload()
	{
		if(_shader)
		{
			_renderer->DestroyShader(_shader);
			_shader = 0;
		}
	}

	GL::ObjectType ShaderRes::GetShaderType(const tchar* file_name)
	{
		if(file_name && *file_name)
		{
			const tchar* dot = tstrrchr(file_name, _t('.'));
			if(dot)
			{
				if(!tstrcmp(dot + 1, _t("vs")))
					return OBJ_GLSL_VERTEX_SHADER;
				else if(!tstrcmp(dot + 1, _t("gs")))
					return OBJ_GLSL_GEOMETRY_SHADER;
				else if(!tstrcmp(dot + 1, _t("fs")))
					return OBJ_GLSL_FRAGMENT_SHADER;
			}
		}

		return OBJ_GLSL_VERTEX_SHADER; // assume it's a vertex shader
	}


	// ============ ModelRes =================

	ModelRes::ModelRes(const tchar* file_name):
		FileResource(file_name)
	{
		_model = 0;
	}

	ModelRes::~ModelRes()
	{
		if(_model)
		{
			_model->Unload();
			delete _model;
		}
	}

	bool ModelRes::Load()
	{
		if(!_model && _fileName && *_fileName)
		{
			Console::PrintLn("Loading model: %ls", _fileName);
			_model = new(mapPool) Model;
			bool result = _model->Load(_fileName);
			if(!result)
			{
				delete _model;
				_model = 0;
				Console::PrintError("Failed to load model: %ls", _fileName);
			}
			return result;
		}
		else
		{
			return false;
		}
	}

	bool ModelRes::LoadDefault()
	{
		return false;
	}

	void ModelRes::Unload()
	{
		if(_model)
		{
			_model->Unload();
			delete _model;
			_model = 0;
		}
	}


	// ============ MaterialRes =================

	MaterialRes::MaterialRes(const tchar* file_name):
		FileResource(file_name)
	{
		_material = 0;
	}

	MaterialRes::~MaterialRes()
	{
		if(_material)
		{
			_material->Unload();
			delete _material;
		}
	}

	bool MaterialRes::Load()
	{
		if(!_material && _fileName && *_fileName)
		{
			Console::PrintLn("Loading material: %ls", _fileName);
			_material = new(mapPool) Material;
			bool result = _material->Load(_fileName);
			if(!result)
			{
				delete _material;
				_material = 0;
				Console::PrintError("Failed to load material: %ls", _fileName);
			}
			return result;
		}
		else
		{
			return false;
		}
	}

	bool MaterialRes::LoadDefault()
	{
		if(_material)
			return false;

		_material = new(mainPool) Material;
		return (_material != 0);
	}

	void MaterialRes::Unload()
	{
		if(_material)
		{
			_material->Unload();
			delete _material;
			_material = 0;
		}
	}

	// ============ ModelEntityRes =================

	ModelEntityRes::ModelEntityRes(const tchar* file_name):
		FileResource(file_name)
	{
		_entity = 0;
	}

	ModelEntityRes::ModelEntityRes(const ModelEntityRes& res):
		FileResource(res._fileName)
	{
		if(res._entity)
			_entity = new(mapPool) ModelEntity(*res._entity);
		else
			_entity = 0;
	}

	ModelEntityRes::~ModelEntityRes()
	{
		if(_entity)
		{
			_entity->Unload();
			delete _entity;
		}
	}

	bool ModelEntityRes::Load()
	{
		if(!_entity && _fileName && *_fileName)
		{
			Console::PrintLn("Loading entity: %ls", _fileName);
			_entity = new(mapPool) ModelEntity;
			bool result = _entity->Load(_fileName);
			if(!result)
			{
				delete _entity;
				_entity = 0;
				Console::PrintError("Failed to load entity: %ls", _fileName);
			}
			return result;
		}
		else
		{
			return false;
		}
	}

	bool ModelEntityRes::LoadDefault()
	{
		return false;
	}

	void ModelEntityRes::Unload()
	{
		if(_entity)
		{
			_entity->Unload();
			delete _entity;
			_entity = 0;
		}
	}

	/*
		Checks if given entity is referenced by this entity. If it's the same entity, returns true.
	*/
	bool ModelEntityRes::IsEntityReferenced(const tchar* file_name)
	{
		if(!_fileName || !_entity)
			return false;

		if(!tstrcmp(_fileName, file_name))
			return true;

		const ModelEntity::JointAttachMap& att = _entity->GetJointAttachments();
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

	AnimationRes::AnimationRes(const tchar* file_name):
		FileResource(file_name)
	{
		_animation = 0;
	}

	AnimationRes::~AnimationRes()
	{
		if(_animation)
		{
			_animation->Unload();
			delete _animation;
		}
	}

	bool AnimationRes::Load()
	{
		if(!_animation && _fileName && *_fileName)
		{
			Console::PrintLn("Loading animation: %ls", _fileName);
			_animation = new(mapPool) Animation;
			bool result = _animation->Load(_fileName);
			if(!result)
			{
				delete _animation;
				_animation = 0;
				Console::PrintError("Failed to load animation: %ls", _fileName);
			}
			return result;
		}
		else
		{
			return false;
		}
	}

	bool AnimationRes::LoadDefault()
	{
		return false;
	}

	void AnimationRes::Unload()
	{
		if(_animation)
		{
			_animation->Unload();
			delete _animation;
			_animation = 0;
		}
	}

	// ============ SoundRes =================

	SoundRes::SoundRes(const tchar* file_name):
		FileResource(file_name)
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
		return false;
	}

	void SoundRes::Unload()
	{
	}


	// ============ AIScriptRes =================

	AIScriptRes::AIScriptRes(const tchar* file_name):
		FileResource(file_name)
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
		return false;
	}

	void AIScriptRes::Unload()
	{
	}

	// ============ PartSysRes =================

	PartSysRes::PartSysRes(const tchar* file_name):
		FileResource(file_name)
	{
		_particleSys = 0;
	}

	PartSysRes::PartSysRes(const PartSysRes& res):
		FileResource(res._fileName)
	{
		if(res._particleSys)
			_particleSys = new(mapPool) ParticleSystem(*res._particleSys);
		else
			_particleSys = 0;
	}

	PartSysRes::~PartSysRes()
	{
		Unload();
	}

	bool PartSysRes::Load()
	{
		return true;
	}

	bool PartSysRes::LoadDefault()
	{
		return false;
	}

	void PartSysRes::Unload()
	{
	}

}
