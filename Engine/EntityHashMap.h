
#ifndef _ENGINE_ENTITY_HASH_MAP_H_
#define _ENGINE_ENTITY_HASH_MAP_H_

#include "BaseLib/HashMap.h"
#include "BaseLib/FreePool.h"
#include "Engine/SpacePartList.h"


namespace Engine
{

	class Entity;
	class World;


	class WorldEntity
	{
	public:
		WorldEntity() {}
		operator Entity* ()
			{ return _entity; }
		operator const Entity* () const
			{ return _entity; }
		Entity* operator -> ()
			{ return _entity; }
		const Entity* operator -> () const
			{ return _entity; }

	private:
		Entity* _entity;
		SpacePartitionList::SPLEntity _splEntity;

		friend class World;
	};

	
	class EntityHashMapTraits
	{
	public:
		EntityHashMapTraits(Memory::Allocator& pool, size_t size)
			: _pool(size, pool) {}
		HashMapNode<WorldEntity>* New()
			{ return _pool.New(); }
		void Delete(HashMapNode<WorldEntity>* ptr)
			{ _pool.Delete(ptr); }
		uint GetHash(Entity* ptr) const
			{ return ::GetHash(ptr); }

	private:
		FreePool<HashMapNode<WorldEntity> > _pool;
	};


	class EntityHashMap: public HashMap<Entity*, WorldEntity, EntityHashMapTraits>
	{
	public:
		EntityHashMap(Memory::Allocator& pool, size_t max_size)
			: HashMap<Entity*, WorldEntity, EntityHashMapTraits>(pool, max_size)
		{
			_maxSize = max_size;
		}

		void Insert(WorldEntity& entity)
		{
			auto el = Find(entity);
			if(el == End() && GetCount() < _maxSize)
				(*this)[entity] = entity;
			else
				*el = entity;
		}

	private:
		size_t _maxSize;
	};

}


#endif // _ENGINE_ENTITY_HASH_MAP_H_
