
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

		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];
		char path[MAX_PATH];
		float ftmp;

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
				Emitter* emitter = new(mapPool) Emitter;

				char name[Emitter::EMITTER_NAME_MAX_LEN];
				if(!parser.ReadIdentifier(name, Emitter::EMITTER_NAME_MAX_LEN))
					{ delete emitter; Unload(); return false; }
				emitter->SetName(name);

				if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
					{ delete emitter; Unload(); return false; }

				if(!parser.ExpectTokenString("emitterType"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					{ delete emitter; Unload(); return false; }
				Emitter::EmitterType em_type = Emitter::GetEmitterTypeByString(buf);
				if(em_type == Emitter::EMITTER_TYPE_COUNT)
				{
					Console::PrintError("Invalid value for emitter type: \'%s\'.", buf);
					delete emitter;
					Unload();
					return false;
				}
				emitter->SetEmitterType(em_type);

				if(!parser.ExpectTokenString("emitterShader"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					{ delete emitter; Unload(); return false; }
				Emitter::EmitterShader em_shader = Emitter::GetEmitterShaderByString(buf);
				if(em_shader == Emitter::EMITTER_SHADER_COUNT)
				{
					Console::PrintError("Invalid value for emitter shader: \'%s\'.", buf);
					delete emitter;
					Unload();
					return false;
				}
				emitter->SetShader(em_shader);

				if(!parser.ExpectTokenString("particleType"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					{ delete emitter; Unload(); return false; }
				Emitter::ParticleType part_type = Emitter::GetParticleTypeByString(buf);
				if(part_type == Emitter::PARTICLE_TYPE_COUNT)
				{
					Console::PrintError("Invalid value for particle type: \'%s\'.", buf);
					delete emitter;
					Unload();
					return false;
				}

				if(!parser.ExpectTokenString("resource"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadString(path, MAX_PATH))
					{ delete emitter; Unload(); return false; }
				tchar* tpath = CharToWideChar(path);

				if(part_type == Emitter::PARTICLE_TYPE_TEXTURE)
					emitter->SetTexture(tpath, false);
				else if(part_type == Emitter::PARTICLE_TYPE_MODEL)
					emitter->SetModel(tpath, false);
				delete[] tpath;

				if(!parser.ExpectTokenString("lifeStart"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadFloat(ftmp))
					{ delete emitter; Unload(); return false; }
				emitter->SetLifeStart(ftmp);

				if(!parser.ExpectTokenString("life"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadFloat(ftmp))
					{ delete emitter; Unload(); return false; }
				emitter->SetLife(ftmp);

				if(!parser.ExpectTokenString("loop"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					return false;
				if(!strcmp(buf, "True"))
					emitter->SetLoop(true);
				else if(!strcmp(buf, "False"))
					emitter->SetLoop(false);
				else
				{
					Console::PrintError("Invalid value for \'loop\': %s", buf);
					delete emitter;
					Unload();
					return false;
				}

				if(!parser.ExpectTokenString("implode"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					return false;
				if(!strcmp(buf, "True"))
					emitter->SetImplode(true);
				else if(!strcmp(buf, "False"))
					emitter->SetImplode(false);
				else
				{
					Console::PrintError("Invalid value for \'implode\': %s", buf);
					delete emitter;
					Unload();
					return false;
				}

				if(!parser.ExpectTokenString("emitFromEdge"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					return false;
				if(!strcmp(buf, "True"))
					emitter->SetEmitFromEdge(true);
				else if(!strcmp(buf, "False"))
					emitter->SetEmitFromEdge(false);
				else
				{
					Console::PrintError("Invalid value for \'emitFromEdge\': %s", buf);
					delete emitter;
					Unload();
					return false;
				}

				if(!parser.ExpectTokenString("animatedTexture"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					return false;
				if(!strcmp(buf, "True"))
					emitter->SetAnimatedTex(true);
				else if(!strcmp(buf, "False"))
					emitter->SetAnimatedTex(false);
				else
				{
					Console::PrintError("Invalid value for \'animatedTexture\': %s", buf);
					delete emitter;
					Unload();
					return false;
				}

				if(!parser.ExpectTokenString("particleLife"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadFloat(ftmp))
					{ delete emitter; Unload(); return false; }
				emitter->SetParticleLife(ftmp);

				if(!parser.ExpectTokenString("particleRandomOrient"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					return false;
				if(!strcmp(buf, "True"))
					emitter->SetParticleRandomOrient(true);
				else if(!strcmp(buf, "False"))
					emitter->SetParticleRandomOrient(false);
				else
				{
					Console::PrintError("Invalid value for \'particleRandomOrient\': %s", buf);
					delete emitter;
					Unload();
					return false;
				}

				// attributes
				if(!parser.ExpectTokenString("attributes"))
					{ delete emitter; Unload(); return false; }

				if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
					return false;

				Attribute** attribs = emitter->GetAttributeArray();
				for(int i = 0; i < Emitter::ATTRIB_COUNT; ++i)
				{
					if(!ReadAttribute(parser, *attribs[i], Emitter::GetAttributeName(i)))
						{ delete emitter; Unload(); return false; }
				}

				// attributes close brace
				if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE))
					{ delete emitter; Unload(); return false; }

				// emitter close brace
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

		return true;
	}

	bool ParticleSystem::Save(const tchar* file_name) const
	{
		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(file_name, _t("wt"));
		if(!file)
		{
			Console::PrintError("Failed to write particle system file: %ls", file_name);
			return false;
		}

		const char* str = "";
		const tchar* tstr = _t("");

		file->Printf("// Daemonium engine particle system file\n\n");
		file->Printf("particleSystem\n{\n");

		for(List<Emitter*>::ConstIterator it = _emitters.Begin(); it != _emitters.End(); ++it)
		{
			Emitter& emitter = **it;
			file->Printf("\n\temitter\t%s\n\t{\n", emitter.GetName());
			file->Printf("\t\temitterType\t\t%s\n", Emitter::GetEmitterTypeString(emitter.GetEmitterType()));
			file->Printf("\t\temitterShader\t\t%s\n", Emitter::GetEmitterShaderString(emitter.GetShader()));
			file->Printf("\t\tparticleType\t\t%s\n", Emitter::GetParticleTypeString(emitter.GetParticleType()));

			if(emitter.GetParticleType() == Emitter::PARTICLE_TYPE_TEXTURE)
				tstr = emitter.GetTexture()? emitter.GetTexture()->GetFileName(): _t("");
			else if(emitter.GetParticleType() == Emitter::PARTICLE_TYPE_MODEL)
				tstr = emitter.GetModel()? emitter.GetModel()->GetFileName(): _t("");
			else
				tstr = _t("");
			file->Printf("\t\tresource\t\t\"%ls\"\n", tstr);

			file->Printf("\t\tlifeStart\t\t%f\n", emitter.GetLifeStart());
			file->Printf("\t\tlife\t\t%f\n", emitter.GetLife());
			file->Printf("\t\tloop\t\t%s\n", emitter.GetLoop()? "True": "False");
			file->Printf("\t\timplode\t\t%s\n", emitter.GetImplode()? "True": "False");
			file->Printf("\t\temitFromEdge\t\t%s\n", emitter.EmitFromEdge()? "True": "False");
			file->Printf("\t\tanimatedTexture\t\t%s\n", emitter.IsAnimatedTex()? "True": "False");
			file->Printf("\t\tparticleLife\t\t%f\n", emitter.GetParticleLife());
			file->Printf("\t\tparticleRandomOrient\t\t%s\n", emitter.GetParticleRandomOrient()? "True": "False");

			file->Printf("\n\t\tattributes\n\t\t{\n");
			Attribute** attribs = emitter.GetAttributeArray();
			for(int i = 0; i < Emitter::ATTRIB_COUNT; ++i)
			{
				int val_count = attribs[i]->GetValueCount();
				const Attribute::Value* values = attribs[i]->GetValues();
				file->Printf("\t\t\t%s\t\t%d\t\t{ ", Emitter::GetAttributeName(i), val_count);
				for(int j = 0; j < val_count; ++j)
				{
					file->Printf("[%f %f]", values[j].variable, values[j].value);
					if(j < val_count - 1)
						file->Printf(", ");
				}
				file->Printf(" }\n");
			}
			// attributes close brace
			file->Printf("\t\t}\n");

			// emitter close brace
			file->Printf("\t}\n");
		}

		// particleSystem close brace
		file->Printf("}\n");
		file->Close();

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

	bool ParticleSystem::ReadAttribute(Parser& parser, Attribute& attrib, const char* attr_name)
	{
		if(!parser.ExpectTokenString(attr_name))
			return false;
		int count;
		if(!parser.ReadInt(count))
			return false;

		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
			return false;

		Attribute::Value* values = new(tempPool) Attribute::Value[count];
		Parser::Token token;

		for(int i = 0; i < count; ++i)
		{
			float vec[2];
			if(!parser.ReadVec2f(vec))
				{ delete[] values; return false; }
			values[i].variable = vec[0];
			values[i].value = vec[1];

			if(!parser.ReadToken(token))
				{ delete[] values; return false; }

			if(	token.type != Parser::TOK_PUNCTUATION ||
				(token.subTypePunct != Parser::PUNCT_COMMA && token.subTypePunct != Parser::PUNCT_CLOSE_BRACE) )
			{
				delete[] values;
				return false;
			}
		}

		attrib.SetValues(values, count);
		delete[] values;

		return true;
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

	const char* ParticleSystem::Emitter::_emitterTypeStrings[EMITTER_TYPE_COUNT] =
	{
		"Sphere",
		"Plane",
		"Circle",
		"Line"
	};

	const char* ParticleSystem::Emitter::_emitterShaderStrings[EMITTER_SHADER_COUNT] =
	{
		"Blend",
		"Additive"
	};

	const char* ParticleSystem::Emitter::_particleTypeStrings[PARTICLE_TYPE_COUNT] =
	{
		"None",
		"Texture",
		"Model"
	};

	const char* ParticleSystem::Emitter::_attribNames[] =
	{
		"Velocity",
		"EmitterSize",
		"EmitAngle",
		"EmitRate",
		"OffsetX",
		"OffsetY",
		"OffsetZ",
		"RotationX",
		"RotationY",
		"RotationZ",
		"ParticleSize",
		"ParticleRotationSpeed",
		"ParticleAlpha",
		"ParticleGravity",
		"ParticleFriction"
	};


	ParticleSystem::Emitter::Emitter()
		: _particlePool(MAX_PARTICLES)
	{
		_liveParticles = new(mapPool) Particle*[MAX_PARTICLES];
		_liveCount = 0;
		_enabled = true;

		_attributes[0] = &Velocity;
		_attributes[1] = &Size;
		_attributes[2] = &EmitAngle;
		_attributes[3] = &EmitRate;
		_attributes[4] = &OffsetX;
		_attributes[5] = &OffsetY;
		_attributes[6] = &OffsetZ;
		_attributes[7] = &RotationX;
		_attributes[8] = &RotationY;
		_attributes[9] = &RotationZ;
		_attributes[10] = &ParticleSize;
		_attributes[11] = &ParticleRotationSpeed;
		_attributes[12] = &ParticleAlpha;
		_attributes[13] = &ParticleGravity;
		_attributes[14] = &ParticleFriction;

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

		_attributes[0] = &Velocity;
		_attributes[1] = &Size;
		_attributes[2] = &EmitAngle;
		_attributes[3] = &EmitRate;
		_attributes[4] = &OffsetX;
		_attributes[5] = &OffsetY;
		_attributes[6] = &OffsetZ;
		_attributes[7] = &RotationX;
		_attributes[8] = &RotationY;
		_attributes[9] = &RotationZ;
		_attributes[10] = &ParticleSize;
		_attributes[11] = &ParticleRotationSpeed;
		_attributes[12] = &ParticleAlpha;
		_attributes[13] = &ParticleGravity;
		_attributes[14] = &ParticleFriction;

		*this = emitter;
	}

	ParticleSystem::Emitter::~Emitter()
	{
		if(_texture)
			engineAPI.textureManager->ReleaseTexture(_texture);

		if(_model)
			engineAPI.modelManager->ReleaseModel(_model);

		delete[] _liveParticles;
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

	void ParticleSystem::Emitter::SetTexture(const tchar* file_name, bool immediate)
	{
		_texture = engineAPI.textureManager->CreateTexture(file_name, immediate);
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

	void ParticleSystem::Emitter::SetModel(const tchar* file_name, bool immediate)
	{
		_model = engineAPI.modelManager->CreateModel(file_name, immediate);
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

	ParticleSystem::Emitter::EmitterType
		ParticleSystem::Emitter::GetEmitterTypeByString(const char* name)
	{
		for(int i = 0; i < EMITTER_TYPE_COUNT; ++i)
		{
			if(!strcmp(name, _emitterTypeStrings[i]))
				return (EmitterType)i;
		}

		return EMITTER_TYPE_COUNT;
	}

	const char* ParticleSystem::Emitter::GetEmitterTypeString(EmitterType type)
	{
		if(type != EMITTER_TYPE_COUNT)
			return _emitterTypeStrings[type];
		else
			return "";
	}

	ParticleSystem::Emitter::EmitterShader
		ParticleSystem::Emitter::GetEmitterShaderByString(const char* name)
	{
		for(int i = 0; i < EMITTER_SHADER_COUNT; ++i)
		{
			if(!strcmp(name, _emitterShaderStrings[i]))
				return (EmitterShader)i;
		}

		return EMITTER_SHADER_COUNT;
	}

	const char* ParticleSystem::Emitter::GetEmitterShaderString(EmitterShader shader)
	{
		if(shader != EMITTER_SHADER_COUNT)
			return _emitterShaderStrings[shader];
		else
			return "";
	}

	ParticleSystem::Emitter::ParticleType
		ParticleSystem::Emitter::GetParticleTypeByString(const char* name)
	{
		for(int i = 0; i < PARTICLE_TYPE_COUNT; ++i)
		{
			if(!strcmp(name, _particleTypeStrings[i]))
				return (ParticleType)i;
		}

		return PARTICLE_TYPE_COUNT;
	}

	const char* ParticleSystem::Emitter::GetParticleTypeString(ParticleType type)
	{
		if(type != PARTICLE_TYPE_COUNT)
			return _particleTypeStrings[type];
		else
			return "";
	}

}
