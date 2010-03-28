
#ifndef _ENGINE_PLAYER_ENTITY_H_
#define _ENGINE_PLAYER_ENTITY_H_

#include "Engine/Common.h"
#include "Engine/ModelEntity.h"


namespace FileUtil
{
	class File;
}

class Parser;


namespace Engine
{

	class ENGINE_API PlayerEntity: public ModelEntity
	{
	public:
		PlayerEntity();

		PlayerEntity* CreateCopy() const;
		void Unload();
		ModelEntityType GetModelEntityType() const
			{ return ME_TYPE_PLAYER; }

		int GetHealth() const
			{ return _health; }
		void SetHealth(int health)
			{ _health = Min(health, _maxHealth); }
		void AddHealth(int pts)
		{
			_health += pts;
			if(_health < 0)
				_health = 0;
			else if(_health > _maxHealth)
				_health = _maxHealth;
		}

		int GetMaxHealth() const
			{ return _maxHealth; }
		void SetMaxHealth(int health)
			{ _maxHealth = health; }

		int GetArmor() const
			{ return _armor; }
		void SetArmor(int armor)
			{ _armor = Min(armor, _maxArmor); }
		void AddArmor(int pts)
		{
			_armor += pts;
			if(_armor < 0)
				_armor = 0;
			else if(_armor > _maxArmor)
				_armor = _maxArmor;
		}

		int GetMaxArmor() const
			{ return _maxArmor; }
		void SetMaxArmor(int armor)
			{ _maxArmor = armor; }

	protected:
		void ReadProperties(Parser& parser);
		void WriteProperties(FileUtil::File& file, const char* indent);

	private:
		int _health;
		int _maxHealth;
		int _armor;
		int _maxArmor;
	};

}

#endif //_ENGINE_PLAYER_ENTITY_H_
