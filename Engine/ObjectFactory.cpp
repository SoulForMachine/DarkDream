
#include "BaseLib/Memory.h"
#include "StaticEntity.h"
#include "PlayerEntity.h"
#include "AIEntity.h"
#include "WeaponEntity.h"
#include "ItemEntity.h"
#include "ParticleSystem.h"
#include "Material.h"
#include "Font.h"
#include "World.h"
#include "ObjectFactory.h"

using namespace Memory;


namespace Engine
{

	ModelEntity* ObjectFactory::NewModelEntity(ModelEntity::ModelEntityType type)
	{
		switch(type)
		{
		case ModelEntity::ME_TYPE_STATIC:
			return new(mapPool) StaticEntity;
		case ModelEntity::ME_TYPE_PLAYER:
			return new(mapPool) PlayerEntity;
		case ModelEntity::ME_TYPE_AI:
			return new(mapPool) AIEntity;
		case ModelEntity::ME_TYPE_WEAPON:
			return new(mapPool) WeaponEntity;
		case ModelEntity::ME_TYPE_ITEM:
			return new(mapPool) ItemEntity;
		default:
			assert(0);
		}

		return 0;
	}

	StaticEntity* ObjectFactory::NewStaticEntity()
	{
		return new(mapPool) StaticEntity;
	}

	PlayerEntity* ObjectFactory::NewPlayerEntity()
	{
		return new(mapPool) PlayerEntity;
	}

	AIEntity* ObjectFactory::NewAIEntity()
	{
		return new(mapPool) AIEntity;
	}

	WeaponEntity* ObjectFactory::NewWeaponEntity()
	{
		return new(mapPool) WeaponEntity;
	}

	ItemEntity* ObjectFactory::NewItemEntity()
	{
		return new(mapPool) ItemEntity;
	}

	ParticleSystem* ObjectFactory::NewParticleSystem()
	{
		return new(mapPool) ParticleSystem;
	}

	Material* ObjectFactory::NewMaterial()
	{
		return new(mapPool) Material;
	}

	Font* ObjectFactory::NewFont()
	{
		return new(mainPool) Font;
	}

	World* ObjectFactory::NewWorld()
	{
		return new(mapPool) World;
	}

	void ObjectFactory::DeleteEntity(Entity* entity)
	{
		delete entity;
	}

	void ObjectFactory::DeleteMaterial(Material* material)
	{
		delete material;
	}

	void ObjectFactory::DeleteFont(Font* font)
	{
		delete font;
	}

	void ObjectFactory::DeleteWorld(World* world)
	{
		delete world;
	}
	
}
