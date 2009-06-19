
#include "BaseLib/Console.h"
#include "Engine/Common.h"
#include "EngineInternal.h"
#include "Engine/Renderer/RenderSystem.h"
#include "ResourceManager.h"

using namespace Memory;


namespace Engine
{

	void CmdReloadShaders(char* args)
	{
		engineAPI.renderSystem->ReloadShaders();
	}

	Console::Command reloadShaders("reloadShaders", "reloads all curently loaded shaders", CmdReloadShaders);



	/*
		If immediate is true, the resource will be immediately loaded
	*/
	const FileResource* ResourceManager::CreateRes(const tchar* file_name, bool immediate)
	{
		if(!file_name)
			return 0;

		// see if this resource is already created
		FileResource* res = (FileResource*)FindRes(file_name);
		if(res)
		{
			res->IncRefCount();
			if(immediate && !res->IsLoaded())
			{
				if(!res->Load())
					res->LoadDefault();
			}
			return res;
		}

		// create new object
		res = CreateResObj(file_name);

		if(res)
		{
			if(*file_name != '\0')
			{
				if(immediate)
				{
					if(!res->Load())
						res->LoadDefault();
				}
				_resources[file_name] = res;
			}
			else
			{
				res->LoadDefault();
			}
		}
		else
		{
			Console::PrintError("Failed to create resource object for %ls", file_name);
		}

		return res;
	}

	bool ResourceManager::ReleaseRes(const FileResource* res)
	{
		if(!res)
			return false;

		// if file name is empty string, just release the resource
		if(!res->GetFileName() || *res->GetFileName() == '\0')
		{
			delete res;
			return true;
		}

		ResHashMap::Iterator it = _resources.Find(res->GetFileName());

		if(it != _resources.End())
		{
			assert(*it == res);
			(*it)->DecRefCount();
			if(!(*it)->IsReferenced())
			{
				(*it)->Unload();
				delete *it;
				_resources.Remove(it);
			}
			return true;
		}
		else
		{
			Console::PrintWarning("Releasing resource: %ls not found in manager.", res->GetFileName());
			return false;
		}
	}

	void ResourceManager::LoadAll()
	{
		for(ResHashMap::Iterator it = _resources.Begin(); it != _resources.End(); ++it)
		{
			FileResource* res = *it;
			if(!res->IsLoaded())
			{
				if(!res->Load())
					res->LoadDefault();
			}
		}
	}

	void ResourceManager::UnloadAll()
	{
		for(ResHashMap::Iterator it = _resources.Begin(); it != _resources.End(); ++it)
		{
			(*it)->Unload();
		}
	}

	void ResourceManager::ReleaseAll()
	{
		for(ResHashMap::Iterator it = _resources.Begin(); it != _resources.End(); ++it)
		{
			(*it)->Unload();
			delete *it;
		}
		_resources.Clear();
	}

	const FileResource* ResourceManager::FindRes(const tchar* file_name)
	{
		ResHashMap::Iterator it = _resources.Find(file_name);
		return (it != _resources.End())? *it: 0;
	}

	TextureRes* TextureManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) TextureRes(file_name, engineAPI.renderSystem->GetRenderer());
	}

	const ShaderRes* ShaderManager::CreateShader(const tchar* file_name, const char* macros, bool immediate)
	{
		if(!file_name)
			return 0;

		// make a hash map key from concatenated file name and macros
		tchar* key = new(tempPool) tchar[tstrlen(file_name) + strlen(macros) + 2];
		tstrcpy(key, file_name);
		tstrcat(key, _t("|"));
		tchar* tm = CharToWideChar(macros);
		tstrcat(key, tm);
		delete[] tm;

		// see if this resource is already created
		FileResource* res = (FileResource*)FindRes(key);
		if(res)
		{
			res->IncRefCount();
			if(immediate && !res->IsLoaded())
			{
				if(!res->Load())
					res->LoadDefault();
			}
			delete[] key;
			return (const ShaderRes*)res;
		}

		// create new object
		res = new(mapPool) ShaderRes(file_name, macros, engineAPI.renderSystem->GetRenderer());

		if(res)
		{
			if(*file_name != '\0')
			{
				if(immediate)
				{
					if(!res->Load())
						res->LoadDefault();
				}
				_resources[key] = res;
			}
			else
			{
				res->LoadDefault();
			}
		}
		else
		{
			Console::PrintError("Failed to create shader object, file: %ls, macros: %s", file_name, macros);
		}

		delete[] key;
		return (const ShaderRes*)res;
	}

	bool ShaderManager::ReleaseShader(const ShaderRes* shader)
	{
		if(!shader)
			return false;

		const tchar* file_name = shader->GetFileName();
		const char* macros = shader->GetMacros();

		// if file name is empty string, just release the resource
		if(!file_name || *file_name == '\0')
		{
			delete shader;
			return true;
		}

		// make a hash map key from concatenated file name and macros
		tchar* key = new(tempPool) tchar[tstrlen(file_name) + strlen(macros) + 2];
		tstrcpy(key, file_name);
		tstrcat(key, _t("|"));
		tchar* tm = CharToWideChar(macros);
		tstrcat(key, tm);
		delete[] tm;

		ResHashMap::Iterator it = _resources.Find(key);
		delete[] key;

		if(it != _resources.End())
		{
			assert(*it == shader);
			(*it)->DecRefCount();
			if(!(*it)->IsReferenced())
			{
				(*it)->Unload();
				delete *it;
				_resources.Remove(it);
			}
			return true;
		}
		else
		{
			Console::PrintWarning("Releasing shader: %ls not found in manager.", shader->GetFileName());
			return false;
		}
	}

	const ShaderRes* ShaderManager::FindShader(const tchar* file_name, const char* macros)
	{
		// make a hash map key from concatenated file name and macros
		tchar* key = new(tempPool) tchar[tstrlen(file_name) + strlen(macros) + 2];
		tstrcpy(key, file_name);
		tstrcat(key, _t("|"));
		tchar* tm = CharToWideChar(macros);
		tstrcat(key, tm);
		delete[] tm;

		const ShaderRes* shader = (const ShaderRes*)ResourceManager::FindRes(key);
		delete[] key;
		return shader;
	}

	ShaderRes* ShaderManager::CreateResObj(const tchar* file_name)
	{
		assert(false);
		return 0;
	}

	MaterialRes* MaterialManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) MaterialRes(file_name);
	}

	ModelRes* ModelManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) ModelRes(file_name);
	}

	AnimationRes* AnimationManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) AnimationRes(file_name);
	}

	const ModelEntityRes* ModelEntityManager::CreateEntity(const tchar* file_name)
	{
		const ModelEntityRes* ent = (const ModelEntityRes*)ResourceManager::CreateRes(file_name, true);
		return new(mapPool) ModelEntityRes(*ent);
	}

	ModelEntityRes* ModelEntityManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) ModelEntityRes(file_name);
	}

	SoundRes* SoundManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) SoundRes(file_name);
	}

	AIScriptRes* AIScriptManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) AIScriptRes(file_name);
	}

	const PartSysRes* PartSysManager::CreatePartSys(const tchar* file_name)
	{
		const PartSysRes* part = (const PartSysRes*)ResourceManager::CreateRes(file_name, true);
		return new(mapPool) PartSysRes(*part);
	}

	PartSysRes* PartSysManager::CreateResObj(const tchar* file_name)
	{
		return new(mapPool) PartSysRes(file_name);
	}

}
