
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Memory.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Console.h"
#include "Model.h"
#include "Material.h"
#include "ParticleSystem.h"
#include "FileSystem.h"
#include "EngineInternal.h"
#include "ModelEntity.h"
#include "AIEntity.h"
#include "PlayerEntity.h"
#include "ItemEntity.h"
#include "WeaponEntity.h"
#include "StaticEntity.h"


using namespace Memory;
using namespace math3d;


namespace Engine
{

	const char* ModelEntity::_mdlEntTypeNames[] =
	{
		"Static",
		"Player",
		"AI",
		"Weapon",
		"Item"
	};


	ModelEntity::ModelEntity()
	{
		_animTime = 0.0f;
		_curAnim = 0;
		_animPlaying = false;

		_model = ModelResPtr::null;

		// set initial common properties
		_name[0] = '\0';
	}

	ModelEntity::ModelEntity(const ModelEntity& entity)
	{
		_model = ModelResPtr::null;
		*this = entity;
	}

	ModelEntity::~ModelEntity()
	{
	}

	ModelEntity& ModelEntity::operator = (const ModelEntity& entity)
	{
		Unload();

		_animTime = 0.0f;
		_curAnim = 0;
		_animPlaying = false;

		// properties
		strcpy(_name, entity._name);

		// resources
		if(entity._model)
			_model = engineAPI.modelManager->CreateModel(entity._model.GetRes()->GetFileName());
		else
			_model = ModelResPtr::null;

		for(MaterialMap::ConstIterator it = entity._materials.Begin(); it != entity._materials.End(); ++it)
		{
			MaterialData md;
			md.name = StringDup(it->name);
			md.materialRes = engineAPI.materialManager->CreateMaterial(it->materialRes.GetRes()->GetFileName());
			_materials[it->name] = md;
		}

		for(JointAttachMap::ConstIterator it = entity._jointAttachments.Begin(); it != entity._jointAttachments.End(); ++it)
		{
			JointAttachData jd;
			jd.name = StringDup(it->name);
			jd.type = it->type;
			if(jd.type == JOINT_ATTACH_MODEL)
			{
				ModelEntityResPtr ptr = ModelEntityResPtr((const ModelEntityRes*)it->attachment);
				jd.attachment = engineAPI.modelEntityManager->CreateCopy(ptr).GetRes();
			}
			else if(jd.type == JOINT_ATTACH_PARTICLE_SYSTEM)
			{
				PartSysResPtr ptr = PartSysResPtr((const PartSysRes*)it->attachment);
				jd.attachment = engineAPI.partSysManager->CreateCopy(ptr).GetRes();
			}
			jd.jointIndex = it->jointIndex;
			_jointAttachments[it->name] = jd;
		}

		for(AnimMap::ConstIterator it = entity._animations.Begin(); it != entity._animations.End(); ++it)
		{
			AnimData ad;
			ad.name = StringDup(it->name);
			ad.type = it->type;
			ad.animation = engineAPI.animationManager->CreateAnimation(it->animation.GetRes()->GetFileName());
			_animations[it->name] = ad;
		}

		for(SoundMap::ConstIterator it = entity._sounds.Begin(); it != entity._sounds.End(); ++it)
		{
			SoundData sd;
			sd.name = StringDup(it->name);
			sd.sound = engineAPI.soundManager->CreateSound(it->sound.GetRes()->GetFileName());
			_sounds[it->name] = sd;
		}

		SetupModelData();

		return *this;
	}

	ModelEntity* ModelEntity::CreateFromFile(const tchar* file_name)
	{
		if(!file_name)
			return 0;

		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(file_name, _t("rb"));
		if(!file)
			return 0;

		Parser parser;
		if(!parser.LoadFile(*file))
			return 0;
		file->Close();

		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];

		try
		{
			parser.ExpectTokenString("entity");
			parser.ReadIdentifier(buf, MAX_IDENT_LEN);
		}
		catch(ParserException& e)
		{
			Console::PrintError(e.GetDesc());
			return 0;
		}

