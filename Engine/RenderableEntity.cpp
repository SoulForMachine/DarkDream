
#include "RenderableEntity.h"

using namespace math3d;


namespace Engine
{

	RenderableEntity::RenderableEntity()
	{
		_worldMat.set_identity();
		_dropped = true;
	}

	void RenderableEntity::SetObjectBoundingBox(const AABBox& bbox)
	{
		_objectBBox = bbox;
		CalcWorldBBox();
	}

	void RenderableEntity::SetWorldTransform(const mat4f& transform)
	{
		_worldMat = transform;
		SetPosition(_worldMat.transl_vec.rvec3);
		CalcWorldBBox();
	}

	void RenderableEntity::SetPosition(const vec3f& pos)
	{
		Entity::SetPosition(pos);
		_worldMat.transl_vec.rvec3 = pos;
		CalcWorldBBox();
	}

	inline
	void RenderableEntity::CalcWorldBBox()
	{
		_objectBBox.GetOrientedBBox(_worldBBox, _worldMat);
	}

}
