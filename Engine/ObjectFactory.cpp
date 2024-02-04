
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
			return New<StaticEntity>(mapPool);
		case ModelEntity::ME_TYPE_PLAYER:
			return New<PlayerEntity>(mapPool);
		case ModelEntity::ME_TYPE_AI:
			return New<AIEntity>(mapPool);
		case ModelEntity::ME_TYPE_WEAPON:
			return New<WeaponEntity>(mapPool);
		case ModelEntity::ME_TYPE_ITEM:
			return New<ItemEntity>(mapPool);
		default:
			assert(0);
		}

		return 0;
	}

	StaticEntity* ObjectFactory::NewStaticEntity()
	{
		return New<StaticEntity>(mapPool);
	}

	PlayerEntity* ObjectFactory::NewPlayerEntity()
	{
		return New<PlayerEntity>(mapPool);
	}

	AIEntity* ObjectFactory::NewAIEntity()
	{
		return New<AIEntity>(mapPool);
	}

	WeaponEntity* ObjectFactory::NewWeaponEntity()
	{
		return New<WeaponEntity>(mapPool);
	}

	ItemEntity* ObjectFactory::NewItemEntity()
	{
		return New<ItemEntity>(mapPool);
	}

	ParticleSystem* ObjectFactory::NewParticleSystem()
	{
		return New<ParticleSystem>(mapPool);
	}

	Material* ObjectFactory::NewMaterial()
	{
		return New<Material>(mapPool);
	}

	Font* ObjectFactory::NewFont()
	{
		return New<Font>(mainPool);
	}

	World* ObjectFactory::NewWorld()
	{
		return New<World>(mapPool);
	}

	void ObjectFactory::DeleteEntity(Entity* entity)
	{
		Delete(entity);
	}

	void ObjectFactory::DeleteMaterial(Material* material)
	{
		Delete(material);
	}

	void ObjectFactory::DeleteFont(Font* font)
	{
		Delete(font);
	}

	void ObjectFactory::DeleteWorld(World* world)
	{
		Delete(world);
	}
	
}
