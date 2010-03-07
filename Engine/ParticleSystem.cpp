
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Console.h"
#include "BaseLib/Parser.h"
#include "BaseLib/GL/GLTexture.h"
#include "EngineInternal.h"
#include "Camera.h"
#include "World.h"
#include "FileSystem.h"
#include "Resource.h"
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
		AABBox bbox(vec3f(-1.0f, -1.0f, -1.0f), vec3f(1.0f, 1.0f, 1.0f));
		SetObjectBoundingBox(bbox);
		_frameTime = 0.0f;
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

		SetObjectBoundingBox(psys.GetObjectBoundingBox());

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

		if(!parser.ExpectTokenString("boundingBox"))
			return false;
		AABBox bbox;
		if(!parser.ReadVec3f(bbox.minPt))
			return false;
		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_COMMA))
			return false;
		if(!parser.ReadVec3f(bbox.maxPt))
			return false;
		SetObjectBoundingBox(bbox);

		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];
		char path[MAX_PATH];
		float ftmp;
		int itmp;

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
				delete[] tpath;

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

				if(!parser.ExpectTokenString("animatedTextureFPS"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadInt(itmp))
					{ delete emitter; Unload(); return false; }
				emitter->SetAnimatedTexFPS(itmp);

				if(!parser.ExpectTokenString("initialRotation"))
					{ delete emitter; Unload(); return false; }
				vec3f init_rot;
				if(!parser.ReadVec3f(init_rot))
					{ delete emitter; Unload(); return false; }
				emitter->SetInitialRotation(init_rot);

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

				if(!parser.ExpectTokenString("particleRandomRotDir"))
					{ delete emitter; Unload(); return false; }
				if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
					return false;
				if(!strcmp(buf, "True"))
					emitter->SetParticleRandomRotDir(true);
				else if(!strcmp(buf, "False"))
					emitter->SetParticleRandomRotDir(false);
				else
				{
					Console::PrintError("Invalid value for \'particleRandomRotDir\': %s", buf);
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

		Reset();
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

		const AABBox& bbox = GetObjectBoundingBox();
		file->Printf("\tboundingBox\t\t[%f %f %f], [%f %f %f]\n",
			bbox.minPt.x, bbox.minPt.y, bbox.minPt.z,
			bbox.maxPt.x, bbox.maxPt.y, bbox.maxPt.z);

		for(List<Emitter*>::ConstIterator it = _emitters.Begin(); it != _emitters.End(); ++it)
		{
			Emitter& emitter = **it;
			file->Printf("\n\temitter\t%s\n\t{\n", emitter.GetName());
			file->Printf("\t\temitterType\t\t%s\n", Emitter::GetEmitterTypeString(emitter.GetEmitterType()));
			file->Printf("\t\temitterShader\t\t%s\n", Emitter::GetEmitterShaderString(emitter.GetShader()));
			file->Printf("\t\tparticleType\t\t%s\n", Emitter::GetParticleTypeString(emitter.GetParticleType()));

			if(emitter.GetParticleType() == Emitter::PARTICLE_TYPE_TEXTURE)
				tstr = emitter.GetTexture()? emitter.GetTexture().GetFileRes()->GetFileName(): _t("");
			else
				tstr = _t("");
			file->Printf("\t\tresource\t\t\"%ls\"\n", tstr);

			file->Printf("\t\tlife\t\t%f\n", emitter.GetLife());
			file->Printf("\t\tloop\t\t%s\n", emitter.GetLoop()? "True": "False");
			file->Printf("\t\timplode\t\t%s\n", emitter.GetImplode()? "True": "False");
			file->Printf("\t\temitFromEdge\t\t%s\n", emitter.EmitFromEdge()? "True": "False");
			file->Printf("\t\tanimatedTexture\t\t%s\n", emitter.IsAnimatedTex()? "True": "False");
			file->Printf("\t\tanimatedTextureFPS\t\t%d\n", emitter.GetAnimatedTexFPS());
			file->Printf("\t\tinitialRotation\t\t[%f %f %f]\n", emitter.GetInitialRotation().x, emitter.GetInitialRotation().y, emitter.GetInitialRotation().z);
			file->Printf("\t\tparticleLife\t\t%f\n", emitter.GetParticleLife());
			file->Printf("\t\tparticleRandomOrient\t\t%s\n", emitter.GetParticleRandomOrient()? "True": "False");
			file->Printf("\t\tparticleRandomRotDir\t\t%s\n", emitter.GetParticleRandomRotDir()? "True": "False");

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

	void ParticleSystem::UpdateTime(float frame_time)
	{
		_frameTime = frame_time;
	}

	void ParticleSystem::UpdateGraphics()
	{
		for(List<Emitter*>::Iterator it = _emitters.Begin(); it != _emitters.End(); ++it)
		{
			(*it)->Update(_frameTime, GetWorldTransform());
		}
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

	void ParticleSystem::Reset()
	{
		for(List<Emitter*>::Iterator it = _emitters.Begin(); it != _emitters.End(); ++it)
		{
			(*it)->Reset();
		}

		_frameTime = 0.0f;
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
		_values = 0;
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
		_liveParticles[0] = new(mapPool) Particle*[MAX_PARTICLES];
		_liveParticles[1] = new(mapPool) Particle*[MAX_PARTICLES];
		_partBufInd = 0;
		_liveCount = 0;
		_peakCount = 0;
		_age = 0.0f;
		_emitCount = 0.0f;
		_enabled = true;
		_alive = true;
		_emitterPos.set_null();
		_worldMatrix.set_identity();
		_rotation.set_null();
		_initialRotation.set_null();
		_curSize = EmitterSize.Evaluate(0.0f);

		_attributes[0] = &Velocity;
		_attributes[1] = &EmitterSize;
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
		_texture = Texture2DResPtr::null;
		_life = 10.0f;
		_loop = false;
		_implode = false;
		_emitFromEdge = false;
		_animatedTex = false;
		_texFrameCount = 0;
		_animTexFPS = 15;

		_partLife = 10.0f;
		_partRandomOrient = false;
		_partRandomRotDir = false;
	}

	ParticleSystem::Emitter::Emitter(const Emitter& emitter)
		: _particlePool(MAX_PARTICLES)
	{
		_liveParticles[0] = new(mapPool) Particle*[MAX_PARTICLES];
		_liveParticles[1] = new(mapPool) Particle*[MAX_PARTICLES];
		_partBufInd = 0;

		_attributes[0] = &Velocity;
		_attributes[1] = &EmitterSize;
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

		delete[] _liveParticles[0];
		delete[] _liveParticles[1];
	}

	ParticleSystem::Emitter& ParticleSystem::Emitter::operator = (const Emitter& emitter)
	{
		_liveCount = 0;
		_peakCount = 0;
		_age = 0.0f;
		_emitCount = 0.0f;
		_enabled = emitter._enabled;
		_alive = true;
		_emitterPos = emitter._emitterPos;
		_worldMatrix = emitter._worldMatrix;
		_rotation = emitter._rotation;
		_initialRotation = emitter._initialRotation;
		_curSize = emitter.EmitterSize.Evaluate(0.0f);

		strcpy(_name, emitter._name);
		_type = emitter._type;
		_shader = emitter._shader;

		if(emitter._texture)
			_texture = engineAPI.textureManager->CreateTexture2D(emitter._texture.GetFileRes()->GetFileName(), true);
		else
			_texture = Texture2DResPtr::null;

		_life = emitter._life;
		_loop = emitter._loop;
		_implode = emitter._implode;
		_emitFromEdge = emitter._emitFromEdge;
		_animatedTex = emitter._animatedTex;
		_texFrameCount = emitter._texFrameCount;
		_animTexFPS = emitter._animTexFPS;

		Velocity = emitter.Velocity;
		EmitterSize = emitter.EmitterSize;
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
		_partRandomRotDir = emitter._partRandomRotDir;
		ParticleSize = emitter.ParticleSize;
		ParticleRotationSpeed = emitter.ParticleRotationSpeed;
		ParticleAlpha = emitter.ParticleAlpha;
		ParticleGravity = emitter.ParticleGravity;
		ParticleFriction = emitter.ParticleFriction;

		return *this;
	}

	static
	int ParticleCompareFunc(const void* el1, const void* el2)
	{
		const ParticleSystem::Particle* part1 = *(ParticleSystem::Particle**)el1;
		const ParticleSystem::Particle* part2 = *(ParticleSystem::Particle**)el2;

		const vec3f& cam_pos = engineAPI.world->GetCamera().GetPosition();
		float dif = (part1->position - cam_pos).length_sq() - (part2->position - cam_pos).length_sq();
		return (dif > 0.0f)? -1: 1;
	}

	void ParticleSystem::Emitter::Update(float frame_time, const mat4f& psys_world_mat)
	{
		if(!_enabled)
			return;

		if(_animatedTex && _texture.IsValid())
		{
			_texFrameCount = _texture->GetWidth() / _texture->GetHeight();
		}

		// update existing particles
		int buf_ind = (_partBufInd + 1) & 1;
		int count = 0;
		for(int i = 0; i < _liveCount; ++i)
		{
			_liveParticles[_partBufInd][i]->age += frame_time;
			if(_liveParticles[_partBufInd][i]->age <= _partLife)
			{
				_liveParticles[buf_ind][count] = _liveParticles[_partBufInd][i];
				Particle* part = _liveParticles[buf_ind][count];
				count++;

				float t = part->age / _partLife;

				float part_size = ParticleSize.Evaluate(t);
				float part_rot_speed = ParticleRotationSpeed.Evaluate(t);
				float part_alpha = ParticleAlpha.Evaluate(t);
				float part_grav = ParticleGravity.Evaluate(t);
				float part_frict = ParticleFriction.Evaluate(t);

				part->size = part_size;
				if(_implode)
				{
					// check if particle is at emitter center
					vec3f old_pos = part->position;
					part->position += part->velocity * frame_time;
					vec3f v1 = part->position - _emitterPos;
					vec3f v2 = old_pos - _emitterPos;
					if(dot(v1, v2) < 0.0f)
					{
						_particlePool.Delete(part);
						count--;
						continue;
					}
				}
				else
				{
					part->position += part->velocity * frame_time;
					part->velocity -= part->velocity * part_frict;
					part->velocity.y -= part_grav * frame_time;
				}
				part->rotation += part->rotDir * part_rot_speed * frame_time;
				wrap(part->rotation, 0.0f, 360.0f);
				part->alpha = part_alpha;

				if(_animatedTex)
				{
					part->frame += _animTexFPS * frame_time;
					if(part->frame > (float)_texFrameCount)
						part->frame = fmod(part->frame, (float)_texFrameCount);
				}
			}
			else
			{
				_particlePool.Delete(_liveParticles[_partBufInd][i]);
			}
		}
		_partBufInd = buf_ind;
		_liveCount = count;

		if(_alive)
		{
			// update emitter age
			float emitter_dt = frame_time;
			float new_age = _age + frame_time;
			if(new_age > _life)
			{
				if(_loop)
				{
					_age = new_age - _life;
				}
				else
				{
					emitter_dt = _life - _age;
					_alive = false;
					_age = _life;
				}
			}
			else
			{
				_age = new_age;
			}

			// update emitter attributes
			float t = _age / _life;

			float velocity = Velocity.Evaluate(t);
			float em_size = EmitterSize.Evaluate(t);
			float em_angle = EmitAngle.Evaluate(t);
			float em_rate = EmitRate.Evaluate(t);
			float offs_x = OffsetX.Evaluate(t);
			float offs_y = OffsetY.Evaluate(t);
			float offs_z = OffsetZ.Evaluate(t);
			float rot_x = RotationX.Evaluate(t);
			float rot_y = RotationY.Evaluate(t);
			float rot_z = RotationZ.Evaluate(t);

			// update emitter position and direction
			mat3f rot, temp;
			_rotation.x += rot_x * emitter_dt;
			wrap(_rotation.x, 0.0f, 360.0f);

			_rotation.y += rot_y * emitter_dt;
			wrap(_rotation.y, 0.0f, 360.0f);

			_rotation.z += rot_z * emitter_dt;
			wrap(_rotation.z, 0.0f, 360.0f);

			mat4f local_mat;
			local_mat.from_euler(deg2rad(_rotation.x), deg2rad(_rotation.y), deg2rad(_rotation.z));
			vec3f em_pos;
			transform(em_pos, vec3f(offs_x, offs_y, offs_z), local_mat);
			local_mat.row3.rvec3 = em_pos;
			mul(_worldMatrix, local_mat, psys_world_mat);
			_emitterPos = _worldMatrix.row3.rvec3;

			_curSize = em_size;

			// generate new particles
			_emitCount += em_rate * emitter_dt;
			int emit_count = Min((int)_emitCount, MAX_PARTICLES - _liveCount);
			_emitCount = frac(_emitCount);

			if(emit_count)
			{
				switch(_type)
				{
				case EMITTER_TYPE_SPHERE:
					EmitSphere(emit_count, em_size, em_angle, velocity);
					break;
				case EMITTER_TYPE_PLANE:
					EmitPlane(emit_count, em_size, em_angle, velocity);
					break;
				case EMITTER_TYPE_CIRCLE:
					EmitCircle(emit_count, em_size, em_angle, velocity);
					break;
				case EMITTER_TYPE_LINE:
					EmitLine(emit_count, em_size, em_angle, velocity);
					break;
				}
			}
		}

		if(_liveCount > _peakCount)
			_peakCount = _liveCount;

		if(_liveCount)
			qsort(_liveParticles[_partBufInd], _liveCount, sizeof(Particle*), ParticleCompareFunc);
	}

	void ParticleSystem::Emitter::Reset()
	{
		_particlePool.Reset();
		_partBufInd = 0;
		_liveCount = 0;
		_peakCount = 0;
		_age = 0.0f;
		_alive = true;
		_emitCount = 0.0f;
		_emitterPos.set_null();
		_worldMatrix.set_identity();
		_rotation = _initialRotation;
		_curSize = EmitterSize.Evaluate(0.0f);
	}

	void ParticleSystem::Emitter::EmitSphere(int count, float em_size, float em_angle, float velocity)
	{
		for(int i = 0; i < count; ++i)
		{
			Particle* part = _particlePool.New();
			if(!part)
				return;

			_liveParticles[_partBufInd][_liveCount++] = part;
			part->age = 0.0f;
			part->alpha = ParticleAlpha.Evaluate(0.0f);
			part->size = ParticleSize.Evaluate(0.0f);

			{
				vec3f dir;
				dir.z = 2.0f * frand<float>() - 1.0f;
				float t = TWO_PI * frand<float>();
				float w = sqrt(1.0f - dir.z * dir.z);
				dir.x = w * cos(t);
				dir.y = w * sin(t);

				if(_emitFromEdge)
					part->position = _emitterPos + dir * (em_size * 0.5f);
				else
					part->position = _emitterPos + dir * (frand<float>() * em_size * 0.5f);
			}

			if(_partRandomOrient)
				part->rotation = 360.0f * frand<float>();
			else
				part->rotation = 0.0f;

			if(_partRandomRotDir)
				part->rotDir = (rand() % 2)? 1.0f: -1.0f;
			else
				part->rotDir = 1.0f;

			if(_implode)
			{
				part->velocity = _emitterPos - part->position;
				part->velocity.normalize();
				part->velocity *= velocity;
			}
			else
			{
				vec3f dir, rdir;
				float r = em_angle / 360.0f;
				dir.y = 2.0f * ((1.0f - r) + frand<float>() * r) - 1.0f;
				float t = TWO_PI * frand<float>();
				float w = sqrt(1.0f - dir.y * dir.y);
				dir.x = w * sin(t);
				dir.z = w * cos(t);
				transform_dir(rdir, dir, _worldMatrix);

				part->velocity = rdir * velocity;
			}

			part->frame = 0.0f;
		}
	}

	void ParticleSystem::Emitter::EmitPlane(int count, float em_size, float em_angle, float velocity)
	{
		for(int i = 0; i < count; ++i)
		{
			Particle* part = _particlePool.New();
			if(!part)
				return;

			_liveParticles[_partBufInd][_liveCount++] = part;
			part->age = 0.0f;
			part->alpha = ParticleAlpha.Evaluate(0.0f);
			part->size = ParticleSize.Evaluate(0.0f);

			if(_emitFromEdge)
			{
				vec3f dir_x, dir_z;
				if(rand() % 2)
				{
					dir_x = (2.0f * frand<float>() - 1.0f) * em_size * 0.5f * _worldMatrix.row0.rvec3;
					dir_z = (em_size * 0.5f * ((rand() % 2)? -1.0f: 1.0f)) * _worldMatrix.row2.rvec3;
				}
				else
				{
					dir_x = (em_size * 0.5f * ((rand() % 2)? -1.0f: 1.0f)) * _worldMatrix.row0.rvec3;
					dir_z = (2.0f * frand<float>() - 1.0f) * em_size * 0.5f * _worldMatrix.row2.rvec3;
				}
				part->position = _emitterPos + dir_x + dir_z;
			}
			else
			{
				vec3f dir_x = (2.0f * frand<float>() - 1.0f) * em_size * 0.5f * _worldMatrix.row0.rvec3;
				vec3f dir_z = (2.0f * frand<float>() - 1.0f) * em_size * 0.5f * _worldMatrix.row2.rvec3;
				part->position = _emitterPos + dir_x + dir_z;
			}

			if(_partRandomOrient)
				part->rotation = 360.0f * frand<float>();
			else
				part->rotation = 0.0f;

			if(_partRandomRotDir)
				part->rotDir = (rand() % 2)? 1.0f: -1.0f;
			else
				part->rotDir = 1.0f;

			if(_implode)
			{
				part->velocity = _emitterPos - part->position;
				part->velocity.normalize();
				part->velocity *= velocity;
			}
			else
			{
				vec3f dir, rdir;
				float r = em_angle / 360.0f;
				dir.y = 2.0f * ((1.0f - r) + frand<float>() * r) - 1.0f;
				float t = TWO_PI * frand<float>();
				float w = sqrt(1.0f - dir.y * dir.y);
				dir.x = w * sin(t);
				dir.z = w * cos(t);
				transform_dir(rdir, dir, _worldMatrix);

				part->velocity = rdir * velocity;
			}

			part->frame = 0.0f;
		}
	}

	void ParticleSystem::Emitter::EmitCircle(int count, float em_size, float em_angle, float velocity)
	{
		for(int i = 0; i < count; ++i)
		{
			Particle* part = _particlePool.New();
			if(!part)
				return;

			_liveParticles[_partBufInd][_liveCount++] = part;
			part->age = 0.0f;
			part->alpha = ParticleAlpha.Evaluate(0.0f);
			part->size = ParticleSize.Evaluate(0.0f);

			{
				float t = TWO_PI * frand<float>();
				vec3f dir_x = _worldMatrix.row0.rvec3 * cos(t);
				vec3f dir_z = _worldMatrix.row2.rvec3 * sin(t);

				if(_emitFromEdge)
					part->position = _emitterPos + (dir_x + dir_z) * (em_size * 0.5f);
				else
					part->position = _emitterPos + (dir_x + dir_z) * (frand<float>() * em_size * 0.5f);
			}

			if(_partRandomOrient)
				part->rotation = 360.0f * frand<float>();
			else
				part->rotation = 0.0f;

			if(_partRandomRotDir)
				part->rotDir = (rand() % 2)? 1.0f: -1.0f;
			else
				part->rotDir = 1.0f;

			if(_implode)
			{
				part->velocity = _emitterPos - part->position;
				part->velocity.normalize();
				part->velocity *= velocity;
			}
			else
			{
				vec3f dir, rdir;
				float r = em_angle / 360.0f;
				dir.y = 2.0f * ((1.0f - r) + frand<float>() * r) - 1.0f;
				float t = TWO_PI * frand<float>();
				float w = sqrt(1.0f - dir.y * dir.y);
				dir.x = w * sin(t);
				dir.z = w * cos(t);
				transform_dir(rdir, dir, _worldMatrix);

				part->velocity = rdir * velocity;
			}

			part->frame = 0.0f;
		}
	}

	void ParticleSystem::Emitter::EmitLine(int count, float em_size, float em_angle, float velocity)
	{
		for(int i = 0; i < count; ++i)
		{
			Particle* part = _particlePool.New();
			if(!part)
				return;

			_liveParticles[_partBufInd][_liveCount++] = part;
			part->age = 0.0f;
			part->alpha = ParticleAlpha.Evaluate(0.0f);
			part->size = ParticleSize.Evaluate(0.0f);

			part->position = _emitterPos + (2.0f * frand<float>() - 1.0f) * em_size * 0.5f * _worldMatrix.row0.rvec3;

			if(_partRandomOrient)
				part->rotation = 360.0f * frand<float>();
			else
				part->rotation = 0.0f;

			if(_partRandomRotDir)
				part->rotDir = (rand() % 2)? 1.0f: -1.0f;
			else
				part->rotDir = 1.0f;

			if(_implode)
			{
				part->velocity = _emitterPos - part->position;
				part->velocity.normalize();
				part->velocity *= velocity;
			}
			else
			{
				vec3f dir, rdir;
				float r = em_angle / 360.0f;
				dir.y = 2.0f * ((1.0f - r) + frand<float>() * r) - 1.0f;
				float t = TWO_PI * frand<float>();
				float w = sqrt(1.0f - dir.y * dir.y);
				dir.x = w * sin(t);
				dir.z = w * cos(t);
				transform_dir(rdir, dir, _worldMatrix);

				part->velocity = rdir * velocity;
			}

			part->frame = 0.0f;
		}
	}

	void ParticleSystem::Emitter::SetTexture(const tchar* file_name, bool immediate)
	{
		if(_texture)
			engineAPI.textureManager->ReleaseTexture(_texture);
		_texture = engineAPI.textureManager->CreateTexture2D(file_name, immediate);
	}

	void ParticleSystem::Emitter::ClearTexture()
	{
		if(_texture)
		{
			engineAPI.textureManager->ReleaseTexture(_texture);
			_texture = Texture2DResPtr::null;
		}
	}

	ParticleSystem::Emitter::ParticleType
		ParticleSystem::Emitter::GetParticleType() const
	{
		if(_texture)
			return PARTICLE_TYPE_TEXTURE;
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
