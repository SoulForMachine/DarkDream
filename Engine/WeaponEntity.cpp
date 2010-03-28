
#include "BaseLib/FileUtil.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Memory.h"
#include "EngineInternal.h"
#include "Resource.h"
#include "WeaponEntity.h"

using namespace Memory;


namespace Engine
{

	const char* WeaponEntity::_weaponTypeNames[] =
	{
		"Gun",
		"Rocket",
		"Granade",
		"Fire",
		"Electric",
		"Melee",
	};

	const char* WeaponEntity::_distAttenNames[] =
	{
		"None",
		"Linear",
		"Quadratic",
	};


	WeaponEntity* WeaponEntity::CreateCopy() const
	{
		return new(mapPool) WeaponEntity(*this);
	}

	void WeaponEntity::Unload()
	{
		ModelEntity::Unload();

		if(_muzzleEffect)
		{
			engineAPI.partSysManager->ReleasePartSys(_muzzleEffect);
			_muzzleEffect = PartSysResPtr::null;
		}
	}

	void WeaponEntity::ReadProperties(Parser& parser)
	{
		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];
		char path[MAX_PATH];

		parser.ExpectTokenString("maxAmmo");
		parser.ReadInt(_maxAmmo);

		parser.ExpectTokenString("damage");
		parser.ReadInt(_damage);

		parser.ExpectTokenString("range");
		parser.ReadFloat(_range);

		parser.ExpectTokenString("distAttenuation");
		parser.ReadIdentifier(buf, MAX_IDENT_LEN);
		_distAtten = GetDistAttenFromName(buf);
		if(_distAtten == DIST_ATTEN_COUNT)
			throw ParserException("Invalid distance attenuation");

		parser.ExpectTokenString("weaponType");
		parser.ReadIdentifier(buf, MAX_IDENT_LEN);
		_weaponType = GetWeaponTypeFromName(buf);
		if(_weaponType == WPN_TYPE_COUNT)
			throw ParserException("Invalid weapon type");

		parser.ExpectTokenString("muzzleEffect");
		parser.ReadString(path, MAX_PATH);
		if(*path)
		{
			tchar* p = CharToWideChar(path);
			_muzzleEffect = engineAPI.partSysManager->CreateParticleSystem(p);
			delete[] p;
		}
		else
		{
			_muzzleEffect = PartSysResPtr::null;
		}
	}

	void WeaponEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
		file.Printf("%smaxAmmo\t\t%d\n", indent, _maxAmmo);
		file.Printf("%sdamage\t\t%d\n", indent, _damage);
		file.Printf("%srange\t\t%f\n", indent, _range);
		file.Printf("%sdistAttenuation\t\t%s\n", indent, GetDistAttenName(_distAtten));
		file.Printf("%sweaponType\t\t%s\n", indent, GetWeaponTypeName(_weaponType));
		file.Printf("%smuzzleEffect\t\t\"%ls\"\n", indent, _muzzleEffect.GetFileRes()->GetFileName());
	}

	WeaponEntity::DistanceAttenuation
		WeaponEntity::GetDistAttenFromName(const char* name)
	{
		for(int i = 0; i < DIST_ATTEN_COUNT; ++i)
		{
			if(!strcmp(name, _distAttenNames[i]))
				return (DistanceAttenuation)i;
		}

		return DIST_ATTEN_COUNT;
	}

	const char* WeaponEntity::GetDistAttenName(DistanceAttenuation atten)
	{
		return (atten == DIST_ATTEN_COUNT)? "": _distAttenNames[atten];
	}

	WeaponEntity::WeaponType
		WeaponEntity::GetWeaponTypeFromName(const char* name)
	{
		for(int i = 0; i < WPN_TYPE_COUNT; ++i)
		{
			if(!strcmp(name, _weaponTypeNames[i]))
				return (WeaponType)i;
		}

		return WPN_TYPE_COUNT;
	}

	const char* WeaponEntity::GetWeaponTypeName(WeaponType type)
	{
		return (type == WPN_TYPE_COUNT)? "": _weaponTypeNames[type];
	}

}
