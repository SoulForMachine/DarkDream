
#ifndef _ENGINE_ENTITY_H_
#define _ENGINE_ENTITY_H_

#include "BaseLib/Math/math3d.h"
#include "Engine/Common.h"


namespace Engine
{

	enum EntityClass
	{
		ENTITY_CLASS_GENERIC,
		ENTITY_CLASS_NPC,
		ENTITY_CLASS_MONSTER,
		ENTITY_CLASS_BOSS,
		ENTITY_CLASS_BUILDING,
		ENTITY_CLASS_FOLIAGE,
		ENTITY_CLASS_DEBRIS,

		ENTITY_CLASS_COUNT,
	};

	enum EntityType
	{
		ENTITY_TYPE_MODEL,
		ENTITY_TYPE_TERRAIN,
		ENTITY_TYPE_GRID,
		ENTITY_TYPE_PARTICLE_SYS,
		ENTITY_TYPE_LIGHT,
		ENTITY_TYPE_CAMERA,
		ENTITY_TYPE_TRIGGER,
		ENTITY_TYPE_SPAWN_POINT,
		ENTITY_TYPE_WATER_PATCH,
		ENTITY_TYPE_RIVER,
		ENTITY_TYPE_ROAD,
	};


	class ENGINE_API Entity
	{
	public:
		virtual ~Entity() = 0 {}

		virtual void SetPosition(const math3d::vec3f& pos)
			{ _position = pos; }
		virtual const math3d::vec3f& GetPosition() const
			{ return _position; }
		virtual EntityType GetType() const = 0;

	protected:
		EntityClass GetClassFromString(const char* name);
		const char* GetClassString(EntityClass c);

		math3d::vec3f _position;
	private:

		static const char* _classNames[ENTITY_CLASS_COUNT];
	};

}


#endif // _ENGINE_ENTITY_H_
