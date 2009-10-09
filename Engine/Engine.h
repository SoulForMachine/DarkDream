
#ifndef _ENGINE_H_
#define _ENGINE_H_


#include "Common.h"
#include "RenderSystem.h"
#include "Model.h"
#include "Animation.h"
#include "Material.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "Terrain.h"
#include "InputSystem.h"
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
