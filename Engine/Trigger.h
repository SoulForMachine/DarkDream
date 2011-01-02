
#ifndef _ENGINE_TRIGGER_H_
#define _ENGINE_TRIGGER_H_

#include "Engine/Common.h"
#include "Engine/Entity.h"


namespace Engine
{

	class ModelEntity;


	/*
		Abstract class for triggers.
	*/
	class ENGINE_API Trigger: public Entity
	{
	public:
		static const int TRIGGER_NAME_MAX_LEN = 32;

		virtual ~Trigger() = 0 {}
		virtual EntityType GetType() const
			{ return ENTITY_TYPE_TRIGGER; }

		virtual void OnTriggered() = 0;

		void SetEnabled(bool enabled)
			{ _enabled = enabled; }
		bool IsEnabled() const
			{ return _enabled; }
		void SetName(const char* name)
		{
			strncpy(_name, name, TRIGGER_NAME_MAX_LEN);
			_name[TRIGGER_NAME_MAX_LEN - 1] = '\0';
		}
		const char* GetName() const
			{ return _name; }
		void SetBounds(const math3d::vec2f& bounds)
			{ _bounds = bounds; }
		const math3d::vec2f& GetBounds() const
			{ return _bounds; }

	protected:
		bool _enabled;
		char _name[TRIGGER_NAME_MAX_LEN];
		math3d::vec2f _bounds;
	};


	/*
		Triggered when player or monsters walk into it.
	*/
	class ENGINE_API TriggerBox: public Trigger
	{
	public:

	private:
	};


	/*
		Player can interact with the switch, it has an entity attached with animations for on and off.
	*/
	class ENGINE_API TriggerSwitch: public Trigger
	{
	public:
		void SetState(bool state)
			{ _state = state; }
		bool GetState() const
			{ return _state; }
		void SetEntity(ModelEntity* entity)
			{ _entity = entity; }
		const ModelEntity* GetEntity() const
			{ return _entity; }

	private:
		bool _state;
		ModelEntity* _entity;
	};


	/*
		Activates after specified time interval.
	*/
	class ENGINE_API TriggerTimer: public Trigger
	{
	public:
		void SetTimeInterval(float time)
			{ _timeInterval = time; }
		float GetTimeInterval() const
			{ return _timeInterval; }
		void Start(bool start)
			{ _started = start; }
		bool IsStarted() const 
			{ return _started; }
		void SetLoop(bool loop)
			{ _loop = loop; }
		bool GetLoop() const
			{ return _loop; }

	private:
		float _timeInterval;
		bool _started;
		bool _loop;
	};


	/*
		Start value can increment and decrement, activates upon reaching the specified value.
	*/
	class ENGINE_API TriggerCounter: public Trigger
	{
	public:
		void SetStartValue(int val)
			{ _startValue = val; }
		int GetStartValue() const
			{ return _startValue; }
		void SetActivateValue(int val)
			{ _activateValue = val; }
		int GetActivateValue() const
			{ return _activateValue; }

	private:
		int _startValue;
		int _activateValue;
	};


	/*
		Takes health from player or monsters that walk into damage box.
		If continuous flag is true, then health is constantly taken from the entity each second.
	*/
	class ENGINE_API TriggerDamageBox: public Trigger
	{
	public:
		enum DamageFilter
		{
			DAMAGE_FILTER_PLAYER,
			DAMAGE_FILTER_MONSTERS,
			DAMAGE_FILTER_ALL
		};

		void SetHealthTaken(int health)
			{ _healthTaken = health; }
		int GetHealthTaken() const
			{ return _healthTaken; }
		void SetContinuous(bool cnt)
			{ _continuous = cnt; }
		bool IsContinuous() const
			{ return _continuous; }
		void SetDamageFilter(DamageFilter filter)
			{ _damageFilter = filter; }
		DamageFilter GetDamageFilter() const
			{ return _damageFilter; }

	private:
		int _healthTaken;
		bool _continuous;
		DamageFilter _damageFilter;
	};


	/*
		Moves bound entity between specified points.
	*/
	class ENGINE_API TriggerMoveController: public Trigger
	{
	public:
		void SetEntity(ModelEntity* entity)
			{ _entity = entity; }
		const ModelEntity* GetEntity() const
			{ return _entity; }
		void SetSpeed(float speed)
			{ _speed = speed; }
		float GetSpeed() const
			{ return _speed; }
		void SetMoving(bool move)
			{ _moving = move; }
		bool IsMoving() const
			{ return _moving; }
		void SetLoop(bool loop)
			{ _loop = loop; }
		bool GetLoop() const
			{ return _loop; }

	private:
		ModelEntity* _entity;
		float _speed;
		bool _moving;
		bool _loop;
	};


	/*
		Blocks the path when closed, it has attached entity that can be any kind of door with open/close animations.
	*/
	class ENGINE_API TriggerGate: public Trigger
	{
	public:
		void SetEntity(ModelEntity* entity)
			{ _entity = entity; }
		const ModelEntity* GetEntity() const
			{ return _entity; }
		void SetOpened(bool opened)
			{ _opened = opened; }
		bool IsOpened() const
			{ return _opened; }

	private:
		ModelEntity* _entity;
		bool _opened;
	};


	/*
		Spawns the monsters.
	*/
	class ENGINE_API TriggerSpawner: public Trigger
	{
	public:
		void SetRadius(float radius)
			{ _radius = radius; }
		float GetRadius() const
			{ return _radius; }
		void SetCount(int count)
			{ _count = count; }
		int GetCount() const
			{ return _count; }

	private:
		float _radius;
		int _count;
	};


	/*
		Shakes the camera when triggered.
	*/
	class ENGINE_API TriggerCameraShake: public Trigger
	{
	public:
		void SetShake(float shake)
			{ _shake = shake; }
		float GetShake() const
			{ return _shake; }

	private:
		float _shake;
	};


	/*
		Plays a sound when triggered.
	*/
	class ENGINE_API TriggerSoundPlayer: public Trigger
	{
	public:

	private:

	};


	/*
		Places more triggers into a group and defines their interaction.
	*/
	class ENGINE_API LogicGroup
	{
	public:
	};

}

#endif // _ENGINE_TRIGGER_H_
