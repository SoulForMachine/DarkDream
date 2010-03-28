
#include "BaseLib/FileUtil.h"
#include "BaseLib/Memory.h"
#include "PlayerEntity.h"

using namespace Memory;


namespace Engine
{

	PlayerEntity::PlayerEntity()
	{
		_health = 100;
		_maxHealth = 100;
		_armor = 100;
		_maxArmor = 100;
	}

	PlayerEntity* PlayerEntity::CreateCopy() const
	{
		return new(mapPool) PlayerEntity(*this);
	}

	void PlayerEntity::Unload()
	{
		ModelEntity::Unload();
	}

	void PlayerEntity::ReadProperties(Parser& parser)
	{
	}

	void PlayerEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
	}

}
