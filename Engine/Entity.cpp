
#include <cstring>
#include "Entity.h"


namespace Engine
{

	const char* Entity::_classNames[ENTITY_CLASS_COUNT] =
	{
		"Generic",
		"NPC",
		"Monster",
		"Boss",
		"Building",
		"Foliage",
		"Debris",
	};

	EntityClass Entity::GetClassFromString(const char* name)
	{
		for(int i = 0; i < ENTITY_CLASS_COUNT; ++i)
		{
			if(!strcmp(name, _classNames[i]))
				return (EntityClass)i;
		}

		return ENTITY_CLASS_COUNT;
	}

	const char* Entity::GetClassString(EntityClass c)
	{
		int i = (int)c;
		if(i >= 0 && i < ENTITY_CLASS_COUNT)
			return _classNames[i];
		else
			return "";
	}

}
