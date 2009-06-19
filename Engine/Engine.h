
#ifndef _ENGINE_H_
#define _ENGINE_H_


#include "Common.h"
#include "Renderer/RenderSystem.h"
#include "Renderer/Model.h"
#include "Renderer/Material.h"
#include "Renderer/Animation.h"
#include "Renderer/ParticleSystem.h"
#include "Renderer/Camera.h"
#include "Renderer/Light.h"
#include "Renderer/Terrain.h"
#include "Input/InputSystem.h"
#include "FileSystem.h"
#include "FileResource.h"
#include "ResourceManager.h"
#include "ModelEntity.h"
#include "World.h"


namespace Engine
{

	ENGINE_API EngineAPI* CreateEngineAPI();
	ENGINE_API void DestroyEngineAPI(EngineAPI* api);

}


#endif // _ENGINE_H_
