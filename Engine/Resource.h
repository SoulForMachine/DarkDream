
#ifndef _ENGINE_RESOURCE_H_
#define _ENGINE_RESOURCE_H_

#include <cassert>
#include "BaseLib/GL/GLRenderer.h"
#include "Common.h"


class Image;


namespace Engine
{

	class ModelEntity;
	class Model;
	class Material;
	class Animation;
	class ParticleSystem;

	class ResourceManager;
	class ShaderManager;
	class MaterialManager;
	class ASMProgManager;
	class TextureManager;
	class ModelManager;
	class AnimationManager;
	class ModelEntityManager;
	class SoundManager;
	class AIScriptManager;
	class PartSysManager;


	//=================================================================================

	template <class _Res>
	class ResourcePtr
	{
	public:
		ResourcePtr() { }
		explicit ResourcePtr(const _Res* res)
			{ _resource = res; }

		operator const typename _Res::ResType* () const
			{ assert(_resource); return _resource->GetResource(); }
		const typename _Res::ResType* operator -> () const
			{ assert(_resource); return _resource->GetResource(); }
		const _Res* GetRes() const
			{ return _resource; }
		bool IsValid() const
			{ return (_resource && _resource->GetResource()); }

		operator bool () const
			{ return (_resource != 0); }

		static const ResourcePtr null;

		friend bool operator == (const ResourcePtr& lhs, const ResourcePtr& rhs)
			{ return lhs._resource == rhs._resource; }
		friend bool operator != (const ResourcePtr& lhs, const ResourcePtr& rhs)
			{ return lhs._resource != rhs._resource; }

	private:
		const _Res* _resource;
	};


	template <class _Res>
	const ResourcePtr<_Res> ResourcePtr<_Res>::null(0);


	//=================================================================================

	class ENGINE_API ResourceBase
	{
	public:
		virtual ~ResourceBase() = 0 { }

		virtual bool IsLoaded() const = 0;
		virtual const tchar* GetFileName() const = 0;

	protected:
		virtual bool Load() = 0;
		virtual bool LoadDefault() = 0;
		virtual void Unload() = 0;
		virtual void IncRefCount() = 0;
		virtual void DecRefCount() = 0;
		virtual bool IsReferenced() const = 0;

		friend class ResourceManager;
	};

	//=================================================================================

	template <class _Res, int _ResSubType = 0>
	class ENGINE_API Resource: public ResourceBase
	{
	public:
		Resource(const tchar* file_name);
		virtual ~Resource() = 0;

		typedef _Res ResType;

		const _Res* GetResource() const
			{ return _resource; }
		bool IsLoaded() const;
		const tchar* GetFileName() const
			{ return _fileName; }

	protected:
		void IncRefCount()
			{ ++_refCount; }
		void DecRefCount()
			{ if(_refCount) --_refCount; }
		bool IsReferenced() const // returns true if anyone's holding this resource
			{ return (_refCount > 0); }

		_Res* _resource;
		static _Res* _null;
		tchar* _fileName;
		size_t _refCount;

	private:
		Resource( const Resource&);
		Resource& operator = ( const Resource&);
	};

	/*template <class _Res, int _ResSubType>
	_Res* Resource<_Res, _ResSubType>::_null;*/


	//=================================================================================

	class ENGINE_API Texture2DRes: public Resource<GL::Texture2D>
	{
	public:
		Texture2DRes(const tchar* file_name);
		~Texture2DRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		GL::Texture2D* LoadTexture2D(const Image& image);

		static GL::Texture2D* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class TextureManager;
		friend class RenderSystem;
	};

	class ENGINE_API Texture3DRes: public Resource<GL::Texture3D>
	{
	public:
		Texture3DRes(const tchar* file_name);
		~Texture3DRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		GL::Texture3D* LoadTexture3D(const Image& image);

		static GL::Texture3D* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class TextureManager;
		friend class RenderSystem;
	};

	class ENGINE_API TextureCubeRes: public Resource<GL::TextureCube>
	{
	public:
		TextureCubeRes(const tchar* file_name);
		~TextureCubeRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		GL::TextureCube* LoadTextureCube(const Image& image);

		static GL::TextureCube* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class TextureManager;
		friend class RenderSystem;
	};

	//=================================================================================

	template <GL::ObjectType _ShaderType>
	class ENGINE_API ShaderRes: public Resource<GL::GLSLShader, _ShaderType>
	{
	public:
		ShaderRes(const tchar* file_name, const char* macros);
		~ShaderRes();

		const char* GetMacros() const
			{ return _macros; }
	protected:
		bool Load();

		const char* _macros;
	};

	class ENGINE_API VertexShaderRes: public ShaderRes<GL::OBJ_GLSL_VERTEX_SHADER>
	{
	public:
		VertexShaderRes(const tchar* file_name, const char* macros);
		~VertexShaderRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static GL::GLSLShader* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ShaderManager;
		friend class RenderSystem;
	};

	class ENGINE_API FragmentShaderRes: public ShaderRes<GL::OBJ_GLSL_FRAGMENT_SHADER>
	{
	public:
		FragmentShaderRes(const tchar* file_name, const char* macros);
		~FragmentShaderRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static GL::GLSLShader* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ShaderManager;
		friend class RenderSystem;
	};

