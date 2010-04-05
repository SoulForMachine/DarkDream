
#pragma once


namespace Engine
{
	class ModelEntity;
}


namespace EntityEditor
{

	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::ComponentModel;
	using namespace System::Drawing::Design;


	public ref class UIFilePathEditor: public UITypeEditor
	{
	public:
		UIFilePathEditor(String^ title, String^ dir, String^ filter);

		virtual UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext^ context) override;
		virtual Object^ EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, Object^ value) override;

	private:
		String^ _title;
		String^ _dir;
		String^ _filter;
	};
	
	public ref class UIAIPathEditor: public UIFilePathEditor
	{
		UIAIPathEditor()
			: UIFilePathEditor(
				"Select IA Script",
				"AIScripts",
				"AI Script Files (*.ai)|*.ai|All Files (*.*)|*.*")
		{ }
	};

	public ref class UIPartSysPathEditor: public UIFilePathEditor
	{
		UIPartSysPathEditor()
			: UIFilePathEditor(
				"Select Particle System",
				"Particles",
				"Particle System Files (*.part)|*.part|All Files (*.*)|*.*")
		{ }
	};



	public ref class EntityProperties abstract
	{
	public:
		enum class EntityClass
		{
			Static,
			Player,
			AI,
			Weapon,
			Item,
		};


		[Category("1. General")]
		[DisplayName("Class")]
		[Description("Entity's class determines it's role in the game.")]
		property EntityClass Class
		{
			EntityClass get()
				{ return _class; }
			void set(EntityClass val)
				{ _class = val; }
		};

		[Category("1. General")]
		[DisplayName("Name")]
		[Description("Entity's name as it apears in the game.")]
		property String^ Name
		{
			String^ get()
				{ return _name; }
			void set(String^ val)
				{ _name = val; }
		}

		virtual void SetProperties(const Engine::ModelEntity& entity);
		virtual void GetProperties(Engine::ModelEntity& entity);

	private:
		EntityClass _class;
		String^ _name;
	};

	public ref class StaticEntityProperties: public EntityProperties
	{
	public:
		[Category("2. Static Entity")]
		[DisplayName("Clip")]
		[Description("Set if entity has collision detection.")]
		property bool Clip
		{
			bool get()
				{ return _clip; }
			void set(bool val)
				{ _clip = val; }
		}

		virtual void SetProperties(const Engine::ModelEntity& entity) override;
		virtual void GetProperties(Engine::ModelEntity& entity) override;

	private:
		bool _clip;
	};


	public ref class PlayerEntityProperties: public EntityProperties
	{
	public:
		[Category("2. Player Entity")]
		[DisplayName("Health")]
		[Description("This is entity's initial health.")]
		property int Health
		{
			int get()
				{ return _health; }
			void set(int val)
				{ _health = val; }
		}

		[Category("2. Player Entity")]
		[DisplayName("Max Health")]
		[Description("This is maximum health entity can have.")]
		property int MaxHealth
		{
			int get()
				{ return _maxHealth; }
			void set(int val)
				{ _maxHealth = val; }
		}

		[Category("2. Player Entity")]
		[DisplayName("Armor")]
		[Description("This is entity's initial armor.")]
		property int Armor
		{
			int get()
				{ return _armor; }
			void set(int val)
				{ _armor = val; }
		}

		[Category("2. Player Entity")]
		[DisplayName("Max Armor")]
		[Description("This is maximum armor entity can have.")]
		property int MaxArmor
		{
			int get()
				{ return _maxArmor; }
			void set(int val)
				{ _maxArmor = val; }
		}

		virtual void SetProperties(const Engine::ModelEntity& entity) override;
		virtual void GetProperties(Engine::ModelEntity& entity) override;

	private:
		int _health;
		int _maxHealth;
		int _armor;
		int _maxArmor;
	};


	public ref class AIEntityProperties: public EntityProperties
	{
	public:
		enum class AITypeEnum
		{
			Monster,
			Boss,
			NPC
		};


		[Category("2. AI Entity")]
		[DisplayName("AI Entity Type")]
		[Description("Type of an AI entity: monster, boss or NPC.")]
		property AITypeEnum AIType
		{
			AITypeEnum get()
				{ return _aiType; }
			void set(AITypeEnum val)
				{ _aiType = val; }
		}

		[Category("2. AI Entity")]
		[DisplayName("Health")]
		[Description("This is entity's initial health.")]
		property int Health
		{
			int get()
				{ return _health; }
			void set(int val)
				{ _health = val; }
		}

		[Category("2. AI Entity")]
		[DisplayName("Max Health")]
		[Description("This is maximum health entity can have.")]
		property int MaxHealth
		{
			int get()
				{ return _maxHealth; }
			void set(int val)
				{ _maxHealth = val; }
		}

		[Category("2. AI Entity")]
		[DisplayName("Armor")]
		[Description("This is entity's initial armor.")]
		property int Armor
		{
			int get()
				{ return _armor; }
			void set(int val)
				{ _armor = val; }
		}

		[Category("2. AI Entity")]
		[DisplayName("Max Armor")]
		[Description("This is maximum armor entity can have.")]
		property int MaxArmor
		{
			int get()
				{ return _maxArmor; }
			void set(int val)
				{ _maxArmor = val; }
		}

		[Category("2. AI Entity")]
		[DisplayName("AI Script")]
		[Description("Script file that determines entity's behavior.")]
		[EditorAttribute(UIAIPathEditor::typeid, UITypeEditor::typeid)]
		property String^ AIScript
		{
			String^ get()
				{ return _aiScript; }
			void set(String^ val)
				{ _aiScript = val; }
		}

		virtual void SetProperties(const Engine::ModelEntity& entity) override;
		virtual void GetProperties(Engine::ModelEntity& entity) override;

	private:
		AITypeEnum _aiType;
		int _health;
		int _maxHealth;
		int _armor;
		int _maxArmor;
		String^ _aiScript;
	};


	public ref class WeaponEntityProperties: public EntityProperties
	{
	public:
		enum class WeaponTypeEnum
		{
			Gun,
			Rocket,
			Granade,
			Fire,
			Electric,
			Melee
		};

		enum class DistanceAttenuationEnum
		{
			None,
			Linear,
			Quadratic
		};


		[Category("2. Weapon Entity")]
		[DisplayName("Weapon Type")]
		[Description("Type of weapon.")]
		property WeaponTypeEnum WeaponType
		{
			WeaponTypeEnum get()
				{ return _weaponType; }
			void set(WeaponTypeEnum val)
				{ _weaponType = val; }
		}

		[Category("2. Weapon Entity")]
		[DisplayName("Max Ammo")]
		[Description("Maximum amount of ammo this weapon can have.")]
		property int MaxAmmo
		{
			int get()
				{ return _maxAmmo; }
			void set(int val)
				{ _maxAmmo = val; }
		}

		[Category("2. Weapon Entity")]
		[DisplayName("Damage")]
		[Description("The damage that weapon inflicts.")]
		property int Damage
		{
			int get()
				{ return _damage; }
			void set(int val)
				{ _damage = val; }
		}

		[Category("2. Weapon Entity")]
		[DisplayName("Range")]
		[Description("Defines the maximum distance at which this weapon is effective."
			" Weapon does not inflict any damage beyond range.")]
		property float Range
		{
			float get()
				{ return _range; }
			void set(float val)
				{ _range = val; }
		}

		[Category("2. Weapon Entity")]
		[DisplayName("Effective Angle")]
		[Description("An angle of weapon effect.")]
		property float EffectiveAngle
		{
			float get()
				{ return _effectiveAngle; }
			void set(float val)
				{ _effectiveAngle = val; }
		}

		[Category("2. Weapon Entity")]
		[DisplayName("Distance Attenuation")]
		[Description("Defines a function of how weapon damage decreases with distance.")]
		property DistanceAttenuationEnum DistanceAttenuation
		{
			DistanceAttenuationEnum get()
				{ return _distAtten; }
			void set(DistanceAttenuationEnum val)
				{ _distAtten = val; }
		}

		[Category("2. Weapon Entity")]
		[DisplayName("Muzzle Effect")]
		[Description("A particle system that apears as an effect when the weapon is fired.")]
		[EditorAttribute(UIPartSysPathEditor::typeid, UITypeEditor::typeid)]
		property String^ MuzzleEffect
		{
			String^ get()
				{ return _muzzleEffect; }
			void set(String^ val)
				{ _muzzleEffect = val; }
		}

		virtual void SetProperties(const Engine::ModelEntity& entity) override;
		virtual void GetProperties(Engine::ModelEntity& entity) override;

	private:
		int _maxAmmo;
		int _damage;
		float _range;
		float _effectiveAngle;
		DistanceAttenuationEnum _distAtten;
		WeaponTypeEnum _weaponType;
		String^ _muzzleEffect;
	};


	public ref class ItemEntityProperties: public EntityProperties
	{
	public:
		enum class ItemTypeEnum
		{
			Health,
			Ammo,
			Key
		};

		[Category("2. Item Entity")]
		[DisplayName("Item Type")]
		[Description("Type of an item: health, ammo, key...")]
		property ItemTypeEnum ItemType
		{
			ItemTypeEnum get()
				{ return _itemType; }
			void set(ItemTypeEnum val)
				{ _itemType = val; }
		}

		[Category("2. Item Entity")]
		[DisplayName("Amount")]
		[Description("An amount of upgrade this item gives to the player.")]
		property int Amount
		{
			int get()
				{ return _amount; }
			void set(int val)
				{ _amount = val; }
		}

		virtual void SetProperties(const Engine::ModelEntity& entity) override;
		virtual void GetProperties(Engine::ModelEntity& entity) override;

	private:
		ItemTypeEnum _itemType;
		int _amount;
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
		void ChangeEntityClass(EntityProperties::EntityClass new_class);

	private:
		void CreateProperties();
		void LoadProperties();
		void SaveProperties();

		Engine::ModelEntity* _entity;
		EntityProperties^ _properties;
		bool _modified;
		String^ _fileName;
	};

}
