
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

}
