
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Console.h"
#include "BaseLib/Parser.h"
#include "EngineInternal.h"
#include "FileSystem.h"
#include "FileResource.h"
#include "ResourceManager.h"
#include "ParticleSystem.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	// ========== ParticleSystem ==========

	ParticleSystem::ParticleSystem()
		: _emitters(mapPool)
	{
	}

	ParticleSystem::ParticleSystem(const ParticleSystem& psys)
	{
		*this = psys;
	}

	ParticleSystem::~ParticleSystem()
	{
		Unload();
	}

	ParticleSystem& ParticleSystem::operator = (const ParticleSystem& psys)
	{
		Unload();
		for(List<Emitter*>::ConstIterator it = psys._emitters.Begin(); it != psys._emitters.End(); ++it)
		{
			_emitters.PushBack(new(mapPool) Emitter(**it));
		}
		return *this;
	}

	bool ParticleSystem::Load(const tchar* file_name)
	{
		if(!file_name)
			return false;

		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(file_name, _t("rb"));
		if(!file)
			return false;

		Parser parser;
		if(!parser.LoadFile(*file))
			return false;
		file->Close();

		Unload();

		if(!parser.ExpectTokenString("particleSystem"))
			return false;
		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
			return false;

		Parser::Token token;
		while(1)
		{
			if(!parser.ReadToken(token))
				{ Unload(); return false; }
			if(	token.type == Parser::TOK_PUNCTUATION &&
				token.subTypePunct == Parser::PUNCT_CLOSE_BRACE )
			{
				break;
			}
			else if(token.type == Parser::TOK_IDENTIFIER &&
				!strcmp(token.str, "emitter"))
			{
				if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
					{ Unload(); return false; }

				Emitter* emitter = new(mapPool) Emitter;
				char name[Emitter::EMITTER_NAME_MAX_LEN];

				if(!parser.ReadString(name, Emitter::EMITTER_NAME_MAX_LEN))
					{ delete emitter; Unload(); return false; }
				emitter->SetName(name);

				

				if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE))
					{ delete emitter; Unload(); return false; }

				_emitters.PushBack(emitter);
			}
			else
			{
				Console::PrintError("Expected \'emitter\' keyword, found \'%s\'", token.str);
				Unload();
				return false;
			}
		}

		// particle system closing brace
		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE))
			{ Unload(); return false; }

		return true;
	}

	bool ParticleSystem::Save(const tchar* file_name) const
	{
		return true;
	}

	void ParticleSystem::Unload()
	{
		for(List<Emitter*>::ConstIterator it = _emitters.Begin(); it != _emitters.End(); ++it)
		{
			delete *it;
		}
		_emitters.Clear();
	}

	void ParticleSystem::Update(float frame_time)
	{
	}

	int ParticleSystem::AddEmitter(Emitter& emitter)
	{
		_emitters.PushBack(&emitter);
		return _emitters.GetCount();
	}

	ParticleSystem::Emitter* ParticleSystem::AddEmitter()
	{
		Emitter* emitter = new(mapPool) Emitter;
		_emitters.PushBack(emitter);
		return emitter;
	}

	void ParticleSystem::RemoveEmitter(Emitter& emitter)
	{
		List<Emitter*>::Iterator it = FindEmitter(&emitter);
		if(it != _emitters.End())
			_emitters.Remove(it);
	}

	void ParticleSystem::MoveEmitterUp(Emitter& emitter)
	{
		List<Emitter*>::Iterator it = FindEmitter(&emitter);
		if(it != _emitters.End())
		{
			List<Emitter*>::Iterator prev_it = it;
			--prev_it;
			if(prev_it != _emitters.End())
			{
				List<Emitter*>::NodeType* node = _emitters.Unlink(it);
				_emitters.Link(prev_it, node);
			}
		}
	}

	void ParticleSystem::MoveEmitterDown(Emitter& emitter)
	{
		List<Emitter*>::Iterator it = FindEmitter(&emitter);
		if(it != _emitters.End())
		{
			List<Emitter*>::Iterator next_it = it;
			++next_it;
			if(next_it != _emitters.End())
			{
				++next_it;
				List<Emitter*>::NodeType* node = _emitters.Unlink(it);
				_emitters.Link(next_it, node);
			}
		}
	}

	List<ParticleSystem::Emitter*>::Iterator ParticleSystem::FindEmitter(Emitter* emitter)
	{
		for(List<Emitter*>::Iterator it = _emitters.Begin(); it != _emitters.End(); ++it)
		{
			if(*it == emitter)
				return it;
		}

		return _emitters.End();
	}


	// ========== ParticleSystem::Attribute ==========

	ParticleSystem::Attribute::Attribute()
	{
		_valueCount = 0;
		_values = 0;
	}

	ParticleSystem::Attribute::Attribute(const Attribute& attr)
	{
		*this = attr;
	}

	ParticleSystem::Attribute::~Attribute()
	{
		delete[] _values;
	}

	ParticleSystem::Attribute&
		ParticleSystem::Attribute::operator = (const Attribute& attr)
	{
		SetValues(attr._values, attr._valueCount);
		return *this;
	}

	void ParticleSystem::Attribute::SetValues(const Value* values, int value_count)
	{
		delete[] _values;
		_valueCount = 0;
		if(value_count)
		{
			_valueCount = value_count;
			_values = new(mapPool) Value[_valueCount];
			memcpy(_values, values, _valueCount * sizeof(Value));
		}
	}

	void ParticleSystem::Attribute::SetValue(int index, float variable, float value)
	{
		if(index >= 0 && index < _valueCount)
		{
			_values[index].variable = variable;
			_values[index].value = value;
		}
	}

	float ParticleSystem::Attribute::Evaluate(float variable) const
	{
		if(_valueCount)
		{
			int i;
			for(i = 0; i < _valueCount; ++i)
			{
				if(_values[i].variable >= variable)
					break;
			}

			if(i == _valueCount)
			{
				return _values[i - 1].value;
			}
			else if(i == 0)
			{
				return _values[0].value;
			}
			else
			{
				float t = (variable - _values[i - 1].variable) / (_values[i].variable - _values[i - 1].variable);
				return lerp(_values[i - 1].value, _values[i].value, t);
			}
		}

		return 0.0f;
	}

	// ========== ParticleSystem::Emitter ==========

	ParticleSystem::Emitter::Emitter()
		: _particlePool(MAX_PARTICLES)
	{
		_liveParticles = new(mapPool) Particle*[MAX_PARTICLES];
		_liveCount = 0;
		_enabled = true;

		_name[0] = '\0';
		_type = EMITTER_TYPE_SPHERE;
		_shader = EMITTER_SHADER_ADDITIVE;
		_texture = 0;
		_model = 0;
		_lifeStart = 0.0f;
		_life = 10.0f;
		_loop = false;
		_implode = false;
		_emitFromEdge = false;
		_animatedTex = false;

		_partLife = 10.0f;
		_partRandomOrient = false;
	}

	ParticleSystem::Emitter::Emitter(const Emitter& emitter)
		: _particlePool(MAX_PARTICLES)
	{
		_liveParticles = new(mapPool) Particle*[MAX_PARTICLES];

		*this = emitter;
	}

	ParticleSystem::Emitter::~Emitter()
	{
		if(_texture)
			engineAPI.textureManager->ReleaseTexture(_texture);

		if(_model)
			engineAPI.modelManager->ReleaseModel(_model);
	}

	ParticleSystem::Emitter& ParticleSystem::Emitter::operator = (const Emitter& emitter)
	{
		_liveCount = 0;
		_enabled = emitter._enabled;

		strcpy(_name, emitter._name);
		_type = emitter._type;
		_shader = emitter._shader;

		if(emitter._texture)
			_texture = engineAPI.textureManager->CreateTexture(emitter._texture->GetFileName(), true);
		else
			_texture = 0;

		if(_model)
			_model = engineAPI.modelManager->CreateModel(emitter._model->GetFileName(), true);
		else
			_model = 0;

		_lifeStart = emitter._lifeStart;
		_life = emitter._life;
		_loop = emitter._loop;
		_implode = emitter._implode;
		_emitFromEdge = emitter._emitFromEdge;
		_animatedTex = emitter._animatedTex;
		Velocity = emitter.Velocity;
		Size = emitter.Size;
		EmitAngle = emitter.EmitAngle;
		EmitRate = emitter.EmitRate;
		OffsetX = emitter.OffsetX;
		OffsetY = emitter.OffsetY;
		OffsetZ = emitter.OffsetZ;
		RotationX = emitter.RotationX;
		RotationY = emitter.RotationY;
		RotationZ = emitter.RotationZ;

		_partLife = emitter._partLife;
		_partRandomOrient = emitter._partRandomOrient;
		ParticleSize = emitter.ParticleSize;
		ParticleRotationSpeed = emitter.ParticleRotationSpeed;
		ParticleAlpha = emitter.ParticleAlpha;
		ParticleGravity = emitter.ParticleGravity;
		ParticleFriction = emitter.ParticleFriction;

		return *this;
	}

	void ParticleSystem::Emitter::Update(float frame_time)
	{
	}

	void ParticleSystem::Emitter::SetTexture(const tchar* file_name)
	{
		_texture = engineAPI.textureManager->CreateTexture(file_name, true);
		if(_texture)
		{
			engineAPI.modelManager->ReleaseModel(_model);
			_model = 0;
		}
	}

	void ParticleSystem::Emitter::ClearTexture()
	{
		if(_texture)
		{
			engineAPI.textureManager->ReleaseTexture(_texture);
			_texture = 0;
		}
	}

	void ParticleSystem::Emitter::SetModel(const tchar* file_name)
	{
		_model = engineAPI.modelManager->CreateModel(file_name, true);
		if(_model)
		{
			engineAPI.textureManager->ReleaseTexture(_texture);
			_texture = 0;
		}
	}

	void ParticleSystem::Emitter::ClearModel()
	{
		if(_model)
		{
			engineAPI.modelManager->ReleaseModel(_model);
			_model = 0;
		}
	}

	ParticleSystem::Emitter::ParticleType
		ParticleSystem::Emitter::GetParticleType() const
	{
		if(_texture)
			return PARTICLE_TYPE_TEXTURE;
		else if(_model)
			return PARTICLE_TYPE_MODEL;
		else
			return PARTICLE_TYPE_NONE;
	}

}
