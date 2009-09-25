
#ifndef _ENGINE_ENTITY_H_
#define _ENGINE_ENTITY_H_

#include "BaseLib/Math/math3d.h"
#include "Engine/Common.h"


namespace Engine
{

	enum EntityType
	{
		ENTITY_TYPE_MODEL,
		ENTITY_TYPE_PARTICLE_SYS,
		ENTITY_TYPE_CAMERA,
		ENTITY_TYPE_TRIGGER,
		ENTITY_TYPE_SPAWN_POINT,
	};


	class ENGINE_API Entity
	{
	public:
		Entity()
			{ _position.set_null(); }
		virtual ~Entity() = 0 {}

		Entity& operator = (const Entity& entity);

		virtual void SetPosition(const math3d::vec3f& pos)
			{ _position = pos; }
		virtual const math3d::vec3f& GetPosition() const
			{ return _position; }
		virtual EntityType GetType() const = 0;

	protected:
		math3d::vec3f _position;
	};

}


#endif // _ENGINE_ENTITY_H_
