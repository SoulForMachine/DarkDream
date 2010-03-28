
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
#include "BgLayer.h"
#include "InputSystem.h"
#include "FileSystem.h"
#include "Resource.h"
#include "ResourceManager.h"
#include "ModelEntity.h"
#include "StaticEntity.h"
#include "PlayerEntity.h"
#include "AIEntity.h"
#include "WeaponEntity.h"
#include "ItemEntity.h"
#include "World.h"
#include "ObjectFactory.h"


namespace Engine
{

	ENGINE_API EngineAPI* CreateEngineAPI();
	ENGINE_API void DestroyEngineAPI(EngineAPI* api);

}


#endif // _ENGINE_H_
