
#ifndef _ENGINE_PARTICLE_SYSTEM_H_
#define _ENGINE_PARTICLE_SYSTEM_H_

#include "BaseLib/List.h"
#include "BaseLib/FreePool.h"
#include "Engine/Common.h"
#include "Engine/RenderableEntity.h"


namespace Engine
{

	class TextureRes;
	class ModelRes;


	class ENGINE_API ParticleSystem: public RenderableEntity
	{
	public:
		class Emitter;
		class Particle;
		class Attribute;

		ParticleSystem();
		ParticleSystem(const ParticleSystem& psys);
		~ParticleSystem();
		ParticleSystem& operator = (const ParticleSystem& psys);

		virtual EntityType GetType() const
			{ return ENTITY_TYPE_PARTICLE_SYS; }

		bool Load(const tchar* file_name);
		bool Save(const tchar* file_name) const;
		void Unload();

		int AddEmitter(Emitter* emitter);
		void RemoveEmitter(const Emitter* emitter);
		void MoveEmitterUp(const Emitter* emitter);
		void MoveEmitterDown(const Emitter* emitter);

	private:
		List<Emitter*> _emitters;
	};


	class ParticleSystem::Attribute
	{
	public:
		struct Value
		{
			float variable;
			float value;
		};

		void SetValues(const Value* values, int value_count);
		const Value* GetValues() const;
		int GetValueCount() const
			{ return _valueCount; }
		float Evaluate(float variable);

	private:
		int _valueCount;
		Value* _values;
	};


	class ParticleSystem::Particle
	{
	public:

	private:
		float _age;
		math3d::vec3f _position;
		float _rotation;
		float _size;
		float _alpha;
		float _gravity;
		float _friction;
	};


	class ParticleSystem::Emitter
	{
	public:
		enum EmitterType
		{
			EMITTER_TYPE_SPHERE,
			EMITTER_TYPE_PLANE,
			EMITTER_TYPE_CIRCLE,
			EMITTER_TYPE_LINE,
		};

		enum EmitterShader
		{
			EMITTER_SHADER_BLEND,
			EMITTER_SHADER_ADDITIVE,
		};

	private:
		static const int EMITTER_NAME_MAX_LEN = 32;

		FreePool<Particle> _particlePool;
		Particle* _liveParticles;
		int _liveCount;

		char _name[EMITTER_NAME_MAX_LEN];
		EmitterType _type;
		EmitterShader _shader;
		TextureRes* _texture;
		ModelRes* _model;
		float _lifeStart;
		float _life;
		bool _loop;
		bool _implode;
		bool _emitFromEdge;
		Attribute _velocity;
		Attribute _size;
		Attribute _emitAngle;
		Attribute _emitRate;
		Attribute _offsetX;
		Attribute _offsetY;
		Attribute _offsetZ;
		Attribute _rotationX;
		Attribute _rotationY;
		Attribute _rotationZ;

		float _partLife;
		bool _partRandomOrient;
		Attribute _partSize;
		Attribute _partRotationSpeed;
		Attribute _partAlpha;
		Attribute _partGravity;
		Attribute _partFriction;
	};


}


#endif // _ENGINE_PARTICLE_SYSTEM_H_
