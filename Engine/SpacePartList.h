
#ifndef _ENGINE_SPACE_PART_LIST_H_
#define _ENGINE_SPACE_PART_LIST_H_

#include "BaseLib/FreeList.h"
#include "Engine/Common.h"
#include "Engine/Terrain.h"
#include "Engine/Entity.h"


namespace Engine
{

	class Camera;


	class ENGINE_API SpacePartitionList
	{
	public:
		typedef FreeList<Entity*>::Iterator EntityListIterator;

		struct SPLEntity
		{
			EntityListIterator it;
			int listIndex;
		};

		SpacePartitionList(size_t max_entities);
		~SpacePartitionList();

		bool AddEntity(Entity* entity, SPLEntity& spl_ent);
		void MoveEntity(SPLEntity& spl_ent);
		void RemoveEntity(const SPLEntity& spl_ent);
		void RemoveAll();
		int GetVisibleEntities(const Camera& camera, Entity** entities, int max_entities, const EntityType* types);

	private:
		static const int PARTITION_COUNT = Terrain::MAX_PATCHES * 2;
		static const int PARTITION_SIZE = Terrain::PATCH_WIDTH / 2;
		FreePool<FreeListNode<Entity*> > _nodePool;
		FreeList<Entity*>* _partitions[PARTITION_COUNT];
	};

}


#endif // _ENGINE_SPACE_PART_LIST_H_
