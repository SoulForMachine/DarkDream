
#include "BaseLib/Memory.h"
#include "Engine.h"

using namespace Memory;


namespace Engine
{

	EngineAPI engineAPI;


	EngineAPI* CreateEngineAPI()
	{
		// initialize graphics resource managers
		engineAPI.shaderManager = new(mainPool) ShaderManager;
		engineAPI.textureManager = new(mainPool) TextureManager;
		engineAPI.materialManager = new(mainPool) MaterialManager;
		engineAPI.modelManager = new(mainPool) ModelManager;
		engineAPI.modelEntityManager = new(mainPool) ModelEntityManager;
		engineAPI.animationManager = new(mainPool) AnimationManager;
		engineAPI.partSysManager = new(mainPool) PartSysManager;		
		engineAPI.aiScriptManager = new(mainPool) AIScriptManager;
		engineAPI.soundManager = new(mainPool) SoundManager;

		engineAPI.fileSystem = new(mainPool) FileSystem;
		engineAPI.inputSystem = new(mainPool) InputSystem;
		engineAPI.renderSystem = new(mainPool) RenderSystem;
		engineAPI.world = new(mainPool) World;

		return &engineAPI;
	}

	void DestroyEngineAPI(EngineAPI* api)
	{
		delete engineAPI.fileSystem;
		engineAPI.fileSystem = 0;
		delete engineAPI.inputSystem;
		engineAPI.inputSystem = 0;
		delete engineAPI.renderSystem;
		engineAPI.renderSystem = 0;
		delete engineAPI.world;
		engineAPI.world = 0;

		delete engineAPI.shaderManager;
		engineAPI.shaderManager = 0;
		delete engineAPI.textureManager;
		engineAPI.textureManager = 0;
		delete engineAPI.materialManager;
		engineAPI.materialManager = 0;
		delete engineAPI.modelManager;
		engineAPI.modelManager = 0;
		delete engineAPI.modelEntityManager;
		engineAPI.modelEntityManager = 0;
		delete engineAPI.animationManager;
		engineAPI.animationManager = 0;
		delete engineAPI.partSysManager;
		engineAPI.partSysManager = 0;
		delete engineAPI.aiScriptManager;
		engineAPI.aiScriptManager = 0;
		delete engineAPI.soundManager;
		engineAPI.soundManager = 0;
	}

}
