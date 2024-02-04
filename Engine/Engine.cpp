
#include "BaseLib/Memory.h"
#include "Engine.h"

using namespace Memory;


namespace Engine
{

	EngineAPI engineAPI;


	EngineAPI* CreateEngineAPI()
	{
		// create engine sub-systems
		engineAPI.fileSystem = New<FileSystem>(mainPool);
		engineAPI.inputSystem = New<InputSystem>(mainPool);
		engineAPI.renderSystem = New<RenderSystem>(mainPool);
		engineAPI.world = New<World>(mainPool);

		// initialize resource managers
		engineAPI.shaderManager = New<ShaderManager>(mainPool);
		engineAPI.materialManager = New<MaterialManager>(mainPool);
		engineAPI.asmProgManager = New<ASMProgManager>(mainPool);
		engineAPI.textureManager = New<TextureManager>(mainPool);
		engineAPI.modelManager = New<ModelManager>(mainPool);
		engineAPI.modelEntityManager = New<ModelEntityManager>(mainPool);
		engineAPI.animationManager = New<AnimationManager>(mainPool);
		engineAPI.partSysManager = New<PartSysManager>(mainPool);		
		engineAPI.aiScriptManager = New<AIScriptManager>(mainPool);
		engineAPI.soundManager = New<SoundManager>(mainPool);

		return &engineAPI;
	}

	void DestroyEngineAPI(EngineAPI* api)
	{
		Delete(engineAPI.shaderManager);
		engineAPI.shaderManager = 0;
		Delete(engineAPI.materialManager);
		engineAPI.materialManager = 0;
		Delete(engineAPI.asmProgManager);
		engineAPI.asmProgManager = 0;
		Delete(engineAPI.textureManager);
		engineAPI.textureManager = 0;
		Delete(engineAPI.modelManager);
		engineAPI.modelManager = 0;
		Delete(engineAPI.modelEntityManager);
		engineAPI.modelEntityManager = 0;
		Delete(engineAPI.animationManager);
		engineAPI.animationManager = 0;
		Delete(engineAPI.partSysManager);
		engineAPI.partSysManager = 0;
		Delete(engineAPI.aiScriptManager);
		engineAPI.aiScriptManager = 0;
		Delete(engineAPI.soundManager);
		engineAPI.soundManager = 0;

		Delete(engineAPI.fileSystem);
		engineAPI.fileSystem = 0;
		Delete(engineAPI.inputSystem);
		engineAPI.inputSystem = 0;
		Delete(engineAPI.renderSystem);
		engineAPI.renderSystem = 0;
		Delete(engineAPI.world);
		engineAPI.world = 0;
	}

}
