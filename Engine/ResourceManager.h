
#ifndef _ENGINE_RESOURCE_MANAGER_H_
#define _ENGINE_RESOURCE_MANAGER_H_

#include "BaseLib/Memory.h"
#include "BaseLib/HashMap.h"
#include "Engine/Resource.h"


namespace Engine
{

	class ShaderManager;
	class ASMProgManager;
	class TextureManager;
	class MaterialManager;
	class ModelManager;
	class AnimationManager;
	class ModelEntityManager;
	class SoundManager;
	class AIScriptManager;
	class PartSysManager;

	// ================================================================================================

	class ENGINE_API ResourceManager
	{
	public:
		ResourceManager():
			_resources(Memory::mapPool) {}

		void LoadAll();
		void UnloadAll();
		void ReleaseAll();

	protected:
		struct ResHashMapTraits
		{
			ResHashMapTraits(Memory::Allocator& pool, size_t size)
				: _pool(pool) {}
			HashMapNode<ResourceBase*>* New()
				{ return Memory::New<HashMapNode<ResourceBase*>>(_pool); }
			void Delete(HashMapNode<ResourceBase*>* ptr)
				{ Memory::Delete(ptr); }
			uint GetHash(const tchar* str)
				{ return ::GetStringiHash(str); }

		private:
			Memory::Allocator& _pool;
		};

		typedef HashMap<const tchar*, ResourceBase*, ResHashMapTraits> ResHashMap;

		struct CreateResObjFunc
		{
			virtual ResourceBase* operator () () const = 0;
		};

		const ResourceBase* CreateRes(const tchar* key, const CreateResObjFunc& create_res_obj, bool immediate);
		bool ReleaseRes(const tchar* key, const ResourceBase* res);
		const ResourceBase* FindRes(const tchar* key);

	private:
		ResHashMap _resources;
	};

	// ================================================================================================

	class ENGINE_API TextureManager: public ResourceManager
	{
	public:
		TextureManager() {}

		Texture2DResPtr CreateTexture2D(const tchar* file_name, bool immediate = false);
		Texture3DResPtr CreateTexture3D(const tchar* file_name, bool immediate = false);
		TextureCubeResPtr CreateTextureCube(const tchar* file_name, bool immediate = false);
		bool ReleaseTexture(Texture2DResPtr texture);
		bool ReleaseTexture(Texture3DResPtr texture);
		bool ReleaseTexture(TextureCubeResPtr texture);
		Texture2DResPtr FindTexture2D(const tchar* file_name);
		Texture3DResPtr FindTexture3D(const tchar* file_name);
		TextureCubeResPtr FindTextureCube(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API ShaderManager: public ResourceManager
	{
	public:
		ShaderManager() {}

		VertexShaderResPtr CreateVertexShader(const tchar* file_name, const char* macros = "", bool immediate = false);
		FragmentShaderResPtr CreateFragmentShader(const tchar* file_name, const char* macros = "", bool immediate = false);
		GeometryShaderResPtr CreateGeometryShader(const tchar* file_name, const char* macros = "", bool immediate = false);
		bool ReleaseShader(VertexShaderResPtr shader);
		bool ReleaseShader(FragmentShaderResPtr shader);
		bool ReleaseShader(GeometryShaderResPtr shader);
		VertexShaderResPtr FindVertexShader(const tchar* file_name, const char* macros);
		FragmentShaderResPtr FindFragmentShader(const tchar* file_name, const char* macros);
		GeometryShaderResPtr FindGeometryShader(const tchar* file_name, const char* macros);

	private:
		tchar* MakeKey(const tchar* file_name, const char* macros);
	};

	// ================================================================================================

	class ENGINE_API ASMProgManager: public ResourceManager
	{
	public:
		ASMProgManager() {}

		VertexASMProgResPtr CreateVertexASMProgram(const tchar* file_name, bool immediate = false);
		FragmentASMProgResPtr CreateFragmentASMProgram(const tchar* file_name, bool immediate = false);
		GeometryASMProgResPtr CreateGeometryASMProgram(const tchar* file_name, bool immediate = false);
		bool ReleaseASMProgram(VertexASMProgResPtr program);
		bool ReleaseASMProgram(FragmentASMProgResPtr program);
		bool ReleaseASMProgram(GeometryASMProgResPtr program);
		VertexASMProgResPtr FindVertexASMProgram(const tchar* file_name);
		FragmentASMProgResPtr FindFragmentASMProgram(const tchar* file_name);
		GeometryASMProgResPtr FindGeometryASMProgram(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API MaterialManager: public ResourceManager
	{
	public:
		MaterialResPtr CreateMaterial(const tchar* file_name, bool immediate = false);
		MaterialResPtr CreateDefaultMaterial();
		bool ReleaseMaterial(MaterialResPtr material);
		MaterialResPtr FindMaterial(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API ModelManager: public ResourceManager
	{
	public:
		ModelResPtr CreateModel(const tchar* file_name, bool immediate = false);
		bool ReleaseModel(ModelResPtr Model);
		ModelResPtr FindModel(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API AnimationManager: public ResourceManager
	{
	public:
		AnimationResPtr CreateAnimation(const tchar* file_name, bool immediate = false);
		bool ReleaseAnimation(AnimationResPtr Animation);
		AnimationResPtr FindAnimation(const tchar* file_name);
	};

	// ================================================================================================
	// Since entity instances are not shared objects, this manager serves only as a cache for entities.
	// It always returns a copy of loaded entity.

	class ENGINE_API ModelEntityManager: public ResourceManager
	{
	public:
		ModelEntityResPtr CreateEntity(const tchar* file_name);
		ModelEntity* CreateEntityObject(const tchar* file_name);
		ModelEntityResPtr CreateCopy(ModelEntityResPtr model_entity);
		void ReleaseModelEntity(ModelEntityResPtr model_entity);
	};

	// ================================================================================================
	// Since particle system instances are not shared objects, this manager serves only as a cache for 
	// particle systems. It always returns a copy of loaded particle system.

	class ENGINE_API PartSysManager: public ResourceManager
	{
	public:
		PartSysResPtr CreateParticleSystem(const tchar* file_name);
		ParticleSystem* CreateParticleSystemObject(const tchar* file_name);
		PartSysResPtr CreateCopy(PartSysResPtr part_sys);
		void ReleasePartSys(PartSysResPtr part_sys);
	};

	// ================================================================================================

	class ENGINE_API SoundManager: public ResourceManager
	{
	public:
		SoundResPtr CreateSound(const tchar* file_name, bool immediate = false);
		bool ReleaseSound(SoundResPtr Sound);
		SoundResPtr FindSound(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API AIScriptManager: public ResourceManager
	{
	public:
		AIScriptResPtr CreateAIScript(const tchar* file_name, bool immediate = false);
		bool ReleaseAIScript(AIScriptResPtr AIScript);
		AIScriptResPtr FindAIScript(const tchar* file_name);
	};

}


#endif // _ENGINE_RESOURCE_MANAGER_H_
