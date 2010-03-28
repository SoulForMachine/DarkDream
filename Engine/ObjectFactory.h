
#ifndef _ENGINE_OBJECT_FACTORY_H_
#define _ENGINE_OBJECT_FACTORY_H_

#include "Engine/Common.h"


namespace Engine
{

	class Entity;
	class ModelEntity;
	class StaticEntity;
	class PlayerEntity;
	class AIEntity;
	class WeaponEntity;
	class ItemEntity;
	class ParticleSystem;
	class Material;
	class Font;
	class World;


	class ENGINE_API ObjectFactory
	{
	public:
		static ModelEntity* NewModelEntity(ModelEntity::ModelEntityType type);
		static StaticEntity* NewStaticEntity();
		static PlayerEntity* NewPlayerEntity();
		static AIEntity* NewAIEntity();
		static WeaponEntity* NewWeaponEntity();
		static ItemEntity* NewItemEntity();
		static ParticleSystem* NewParticleSystem();
		static Material* NewMaterial();
		static Font* NewFont();
		static World* NewWorld();

		static void DeleteEntity(Entity* entity);
		static void DeleteMaterial(Material* material);
		static void DeleteFont(Font* font);
		static void DeleteWorld(World* world);
	};

}


#endif // _ENGINE_OBJECT_FACTORY_H_
