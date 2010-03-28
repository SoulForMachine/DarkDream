
#ifndef _ENGINE_MODEL_ENTITY_H_
#define _ENGINE_MODEL_ENTITY_H_

#include "BaseLib/Bounds.h"
#include "BaseLib/HashMap.h"
#include "BaseLib/Math/mat4.h"
#include "ResourceManager.h"
#include "Animation.h"
#include "RenderableEntity.h"
#include "Entity.h"


class Parser;

namespace FileUtil
{
	class File;
}


namespace Engine
{

	class ResourceBase;
	class ModelRes;
	class MaterialRes;
	class AIScriptRes;
	class ModelEntityRes;
	class AnimationRes;
	class SoundRes;
	class Material;



	class ENGINE_API ModelEntity: public RenderableEntity
	{
	public:
		enum ModelEntityType
		{
			ME_TYPE_STATIC,
			ME_TYPE_PLAYER,
			ME_TYPE_AI,
			ME_TYPE_WEAPON,
			ME_TYPE_ITEM,
		};

		enum JointAttachType
		{
			JOINT_ATTACH_UNKNOWN = -1,
			JOINT_ATTACH_MODEL,
			JOINT_ATTACH_PARTICLE_SYSTEM,
			JOINT_ATTACH_LIGHT,
		};

		struct MaterialData
		{
			char* name;
			MaterialResPtr materialRes;
		};

		struct JointAttachData
		{
			char* name;
			JointAttachType type;
			int jointIndex;
			const ResourceBase* attachment;
		};

		struct AnimData
		{
			char* name;
			Animation::AnimType type;
			AnimationResPtr animation;
		};

		struct SoundData
		{
			char* name;
			SoundResPtr sound;
		};

		struct MeshData
		{
			MaterialData* materialData;
			int shaderIndex;
		};

		typedef HashMap<const char*, ModelEntity::MaterialData> MaterialMap;
		typedef StaticArray<MeshData> MeshDataArray;
		typedef HashMap<const char*, ModelEntity::JointAttachData> JointAttachMap;
		typedef StaticArray<const ResourceBase*> JointAttachArray;
		typedef HashMap<const char*, ModelEntity::AnimData> AnimMap;
		typedef HashMap<const char*, ModelEntity::SoundData> SoundMap;

		ModelEntity();
		ModelEntity(const ModelEntity& entity);
		~ModelEntity() = 0;
		ModelEntity& operator = (const ModelEntity& entity);

		virtual EntityType GetType() const
			{ return ENTITY_TYPE_MODEL; }

		static ModelEntity* CreateFromFile(const tchar* file_name);
		virtual ModelEntity* CreateCopy() const = 0;
		bool Load(const tchar* file_name);
		void SetupModelData(); // call after model resources have been loaded
		bool Save(const tchar* file_name);
		virtual void Unload();

		const StaticArray<math3d::mat4f>& GetJointTransforms() const
			{ return _jointMatPalette; }
		void MaterialChanged(const Material* material);
		virtual void UpdateTime(float frame_time);
		virtual void UpdateGraphics();
		bool SetActiveAnimation(const char* anim_name);
		void PlayAnimation()
			{ _animPlaying = true; }
		void PauseAnimation()
			{ _animPlaying = false; }
		bool IsAnimationPlaying() const
			{ return _animPlaying; }

		ModelResPtr GetModelRes() const
			{ return _model; }
		const MeshDataArray& GetMeshDataArray() const
			{ return _meshDataArray; }
		const MaterialMap& GetMaterials() const
			{ return _materials; }
		const JointAttachMap& GetJointAttachments() const
			{ return _jointAttachments; }
		const AnimMap& GetAnimations() const
			{ return _animations; }
		const SoundMap& GetSounds() const
			{ return _sounds; }
		const AnimData* GetCurrentAnimation() const
			{ return _curAnim; }

		bool SetModel(const tchar* file_name);
		bool SetMaterial(const char* mat_name, const tchar* file_name);
		bool SetJointAttachment(const char* joint_name, const tchar* file_name);
		bool RemoveJointAttachment(const char* joint_name);
		bool AddAnimation(const char* anim_name, const tchar* file_name);
		bool RemoveAnimation(const char* anim_name);
		bool AddSound(const char* snd_name, const tchar* file_name);
		bool RemoveSound(const char* snd_name);

		virtual ModelEntityType GetModelEntityType() const = 0;

		const char* GetName() const
			{ return _name; }
		void SetName(const char* name)
		{
			strncpy(_name, name, MAX_NAME_LENGTH);
			_name[MAX_NAME_LENGTH - 1] = '\0';
		}

		static JointAttachType GetJointAttachTypeByExt(const tchar* file_name);

	protected:
		virtual void ReadProperties(Parser& parser) = 0;
		virtual void WriteProperties(FileUtil::File& file, const char* indent) = 0;

	private:
		bool Load(Parser& parser);
		void ClearModelData();
		void CalcWorldBBox();
		void BindPoseTransforms();
		int GetShaderIndex(uint vert_flags, const Material* material);
		bool IsAnimCompatible(const Animation& anim);

		StaticArray<math3d::mat4f> _jointMatPalette;
		float _animTime;
		AnimData* _curAnim;
		bool _animPlaying;

		ModelResPtr _model;
		MaterialMap _materials;
		MeshDataArray _meshDataArray;
		JointAttachMap _jointAttachments;
		JointAttachArray _jointAttachArray;
		AnimMap _animations;
		SoundMap _sounds;

		static const int MAX_NAME_LENGTH = 64;
		char _name[MAX_NAME_LENGTH];
	};

}


#endif // _ENGINE_MODEL_ENTITY_H_
