
#ifndef _ENGINE_ENTITY_HASH_MAP_H_
#define _ENGINE_ENTITY_HASH_MAP_H_

#include "BaseLib/HashMap.h"
#include "BaseLib/FreePool.h"


namespace Engine
{

	class Entity;

	
	class EntityHashMapTraits
	{
	public:
		EntityHashMapTraits(Memory::Allocator& pool, size_t size)
			: _pool(size, pool) {}
		HashMapNode<Entity*>* New()
			{ return _pool.New(); }
		void Delete(HashMapNode<Entity*>* ptr)
			{ _pool.Delete(ptr); }
		uint GetHash(Entity* ptr) const
			{ return ::GetHash(ptr); }

	private:
		FreePool<HashMapNode<Entity*> > _pool;
	};


	class EntityHashMap: public HashMap<Entity*, Entity*, EntityHashMapTraits>
	{
	public:
		EntityHashMap(Memory::Allocator& pool, size_t max_size)
			: HashMap<Entity*, Entity*, EntityHashMapTraits>(pool, max_size) {}
	};

}


#endif // _ENGINE_ENTITY_HASH_MAP_H_
