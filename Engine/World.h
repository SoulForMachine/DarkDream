
#ifndef _ENGINE_WORLD_H_
#define _ENGINE_WORLD_H_

#include "BaseLib/FreeList.h"
#include "BaseLib/FreePool.h"
#include "BaseLib/List.h"
#include "Engine/Common.h"
#include "Engine/Entity.h"
#include "Engine/ModelEntity.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Light.h"


namespace Engine
{

	class Terrain;


	class ENGINE_API World
	{
	public:
		typedef FreeList<const Entity*> EntityList;
		typedef List<Terrain*> TerrainPatchList;

		World();

		bool Init();
		void Deinit();

		bool LoadMap(const tchar* file_name);
		bool SaveMap(const tchar* file_name);
		Camera& GetCamera()
			{ return _camera; }
		Light& GetMainLight()
			{ return _mainLight; }
		void AddEntity(const Entity* entity)
			{ _entities.PushBack(entity); }
		void RemoveAllEntities()
			{ _entities.Reset(); }
		int GetVisibleEntities(ModelEntity** entities, int max_entities);
		int GetVisibleShadowcasters(ModelEntity** entities, int max_entities);
		int GetVisibleLights(Light** lights, int max_lights);
		Terrain* AddTerrain(int width, int height);
		void AddTerrain(Terrain* terrain);
		const TerrainPatchList& GetTerrainPatches() const
			{ return _terrainPatches; }

	private:
		static const int MAX_NUM_ENTITIES = 16 * 1024;

		void PositionTerrain(Terrain* terrain);

		Camera _camera;
		Light _mainLight;
		FreePool<FreeListNode<const Entity*> > _entityPool;
		EntityList _entities;
		TerrainPatchList _terrainPatches;
	};

}


#endif // _ENGINE_WORLD_H_
