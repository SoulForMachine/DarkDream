
#include <cstring>
#include "Entity.h"


namespace Engine
{

	Entity& Entity::operator = (const Entity& entity)
	{
		_position = entity._position;
		return *this;
	}

}
