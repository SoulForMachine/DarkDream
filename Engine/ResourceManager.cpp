
#include "BaseLib/Console.h"
#include "Engine/Common.h"
#include "EngineInternal.h"
#include "Engine/RenderSystem.h"
#include "Engine/ModelEntity.h"
#include "Engine/Material.h"
#include "ResourceManager.h"

using namespace Memory;


namespace Engine
{

	void CmdReloadShaders(char* args)
	{
	//	engineAPI.renderSystem->ReloadShaders();
		engineAPI.asmProgManager->UnloadAll();
		engineAPI.asmProgManager->LoadAll();
	}

	Console::Command reloadShaders("reloadShaders", "reloads all curently loaded shaders", CmdReloadShaders);



	/*
		If immediate is true, the resource will be immediately loaded. If file_name is empty string,
		created resource will point to null resource and it won't be added to manager map.
	*/
	const ResourceBase* ResourceManager::CreateRes(const tchar* key, const CreateResObjFunc& create_res_obj, bool immediate)
	{
		if(!key)
			return 0;

		// see if this resource is already created
		ResourceBase* res = (ResourceBase*)FindRes(key);
		if(res)
		{
			res->IncRefCount();
			if(immediate && !res->IsLoaded())
			{
				res->Load();
			}
			return res;
		}

		// create new object
		res = create_res_obj();

		if(res)
		{
			if(*key)
			{
				if(immediate)
					res->Load();
				_resources[key] = res;
			}
			else
			{
				res->Load();
			}
		}
		else
		{
			Console::PrintError("Failed to create resource: %ls", key);
		}

		return res;
	}

