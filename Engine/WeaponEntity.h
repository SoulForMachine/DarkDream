
#ifndef _ENGINE_WEAPON_ENTITY_H_
#define _ENGINE_WEAPON_ENTITY_H_

#include "Engine/Common.h"
#include "Engine/ModelEntity.h"


namespace FileUtil
{
	class File;
}

class Parser;


namespace Engine
{

	class ENGINE_API WeaponEntity: public ModelEntity
	{
	public:
		enum WeaponType
		{
			WPN_TYPE_GUN,
			WPN_TYPE_ROCKET,
			WPN_TYPE_GRANADE,
			WPN_TYPE_FIRE,
			WPN_TYPE_ELECTRIC,
			WPN_TYPE_MELEE,

			WPN_TYPE_COUNT
		};

		enum DistanceAttenuation
		{
			DIST_ATTEN_NONE,
			DIST_ATTEN_LINEAR,
			DIST_ATTEN_QUADRATIC,

			DIST_ATTEN_COUNT
		};

		WeaponEntity* CreateCopy() const;
		void Unload();
		ModelEntityType GetModelEntityType() const
			{ return ME_TYPE_WEAPON; }

	protected:
		void ReadProperties(Parser& parser);
		void WriteProperties(FileUtil::File& file, const char* indent);

	private:
		static DistanceAttenuation GetDistAttenFromName(const char* name);
		static const char* GetDistAttenName(DistanceAttenuation atten);
		static WeaponType GetWeaponTypeFromName(const char* name);
		static const char* GetWeaponTypeName(WeaponType type);

		int _maxAmmo;
		int _damage;
		float _range;
		DistanceAttenuation _distAtten;
		WeaponType _weaponType;
		PartSysResPtr _muzzleEffect;

		static const char* _weaponTypeNames[];
		static const char* _distAttenNames[];
	};

}

#endif //_ENGINE_WEAPON_ENTITY_H_
