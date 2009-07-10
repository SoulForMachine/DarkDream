
#ifndef _ENGINE_WORLD_H_
#define _ENGINE_WORLD_H_

#include "BaseLib/FreeList.h"
#include "BaseLib/FreePool.h"
#include "BaseLib/List.h"
#include "Engine/Common.h"
#include "Engine/Entity.h"
#include "Engine/ModelEntity.h"
#include "Engine/Camera.h"


namespace Engine
{

	class Terrain;


	class ENGINE_API World
	{
	public:
		typedef FreeList<const Entity*> EntityList;

		World();

		bool Init();
		void Deinit();

		bool LoadMap(const tchar* file_name);
		bool SaveMap(const tchar* file_name);
		Camera& GetCamera()
			{ return _camera; }
		void AddEntity(const Entity* entity)
			{ _entities.PushBack(entity); }
		void RemoveAllEntities()
			{ _entities.Reset(); }
		int GetVisibleEntities(ModelEntity** entities, int max_entities);
		int GetVisibleShadowcasters(ModelEntity** entities, int max_entities);

	private:
		static const int MAX_NUM_ENTITIES = 16 * 1024;

		void PositionTerrain(Terrain* terrain);

		Camera _camera;
		FreePool<FreeListNode<const Entity*> > _entityPool;
		EntityList _entities;
	};

}


#endif // _ENGINE_WORLD_H_
