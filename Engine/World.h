
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


namespace Engine
{



	class ENGINE_API World
	{
	public:
		typedef FreeList<Entity*> EntityList;

		World();

		bool Init();
		void Deinit();

		bool LoadMap(const tchar* file_name);
		bool SaveMap(const tchar* file_name);
		void AddEntity(Entity* entity);
		void RemoveAllEntities();
		int GetVisibleEntities(ModelEntity** entities, int max_entities);
		int GetVisibleTerrainPatches(const Terrain::TerrainPatch** patches, int max_patches);
		Camera& GetCamera()
			{ return _camera; }
		Terrain& GetTerrain()
			{ return _terrain; }

	private:
		static const int MAX_NUM_ENTITIES = 16 * 1024;

		Camera _camera;
		FreePool<FreeListNode<Entity*> > _entityPool;
		EntityList _entities;
		Terrain _terrain;
	};

}


#endif // _ENGINE_WORLD_H_
