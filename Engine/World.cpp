
#include "Engine/Model.h"
#include "Engine/Terrain.h"
#include "Engine/EngineInternal.h"
#include "ResourceManager.h"
#include "Resource.h"
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
		if(!_layerManager.Init())
		{
			Deinit();
			return false;
		}

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

		_layerManager.Deinit();
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

		_entities.Insert(entity);
		return true;
	}

	bool World::RemoveEntity(Entity* entity)
	{
		_entities.Remove(entity);
		return false;
	}

	void World::RemoveAllEntities()
	{
		_entities.Clear();
	}

	int World::GetVisibleRenderableEntities(RenderableEntity** entities, int max_entities)
	{
		if(!entities || max_entities <= 0)
			return 0;

		int count = 0;
		for(EntityHashMap::Iterator it = _entities.Begin(); it != _entities.End(); ++it)
		{
			if(	(*it)->GetType() == ENTITY_TYPE_MODEL ||
				(*it)->GetType() == ENTITY_TYPE_PARTICLE_SYS )
			{
				RenderableEntity* re = (RenderableEntity*)*it;
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

	int World::GetVisibleModelEntities(ModelEntity** entities, int max_entities)
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
	
	int World::GetVisibleLayerSprites(const BgLayer::Sprite** sprites, int max_sprites)
	{
		if(!sprites || max_sprites <= 0)
			return 0;

		return _layerManager.GetVisibleSprites(sprites, max_sprites);
	}

	int World::GetVisibleParticleSystems(ParticleSystem** part_sys, int max_ps)
	{
		if(!part_sys || max_ps <= 0)
			return 0;

		int count = 0;
		for(EntityHashMap::Iterator it = _entities.Begin(); it != _entities.End(); ++it)
		{
			if((*it)->GetType() == ENTITY_TYPE_PARTICLE_SYS)
			{
				ParticleSystem* ps = (ParticleSystem*)*it;
				if(_camera.IsInsideFrustum(ps->GetWorldBoundingBox()))
				{
					part_sys[count++] = ps;
					if(count == max_ps)
						break;
				}
			}
		}

		return count;
	}

}
