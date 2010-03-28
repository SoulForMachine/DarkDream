
#include "StdAfx.h"
#include "Utility.h"
#include "Entity.h"

using namespace Memory;
using namespace Engine;


namespace EntityEditor
{

	Entity::Entity()
	{
		_entity = ObjectFactory::NewStaticEntity();
		_properties = gcnew EntityProperties;
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

		tchar* fn = GetRelativePath(file_name);
		::Console::PrintLn("Loading entity: %ls", fn);
		_entity = engineAPI->modelEntityManager->CreateEntityObject(fn);
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
			delete[] fn;
			return true;
		}
		else
		{
			::Console::PrintError("Failed to load entity: %ls", fn);
			delete[] fn;
			return false;
		}
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
			_entity->Unload();
			ObjectFactory::DeleteEntity(_entity);
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
			_properties->Name = gcnew String(_entity->GetName());
		}
		else
		{
			_properties->Name = "";
		}
	}

	void Entity::SaveProperties()
	{
		if(_entity)
		{
			char* name = ConvertString<char>(_properties->Name);
			_entity->SetName(name);
			delete[] name;
		}
	}

}
