
#include "Engine/Model.h"
#include "Engine/Terrain.h"
#include "Engine/EngineInternal.h"
#include "ResourceManager.h"
#include "FileResource.h"
#include "World.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	World::World():
		_entities(mapPool, MAX_NUM_ENTITIES)
	{
	}

	bool World::Init()
	{
		if(!_terrain.Init())
			return false;

		return true;
	}

	void World::Deinit()
	{
		_terrain.Deinit();

		for(EntityHashMap::Iterator it = _entities.Begin(); it != _entities.End(); ++it)
		{
			delete *it;
		}
		_entities.Clear();
	}

	bool World::LoadMap(const tchar* file_name)
	{
		return true;
	}

	bool World::SaveMap(const tchar* file_name)
	{
		return true;
	}

	bool World::AddEntity(Entity* entity)
	{
		if(_entities.GetCount() == MAX_NUM_ENTITIES)
			return false;

		_entities[entity] = entity;
		return true;
	}

	bool World::RemoveEntity(Entity* entity)
	{
		for(EntityHashMap::Iterator it = _entities.Begin(); it != _entities.End(); ++it)
		{
			if(*it == entity)
			{
				_entities.Remove(it);
				return true;
			}
		}
		return false;
	}

	void World::RemoveAllEntities()
	{
		_entities.Clear();
	}

	int World::GetVisibleEntities(ModelEntity** entities, int max_entities)
	{
		if(!entities || max_entities <= 0)
			return 0;

		int count = 0;
		for(EntityHashMap::Iterator it = _entities.Begin(); it != _entities.End(); ++it)
		{
			if((*it)->GetType() == ENTITY_TYPE_MODEL)
			{
				ModelEntity* re = (ModelEntity*)*it;
				if(_camera.IsInsideFrustum(re->GetWorldBoundingBox()))
				{
					entities[count++] = re;
					if(count == max_entities)
						break;
				}
			}
		}

		return count;
	}

	int World::GetVisibleTerrainPatches(const Terrain::TerrainPatch** patches, int max_patches)
	{
		if(!patches || max_patches <= 0)
			return 0;

		int count = 0;
		const Terrain::TerrainPatch* patch_array = _terrain.GetPatches();
		for(int i = 0; i < _terrain.GetPatchCount(); ++i)
		{
			if(_camera.IsInsideFrustum(patch_array[i].boundBox))
			{
				patches[count++] = &patch_array[i];
				if(count == max_patches)
					break;
			}
		}

		return count;
	}

}
