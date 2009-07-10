
#ifndef _ENGINE_PARTICLE_SYSTEM_H_
#define _ENGINE_PARTICLE_SYSTEM_H_

#include "Engine/Common.h"
#include "Engine/RenderableEntity.h"


namespace Engine
{

	class ENGINE_API ParticleSystem: public RenderableEntity
	{
	public:
		virtual EntityType GetType() const
			{ return ENTITY_TYPE_PARTICLE_SYS; }
	};

}


#endif // _ENGINE_PARTICLE_SYSTEM_H_
