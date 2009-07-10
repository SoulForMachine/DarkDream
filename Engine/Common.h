
#ifndef _ENGINE_COMMON_H_
#define _ENGINE_COMMON_H_

#pragma warning(disable : 4251) // class 'x' needs to have dll-interface to be used by clients of class 'y'

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#pragma comment(lib, "../Debug/BaseLib.lib")
#else
#pragma comment(lib, "../Release/BaseLib.lib")
#endif


namespace Engine
{

	class FileSystem;
	class InputSystem;
	class RenderSystem;
	class World;
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


	struct EngineAPI
	{
		FileSystem* fileSystem;
		InputSystem* inputSystem;
		RenderSystem* renderSystem;
		World* world;

		ShaderManager* shaderManager;
		ASMProgManager* asmProgManager;
		TextureManager* textureManager;
		MaterialManager* materialManager;
		ModelManager* modelManager;
		AnimationManager* animationManager;
		ModelEntityManager* modelEntityManager;
		SoundManager* soundManager;
		AIScriptManager* aiScriptManager;
		PartSysManager* partSysManager;
	};

}


#endif // _ENGINE_COMMON_H_
