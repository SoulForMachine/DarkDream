
#ifndef _ENGINE_RENDERABLE_ENTITY_
#define _ENGINE_RENDERABLE_ENTITY_

#include "BaseLib/Bounds.h"
#include "BaseLib/Math/math3d.h"
#include "Engine/Entity.h"


namespace Engine
{

	class ENGINE_API RenderableEntity: public Entity
	{
	public:
		RenderableEntity();

		const OBBox& GetWorldBoundingBox() const
			{ return _worldBBox; }
		void SetObjectBoundingBox(const AABBox& bbox);
		const AABBox& GetObjectBoundingBox() const
			{ return _objectBBox; }
		const math3d::mat4f& GetWorldTransform() const
			{ return _worldMat; }
		void SetWorldTransform(const math3d::mat4f& transform);
		virtual void SetPosition(const math3d::vec3f& pos);
		virtual void UpdateGraphics() = 0;

		bool IsDropped() const
			{ return _dropped; }
		void SetDropped(bool dropped)
			{ _dropped = dropped; }

	private:
		void CalcWorldBBox();

		math3d::mat4f _worldMat;
		AABBox _objectBBox; // entity's object space bounding box
		OBBox _worldBBox; // entity's world space bounding box
		bool _dropped; // true if object is on terrain
	};

}

#endif // _ENGINE_RENDERABLE_ENTITY_
