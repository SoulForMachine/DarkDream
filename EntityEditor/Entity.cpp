
#include "StdAfx.h"
#include "EditorCommon/UtilityTempl.h"
#include "Entity.h"

using namespace EditorCommon;
using namespace Memory;
using namespace Engine;


namespace EntityEditor
{

	UIFilePathEditor::UIFilePathEditor(String^ title, String^ dir, String^ filter)
	{
		_title = title;
		_dir = dir;
		_filter = filter;
	}

	UITypeEditorEditStyle UIFilePathEditor::GetEditStyle(ITypeDescriptorContext^ context)
	{
		return UITypeEditorEditStyle::Modal;
	}

	Object^ UIFilePathEditor::EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, Object^ value)
	{
		OpenFileDialog^ file_dlg = gcnew OpenFileDialog;
		file_dlg->Title = _title;
		file_dlg->InitialDirectory = gcnew String(engineAPI->fileSystem->GetBaseDirPath()) + _dir;
		file_dlg->Filter = _filter;
		file_dlg->RestoreDirectory = true;

		if(file_dlg->ShowDialog() == DialogResult::OK)
		{
			tchar* file_name = EditorUtil::GetRelativePath(file_dlg->FileName);
			String^ result = gcnew String(file_name);
			Memory::Delete(file_name);
			return result;
		}
		else
		{
			return value;
		}
	}

	// ===========================================================================

	void EntityProperties::SetProperties(const Engine::ModelEntity& entity)
	{
		Name = gcnew String(entity.GetName());
		Class = (EntityClass)entity.GetModelEntityType();
	}

	void EntityProperties::GetProperties(Engine::ModelEntity& entity)
	{
		char* name = ConvertString<char>(Name);
		entity.SetName(name);
		Memory::Delete(name);
	}

	void StaticEntityProperties::SetProperties(const Engine::ModelEntity& entity)
	{
		EntityProperties::SetProperties(entity);

		StaticEntity& stat = (StaticEntity&)entity;
		Clip = stat.GetClip();
	}

	void StaticEntityProperties::GetProperties(Engine::ModelEntity& entity)
	{
		EntityProperties::GetProperties(entity);

		StaticEntity& stat = (StaticEntity&)entity;
		stat.SetClip(Clip);
	}

	void PlayerEntityProperties::SetProperties(const Engine::ModelEntity& entity)
	{
		EntityProperties::SetProperties(entity);

		PlayerEntity& player = (PlayerEntity&)entity;
		Health = player.GetHealth();
		MaxHealth = player.GetMaxHealth();
		Armor = player.GetArmor();
		MaxArmor = player.GetMaxArmor();
	}

	void PlayerEntityProperties::GetProperties(Engine::ModelEntity& entity)
	{
		EntityProperties::GetProperties(entity);

		PlayerEntity& player = (PlayerEntity&)entity;
		player.SetHealth(Health);
		player.SetMaxHealth(MaxHealth);
		player.SetArmor(Armor);
		player.SetMaxArmor(MaxArmor);
	}

	void AIEntityProperties::SetProperties(const Engine::ModelEntity& entity)
	{
		EntityProperties::SetProperties(entity);

		AIEntity& ai = (AIEntity&)entity;
		AIType = (AITypeEnum)ai.GetAIType();
		Health = ai.GetHealth();
		MaxHealth = ai.GetMaxHealth();
		Armor = ai.GetArmor();
		MaxArmor = ai.GetMaxArmor();

		AIScriptResPtr script = ai.GetAIScriptRes();
		if(script)
			AIScript = gcnew String(script.GetRes()->GetFileName());
		else
			AIScript = "";
	}

	void AIEntityProperties::GetProperties(Engine::ModelEntity& entity)
	{
		EntityProperties::GetProperties(entity);

		AIEntity& ai = (AIEntity&)entity;
		ai.SetAIType((AIEntity::AIType)AIType);
		ai.SetHealth(Health);
		ai.SetMaxHealth(MaxHealth);
		ai.SetArmor(Armor);
		ai.SetMaxArmor(MaxArmor);

		tchar* path = ConvertString<tchar>(AIScript);
		ai.SetAIScript(path);
		Memory::Delete(path);
	}

	void WeaponEntityProperties::SetProperties(const Engine::ModelEntity& entity)
	{
		EntityProperties::SetProperties(entity);

		WeaponEntity& weapon = (WeaponEntity&)entity;
		MaxAmmo = weapon.GetMaxAmmo();
		Damage = weapon.GetDamage();
		Range = weapon.GetRange();
		EffectiveAngle = weapon.GetEffectiveAngle();
		DistanceAttenuation = (DistanceAttenuationEnum)weapon.GetDistanceAttenuation();
		WeaponType = (WeaponTypeEnum)weapon.GetWeaponType();

		PartSysResPtr part_sys = weapon.GetMuzzleEffect();
		if(part_sys)
			MuzzleEffect = gcnew String(part_sys.GetRes()->GetFileName());
		else
			MuzzleEffect = "";
	}

	void WeaponEntityProperties::GetProperties(Engine::ModelEntity& entity)
	{
		EntityProperties::GetProperties(entity);

		WeaponEntity& weapon = (WeaponEntity&)entity;
		weapon.SetMaxAmmo(MaxAmmo);
		weapon.SetDamage(Damage);
		weapon.SetRange(Range);
		weapon.SetEffectiveAngle(EffectiveAngle);
		weapon.SetDistanceAttenuation((WeaponEntity::DistanceAttenuation)DistanceAttenuation);
		weapon.SetWeaponType((WeaponEntity::WeaponType)WeaponType);

		tchar* path = ConvertString<tchar>(MuzzleEffect);
		weapon.SetMuzzleEffect(path);
		Memory::Delete(path);
	}

	void ItemEntityProperties::SetProperties(const Engine::ModelEntity& entity)
	{
		EntityProperties::SetProperties(entity);

		ItemEntity& item = (ItemEntity&)entity;
		ItemType = (ItemTypeEnum)item.GetItemType();
		Amount = item.GetAmount();
	}

	void ItemEntityProperties::GetProperties(Engine::ModelEntity& entity)
	{
		EntityProperties::GetProperties(entity);

		ItemEntity& item = (ItemEntity&)entity;
		item.SetItemType((ItemEntity::ItemType)ItemType);
		item.SetAmount(Amount);
	}


	// =================================================================

	Entity::Entity()
	{
		_entity = ObjectFactory::NewStaticEntity();
		_properties = gcnew StaticEntityProperties;
		LoadProperties();
		_modified = false;
		_fileName = nullptr;
	}

	Entity::~Entity()
	{
		Unload();
	}

	bool Entity::Load(String^ file_name)
	{
		Unload();

		tchar* fn = EditorUtil::GetRelativePath(file_name);
		::Console::PrintLn("testing");
		::Console::PrintLn("Loading entity: %ls", fn);
		_entity = engineAPI->modelEntityManager->CreateEntityObject(fn);
		CreateProperties();
		LoadProperties();

		if(_entity != 0)
		{
			engineAPI->materialManager->LoadAll();
			engineAPI->textureManager->LoadAll();
			engineAPI->modelManager->LoadAll();
			engineAPI->animationManager->LoadAll();
			_entity->SetupModelData();
			_modified = false;
			_fileName = file_name;
			Memory::Delete(fn);
			return true;
		}
		else
		{
			::Console::PrintError("Failed to load entity: %ls", fn);
			Memory::Delete(fn);
			return false;
		}
	}

	// if file_name is nullptr or empty string, save with current file name
	bool Entity::Save(String^ file_name)
	{
		if(!_entity)
			return false;

		tchar* fn = EditorUtil::GetRelativePath(file_name);
		::Console::PrintLn("Saving entity: %ls", fn);
		SaveProperties();
		bool result = _entity->Save(fn);
		if(result)
		{
			_modified = false;
			_fileName = file_name;
		}
		else
		{
			::Console::PrintError("Failed to save entity: %ls", fn);
		}
		Memory::Delete(fn);

		return result;
	}

	void Entity::Unload()
	{
		if(_entity)
		{
			_entity->Unload();
			ObjectFactory::DeleteEntity(_entity);
			_entity = 0;
			_modified = false;
			_fileName = nullptr;
			_properties = nullptr;
		}
	}

	void Entity::ChangeEntityClass(EntityProperties::EntityClass new_class)
	{
		if(!_entity)
			return;

		delete _properties;

		ModelEntity* new_ent;
		switch(new_class)
		{
		case EntityProperties::EntityClass::Static:
			new_ent = ObjectFactory::NewStaticEntity();
			_properties = gcnew StaticEntityProperties;
			break;
		case EntityProperties::EntityClass::Player:
			new_ent = ObjectFactory::NewPlayerEntity();
			_properties = gcnew PlayerEntityProperties;
			break;
		case EntityProperties::EntityClass::AI:
			new_ent = ObjectFactory::NewAIEntity();
			_properties = gcnew AIEntityProperties;
			break;
		case EntityProperties::EntityClass::Weapon:
			new_ent = ObjectFactory::NewWeaponEntity();
			_properties = gcnew WeaponEntityProperties;
			break;
		case EntityProperties::EntityClass::Item:
			new_ent = ObjectFactory::NewItemEntity();
			_properties = gcnew ItemEntityProperties;
			break;
		}

		// copy ModelEntity sub object and delete old entity
		ModelEntity& mdl_sub = *new_ent;
		mdl_sub = *(ModelEntity*)_entity;
		ObjectFactory::DeleteEntity(_entity);
		_entity = new_ent;
		LoadProperties();
	}

	void Entity::CreateProperties()
	{
		if(!_entity)
			return;

		switch(_entity->GetModelEntityType())
		{
		case ModelEntity::ME_TYPE_STATIC:
			_properties = gcnew StaticEntityProperties;
			break;
		case ModelEntity::ME_TYPE_PLAYER:
			_properties = gcnew PlayerEntityProperties;
			break;
		case ModelEntity::ME_TYPE_AI:
			_properties = gcnew AIEntityProperties;
			break;
		case ModelEntity::ME_TYPE_WEAPON:
			_properties = gcnew WeaponEntityProperties;
			break;
		case ModelEntity::ME_TYPE_ITEM:
			_properties = gcnew ItemEntityProperties;
			break;
		}
	}

	void Entity::LoadProperties()
	{
		if(_entity)
			_properties->SetProperties(*_entity);
	}

	void Entity::SaveProperties()
	{
		if(_entity)
			_properties->GetProperties(*_entity);
	}

}