		ModelEntity* ent = 0;
		if(!strcmp(buf, _mdlEntTypeNames[ME_TYPE_STATIC]))
		{
			ent = new(mapPool) StaticEntity;
		}
		else if(!strcmp(buf, _mdlEntTypeNames[ME_TYPE_AI]))
		{
			ent = new(mapPool) AIEntity;
		}
		else if(!strcmp(buf, _mdlEntTypeNames[ME_TYPE_ITEM]))
		{
			ent = new(mapPool) ItemEntity;
		}
		else if(!strcmp(buf, _mdlEntTypeNames[ME_TYPE_WEAPON]))
		{
			ent = new(mapPool) WeaponEntity;
		}
		else if(!strcmp(buf, _mdlEntTypeNames[ME_TYPE_PLAYER]))
		{
			ent = new(mapPool) PlayerEntity;
		}

		if(!ent->Load(parser))
		{
			delete ent;
			ent = 0;
		}

		return ent;
	}

	bool ModelEntity::Load(Parser& parser)
	{
		Unload();

		try
		{
			parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE);

			// read properties
			parser.ExpectTokenString("properties");
			parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE);
			// common properties
			parser.ExpectTokenString("name");
			parser.ReadString(_name, MAX_NAME_LENGTH);
			// read type specific properties
			ReadProperties(parser);
			parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE);

			// read entity resources

			char path[MAX_PATH];

			parser.ExpectTokenString("model");
			parser.ReadString(path, MAX_PATH);
			tchar* p = CharToWideChar(path);
			_model = engineAPI.modelManager->CreateModel(p);
			delete[] p;

			Parser::Token tok_name;

			// materials
			{
				parser.ExpectTokenString("materials");
				parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE);

				MaterialData md;
				while(1)
				{
					parser.ReadToken(tok_name);
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

					parser.ReadString(path, MAX_PATH);
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

			// joint attachments
			{
				parser.ExpectTokenString("jointAttachments");
				parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE);

				JointAttachData jd;
				while(1)
				{
					parser.ReadToken(tok_name);
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

					parser.ReadInt(jd.jointIndex);

					parser.ReadString(path, MAX_PATH);
					tchar* p = CharToWideChar(path);
					jd.type = GetJointAttachTypeByExt(p);
					if(jd.type == JOINT_ATTACH_MODEL)
					{
						jd.attachment = engineAPI.modelEntityManager->CreateEntity(p).GetRes();
					}
					else if(jd.type == JOINT_ATTACH_PARTICLE_SYSTEM)
					{
						jd.attachment = engineAPI.partSysManager->CreateParticleSystem(p).GetRes();
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

			// animations
			{
				parser.ExpectTokenString("animations");
				parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE);

				AnimData ad;
				while(1)
				{
					parser.ReadToken(tok_name);
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

					parser.ReadString(path, MAX_PATH);
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

			// sounds
			{
				parser.ExpectTokenString("sounds");
				parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE);

				SoundData sd;
				while(1)
				{
					parser.ReadToken(tok_name);
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

					parser.ReadString(path, MAX_PATH);					
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

			// entity closing brace
			parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE);
		}
		catch(ParserException& e)
		{
			Console::PrintError(e.GetDesc());
			Unload();
			return false;
		}

		return true;
	}

	bool ModelEntity::Save(const tchar* file_name)
	{
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(file_name, _t("wt"));
		if(!file)
		{
			Console::PrintError("Failed to write entity file: %ls", file_name);
			return false;
		}

		file->Printf("// Daemonium engine entity file\n\n");
		file->Printf("entity %s\n{\n", _mdlEntTypeNames[GetModelEntityType()]);

		// properties
		file->Printf("\tproperties\n\t{\n");
		file->Printf("\t\tname\t\t\"%s\"\n", _name);
		WriteProperties(*file, "\t\t");
		file->Printf("\t}\n\n");

		// entity resources

		const tchar* fn;

		fn = _model? _model.GetRes()->GetFileName(): _T("");
		file->Printf("\tmodel\t\t\"%ls\"\n\n", fn);

		// materials
		file->Printf("\tmaterials\n\t{\n");
		for(MaterialMap::ConstIterator it = _materials.Begin(); it != _materials.End(); ++it)
		{
			file->Printf("\t\t%s\t\t\"%ls\"\n", it->name, it->materialRes.GetRes()->GetFileName());
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
			file->Printf("\t\t%s\t\t\"%ls\"\n", it->name, it->animation.GetRes()->GetFileName());
		}
		file->Printf("\t}\n\n");

		// sounds
		file->Printf("\tsounds\n\t{\n");
		for(SoundMap::ConstIterator it = _sounds.Begin(); it != _sounds.End(); ++it)
		{
			file->Printf("\t\t%s\t\t\"%ls\"\n", it->name, it->sound.GetRes()->GetFileName());
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
			_model = ModelResPtr::null;
		}

		ClearModelData();
	}

	void ModelEntity::MaterialChanged(const Material* material)
	{
		if(_meshDataArray.GetCount())
		{
			const StaticArray<Mesh>& meshes = _model->GetMeshes();
			for(size_t i = 0; i < _meshDataArray.GetCount(); ++i)
			{
				if(_meshDataArray[i].materialData->materialRes == material)
				{
					_meshDataArray[i].shaderIndex = GetShaderIndex(meshes[i].flags, material);
				}
			}
		}
	}

	void ModelEntity::UpdateTime(float frame_time)
	{
		if(_curAnim)
		{
			if(_animPlaying)
			{
				_animTime += frame_time;
				AnimationResPtr anim = _curAnim->animation;
				if(_animTime > anim->GetLength())
					_animTime = fmod(_animTime, anim->GetLength());
			}
		}
	}

	void ModelEntity::UpdateGraphics()
	{
		if(_curAnim)
		{
			AnimationResPtr anim = _curAnim->animation;

			if(_animPlaying)
			{
				// update joint transform matrices for current animation time
				anim->GetJointTransforms(
					_animTime,
					_jointMatPalette.GetData(),
					_model->GetRootJoint(),
					_model->GetJoints().GetData());
			}

			// update attachments world matrices
			const StaticArray<mat4f>& cmb_xforms = anim->GetCombinedTransforms();
			for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
			{
				RenderableEntity* entity;
				if(it->type == JOINT_ATTACH_MODEL)
					entity = (ModelEntity*) ((ModelEntityRes*)it->attachment)->GetResource();
				else if(it->type == JOINT_ATTACH_PARTICLE_SYSTEM)
					entity = (ParticleSystem*) ((PartSysRes*)it->attachment)->GetResource();

				if(entity)
				{
					mat4f ent_mat;
					mul(ent_mat, cmb_xforms[it->jointIndex], GetWorldTransform());
					entity->SetWorldTransform(ent_mat);
				}
			}
		}
		else
		{
			for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
			{
				RenderableEntity* entity;
				if(it->type == JOINT_ATTACH_MODEL)
					entity = (ModelEntity*) ((ModelEntityRes*)it->attachment)->GetResource();
				else if(it->type == JOINT_ATTACH_PARTICLE_SYSTEM)
					entity = (ParticleSystem*) ((PartSysRes*)it->attachment)->GetResource();

				if(entity)
				{
					mat4f ent_mat;
					const Joint* joints = _model->GetJoints().GetData();
					mul(ent_mat, joints[it->jointIndex].invOffsetMatrix, GetWorldTransform());
					entity->SetWorldTransform(ent_mat);
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
		if(!_model || !_model.GetRes()->IsLoaded())
			return;

		// Bind meshes to their data (materials and gpu program indices)
		const StaticArray<Mesh>& meshes = _model->GetMeshes();
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
				MaterialResPtr mat = engineAPI.materialManager->CreateDefaultMaterial();
				// add it to material map
				char* name = StringDup(meshes[i].material);
				MaterialData md = { name, mat };
				_materials[name] = md;
				_meshDataArray[i].materialData = &_materials[name];
			}
			_meshDataArray[i].shaderIndex = GetShaderIndex(meshes[i].flags, _meshDataArray[i].materialData->materialRes);
		}

		// Bind joints to their attached entities (i-th entity in attachment array corresponds to i-th joint in model's joint array).
		const StaticArray<Joint>& joints = _model->GetJoints();
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
				((ModelEntity*)res->GetResource())->SetupModelData();
			}
		}

		SetObjectBoundingBox(_model->GetBoundingBox());

		_animTime = 0.0f;
		_curAnim = 0;
	}

	bool ModelEntity::SetModel(const tchar* file_name)
	{
		ModelResPtr model = engineAPI.modelManager->CreateModel(file_name, true);
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

	bool ModelEntity::SetMaterial(const char* mat_name, const tchar* file_name)
	{
		if(!_model || !_model.GetRes()->IsLoaded())
			return false;

		MaterialMap::Iterator it = _materials.Find(mat_name);
		if(it != _materials.End())
		{
			MaterialResPtr mat = engineAPI.materialManager->CreateMaterial(file_name, true);
			if(mat)
			{
				// release old material and set new
				engineAPI.materialManager->ReleaseMaterial(it->materialRes);
				it->materialRes = mat;

				// set pointers to new material in _materialArray
				const StaticArray<Mesh>& meshes = _model->GetMeshes();
				assert(meshes.GetCount() == _meshDataArray.GetCount());
				for(size_t i = 0; i < meshes.GetCount(); ++i)
				{
					if(!strcmp(mat_name, meshes[i].material))
					{
						_meshDataArray[i].materialData->materialRes = mat;
						_meshDataArray[i].shaderIndex = GetShaderIndex(meshes[i].flags, mat);
					}
				}
			}
			return true;
		}

		return false;
	}

	bool ModelEntity::SetJointAttachment(const char* joint_name, const tchar* file_name)
	{
		if(!_model || !_model.GetRes()->IsLoaded())
			return false;

		// find joint with this name in model skelet
		const StaticArray<Joint>& joints = _model->GetJoints();
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

		const ResourceBase* res;
		if(type == JOINT_ATTACH_MODEL)
		{
			ModelEntityResPtr ent = engineAPI.modelEntityManager->CreateEntity(file_name);
			if(ent.GetRes() && ent.GetRes()->IsLoaded())
			{
				// This entity must not have attachments; only 1 level allowed.
				if(ent->_jointAttachments.GetCount())
				{
					delete ent;
					return false;
				}
			}
			res = ent.GetRes();
		}
		else if(type == JOINT_ATTACH_PARTICLE_SYSTEM)
		{
			res = engineAPI.partSysManager->CreateParticleSystem(file_name).GetRes();
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
		AnimationResPtr anim = engineAPI.animationManager->CreateAnimation(file_name, true);
		if(anim)
		{
			if(IsAnimCompatible(*anim))
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
			else
			{
				engineAPI.animationManager->ReleaseAnimation(anim);
			}
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
		SoundResPtr sound = engineAPI.soundManager->CreateSound(file_name, true);
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

	void ModelEntity::BindPoseTransforms()
	{
		// set joint matrix palette to identity
		for(size_t i = 0; i < _jointMatPalette.GetCount(); ++i)
			_jointMatPalette[i].set_identity();

		// joint attachments transforms
		for(JointAttachMap::Iterator it = _jointAttachments.Begin(); it != _jointAttachments.End(); ++it)
		{
			RenderableEntity* entity;
			if(it->type == JOINT_ATTACH_MODEL)
				entity = (ModelEntity*) ((ModelEntityRes*)it->attachment)->GetResource();
			else if(it->type == JOINT_ATTACH_PARTICLE_SYSTEM)
				entity = (ParticleSystem*) ((PartSysRes*)it->attachment)->GetResource();

			if(entity)
			{
				mat4f ent_mat;
				const Joint* joints = _model->GetJoints().GetData();
				mul(ent_mat, joints[it->jointIndex].invOffsetMatrix, GetWorldTransform());
				entity->SetWorldTransform(ent_mat);
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

	bool ModelEntity::IsAnimCompatible(const Animation& anim)
	{
		if(!_model.GetRes() || !_model.GetRes()->IsLoaded() || !_model->GetRootJoint())
			return false;

		return anim.IsSkeletCompatible(_model->GetJoints());
	}

}
