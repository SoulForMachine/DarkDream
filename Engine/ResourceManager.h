
#ifndef _ENGINE_RESOURCE_MANAGER_H_
#define _ENGINE_RESOURCE_MANAGER_H_

#include "BaseLib/Memory.h"
#include "BaseLib/HashMap.h"
#include "Engine/FileResource.h"


namespace Engine
{

	class ShaderManager;
	class ASMProgManager;
	class TextureManager;
	class ModelManager;
	class AnimationManager;
	class ModelEntityManager;
	class SoundManager;
	class AIScriptManager;
	class PartSysManager;


	class ENGINE_API ResourceManager
	{
	public:
		ResourceManager():
			_resources(Memory::mapPool) {}

		void LoadAll();
		void UnloadAll();
		void ReleaseAll();

	protected:
		struct HashMapTraits
		{
			static uint GetHash(const tchar* str)
				{ return ::GetStringiHash(str); }
		};

		const FileResource* CreateRes(const tchar* file_name, bool immediate);
		bool ReleaseRes(const FileResource* res);
		const FileResource* FindRes(const tchar* file_name);

		typedef HashMap<const tchar*, FileResource*, HashMapTraits> ResHashMap;
		virtual FileResource* CreateResObj(const tchar* file_name) = 0;
		ResHashMap _resources;
	};

	// ================================================================================================

	class ENGINE_API TextureManager: public ResourceManager
	{
	public:
		TextureManager() {}

		const TextureRes* CreateTexture(const tchar* file_name, bool immediate = false)
			{ return (const TextureRes*)CreateRes(file_name, immediate); }
		bool ReleaseTexture(const TextureRes* texture)
			{ return ReleaseRes(texture); }
		const TextureRes* FindTexture(const tchar* file_name)
			{ return (const TextureRes*)FindRes(file_name); }
	
	protected:
		virtual TextureRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API ShaderManager: public ResourceManager
	{
	public:
		ShaderManager() {}

		const ShaderRes* CreateShader(const tchar* file_name, const char* macros = "", bool immediate = false);
		bool ReleaseShader(const ShaderRes* shader);
		const ShaderRes* FindShader(const tchar* file_name, const char* macros);

	protected:
		virtual ShaderRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API ASMProgManager: public ResourceManager
	{
	public:
		ASMProgManager() {}

		const ASMProgRes* CreateASMProgram(const tchar* file_name, bool immediate = false);
		bool ReleaseASMProgram(const ASMProgRes* program);
		const ASMProgRes* FindASMProgram(const tchar* file_name);

	protected:
		virtual ASMProgRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API ModelManager: public ResourceManager
	{
	public:
		const ModelRes* CreateModel(const tchar* file_name, bool immediate = false)
			{ return (const ModelRes*)CreateRes(file_name, immediate); }
		bool ReleaseModel(const ModelRes* model)
			{ return ReleaseRes(model); }
		const ModelRes* FindModel(const tchar* file_name)
			{ return (const ModelRes*)FindRes(file_name); }

	protected:
		virtual ModelRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API AnimationManager: public ResourceManager
	{
	public:
		const AnimationRes* CreateAnimation(const tchar* file_name, bool immediate = false)
			{ return (const AnimationRes*)CreateRes(file_name, immediate); }
		bool ReleaseAnimation(const AnimationRes* anim)
			{ return ReleaseRes(anim); }
		const AnimationRes* FindAnimation(const tchar* file_name)
			{ return (const AnimationRes*)FindRes(file_name); }

	protected:
		virtual AnimationRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================
	// Since entity instances are not shared objects, this manager serves only as a cache for entities.
	// It always returns a copy of loaded entity.

	class ENGINE_API ModelEntityManager: public ResourceManager
	{
	public:
		ModelEntityRes* CreateEntity(const tchar* file_name);

	private:
		virtual ModelEntityRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API SoundManager: public ResourceManager
	{
	public:
		const SoundRes* CreateSound(const tchar* file_name, bool immediate = false)
			{ return (const SoundRes*)CreateRes(file_name, immediate); }
		bool ReleaseSound(const SoundRes* sound)
			{ return ReleaseRes(sound); }
		const SoundRes* FindSound(const tchar* file_name)
			{ return (const SoundRes*)FindRes(file_name); }

	protected:
		virtual SoundRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================

	class ENGINE_API AIScriptManager: public ResourceManager
	{
	public:
		const AIScriptRes* CreateAIScript(const tchar* file_name, bool immediate = false)
			{ return (const AIScriptRes*)CreateRes(file_name, immediate); }
		bool ReleaseAIScript(const AIScriptRes* ai)
			{ return ReleaseRes(ai); }
		const AIScriptRes* FindAIScript(const tchar* file_name)
			{ return (const AIScriptRes*)FindRes(file_name); }

	protected:
		virtual AIScriptRes* CreateResObj(const tchar* file_name);
	};

	// ================================================================================================
	// Since particle system instances are not shared objects, this manager serves only as a cache for 
	// particle systems. It always returns a copy of loaded particle system.

	class ENGINE_API PartSysManager: public ResourceManager
	{
	public:
		const PartSysRes* CreatePartSys(const tchar* file_name);

	private:
		virtual PartSysRes* CreateResObj(const tchar* file_name);
	};

}


#endif // _ENGINE_RESOURCE_MANAGER_H_
