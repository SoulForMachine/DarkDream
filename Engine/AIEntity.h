
#ifndef _ENGINE_AI_ENTITY_H_
#define _ENGINE_AI_ENTITY_H_

#include "Engine/Common.h"
#include "Engine/ModelEntity.h"

namespace FileUtil
{
	class File;
}

class Parser;



namespace Engine
{

	class ENGINE_API AIEntity: public ModelEntity
	{
	public:
		enum AIType
		{
			AI_TYPE_MONSTER,
			AI_TYPE_BOSS,
			AI_TYPE_NPC,

			AI_TYPE_COUNT
		};

		AIEntity();
		AIEntity(const AIEntity& rhs);
		~AIEntity();
		AIEntity& operator = (const AIEntity& rhs);

		AIEntity* CreateCopy() const;
		void Unload();

		ModelEntityType GetModelEntityType() const
			{ return ME_TYPE_AI; }

		AIType GetAIType() const
			{ return _aiType; }
		void SetAIType(AIType type)
			{ _aiType = type; }
		
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
		
		AIScriptResPtr GetAIScriptRes() const
			{ return _aiScript; }
		bool SetAIScript(const tchar* file_name);

	protected:
		void ReadProperties(Parser& parser);
		void WriteProperties(FileUtil::File& file, const char* indent);

	private:
		static AIType GetAITypeFromName(const char* name);
		static const char* GetAITypeName(AIType type);

		AIType _aiType;
		int _health;
		int _maxHealth;
		int _armor;
		int _maxArmor;
		AIScriptResPtr _aiScript;

		static const char* _aiTypeNames[];
	};

}


#endif // _ENGINE_AI_ENTITY_H_
