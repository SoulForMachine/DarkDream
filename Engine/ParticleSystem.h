
#ifndef _ENGINE_PARTICLE_SYSTEM_H_
#define _ENGINE_PARTICLE_SYSTEM_H_

#include "BaseLib/List.h"
#include "BaseLib/FreePool.h"
#include "Engine/Common.h"
#include "Engine/RenderableEntity.h"


class Parser;


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
		virtual void UpdateGraphics(int frame_time);

		int AddEmitter(Emitter& emitter);
		Emitter* AddEmitter();
		void RemoveEmitter(Emitter& emitter);
		void MoveEmitterUp(Emitter& emitter);
		void MoveEmitterDown(Emitter& emitter);
		List<Emitter*>& GetEmitterList()
			{ return _emitters; }
		const List<Emitter*>& GetEmitterList() const
			{ return _emitters; }
		void Reset();

	private:
		List<Emitter*>::Iterator FindEmitter(Emitter* emitter);
		bool ReadAttribute(Parser& parser, Attribute& attrib, const char* attr_name);

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
		Value* GetValues()
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
		float age;
		math3d::vec3f position;
		math3d::vec3f velocity;
		float rotation;
		float size;
		float alpha;
		float frame; // animated texture frame
	};


	class ENGINE_API ParticleSystem::Emitter
	{
	public:
		static const int EMITTER_NAME_MAX_LEN = 32;
		static const int MAX_PARTICLES = 1024;
		static const int ATTRIB_COUNT = 15;

		enum EmitterType
		{
			EMITTER_TYPE_SPHERE,
			EMITTER_TYPE_PLANE,
			EMITTER_TYPE_CIRCLE,
			EMITTER_TYPE_LINE,

			EMITTER_TYPE_COUNT
		};

		enum EmitterShader
		{
			EMITTER_SHADER_BLEND,
			EMITTER_SHADER_ADDITIVE,

			EMITTER_SHADER_COUNT
		};

		enum ParticleType
		{
			PARTICLE_TYPE_NONE,
			PARTICLE_TYPE_TEXTURE,
			PARTICLE_TYPE_MODEL,

			PARTICLE_TYPE_COUNT
		};

		Emitter();
		Emitter(const Emitter& emitter);
		~Emitter();
		Emitter& operator = (const Emitter& emitter);

		void Update(float frame_time, const math3d::mat4f& psys_world_mat);
		void Reset();

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
		void SetTexture(const tchar* file_name, bool immediate = true);
		void ClearTexture();
		const ModelRes* GetModel() const
			{ return _model; }
		void SetModel(const tchar* file_name, bool immediate = true);
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
		int GetTextureFrameCount() const
			{ return _texFrameCount; }

		float GetParticleLife() const
			{ return _partLife; }
		void SetParticleLife(float val)
			{ _partLife = val; }
		bool GetParticleRandomOrient() const
			{ return _partRandomOrient; }
		void SetParticleRandomOrient(bool val)
			{ _partRandomOrient = val; }
		ParticleType GetParticleType() const;

		Attribute** GetAttributeArray()
			{ return _attributes; }
		static const char* GetAttributeName(int index)
			{ return _attribNames[index]; }

		const Particle* const * GetParticles() const
			{ return _liveParticles[_partBufInd]; }
		int GetParticleCount() const
			{ return _liveCount; }
		const math3d::vec3f& GetPosition() const
			{ return _emitterPos; }

		static EmitterType GetEmitterTypeByString(const char* name);
		static const char* GetEmitterTypeString(EmitterType type);
		static EmitterShader GetEmitterShaderByString(const char* name);
		static const char* GetEmitterShaderString(EmitterShader shader);
		static ParticleType GetParticleTypeByString(const char* name);
		static const char* GetParticleTypeString(ParticleType type);

		/*
			When adding new attribute, don't forget to update:
			ATTRIB_COUNT
			_attributes
			_attribNames
		*/
		Attribute Velocity;
		Attribute EmitterSize;
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
		void EmitSphere(int count, float em_size, float em_angle, float velocity);
		void EmitPlane(int count, float em_size, float em_angle, float velocity);
		void EmitCircle(int count, float em_size, float em_angle, float velocity);
		void EmitLine(int count, float em_size, float em_angle, float velocity);

		static const char* _emitterTypeStrings[EMITTER_TYPE_COUNT];
		static const char* _emitterShaderStrings[EMITTER_SHADER_COUNT];
		static const char* _particleTypeStrings[PARTICLE_TYPE_COUNT];
		static const char* _attribNames[ATTRIB_COUNT];

		FreePool<Particle> _particlePool;
		Particle** _liveParticles[2];
		int _partBufInd;
		Attribute* _attributes[ATTRIB_COUNT];
		int _liveCount;
		float _age;
		float _emitCount;
		bool _enabled;
		bool _alive;
		math3d::vec3f _emitterPos;
		math3d::mat3f _rotMatrix;

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
		int _texFrameCount;

		float _partLife;
		bool _partRandomOrient;
	};


}


#endif // _ENGINE_PARTICLE_SYSTEM_H_
