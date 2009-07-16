
#include "StdAfx.h"
#include "Utility.h"
#include "Entity.h"

using namespace Memory;
using namespace Engine;


namespace EntityEditor
{

	Entity::Entity()
	{
		_entity = new(mainPool) ModelEntity;
		_properties = gcnew EntityProperties;
		LoadProperties();
		_modified = false;
		_fileName = nullptr;
	}

	bool Entity::Load(String^ file_name)
	{
		if(!_entity)
			_entity = new(mainPool) ModelEntity;

		tchar* fn = GetRelativePath(file_name);
		::Console::PrintLn("Loading entity: %ls", fn);
		bool result = _entity->Load(fn);
		LoadProperties();

		if(result)
		{
			engineAPI->textureManager->LoadAll();
			engineAPI->modelManager->LoadAll();
			engineAPI->animationManager->LoadAll();
			_entity->SetupModelData();
			_modified = false;
			_fileName = file_name;
		}
		else
			::Console::PrintError("Failed to load entity: %ls", fn);
		delete[] fn;

		return result;
	}

	// if file_name is nullptr or empty string, save with current file name
	bool Entity::Save(String^ file_name)
	{
		if(!_entity)
			return false;

		tchar* fn = GetRelativePath(file_name);
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
		delete[] fn;

		return result;
	}

	void Entity::Unload()
	{
		if(_entity)
		{
			//! resiti ovo sranje
			_entity->ModelEntity::~ModelEntity();
			::operator delete(_entity);
			_entity = 0;
			_modified = false;
			_fileName = nullptr;
			LoadProperties();
		}
	}

	void Entity::LoadProperties()
	{
		if(_entity)
		{
			_properties->Class = (EntityClass)(int)_entity->GetClass();
			_properties->Name = gcnew String(_entity->GetName());
			_properties->Clip = _entity->GetClip();
			_properties->LifePoints = _entity->GetLifePoints();
		}
		else
		{
			_properties->Class = EntityClass::Generic;
			_properties->Name = "";
			_properties->Clip = true;
			_properties->LifePoints = 100;
		}
	}

	void Entity::SaveProperties()
	{
		if(_entity)
		{
			_entity->SetClass((Engine::EntityClass)(int)_properties->Class);
			char* name = ConvertString<char>(_properties->Name);
			_entity->SetName(name);
			delete[] name;
			_entity->SetClip(_properties->Clip);
			_entity->SetLifePoints(_properties->LifePoints);
		}
	}

}
