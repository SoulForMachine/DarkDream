
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Memory.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Console.h"
#include "Model.h"
#include "ParticleSystem.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "EngineInternal.h"
#include "ModelEntity.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	const char* ModelEntity::_classNames[MODEL_CLASS_COUNT] =
	{
		"Generic",
		"NPC",
		"Monster",
		"Boss",
		"Building",
		"Foliage",
		"Debris",
	};

	ModelEntity::ModelEntity()
	{
		_worldMat.set_identity();
		_animTime = 0.0f;
		_curAnim = 0;

		_model = 0;
		_aiScript = 0;

		// set initial properties
		_class = MODEL_CLASS_GENERIC;
		_name[0] = '\0';
		_clip = true;;
		_lifePoints = 100;
		_transparency = 0.0f;
	}

	ModelEntity::ModelEntity(const ModelEntity& entity)
	{
		_model = 0;
		_aiScript = 0;
		*this = entity;
	}

	ModelEntity::~ModelEntity()
	{
		Unload();
	}

	ModelEntity& ModelEntity::operator = (const ModelEntity& entity)
	{
		Unload();

		_worldMat = entity._worldMat;

		// properties
		_class = entity._class;
		strcpy(_name, entity._name);
		_clip = entity._clip;
		_lifePoints = entity._lifePoints;
		_transparency = entity._transparency;

		// resources
		if(entity._model)
			_model = engineAPI.modelManager->CreateModel(entity._model->GetFileName());

		if(entity._aiScript)
			_aiScript = engineAPI.aiScriptManager->CreateAIScript(entity._aiScript->GetFileName());

		for(JointAttachMap::ConstIterator it = entity._jointAttachments.Begin(); it != entity._jointAttachments.End(); ++it)
		{
			JointAttachData jd;
			jd.name = StringDup(it->name);
			jd.type = it->type;
			if(jd.type == JOINT_ATTACH_MODEL)
				jd.attachment = new(mapPool) ModelEntityRes((const ModelEntityRes&)*it->attachment);
			else if(jd.type == JOINT_ATTACH_PARTICLE_SYSTEM)
				jd.attachment = new(mapPool) PartSysRes((const PartSysRes&)*it->attachment);
			_jointAttachments[it->name] = jd;
		}

		for(AnimMap::ConstIterator it = entity._animations.Begin(); it != entity._animations.End(); ++it)
		{
			AnimData ad;
			ad.name = StringDup(it->name);
			ad.type = it->type;
			ad.animation = engineAPI.animationManager->CreateAnimation(it->animation->GetFileName());
			_animations[it->name] = ad;
		}

		for(SoundMap::ConstIterator it = entity._sounds.Begin(); it != entity._sounds.End(); ++it)
		{
			SoundData sd;
			sd.name = StringDup(it->name);
			sd.sound = engineAPI.soundManager->CreateSound(it->sound->GetFileName());
			_sounds[it->name] = sd;
		}

		ClearModelData();
		SetupModelData();

		return *this;
	}

	bool ModelEntity::Load(const tchar* file_name)
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

		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];

		if(!parser.ExpectTokenString("entity"))
			return false;
		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
			return false;

		// read properties
		if(!parser.ExpectTokenString("properties"))
			return false;
		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
			return false;
		if(!parser.ExpectTokenString("class"))
			return false;
		if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
			return false;
		_class = GetClassFromString(buf);
		if(_class == MODEL_CLASS_COUNT)
		{
			Console::PrintError("Invalid value for entity class: %s", buf);
			return false;
		}
		if(!parser.ExpectTokenString("name"))
			return false;
		if(!parser.ReadString(_name, MAX_NAME_LENGTH))
			return false;
		if(!parser.ExpectTokenString("clip"))
			return false;
		if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
			return false;
		if(!strcmp(buf, "True"))
			_clip = true;
		else if(!strcmp(buf, "False"))
			_clip = false;
		else
		{
			Console::PrintError("Invalid value for clip: %s", buf);
			return false;
		}
		if(!parser.ExpectTokenString("lifePoints"))
			return false;
		if(!parser.ReadInt(_lifePoints))
			return false;
		if(!parser.ExpectTokenString("transparency"))
			return false;
		if(!parser.ReadFloat(_transparency))
			return false;
		// properties closing brace
		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE))
			return false;

		// read entity resources

		char path[MAX_PATH];

		if(!parser.ExpectTokenString("model"))
			return false;
		if(!parser.ReadString(path, MAX_PATH))
			return false;
		tchar* p = CharToWideChar(path);
		_model = engineAPI.modelManager->CreateModel(p);
		delete[] p;

		if(!parser.ExpectTokenString("aiScript"))
			{ Unload(); return false; }
		if(!parser.ReadString(path, MAX_PATH))
			{ Unload(); return false; }
		p = CharToWideChar(path);
		_aiScript = engineAPI.aiScriptManager->CreateAIScript(p);
		delete[] p;

		Parser::Token tok_name;

		// joint attachments
		{
			if(!parser.ExpectTokenString("jointAttachments"))
				{ Unload(); return false; }
			if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
				{ Unload(); return false; }
			JointAttachData jd;
			while(1)
			{
				if(!parser.ReadToken(tok_name))
					{ Unload(); return false; }
				if(tok_name.type == Parser::TOK_PUNCTUATION &&
					tok_name.subTypePunct == Parser::PUNCT_CLOSE_BRACE)
				{
					break;
				}
				else if(tok_name.type == Parser::TOK_IDENTIFIER)
				{
					jd.name = StringDup(tok_name.str);
				}
				else
				{
					Console::PrintError("Expected joint attachment name, found \'%s\'", tok_name.str);
					Unload();
					return false;
				}

				if(!parser.ReadString(path, MAX_PATH))
				{
					Console::PrintError("Expected joint attachment path");
					Unload();
					return false;
				}
				else
				{
					tchar* p = CharToWideChar(path);
					jd.type = GetJointAttachTypeByExt(p);
					if(jd.type == JOINT_ATTACH_MODEL)
					{
						jd.attachment = engineAPI.modelEntityManager->CreateEntity(p);
					}
					else if(jd.type == JOINT_ATTACH_PARTICLE_SYSTEM)
					{
						jd.attachment = engineAPI.partSysManager->CreatePartSys(p);
					}
					delete[] p;
					JointAttachMap::ConstIterator it = _jointAttachments.Find(jd.name);
					if(it != _jointAttachments.End())
					{
						Console::PrintWarning("Duplicate joint name found: %s", jd.name);
						delete[] jd.name;
					}
					else
					{
						_jointAttachments[jd.name] = jd;
					}
				}
			}
		}

		// animations
		{
			if(!parser.ExpectTokenString("animations"))
				{ Unload(); return false; }
			if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
				{ Unload(); return false; }
			AnimData ad;
			while(1)
			{
				if(!parser.ReadToken(tok_name))
					{ Unload(); return false; }
				if(tok_name.type == Parser::TOK_PUNCTUATION &&
					tok_name.subTypePunct == Parser::PUNCT_CLOSE_BRACE)
				{
					break;
				}
				else if(tok_name.type == Parser::TOK_IDENTIFIER)
				{
					ad.name = StringDup(tok_name.str);
				}
				else
				{
					Console::PrintError("Expected animation name, found \'%s\'", tok_name.str);
					Unload();
					return false;
				}

				if(!parser.ReadString(path, MAX_PATH))
				{
					Console::PrintError("Expected animation path");
					Unload();
					return false;
				}
				else
				{
					tchar* p = CharToWideChar(path);
					ad.type = Animation::GetAnimTypeByName(ad.name);
					ad.animation = engineAPI.animationManager->CreateAnimation(p);
					delete[] p;
					AnimMap::ConstIterator it = _animations.Find(ad.name);
					if(it != _animations.End())
					{
						Console::PrintWarning("Duplicate animation name found: %s", ad.name);
						delete[] ad.name;
					}
					else
					{
						_animations[ad.name] = ad;
					}
				}
			}
		}

		// sounds
		{
			if(!parser.ExpectTokenString("sounds"))
				{ Unload(); return false; }
			if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
				{ Unload(); return false; }
			SoundData sd;
			while(1)
			{
				if(!parser.ReadToken(tok_name))
					{ Unload(); return false; }
				if(tok_name.type == Parser::TOK_PUNCTUATION &&
					tok_name.subTypePunct == Parser::PUNCT_CLOSE_BRACE)
				{
					break;
				}
				else if(tok_name.type == Parser::TOK_IDENTIFIER)
				{
					sd.name = StringDup(tok_name.str);
				}
				else
				{
					Console::PrintError("Expected sound name, found \'%s\'", tok_name.str);
					Unload();
					return false;
				}

				if(!parser.ReadString(path, MAX_PATH))
				{
					Console::PrintError("Expected sound path");
					Unload();
					return false;
				}
				else
				{
					tchar* p = CharToWideChar(path);
					sd.sound = engineAPI.soundManager->CreateSound(p);
					delete[] p;
					SoundMap::ConstIterator it = _sounds.Find(sd.name);
					if(it != _sounds.End())
					{
						Console::PrintWarning("Duplicate sound name found: %s", sd.name);
						delete[] sd.name;
					}
					else
					{
						_sounds[sd.name] = sd;
					}
				}
			}
		}

		// entity closing brace
		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE))
			{ Unload(); return false; }

		return true;
	}

	bool ModelEntity::Save(const tchar* file_name)
	{
		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(file_name, _t("wt"));
		if(!file)
		{
			Console::PrintError("Failed to write entity file: %ls", file_name);
			return false;
		}

		file->Printf("// Daemonium engine entity file\n\n");
		file->Printf("entity\n{\n");

		// properties
		file->Printf("\tproperties\n\t{\n");
		file->Printf("\t\tclass\t\t%s\n", GetClassString(_class));
		file->Printf("\t\tname\t\t\"%s\"\n", _name);
		file->Printf("\t\tclip\t\t%s\n", _clip? "True": "False");
		file->Printf("\t\tlifePoints\t\t%d\n", _lifePoints);
		file->Printf("\t\ttransparency\t\t%f\n", _transparency);
		file->Printf("\t}\n\n");

		// entity resources

		const tchar* fn;

		fn = _model? _model->GetFileName(): _T("");
		file->Printf("\tmodel\t\t\"%ls\"\n", fn);

		fn = _aiScript? _aiScript->GetFileName(): _T("");
		file->Printf("\taiScript\t\t\"%ls\"\n\n", fn);

		// joint attachments
		file->Printf("\tjointAttachments\n\t{\n");
		for(JointAttachMap::ConstIterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
		{
			file->Printf("\t\t%s\t\t\"%ls\"\n", it->name, it->attachment->GetFileName());
		}
		file->Printf("\t}\n\n");

		// animations
		file->Printf("\tanimations\n\t{\n");
		for(AnimMap::ConstIterator it = _animations.Begin(); it != _animations.End(); ++it)
		{
			file->Printf("\t\t%s\t\t\"%ls\"\n", it->name, it->animation->GetFileName());
		}
		file->Printf("\t}\n\n");

		// sounds
		file->Printf("\tsounds\n\t{\n");
		for(SoundMap::ConstIterator it = _sounds.Begin(); it != _sounds.End(); ++it)
		{
			file->Printf("\t\t%s\t\t\"%ls\"\n", it->name, it->sound->GetFileName());
		}
		file->Printf("\t}\n\n");

		file->Printf("}\n"); // entity

		return true;
	}

	void ModelEntity::Unload()
	{
		if(_model)
		{
			engineAPI.modelManager->ReleaseModel(_model);
			_model = 0;
		}

		if(_aiScript)
		{
			engineAPI.aiScriptManager->ReleaseAIScript(_aiScript);
			_aiScript = 0;
		}

		ClearModelData();
	}

	void ModelEntity::SetWorldTransform(const mat4f& transform)
	{
		_worldMat = transform;
		_position = _worldMat.transl_vec.rvec3;
		CalcWorldBBox();
	}

	void ModelEntity::UpdateGraphics(int frame_time)
	{
		float tsec = frame_time * 0.001f;

		if(_curAnim)
		{
			const Animation* anim = _curAnim->animation->GetAnimation();
			_animTime += tsec;
			if(_animTime > anim->GetLength())
				_animTime = fmod(_animTime, anim->GetLength());

			anim->GetJointTransforms(
				_animTime,
				_jointMatPalette.GetData(),
				_model->GetModel()->GetRootJoint(),
				_model->GetModel()->GetJoints().GetData());
		}
	}

	void ModelEntity::ActiveAnimation(const char* anim_name)
	{
		AnimMap::Iterator it = _animations.Find(anim_name);
		if(it != _animations.End())
		{
			_curAnim = &(*it);
		}
		else
		{
			_curAnim = 0;
		}
		_animTime = 0.0f;
	}

	void ModelEntity::SetPosition(const vec3f& pos)
	{
		_position = pos;
		_worldMat.transl_vec.rvec3 = pos;
		CalcWorldBBox();
	}

	void ModelEntity::ClearModelData()
	{
		for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
		{
			delete[] it->name;
			delete it->attachment;
		}
		_jointAttachments.Clear();

		for(AnimMap::Iterator it = _animations.Begin(); it != _animations.End(); ++it)
		{
			delete[] it->name;
			engineAPI.animationManager->ReleaseAnimation(it->animation);
		}
		_animations.Clear();

		for(SoundMap::Iterator it = _sounds.Begin(); it != _sounds.End(); ++it)
		{
			delete[] it->name;
			engineAPI.soundManager->ReleaseSound(it->sound);
		}
		_sounds.Clear();

		_jointMatPalette.Clear();
		_jointAttachArray.Clear();
	}

	/*
		World space bounding box
	*/
	void ModelEntity::CalcWorldBBox()
	{
		const Model* model = _model->GetModel();
		const AABBox& obb = model->GetBoundingBox();
		transform(_worldBBox.minPt, obb.minPt, _worldMat);
		transform(_worldBBox.maxPt, obb.maxPt, _worldMat);
	}

	/*
		This function is to be called after model resources have been loaded to set up some data
	*/
	void ModelEntity::SetupModelData()
	{
		if(!_model || !_model->GetModel())
			return;

		const Model* model = _model->GetModel();

		// Bind joints to their attached entities (i-th entity in attachment array corresponds to i-th joint in model's joint array).
		const StaticArray<Joint>& joints = model->GetJoints();
		size_t count = joints.GetCount();
		_jointAttachArray.SetCount(count);
		_jointMatPalette.SetCount(count);
		for(size_t i = 0; i < count; ++i)
		{
			JointAttachMap::ConstIterator it = _jointAttachments.Find(joints[i].name);
			_jointAttachArray[i] = (it != _jointAttachments.End())? it->attachment: 0;

			_jointMatPalette[i].set_identity();
		}

		CalcWorldBBox();

		_animTime = 0.0f;
		_curAnim = 0;
	}

	bool ModelEntity::SetModel(const tchar* file_name)
	{
		const ModelRes* model = engineAPI.modelManager->CreateModel(file_name, true);
		if(model)
		{
			engineAPI.modelManager->ReleaseModel(_model);
			_model = model;
			ClearModelData();
			SetupModelData();
			return true;
		}
		else
			return false;
	}

	bool ModelEntity::SetAIScript(const tchar* file_name)
	{
		const AIScriptRes* ai = engineAPI.aiScriptManager->CreateAIScript(file_name, true);
		if(ai)
		{
			engineAPI.aiScriptManager->ReleaseAIScript(_aiScript);
			_aiScript = ai;
			return true;
		}
		else
			return false;
	}

	bool ModelEntity::SetJointAttachment(const char* joint_name, const tchar* file_name)
	{
		if(!_model || !_model->GetModel())
			return false;

		// find joint with this name in model skelet
		const StaticArray<Joint>& joints = _model->GetModel()->GetJoints();
		int joint_index = -1;
		for(size_t i = 0; i < joints.GetCount(); ++i)
		{
			if(!strcmp(joint_name, joints[i].name))
			{
				joint_index = i;
				break;
			}
		}

		if(joint_index == -1)
			return false;

		JointAttachType type = GetJointAttachTypeByExt(file_name);
		if(type == JOINT_ATTACH_UNKNOWN)
			return false;

		const FileResource* res;
		if(type == JOINT_ATTACH_MODEL)
		{
			ModelEntityRes* ent = engineAPI.modelEntityManager->CreateEntity(file_name);
			if(ent && ent->GetEntity())
			{
				// This entity must not have attachments; only 1 level allowed.
				if(ent->GetEntity()->_jointAttachments.GetCount())
				{
					delete ent;
					return false;
				}
			}
			res = ent;
		}
		else if(type == JOINT_ATTACH_PARTICLE_SYSTEM)
		{
			res = engineAPI.partSysManager->CreatePartSys(file_name);
		}

		if(res)
		{
			JointAttachMap::Iterator it = _jointAttachments.Find(joint_name);
			if(it != _jointAttachments.End())
			{
				assert(!strcmp(joint_name, it->name));
				delete it->attachment;
				it->type = type;
				it->attachment = res;
			}
			else
			{
				JointAttachData jd;
				jd.name = StringDup(joint_name);
				jd.attachment = res;
				jd.type = type;
				_jointAttachments[joint_name] = jd;
			}
			_jointAttachArray[joint_index] = res;
			return true;
		}

		return false;
	}

	bool ModelEntity::RemoveJointAttachment(const char* joint_name)
	{
		JointAttachMap::Iterator it = _jointAttachments.Find(joint_name);
		if(it != _jointAttachments.End())
		{
			delete it->attachment;
			delete[] it->name;
			_jointAttachments.Remove(it);
			return true;
		}
		else
			return false;
	}

	bool ModelEntity::AddAnimation(const char* anim_name, const tchar* file_name)
	{
		const AnimationRes* anim = engineAPI.animationManager->CreateAnimation(file_name, true);
		if(anim)
		{
			AnimMap::Iterator it = _animations.Find(anim_name);
			if(it != _animations.End())
			{
				assert(!strcmp(anim_name, it->name));
				engineAPI.animationManager->ReleaseAnimation(it->animation);
				it->animation = anim;
			}
			else
			{
				AnimData ad;
				ad.name = StringDup(anim_name);
				ad.type = Animation::GetAnimTypeByName(anim_name);
				ad.animation = anim;
				_animations[anim_name] = ad;
			}
			return true;
		}

		return false;
	}

	bool ModelEntity::RemoveAnimation(const char* anim_name)
	{
		AnimMap::Iterator it = _animations.Find(anim_name);
		if(it != _animations.End())
		{
			delete[] it->name;
			engineAPI.animationManager->ReleaseAnimation(it->animation);
			_animations.Remove(it);
			return true;
		}
		else
			return false;
	}

	bool ModelEntity::AddSound(const char* snd_name, const tchar* file_name)
	{
		const SoundRes* sound = engineAPI.soundManager->CreateSound(file_name, true);
		if(sound)
		{
			SoundMap::Iterator it = _sounds.Find(snd_name);
			if(it != _sounds.End())
			{
				assert(!strcmp(snd_name, it->name));
				engineAPI.soundManager->ReleaseSound(it->sound);
				it->sound = sound;
			}
			else
			{
				SoundData sd;
				sd.name = StringDup(snd_name);
				sd.sound = sound;
				_sounds[snd_name] = sd;
			}
			return true;
		}

		return false;
	}

	bool ModelEntity::RemoveSound(const char* snd_name)
	{
		SoundMap::Iterator it = _sounds.Find(snd_name);
		if(it != _sounds.End())
		{
			delete[] it->name;
			engineAPI.soundManager->ReleaseSound(it->sound);
			_sounds.Remove(it);
			return true;
		}
		else
			return false;
	}

	ModelEntity::JointAttachType ModelEntity::GetJointAttachTypeByExt(const tchar* file_name)
	{
		const tchar* dot = tstrrchr(file_name, _t('.'));
		if(dot)
		{
			if(!tstrcmp(dot + 1, _t("entity")))
				return JOINT_ATTACH_MODEL;
			else if(!tstrcmp(dot + 1, _t("part")))
				return JOINT_ATTACH_PARTICLE_SYSTEM;
		}
		return JOINT_ATTACH_UNKNOWN;
	}

	ModelClass ModelEntity::GetClassFromString(const char* name)
	{
		for(int i = 0; i < MODEL_CLASS_COUNT; ++i)
		{
			if(!strcmp(name, _classNames[i]))
				return (ModelClass)i;
		}

		return MODEL_CLASS_COUNT;
	}

	const char* ModelEntity::GetClassString(ModelClass c)
	{
		int i = (int)c;
		if(i >= 0 && i < MODEL_CLASS_COUNT)
			return _classNames[i];
		else
			return "";
	}

}
