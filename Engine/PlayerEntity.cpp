
#include "BaseLib/FileUtil.h"
#include "BaseLib/Parser.h"
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

	PlayerEntity::PlayerEntity(const PlayerEntity& rhs)
	{
		*this = rhs;
	}

	PlayerEntity::~PlayerEntity()
	{
		Unload();
	}

	PlayerEntity& PlayerEntity::operator = (const PlayerEntity& rhs)
	{
		if(&rhs != this)
		{
			ModelEntity::operator = (rhs);

			_health = rhs._health;
			_maxHealth = rhs._maxHealth;
			_armor = rhs._armor;
			_maxArmor = rhs._maxArmor;
		}

		return *this;
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
		parser.ExpectTokenString("health");
		parser.ReadInt(_health);
		parser.ExpectTokenString("maxHealth");
		parser.ReadInt(_maxHealth);
		parser.ExpectTokenString("armor");
		parser.ReadInt(_armor);
		parser.ExpectTokenString("maxArmor");
		parser.ReadInt(_maxArmor);
	}

	void PlayerEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
		file.Printf("%shealth\t\t%d\n", indent, _health);
		file.Printf("%smaxHealth\t\t%d\n", indent, _maxHealth);
		file.Printf("%sarmor\t\t%d\n", indent, _armor);
		file.Printf("%smaxArmor\t\t%d\n", indent, _maxArmor);
	}

}
