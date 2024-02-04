
#include "BaseLib/Memory.h"
#include "BaseLib/Math/math3d.h"
#include "Entity.h"
#include "RenderableEntity.h"
#include "Camera.h"
#include "SpacePartList.h"

using namespace Memory;
using namespace math3d;

namespace Engine
{

	SpacePartitionList::SpacePartitionList(size_t max_entities)
		: _nodePool(max_entities, mapPool)
	{
		for(int i = 0; i < PARTITION_COUNT; ++i)
			_partitions[i] = New<FreeList<Entity*>>(mapPool, _nodePool);
	}

	SpacePartitionList::~SpacePartitionList()
	{
		for(int i = 0; i < PARTITION_COUNT; ++i)
			Delete(_partitions[i]);
	}

	bool SpacePartitionList::AddEntity(Entity* entity, SPLEntity& spl_ent)
	{
		if(_nodePool.GetFreeCount() == 0)
			return false;

		int list_ind = int(entity->GetPosition().x / (Terrain::PATCH_WIDTH / 2));
		assert(list_ind >= 0 && list_ind < PARTITION_COUNT);
		_partitions[list_ind]->PushBack(entity);
		spl_ent.it = _partitions[list_ind]->End();
		--spl_ent.it;
		spl_ent.listIndex = list_ind;

		return true;
	}

	void SpacePartitionList::MoveEntity(SPLEntity& spl_ent)
	{
		Entity* entity = *spl_ent.it;
		int list_ind = int(entity->GetPosition().x / (Terrain::PATCH_WIDTH / 2));
		if(list_ind != spl_ent.listIndex)
		{
			_partitions[spl_ent.listIndex]->Remove(spl_ent.it);
			_partitions[list_ind]->PushBack(entity);
			spl_ent.it = _partitions[list_ind]->End();
			--spl_ent.it;
			spl_ent.listIndex = list_ind;
		}
	}

	void SpacePartitionList::RemoveEntity(const SPLEntity& spl_ent)
	{
		_partitions[spl_ent.listIndex]->Remove(spl_ent.it);
	}

	void SpacePartitionList::RemoveAll()
	{
		for(int i = 0; i < PARTITION_COUNT; ++i)
			_partitions[i]->Reset();
	}

	int SpacePartitionList::GetVisibleEntities(const Camera& camera, Entity** entities, int max_entities, const EntityType* types)
	{
		const vec4f& left_cp = camera.GetClipPlane(Camera::CP_LEFT);
		const vec4f& right_cp = camera.GetClipPlane(Camera::CP_RIGHT);
		vec3f pt_left, pt_right;
		int count = 0;

		if(intersect_line_plane(pt_left, vec3f::null, vec3f::x_axis, left_cp))
		{
			if(intersect_line_plane(pt_right, vec3f::null, vec3f::x_axis, right_cp))
			{
				int ind_left = Max(int(pt_left.x) / PARTITION_SIZE, 0);
				int ind_right = Min(int(pt_right.x) / PARTITION_SIZE, PARTITION_COUNT - 1);

				for(int i = ind_left; i <= ind_right; ++i)
				{
					for(auto it = _partitions[i]->Begin(); it != _partitions[i]->End(); ++it)
					{
						const EntityType* type = types;
						while(*type != ENTITY_TYPE_UNDEFINED)
						{
							if(*type == (*it)->GetType())
							{
								if(count < max_entities)
								{
									if(	*type == ENTITY_TYPE_MODEL ||
										*type == ENTITY_TYPE_PARTICLE_SYS )
									{
										RenderableEntity* entity = (RenderableEntity*)*it;
										if(camera.IsInsideFrustum(entity->GetWorldBoundingBox()))
											entities[count++] = *it;
									}
									else
									{
										if(camera.IsInsideFrustum((*it)->GetPosition()))
											entities[count++] = *it;
									}
								}
								else
								{
									return count;
								}
							}

							type++;
						}
					}
				}
			}
		}

		return count;
	}

}
