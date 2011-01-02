
#include <cstring>
#include "World.h"
#include "Entity.h"


namespace Engine
{

	Entity& Entity::operator = (const Entity& entity)
	{
		_position = entity._position;
		_world = 0;
		return *this;
	}

	void Entity::SetPosition(const math3d::vec3f& pos)
	{
		_position = pos;
		if(_world)
			_world->EntityMoved(this);
	}

}
