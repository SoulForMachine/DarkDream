
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
		_entities(mapPool, MAX_NUM_ENTITIES),
		_spl(MAX_NUM_ENTITIES)
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

		for(auto it = _entities.Begin(); it != _entities.End(); ++it)
		{
			Memory::Delete(&(*it));
		}
		_entities.Clear();
		_spl.RemoveAll();

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

		WorldEntity we;
		we._entity = entity;
		if(_spl.AddEntity(entity, we._splEntity))
		{
			entity->SetWorld(this);
			_entities.Insert(we);
			return true;
		}
		else
			return false;
	}

	void World::RemoveEntity(Entity* entity)
	{
		auto it = _entities.Find(entity);
		if(it != _entities.End())
		{
			_spl.RemoveEntity(it->_splEntity);
			_entities.Remove(entity);
			entity->SetWorld(0);
		}
	}

	void World::RemoveAllEntities()
	{
		for(auto it = _entities.Begin(); it != _entities.End(); ++it)
			(*it)->SetWorld(0);

		_spl.RemoveAll();
		_entities.Clear();
	}

	void World::EntityMoved(Entity* entity)
	{
		auto it = _entities.Find(entity);
		if(it != _entities.End())
		{
			_spl.MoveEntity(it->_splEntity);
		}
	}

	int World::GetVisibleRenderableEntities(RenderableEntity** entities, int max_entities)
	{
		assert(entities && max_entities > 0);

		EntityType types[] = { ENTITY_TYPE_MODEL, ENTITY_TYPE_PARTICLE_SYS, ENTITY_TYPE_UNDEFINED };
		return _spl.GetVisibleEntities(_camera, (Entity**)entities, max_entities, types);
	}

	int World::GetVisibleModelEntities(ModelEntity** entities, int max_entities)
	{
		assert(entities && max_entities > 0);

		EntityType types[] = { ENTITY_TYPE_MODEL, ENTITY_TYPE_UNDEFINED };
		return _spl.GetVisibleEntities(_camera, (Entity**)entities, max_entities, types);
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
		assert(part_sys && max_ps > 0);

		EntityType types[] = { ENTITY_TYPE_PARTICLE_SYS, ENTITY_TYPE_UNDEFINED };
		return _spl.GetVisibleEntities(_camera, (Entity**)part_sys, max_ps, types);
	}

}
