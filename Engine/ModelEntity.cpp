
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Memory.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Console.h"
#include "Model.h"
#include "Material.h"
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
		SetWorldTransform(mat4f::identity);
		_animTime = 0.0f;
		_curAnim = 0;
		_dropped = true;
		_animPlaying = false;

		_model = 0;
		_aiScript = 0;

		// set initial properties
		_class = MODEL_CLASS_GENERIC;
		_name[0] = '\0';
		_clip = true;;
		_lifePoints = 100;
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

		SetWorldTransform(entity.GetWorldTransform());
		_dropped = entity._dropped;
		_animPlaying = entity._animPlaying;

		// properties
		_class = entity._class;
		strcpy(_name, entity._name);
		_clip = entity._clip;
		_lifePoints = entity._lifePoints;

		// resources
		if(entity._model)
			_model = engineAPI.modelManager->CreateModel(entity._model->GetFileName());

		if(entity._aiScript)
			_aiScript = engineAPI.aiScriptManager->CreateAIScript(entity._aiScript->GetFileName());

		for(MaterialMap::ConstIterator it = entity._materials.Begin(); it != entity._materials.End(); ++it)
		{
			MaterialData md;
			md.name = StringDup(it->name);
			md.materialRes = engineAPI.materialManager->CreateMaterial(it->materialRes->GetFileName());
			_materials[it->name] = md;
		}

		for(JointAttachMap::ConstIterator it = entity._jointAttachments.Begin(); it != entity._jointAttachments.End(); ++it)
		{
			JointAttachData jd;
			jd.name = StringDup(it->name);
			jd.type = it->type;
			if(jd.type == JOINT_ATTACH_MODEL)
				jd.attachment = new(mapPool) ModelEntityRes((const ModelEntityRes&)*it->attachment);
			else if(jd.type == JOINT_ATTACH_PARTICLE_SYSTEM)
				jd.attachment = new(mapPool) PartSysRes((const PartSysRes&)*it->attachment);
			jd.jointIndex = it->jointIndex;
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

		// materials
		{
			if(!parser.ExpectTokenString("materials"))
				{ Unload(); return false; }
			if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
				{ Unload(); return false; }
			MaterialData md;
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
					md.name = StringDup(tok_name.str);
				}
				else
				{
					Console::PrintError("Expected material name, found \'%s\'", tok_name.str);
					Unload();
					return false;
				}

				if(!parser.ReadString(path, MAX_PATH))
				{
					Console::PrintError("Expected material path");
					Unload();
					return false;
				}
				else
				{
					tchar* p = CharToWideChar(path);
					md.materialRes = engineAPI.materialManager->CreateMaterial(p);
					delete[] p;
					MaterialMap::ConstIterator it = _materials.Find(md.name);
					if(it != _materials.End())
					{
						Console::PrintWarning("Duplicate material name found: %s", md.name);
						delete[] md.name;
					}
					else
					{
						_materials[md.name] = md;
					}
				}
			}
		}

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

				if(!parser.ReadInt(jd.jointIndex))
				{
					Console::PrintError("Expected attachment joint index");
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
		file->Printf("\t}\n\n");

		// entity resources

		const tchar* fn;

		fn = _model? _model->GetFileName(): _T("");
		file->Printf("\tmodel\t\t\"%ls\"\n", fn);

		fn = _aiScript? _aiScript->GetFileName(): _T("");
		file->Printf("\taiScript\t\t\"%ls\"\n\n", fn);

		// materials
		file->Printf("\tmaterials\n\t{\n");
		for(MaterialMap::ConstIterator it = _materials.Begin(); it != _materials.End(); ++it)
		{
			file->Printf("\t\t%s\t\t\"%ls\"\n", it->name, it->materialRes->GetFileName());
		}
		file->Printf("\t}\n\n");

		// joint attachments
		file->Printf("\tjointAttachments\n\t{\n");
		for(JointAttachMap::ConstIterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
		{
			file->Printf("\t\t%s\t\t%d\t\t\"%ls\"\n", it->name, it->jointIndex, it->attachment->GetFileName());
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

	void ModelEntity::MaterialChanged(const Material* material)
	{
		if(_meshDataArray.GetCount())
		{
			const StaticArray<Mesh>& meshes = _model->GetModel()->GetMeshes();
			for(size_t i = 0; i < _meshDataArray.GetCount(); ++i)
			{
				if(_meshDataArray[i].materialData->materialRes->GetMaterial() == material)
				{
					_meshDataArray[i].shaderIndex = GetShaderIndex(meshes[i].flags, material);
				}
			}
		}
	}

	void ModelEntity::UpdateGraphics(int frame_time)
	{
		float tsec = frame_time * 0.001f;

		if(_curAnim)
		{
			const Animation* anim = _curAnim->animation->GetAnimation();

			if(_animPlaying)
			{
				// update joint transform matrices for current animation time
				_animTime += tsec;
				if(_animTime > anim->GetLength())
					_animTime = fmod(_animTime, anim->GetLength());

				anim->GetJointTransforms(
					_animTime,
					_jointMatPalette.GetData(),
					_model->GetModel()->GetRootJoint(),
					_model->GetModel()->GetJoints().GetData());
			}

			// update attachments world matrices
			const StaticArray<mat4f>& cmb_xforms = anim->GetCombinedTransforms();
			for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
			{
				if(it->type == JOINT_ATTACH_MODEL)
				{
					ModelEntity* entity = ((ModelEntityRes*)it->attachment)->GetEntity();
					if(entity)
					{
						mat4f ent_mat;
						mul(ent_mat, cmb_xforms[it->jointIndex], GetWorldTransform());
						entity->SetWorldTransform(ent_mat);
					}
				}
			}
		}
		else
		{
			for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
			{
				if(it->type == JOINT_ATTACH_MODEL)
				{
					ModelEntity* entity = ((ModelEntityRes*)it->attachment)->GetEntity();
					if(entity)
					{
						mat4f ent_mat;
						const Joint* joints = _model->GetModel()->GetJoints().GetData();
						mul(ent_mat, joints[it->jointIndex].invOffsetMatrix, GetWorldTransform());
						entity->SetWorldTransform(ent_mat);
					}
				}
			}
		}
	}

	bool ModelEntity::SetActiveAnimation(const char* anim_name)
	{
		bool result;
		AnimMap::Iterator it = _animations.Find(anim_name);
		if(it != _animations.End())
		{
			_curAnim = &(*it);
			_animPlaying = true;
			result = true;
		}
		else
		{
			_curAnim = 0;
			_animPlaying = false;
			BindPoseTransforms();
			result = false;
		}
		_animTime = 0.0f;
		return result;
	}

	void ModelEntity::ClearModelData()
	{
		for(MaterialMap::Iterator it = _materials.Begin(); it!= _materials.End(); ++it)
		{
			delete[] it->name;
			engineAPI.materialManager->ReleaseMaterial(it->materialRes);
		}
		_materials.Clear();

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

		_meshDataArray.Clear();
		_jointMatPalette.Clear();
		_jointAttachArray.Clear();
	}

	/*
		This function is to be called after model resources have been loaded to set up some data
	*/
	void ModelEntity::SetupModelData()
	{
		if(!_model || !_model->GetModel())
			return;

		const Model* model = _model->GetModel();

		// Bind meshes to their data (materials and gpu program indices)
		const StaticArray<Mesh>& meshes = model->GetMeshes();
		size_t mesh_count = meshes.GetCount();
		_meshDataArray.SetCount(mesh_count);
		for(size_t i = 0; i < mesh_count; ++i)
		{
			MaterialMap::Iterator it = _materials.Find(meshes[i].material);
			if(it != _materials.End())
			{
				// use material from map
				_meshDataArray[i].materialData = &(*it);
			}
			else
			{
				// create default material
				const MaterialRes* mat = engineAPI.materialManager->CreateMaterial(_T(""), true);
				// add it to material map
				char* name = StringDup(meshes[i].material);
				MaterialData md = { name, mat };
				_materials[name] = md;
				_meshDataArray[i].materialData = &_materials[name];
			}
			_meshDataArray[i].shaderIndex = GetShaderIndex(meshes[i].flags, _meshDataArray[i].materialData->materialRes->GetMaterial());
		}

		// Bind joints to their attached entities (i-th entity in attachment array corresponds to i-th joint in model's joint array).
		const StaticArray<Joint>& joints = model->GetJoints();
		size_t joint_count = joints.GetCount();
		_jointAttachArray.SetCount(joint_count);
		_jointMatPalette.SetCount(joint_count);
		for(size_t i = 0; i < joint_count; ++i)
		{
			JointAttachMap::ConstIterator it = _jointAttachments.Find(joints[i].name);
			_jointAttachArray[i] = (it != _jointAttachments.End())? it->attachment: 0;

			_jointMatPalette[i].set_identity();
		}

		// setup data of attached entities
		for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
		{
			if(it->type == JOINT_ATTACH_MODEL)
			{
				ModelEntityRes* res = (ModelEntityRes*)it->attachment;
				res->GetEntity()->SetupModelData();
			}
		}

		SetObjectBoundingBox(model->GetBoundingBox());

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

	bool ModelEntity::SetMaterial(const char* mat_name, const tchar* file_name)
	{
		if(!_model || !_model->GetModel())
			return false;

		MaterialMap::Iterator it = _materials.Find(mat_name);
		if(it != _materials.End())
		{
			const MaterialRes* mat = engineAPI.materialManager->CreateMaterial(file_name, true);
			if(mat)
			{
				// release old material and set new
				engineAPI.materialManager->ReleaseMaterial(it->materialRes);
				it->materialRes = mat;

				// set pointers to new material in _materialArray
				const StaticArray<Mesh>& meshes = _model->GetModel()->GetMeshes();
				assert(meshes.GetCount() == _meshDataArray.GetCount());
				for(size_t i = 0; i < meshes.GetCount(); ++i)
				{
					if(!strcmp(mat_name, meshes[i].material))
					{
						_meshDataArray[i].materialData->materialRes = mat;
						_meshDataArray[i].shaderIndex = GetShaderIndex(meshes[i].flags, mat->GetMaterial());
					}
				}
			}
			return true;
		}

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
				jd.jointIndex = joint_index;
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
			if(&(*it) == _curAnim)
			{
				_curAnim = 0;
				_animPlaying = false;
				BindPoseTransforms();
			}
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

	void ModelEntity::BindPoseTransforms()
	{
		// set joint matrix palette to identity
		for(size_t i = 0; i < _jointMatPalette.GetCount(); ++i)
			_jointMatPalette[i].set_identity();

		// joint attachments transforms
		for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
		{
			if(it->type == JOINT_ATTACH_MODEL)
			{
				ModelEntity* entity = ((ModelEntityRes*)it->attachment)->GetEntity();
				if(entity)
				{
					mat4f ent_mat;
					const Joint* joints = _model->GetModel()->GetJoints().GetData();
					mul(ent_mat, joints[it->jointIndex].invOffsetMatrix, GetWorldTransform());
					entity->SetWorldTransform(ent_mat);
				}
			}
		}
	}

	int ModelEntity::GetShaderIndex(uint vert_flags, const Material* material)
	{
		int index = 0;
		if(vert_flags & Mesh::FLAG_SKIN_DATA)
			index |= 2;
		if(material->HasNormalMap() && (vert_flags & Mesh::FLAG_TANGENTS))
			index |= 1;

		return index;
	}

}