	class ENGINE_API GeometryShaderRes: public ShaderRes<GL::OBJ_GLSL_GEOMETRY_SHADER>
	{
	public:
		GeometryShaderRes(const tchar* file_name, const char* macros);
		~GeometryShaderRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static GL::GLSLShader* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ShaderManager;
		friend class RenderSystem;
	};

	//=================================================================================

	template <GL::ObjectType _ShaderType>
	class ENGINE_API ASMProgRes: public Resource<GL::ASMProgram, _ShaderType>
	{
	public:
		ASMProgRes(const tchar* file_name);
		~ASMProgRes();

	protected:
		bool Load();
	};


	class ENGINE_API VertexASMProgRes: public ASMProgRes<GL::OBJ_ASM_VERTEX_PROGRAM>
	{
	public:
		VertexASMProgRes(const tchar* file_name);
		~VertexASMProgRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static GL::ASMProgram* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ASMProgManager;
		friend class RenderSystem;
	};

	class ENGINE_API FragmentASMProgRes: public ASMProgRes<GL::OBJ_ASM_FRAGMENT_PROGRAM>
	{
	public:
		FragmentASMProgRes(const tchar* file_name);
		~FragmentASMProgRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static GL::ASMProgram* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ASMProgManager;
		friend class RenderSystem;
	};

	class ENGINE_API GeometryASMProgRes: public ASMProgRes<GL::OBJ_ASM_GEOMETRY_PROGRAM>
	{
	public:
		GeometryASMProgRes(const tchar* file_name);
		~GeometryASMProgRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static GL::ASMProgram* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ASMProgManager;
		friend class RenderSystem;
	};

	//=================================================================================

	class ENGINE_API ModelRes: public Resource<Model>
	{
	public:
		ModelRes(const tchar* file_name);
		~ModelRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static Model* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ModelManager;
		friend class RenderSystem;
	};

	//=================================================================================

	class ENGINE_API MaterialRes: public Resource<Material>
	{
	public:
		MaterialRes(const tchar* file_name);
		~MaterialRes();

		/*
			If set to true, loads a copy of a default material if requested material loading fails. Otherwise,
			it points to null material. This is intended for use by editors to prevent changing null material.
		*/
		static void LoadDefaultOnFail(bool on)
			{ _loadDefaultOnFail = on; }

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static Material* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		static bool _loadDefaultOnFail;

		friend class MaterialManager;
		friend class RenderSystem;
	};

	//=================================================================================

	class ENGINE_API AIScriptRes: public Resource<int>
	{
	public:
		AIScriptRes(const tchar* file_name);
		~AIScriptRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static int* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class AIScriptManager;
		friend class RenderSystem;
	};

	//=================================================================================

	class ENGINE_API ModelEntityRes: public Resource<ModelEntity>
	{
	public:
		ModelEntityRes(const tchar* file_name);
		ModelEntityRes(const ModelEntityRes& res);
		~ModelEntityRes();

		bool IsEntityReferenced(const tchar* file_name); //! premesti u ModelEntity

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static ModelEntity* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class ModelEntityManager;
		friend class RenderSystem;
	};

	//=================================================================================

	class ENGINE_API AnimationRes: public Resource<Animation>
	{
	public:
		AnimationRes(const tchar* file_name);
		~AnimationRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static Animation* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class AnimationManager;
		friend class RenderSystem;
	};

	//=================================================================================

	class ENGINE_API SoundRes: public Resource<int>
	{
	public:
		SoundRes(const tchar* file_name);
		~SoundRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static int* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class SoundManager;
		friend class RenderSystem;
	};

	//=================================================================================

	class ENGINE_API PartSysRes: public Resource<ParticleSystem>
	{
	public:
		PartSysRes(const tchar* file_name);
		PartSysRes(const PartSysRes& res);
		~PartSysRes();

	protected:
		bool Load();
		bool LoadDefault();
		void Unload();

	private:
		static ParticleSystem* CreateDefault();
		static bool CreateNull();
		static void DestroyNull();

		friend class PartSysManager;
		friend class RenderSystem;
	};

	//=================================================================================

	typedef ResourcePtr<Texture2DRes> Texture2DResPtr;
	typedef ResourcePtr<Texture3DRes> Texture3DResPtr;
	typedef ResourcePtr<TextureCubeRes> TextureCubeResPtr;
	typedef ResourcePtr<VertexShaderRes> VertexShaderResPtr;
	typedef ResourcePtr<FragmentShaderRes> FragmentShaderResPtr;
	typedef ResourcePtr<GeometryShaderRes> GeometryShaderResPtr;
	typedef ResourcePtr<VertexASMProgRes> VertexASMProgResPtr;
	typedef ResourcePtr<FragmentASMProgRes> FragmentASMProgResPtr;
	typedef ResourcePtr<GeometryASMProgRes> GeometryASMProgResPtr;
	typedef ResourcePtr<ModelRes> ModelResPtr;
	typedef ResourcePtr<MaterialRes> MaterialResPtr;
	typedef ResourcePtr<AIScriptRes> AIScriptResPtr;
	typedef ResourcePtr<ModelEntityRes> ModelEntityResPtr;
	typedef ResourcePtr<AnimationRes> AnimationResPtr;
	typedef ResourcePtr<SoundRes> SoundResPtr;
	typedef ResourcePtr<PartSysRes> PartSysResPtr;

}


#endif // _ENGINE_RESOURCE_H_
