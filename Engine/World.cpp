
#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Terrain.h"
#include "Engine/EngineInternal.h"
#include "ResourceManager.h"
#include "FileResource.h"
#include "World.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	World::World():
		_entityPool(MAX_NUM_ENTITIES, mapPool),
		_entities(_entityPool)
	{
	}

	bool World::Init()
	{
		return true;
	}

	void World::Deinit()
	{
	}

	int World::GetVisibleEntities(ModelEntity** entities, int max_entities)
	{
		if(!entities || max_entities <= 0)
			return 0;

		int count = 0;
		for(EntityList::Iterator it = _entities.Begin(); it != _entities.End(); ++it)
		{
			if((*it)->GetType() == ENTITY_TYPE_MODEL)
			{
				ModelEntity* re = (ModelEntity*)*it;
				if(_camera.IsInsideFrustum(re->GetWorldBoundingBox()) != Camera::CLIP_OUTSIDE)
				{
					entities[count++] = re;
					if(count == max_entities)
						break;
				}
			}
		}

		return count;
	}

	int World::GetVisibleShadowcasters(ModelEntity** entities, int max_entities)
	{
		return 0;
	}

	int World::GetVisibleLights(Light** lights, int max_lights)
	{
		return 0;
	}

	Terrain* World::AddTerrain(int width, int height)
	{
		Terrain* terrain = new(mapPool) Terrain;
		if(!terrain->Create(width, height))
		{
			delete terrain;
			return 0;
		}

		_terrainPatches.PushBack(terrain);
		PositionTerrain(terrain);
		return terrain;
	}

	void World::AddTerrain(Terrain* terrain)
	{
		assert(terrain);
		_terrainPatches.PushBack(terrain);
	}

	/*
		Place the terrain patch so that it doesn't overlap any other patch
	*/
	void World::PositionTerrain(Terrain* terrain)
	{
		float max_x = 0.0f;
		float min_z = 0.0f;
		for(TerrainPatchList::ConstIterator it = _terrainPatches.Begin(); it != _terrainPatches.End(); ++it)
		{
			float x = (*it)->GetPosition().x + (*it)->GetWidth();
			float z = (*it)->GetPosition().z;
			if(x > max_x)
				max_x = x;
			if(z < min_z)
				min_z = z;
		}
		terrain->SetPosition(vec3f(max_x, 0.0f, min_z));
	}

}
