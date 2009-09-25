
#pragma once


namespace Engine
{
	class ModelEntity;
}


namespace EntityEditor
{

	using namespace System;
	using namespace System::ComponentModel;

	public enum class EntityClass
	{
		Generic,
		NPC,
		Monster,
		Boss,
		Building,
		Foliage,
		Debris,
	};

	public ref class EntityProperties
	{
	public:
		[Category("General")]
		[Description("Entity's class determines it's role in the game.")]
		property EntityClass Class
		{
			EntityClass get()
				{ return _class; }
			void set(EntityClass val)
				{ _class = val; }
		};

		[Category("General")]
		[Description("Entity's name as it apears in the game.")]
		property String^ Name
		{
			String^ get()
				{ return _name; }
			void set(String^ val)
				{ _name = val; }
		}

		[Category("General")]
		[Description("Set if entity has collision detection.")]
		property bool Clip
		{
			bool get()
				{ return _clip; }
			void set(bool val)
				{ _clip = val; }
		}

		[Category("General")]
		[DisplayName("Life Points")]
		[Description("This is entity's maximum health.")]
		property int LifePoints
		{
			int get()
				{ return _lifePoints; }
			void set(int val)
				{ _lifePoints = val; }
		}

	private:
		EntityClass _class;
		String^ _name;
		bool _clip;
		int _lifePoints;
	};

	public ref class Entity
	{
	public:
		Entity();
		~Entity();

		bool Load(String^ file_name);
		bool Save(String^ file_name);
		void Unload();
		Engine::ModelEntity* GetEntity()
			{ return _entity; }
		EntityProperties^ GetProperties()
			{ return _properties; }
		bool IsModified()
			{ return _modified; }
		void SetModified(bool modified)
			{ _modified = modified; }
		bool IsUntitled()
			{ return (_fileName == nullptr); }
		String^ GetFileName()
			{ return _fileName; }

	private:
		void LoadProperties();
		void SaveProperties();

		Engine::ModelEntity* _entity;
		EntityProperties^ _properties;
		bool _modified;
		String^ _fileName;
	};

}