	bool ResourceManager::ReleaseRes(const tchar* key, const ResourceBase* res)
	{
		if(!res)
			return false;

		// if file name is an empty string, just release the resource
		if(!key || *key == '\0')
		{
			delete res;
			return true;
		}

		ResHashMap::Iterator it = _resources.Find(key);

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
			(*it)->Load();
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

	const ResourceBase* ResourceManager::FindRes(const tchar* file_name)
	{
		ResHashMap::Iterator it = _resources.Find(file_name);
		return (it != _resources.End())? *it: 0;
	}

	//=============================================================================================

	Texture2DResPtr TextureManager::CreateTexture2D(const tchar* file_name, bool immediate)
	{
		struct CreateTex2D: CreateResObjFunc
		{
			CreateTex2D(const tchar* file_name)
				{ _fileName = file_name; }
			ResourceBase* operator () () const
				{ return new(mapPool) Texture2DRes(_fileName); }
		private:
			const tchar* _fileName;
		};

		return Texture2DResPtr((const Texture2DRes*)CreateRes(file_name, CreateTex2D(file_name), immediate));
	}

	Texture3DResPtr TextureManager::CreateTexture3D(const tchar* file_name, bool immediate)
	{
		struct CreateTex3D: CreateResObjFunc
		{
			CreateTex3D(const tchar* file_name)
				{ _fileName = file_name; }
			ResourceBase* operator () () const
				{ return new(mapPool) Texture3DRes(_fileName); }
		private:
			const tchar* _fileName;
		};

		return Texture3DResPtr((const Texture3DRes*)CreateRes(file_name, CreateTex3D(file_name), immediate));
	}

	TextureCubeResPtr TextureManager::CreateTextureCube(const tchar* file_name, bool immediate)
	{
		struct CreateTexCube: CreateResObjFunc
		{
			CreateTexCube(const tchar* file_name)
				{ _fileName = file_name; }
			ResourceBase* operator () () const
				{ return new(mapPool) TextureCubeRes(_fileName); }
		private:
			const tchar* _fileName;
		};

		return TextureCubeResPtr((const TextureCubeRes*)CreateRes(file_name, CreateTexCube(file_name), immediate));
	}

	bool TextureManager::ReleaseTexture(Texture2DResPtr texture)
	{
		if(!texture)
			return false;
		return ReleaseRes(texture.GetRes()->GetFileName(), texture.GetRes());
	}

	bool TextureManager::ReleaseTexture(Texture3DResPtr texture)
	{
		if(!texture)
			return false;
		return ReleaseRes(texture.GetRes()->GetFileName(), texture.GetRes());
	}

	bool TextureManager::ReleaseTexture(TextureCubeResPtr texture)
	{
		if(!texture)
			return false;
		return ReleaseRes(texture.GetRes()->GetFileName(), texture.GetRes());
	}

	Texture2DResPtr TextureManager::FindTexture2D(const tchar* file_name)
	{
		return Texture2DResPtr((const Texture2DRes*)FindRes(file_name));
	}

	Texture3DResPtr TextureManager::FindTexture3D(const tchar* file_name)
	{
		return Texture3DResPtr((const Texture3DRes*)FindRes(file_name));
	}

	TextureCubeResPtr TextureManager::FindTextureCube(const tchar* file_name)
	{
		return TextureCubeResPtr((const TextureCubeRes*)FindRes(file_name));
	}

	//=============================================================================================

	VertexShaderResPtr ShaderManager::CreateVertexShader(const tchar* file_name, const char* macros, bool immediate)
	{
		tchar* key = MakeKey(file_name, macros);

		struct CreateVertShaderObj: CreateResObjFunc
		{
			CreateVertShaderObj(const tchar* file_name, const char* macros)
			{
				_fileName = file_name;
				_macros = macros;
			}

			ResourceBase* operator () () const
				{ return new(mapPool) VertexShaderRes(_fileName, _macros); }

		private:
			const tchar* _fileName;
			const char* _macros;
		};

		VertexShaderResPtr ptr = VertexShaderResPtr((const VertexShaderRes*)CreateRes(key, CreateVertShaderObj(file_name, macros), immediate));
		delete[] key;
		return ptr;
	}

	FragmentShaderResPtr ShaderManager::CreateFragmentShader(const tchar* file_name, const char* macros, bool immediate)
	{
		tchar* key = MakeKey(file_name, macros);

		struct CreateFragShaderObj: CreateResObjFunc
		{
			CreateFragShaderObj(const tchar* file_name, const char* macros)
			{
				_fileName = file_name;
				_macros = macros;
			}

			ResourceBase* operator () () const
				{ return new(mapPool) FragmentShaderRes(_fileName, _macros); }

		private:
			const tchar* _fileName;
			const char* _macros;
		};

		FragmentShaderResPtr ptr = FragmentShaderResPtr((const FragmentShaderRes*)CreateRes(key, CreateFragShaderObj(file_name, macros), immediate));
		delete[] key;
		return ptr;
	}

	GeometryShaderResPtr ShaderManager::CreateGeometryShader(const tchar* file_name, const char* macros, bool immediate)
	{
		tchar* key = MakeKey(file_name, macros);

		struct CreateGeomShaderObj: CreateResObjFunc
		{
			CreateGeomShaderObj(const tchar* file_name, const char* macros)
			{
				_fileName = file_name;
				_macros = macros;
			}

			ResourceBase* operator () () const
				{ return new(mapPool) GeometryShaderRes(_fileName, _macros); }

		private:
			const tchar* _fileName;
			const char* _macros;
		};

		GeometryShaderResPtr ptr = GeometryShaderResPtr((const GeometryShaderRes*)CreateRes(key, CreateGeomShaderObj(file_name, macros), immediate));
		delete[] key;
		return ptr;
	}

	bool ShaderManager::ReleaseShader(VertexShaderResPtr shader)
	{
		if(!shader)
			return false;

		tchar* key = MakeKey(shader.GetRes()->GetFileName(), shader.GetRes()->GetMacros());
		bool result = ReleaseRes(key, shader.GetRes());
		delete[] key;
		return result;
	}

	bool ShaderManager::ReleaseShader(FragmentShaderResPtr shader)
	{
		if(!shader)
			return false;

		tchar* key = MakeKey(shader.GetRes()->GetFileName(), shader.GetRes()->GetMacros());
		bool result = ReleaseRes(key, shader.GetRes());
		delete[] key;
		return result;
	}

	bool ShaderManager::ReleaseShader(GeometryShaderResPtr shader)
	{
		if(!shader)
			return false;

		tchar* key = MakeKey(shader.GetRes()->GetFileName(), shader.GetRes()->GetMacros());
		bool result = ReleaseRes(key, shader.GetRes());
		delete[] key;
		return result;
	}

	VertexShaderResPtr ShaderManager::FindVertexShader(const tchar* file_name, const char* macros)
	{
		tchar* key = MakeKey(file_name, macros);

		VertexShaderResPtr ptr = VertexShaderResPtr((const VertexShaderRes*)ResourceManager::FindRes(key));
		delete[] key;
		return ptr;
	}

	FragmentShaderResPtr ShaderManager::FindFragmentShader(const tchar* file_name, const char* macros)
	{
		tchar* key = MakeKey(file_name, macros);

		FragmentShaderResPtr ptr = FragmentShaderResPtr((const FragmentShaderRes*)ResourceManager::FindRes(key));
		delete[] key;
		return ptr;
	}

	GeometryShaderResPtr ShaderManager::FindGeometryShader(const tchar* file_name, const char* macros)
	{
		tchar* key = MakeKey(file_name, macros);

		GeometryShaderResPtr ptr = GeometryShaderResPtr((const GeometryShaderRes*)ResourceManager::FindRes(key));
		delete[] key;
		return ptr;
	}

	tchar* ShaderManager::MakeKey(const tchar* file_name, const char* macros)
	{
		// make a hash map key from concatenated file name and macros
		tchar* key = new(tempPool) tchar[tstrlen(file_name) + strlen(macros) + 2];
		tstrcpy(key, file_name);
		tstrcat(key, _t("|"));
		tchar* tm = CharToWideChar(macros);
		tstrcat(key, tm);
		delete[] tm;
		return key;
	}

	//=============================================================================================

	VertexASMProgResPtr ASMProgManager::CreateVertexASMProgram(const tchar* file_name, bool immediate)
	{
		struct CreateVertexProgObj: CreateResObjFunc
		{
			CreateVertexProgObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) VertexASMProgRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return VertexASMProgResPtr((VertexASMProgRes*)CreateRes(file_name, CreateVertexProgObj(file_name), immediate));
	}

	FragmentASMProgResPtr ASMProgManager::CreateFragmentASMProgram(const tchar* file_name, bool immediate)
	{
		struct CreateFragmentProgObj: CreateResObjFunc
		{
			CreateFragmentProgObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) FragmentASMProgRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return FragmentASMProgResPtr((FragmentASMProgRes*)CreateRes(file_name, CreateFragmentProgObj(file_name), immediate));
	}

	GeometryASMProgResPtr ASMProgManager::CreateGeometryASMProgram(const tchar* file_name, bool immediate)
	{
		struct CreateGeometryProgObj: CreateResObjFunc
		{
			CreateGeometryProgObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) GeometryASMProgRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return GeometryASMProgResPtr((GeometryASMProgRes*)CreateRes(file_name, CreateGeometryProgObj(file_name), immediate));
	}


	bool ASMProgManager::ReleaseASMProgram(VertexASMProgResPtr program)
	{
		if(!program)
			return false;

		return ReleaseRes(program.GetRes()->GetFileName(), program.GetRes());
	}

	bool ASMProgManager::ReleaseASMProgram(FragmentASMProgResPtr program)
	{
		if(!program)
			return false;

		return ReleaseRes(program.GetRes()->GetFileName(), program.GetRes());
	}

	bool ASMProgManager::ReleaseASMProgram(GeometryASMProgResPtr program)
	{
		if(!program)
			return false;

		return ReleaseRes(program.GetRes()->GetFileName(), program.GetRes());
	}

	VertexASMProgResPtr ASMProgManager::FindVertexASMProgram(const tchar* file_name)
	{
		return VertexASMProgResPtr((VertexASMProgRes*)ResourceManager::FindRes(file_name));
	}

	FragmentASMProgResPtr ASMProgManager::FindFragmentASMProgram(const tchar* file_name)
	{
		return FragmentASMProgResPtr((FragmentASMProgRes*)ResourceManager::FindRes(file_name));
	}

	GeometryASMProgResPtr ASMProgManager::FindGeometryASMProgram(const tchar* file_name)
	{
		return GeometryASMProgResPtr((GeometryASMProgRes*)ResourceManager::FindRes(file_name));
	}

	//=============================================================================================


	MaterialResPtr MaterialManager::CreateMaterial(const tchar* file_name, bool immediate)
	{
		struct CreateMaterialObj: CreateResObjFunc
		{
			CreateMaterialObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) MaterialRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return MaterialResPtr((const MaterialRes*)CreateRes(file_name, CreateMaterialObj(file_name), immediate));
	}

	MaterialResPtr MaterialManager::CreateDefaultMaterial()
	{
		MaterialRes* mat = new(mapPool) MaterialRes(_t(""));
		mat->_resource = new(mapPool) Material;
		return MaterialResPtr(mat);
	}

	bool MaterialManager::ReleaseMaterial(MaterialResPtr material)
	{
		if(!material)
			return false;

		return ReleaseRes(material.GetRes()->GetFileName(), material.GetRes());
	}

	MaterialResPtr MaterialManager::FindMaterial(const tchar* file_name)
	{
		return MaterialResPtr((const MaterialRes*)FindRes(file_name));
	}

	//=============================================================================================

	ModelResPtr ModelManager::CreateModel(const tchar* file_name, bool immediate)
	{
		struct CreateModelObj: CreateResObjFunc
		{
			CreateModelObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) ModelRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return ModelResPtr((const ModelRes*)CreateRes(file_name, CreateModelObj(file_name), immediate));
	}

	bool ModelManager::ReleaseModel(ModelResPtr Model)
	{
		if(!Model)
			return false;

		return ReleaseRes(Model.GetRes()->GetFileName(), Model.GetRes());
	}

	ModelResPtr ModelManager::FindModel(const tchar* file_name)
	{
		return ModelResPtr((const ModelRes*)FindRes(file_name));
	}

	//=============================================================================================

	AnimationResPtr AnimationManager::CreateAnimation(const tchar* file_name, bool immediate)
	{
		struct CreateAnimationObj: CreateResObjFunc
		{
			CreateAnimationObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) AnimationRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return AnimationResPtr((const AnimationRes*)CreateRes(file_name, CreateAnimationObj(file_name), immediate));
	}

	bool AnimationManager::ReleaseAnimation(AnimationResPtr Animation)
	{
		if(!Animation)
			return false;

		return ReleaseRes(Animation.GetRes()->GetFileName(), Animation.GetRes());
	}

	AnimationResPtr AnimationManager::FindAnimation(const tchar* file_name)
	{
		return AnimationResPtr((const AnimationRes*)FindRes(file_name));
	}

	//=============================================================================================

	ModelEntityResPtr ModelEntityManager::CreateEntity(const tchar* file_name)
	{
		struct CreateModelEntityObj: CreateResObjFunc
		{
			CreateModelEntityObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) ModelEntityRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		const ModelEntityRes* ent = (const ModelEntityRes*)ResourceManager::CreateRes(file_name, CreateModelEntityObj(file_name), true);
		return ModelEntityResPtr(new(mapPool) ModelEntityRes(*ent));
	}

	ModelEntity* ModelEntityManager::CreateEntityObject(const tchar* file_name)
	{
		struct CreateModelEntityObj: CreateResObjFunc
		{
			CreateModelEntityObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) ModelEntityRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		ModelEntityRes* ent = (ModelEntityRes*)ResourceManager::CreateRes(file_name, CreateModelEntityObj(file_name), true);
		return ent->GetResource()->CreateCopy();
	}

	ModelEntityResPtr ModelEntityManager::CreateCopy(ModelEntityResPtr model_entity)
	{
		if(!model_entity || !model_entity.GetRes()->IsLoaded())
			return ModelEntityResPtr::null;

		return ModelEntityResPtr(new(mapPool) ModelEntityRes(*model_entity.GetRes()));
	}

	void ModelEntityManager::ReleaseModelEntity(ModelEntityResPtr model_entity)
	{
		if(model_entity.GetRes())
			delete model_entity;
	}

	//=============================================================================================

	PartSysResPtr PartSysManager::CreateParticleSystem(const tchar* file_name)
	{
		struct CreatePartSysObj: CreateResObjFunc
		{
			CreatePartSysObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) PartSysRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		const PartSysRes* ent = (const PartSysRes*)ResourceManager::CreateRes(file_name, CreatePartSysObj(file_name), true);
		return PartSysResPtr(new(mapPool) PartSysRes(*ent));
	}

	ParticleSystem* PartSysManager::CreateParticleSystemObject(const tchar* file_name)
	{
		struct CreatePartSysObj: CreateResObjFunc
		{
			CreatePartSysObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) PartSysRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		PartSysRes* ps = (PartSysRes*)ResourceManager::CreateRes(file_name, CreatePartSysObj(file_name), true);
		return new(mapPool) ParticleSystem(*ps->GetResource());
	}

	PartSysResPtr PartSysManager::CreateCopy(PartSysResPtr part_sys)
	{
		if(!part_sys || !part_sys.GetRes()->IsLoaded())
			return PartSysResPtr::null;

		return PartSysResPtr(new(mapPool) PartSysRes(*part_sys.GetRes()));
	}

	void PartSysManager::ReleasePartSys(PartSysResPtr part_sys)
	{
		if(part_sys.GetRes())
			delete part_sys;
	}

	//=============================================================================================

	SoundResPtr SoundManager::CreateSound(const tchar* file_name, bool immediate)
	{
		struct CreateSoundObj: CreateResObjFunc
		{
			CreateSoundObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) SoundRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return SoundResPtr((const SoundRes*)CreateRes(file_name, CreateSoundObj(file_name), immediate));
	}

	bool SoundManager::ReleaseSound(SoundResPtr Sound)
	{
		if(!Sound)
			return false;

		return ReleaseRes(Sound.GetRes()->GetFileName(), Sound.GetRes());
	}

	SoundResPtr SoundManager::FindSound(const tchar* file_name)
	{
		return SoundResPtr((const SoundRes*)FindRes(file_name));
	}

	//=============================================================================================

	AIScriptResPtr AIScriptManager::CreateAIScript(const tchar* file_name, bool immediate)
	{
		struct CreateAIScriptObj: CreateResObjFunc
		{
			CreateAIScriptObj(const tchar* file_name)
				{ _fileName = file_name; }

			ResourceBase* operator () () const
				{ return new(mapPool) AIScriptRes(_fileName); }

		private:
			const tchar* _fileName;
		};

		return AIScriptResPtr((const AIScriptRes*)CreateRes(file_name, CreateAIScriptObj(file_name), immediate));
	}

	bool AIScriptManager::ReleaseAIScript(AIScriptResPtr AIScript)
	{
		if(!AIScript)
			return false;

		return ReleaseRes(AIScript.GetRes()->GetFileName(), AIScript.GetRes());
	}

	AIScriptResPtr AIScriptManager::FindAIScript(const tchar* file_name)
	{
		return AIScriptResPtr((const AIScriptRes*)FindRes(file_name));
	}

}
