
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
		struct Particle;
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
		void Update(float frame_time);

		int AddEmitter(Emitter& emitter);
		Emitter* AddEmitter();
		void RemoveEmitter(Emitter& emitter);
		void MoveEmitterUp(Emitter& emitter);
		void MoveEmitterDown(Emitter& emitter);
		List<Emitter*>& GetEmitterList()
			{ return _emitters; }

	private:
		List<Emitter*>::Iterator FindEmitter(Emitter* emitter);

		List<Emitter*> _emitters;
	};


	class ENGINE_API ParticleSystem::Attribute
	{
	public:
		struct Value
		{
			float variable;
			float value;
		};

		Attribute();
		Attribute(const Attribute& attr);
		~Attribute();
		Attribute& operator = (const Attribute& attr);

		void SetValues(const Value* values, int value_count);
		void SetValue(int index, float variable, float value);
		const Value* GetValues() const
			{ return _values; }
		int GetValueCount() const
			{ return _valueCount; }
		float Evaluate(float variable) const;

	private:
		int _valueCount;
		Value* _values;
	};


	struct ParticleSystem::Particle
	{
		float _age;
		math3d::vec3f _position;
		float _rotation;
		float _size;
		float _alpha;
		float _gravity;
		float _friction;
	};


	class ENGINE_API ParticleSystem::Emitter
	{
	public:
		static const int EMITTER_NAME_MAX_LEN = 32;
		static const int MAX_PARTICLES = 512;

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

		enum ParticleType
		{
			PARTICLE_TYPE_NONE,
			PARTICLE_TYPE_TEXTURE,
			PARTICLE_TYPE_MODEL,
		};

		Emitter();
		Emitter(const Emitter& emitter);
		~Emitter();
		Emitter& operator = (const Emitter& emitter);

		void Update(float frame_time);

		bool IsEnabled() const
			{ return _enabled; }
		void Enable(bool enable)
			{ _enabled = enable; }
		const char* GetName() const
			{ return _name; }
		void SetName(const char* name)
		{
			strncpy(_name, name, EMITTER_NAME_MAX_LEN);
			_name[EMITTER_NAME_MAX_LEN - 1] = '\0';
		}
		EmitterType GetEmitterType() const
			{ return _type; }
		void SetEmitterType(EmitterType type)
			{ _type = type; }
		EmitterShader GetShader() const
			{ return _shader; }
		void SetShader(EmitterShader shader)
			{ _shader = shader; }
		const TextureRes* GetTexture() const
			{ return _texture; }
		void SetTexture(const tchar* file_name);
		void ClearTexture();
		const ModelRes* GetModel() const
			{ return _model; }
		void SetModel(const tchar* file_name);
		void ClearModel();
		float GetLifeStart() const
			{ return _lifeStart; }
		void SetLifeStart(float life_start)
			{ _lifeStart = life_start; }
		float GetLife() const
			{ return _life; }
		void SetLife(float life)
			{ _life = life; }
		bool GetLoop() const
			{ return _loop; }
		void SetLoop(bool loop)
			{ _loop = loop; }
		bool GetImplode() const
			{ return _implode; }
		void SetImplode(bool implode)
			{ _implode = implode; }
		bool EmitFromEdge() const
			{ return _emitFromEdge; }
		void SetEmitFromEdge(bool val)
			{ _emitFromEdge = val; }
		bool IsAnimatedTex() const
			{ return _animatedTex; }
		void SetAnimatedTex(bool val)
			{ _animatedTex = val; }

		float GetParticleLife() const
			{ return _partLife; }
		void SetParticleLife(float val)
			{ _partLife = val; }
		bool GetParticleRandomOrient() const
			{ return _partRandomOrient; }
		void SetParticleRandomOrient(bool val)
			{ _partRandomOrient = val; }
		ParticleType GetParticleType() const;

		Attribute Velocity;
		Attribute Size;
		Attribute EmitAngle;
		Attribute EmitRate;
		Attribute OffsetX;
		Attribute OffsetY;
		Attribute OffsetZ;
		Attribute RotationX;
		Attribute RotationY;
		Attribute RotationZ;

		Attribute ParticleSize;
		Attribute ParticleRotationSpeed;
		Attribute ParticleAlpha;
		Attribute ParticleGravity;
		Attribute ParticleFriction;

	private:
		FreePool<Particle> _particlePool;
		Particle** _liveParticles;
		int _liveCount;
		bool _enabled;

		char _name[EMITTER_NAME_MAX_LEN];
		EmitterType _type;
		EmitterShader _shader;
		const TextureRes* _texture;
		const ModelRes* _model;
		float _lifeStart;
		float _life;
		bool _loop;
		bool _implode;
		bool _emitFromEdge;
		bool _animatedTex;

		float _partLife;
		bool _partRandomOrient;
	};


}


#endif // _ENGINE_PARTICLE_SYSTEM_H_
