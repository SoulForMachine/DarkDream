
#ifndef _ENGINE_WORLD_H_
#define _ENGINE_WORLD_H_

#include "BaseLib/FreeList.h"
#include "BaseLib/FreePool.h"
#include "BaseLib/List.h"
#include "Engine/Common.h"
#include "Engine/Entity.h"
#include "Engine/ModelEntity.h"
#include "Engine/Camera.h"
#include "Engine/Terrain.h"
#include "Engine/BgLayer.h"
#include "Engine/ParticleSystem.h"
#include "Engine/EntityHashMap.h"


namespace Engine
{



	class ENGINE_API World
	{
	public:
		static const int MAX_NUM_ENTITIES = 16 * 1024;

		World();

		bool Init();
		void Deinit();

		bool LoadMap(const tchar* file_name);
		bool SaveMap(const tchar* file_name);
		bool AddEntity(Entity* entity);
		bool RemoveEntity(Entity* entity);
		void RemoveAllEntities();
		int GetVisibleRenderableEntities(RenderableEntity** entities, int max_entities);
		int GetVisibleModelEntities(ModelEntity** entities, int max_entities);
		int GetVisibleTerrainPatches(const Terrain::TerrainPatch** patches, int max_patches);
		int GetVisibleLayerSprites(const BgLayer::Sprite** sprites, int max_sprites);
		int GetVisibleParticleSystems(ParticleSystem** part_sys, int max_ps);
		Camera& GetCamera()
			{ return _camera; }
		Terrain& GetTerrain()
			{ return _terrain; }
		EntityHashMap& GetEntities()
			{ return _entities; }
		BgLayerManager& GetLayerManager()
			{ return _layerManager; }

	private:
		Camera _camera;
		EntityHashMap _entities;
		Terrain _terrain;
		BgLayerManager _layerManager;
	};

}


#endif // _ENGINE_WORLD_H_
