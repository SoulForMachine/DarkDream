
#ifndef _FILE_RESOURCE_H_
#define _FILE_RESOURCE_H_

#include "BaseLib/String.h"
#include "BaseLib/Image.h"
#include "Common.h"


namespace GL
{

	class Renderer;
	class Texture;
	class Texture2D;
	class Texture3D;
	class TextureCube;
	class GLSLShader;
	class ASMProgram;
	enum ObjectType;
	enum PixelFormat;
	enum ImageFormat;

}


namespace Engine
{

	class ModelEntity;
	class Model;
	class Material;
	class Animation;
	class ParticleSystem;


	class ENGINE_API FileResource
	{
	public:
		FileResource(const tchar* file_name);
		virtual ~FileResource() = 0;

		virtual bool Load() = 0;
		virtual bool LoadDefault() = 0;
		virtual void Unload() = 0;
		virtual bool IsLoaded() const = 0;
		const tchar* GetFileName() const
			{ return _fileName; }
		void IncRefCount()
			{ ++_refCount; }
		void DecRefCount()
			{ if(_refCount) --_refCount; }
		bool IsReferenced() const // returns true if anyone's holding this resource
			{ return (_refCount > 0); }

	protected:
		tchar* _fileName;
		size_t _refCount;

	private:
		FileResource( const FileResource&);
		FileResource& operator = ( const FileResource&);
	};


	class ENGINE_API TextureRes: public FileResource
	{
	public:
		TextureRes(const tchar* file_name, GL::Renderer* renderer);
		~TextureRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return (_texture != 0); }
		const GL::Texture* GetTexture() const
			{ return _texture; }

	private:
		GL::Texture2D* LoadTexture2D(const Image& image);
		GL::Texture3D* LoadTexture3D(const Image& image);
		GL::TextureCube* LoadTextureCube(const Image& image);
		GL::PixelFormat GetGLInternalFormat(ImagePixelFormat image_format);
		GL::ImageFormat GetGLFormat(ImagePixelFormat image_format);
		GL::PixelFormat GetGLCompressedFormat(ImagePixelFormat image_format);

		GL::Renderer* _renderer;
		GL::Texture* _texture;
	};


	class ENGINE_API ShaderRes: public FileResource
	{
	public:
		ShaderRes(const tchar* file_name, const char* macros, GL::Renderer* renderer);
		~ShaderRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return (_shader != 0); }
		const GL::GLSLShader* GetShader() const
			{ return _shader; }
		const char* GetMacros() const
			{ return _macros; }

	private:
		GL::ObjectType GetShaderType(const tchar* file_name);

		GL::Renderer* _renderer;
		GL::GLSLShader* _shader;
		const char* _macros;
	};


	class ENGINE_API ASMProgRes: public FileResource
	{
	public:
		ASMProgRes(const tchar* file_name, GL::Renderer* renderer);
		~ASMProgRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return (_program != 0); }
		const GL::ASMProgram* GetASMProgram() const
			{ return _program; }

	private:
		GL::ObjectType GetProgramType(const tchar* file_name);

		GL::Renderer* _renderer;
		GL::ASMProgram* _program;
	};


	class ENGINE_API ModelRes: public FileResource
	{
	public:
		ModelRes(const tchar* file_name);
		~ModelRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return (_model != 0); }
		const Model* GetModel() const
			{ return _model; }

	private:
		Model* _model;
	};


	class ENGINE_API MaterialRes: public FileResource
	{
	public:
		MaterialRes(const tchar* file_name);
		~MaterialRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return (_material != 0); }
		const Material* GetMaterial() const
			{ return _material; }

	private:
		Material* _material;
	};


	class ENGINE_API AIScriptRes: public FileResource
	{
	public:
		AIScriptRes(const tchar* file_name);
		~AIScriptRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return false; }
	private:
	};


	class ENGINE_API ModelEntityRes: public FileResource
	{
	public:
		ModelEntityRes(const tchar* file_name);
		ModelEntityRes(const ModelEntityRes& res);
		~ModelEntityRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return (_entity != 0); }
		ModelEntity* GetEntity()
			{ return _entity; }
		bool IsEntityReferenced(const tchar* file_name);

	private:
		ModelEntity* _entity;
	};


	class ENGINE_API AnimationRes: public FileResource
	{
	public:
		AnimationRes(const tchar* file_name);
		~AnimationRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return (_animation != 0); }
		const Animation* GetAnimation() const
			{ return _animation; }

	private:
		Animation* _animation;
	};


	class ENGINE_API SoundRes: public FileResource
	{
	public:
		SoundRes(const tchar* file_name);
		~SoundRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return false; }
	};

	class ENGINE_API PartSysRes: public FileResource
	{
	public:
		PartSysRes(const tchar* file_name);
		PartSysRes(const PartSysRes& res);
		~PartSysRes();

		bool Load();
		bool LoadDefault();
		void Unload();
		bool IsLoaded() const
			{ return false; }
		ParticleSystem* GetParticleSystem()
			{ return _particleSys; }

	private:
		ParticleSystem* _particleSys;
	};

}


#endif // _FILE_RESOURCE_H_
