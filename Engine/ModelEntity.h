
#ifndef _ENGINE_MODEL_ENTITY_H_
#define _ENGINE_MODEL_ENTITY_H_

#include "BaseLib/Bounds.h"
#include "BaseLib/HashMap.h"
#include "BaseLib/Math/mat4.h"
#include "Animation.h"
#include "RenderableEntity.h"
#include "Entity.h"



namespace Engine
{

	class FileResource;
	class ModelRes;
	class MaterialRes;
	class AIScriptRes;
	class ModelEntityRes;
	class AnimationRes;
	class SoundRes;
	class Material;


	enum ModelClass
	{
		MODEL_CLASS_GENERIC,
		MODEL_CLASS_NPC,
		MODEL_CLASS_MONSTER,
		MODEL_CLASS_BOSS,
		MODEL_CLASS_BUILDING,
		MODEL_CLASS_FOLIAGE,
		MODEL_CLASS_DEBRIS,

		MODEL_CLASS_COUNT,
	};


	class ENGINE_API ModelEntity: public RenderableEntity
	{
	public:
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
			const MaterialRes* materialRes;
		};

		struct JointAttachData
		{
			char* name;
			JointAttachType type;
			int jointIndex;
			const FileResource* attachment;
		};

		struct AnimData
		{
			char* name;
			Animation::AnimType type;
			const AnimationRes* animation;
		};

		struct SoundData
		{
			char* name;
			const SoundRes* sound;
		};

		struct MeshData
		{
			MaterialData* materialData;
			int shaderIndex;
		};

		typedef HashMap<const char*, ModelEntity::MaterialData> MaterialMap;
		typedef StaticArray<MeshData> MeshDataArray;
		typedef HashMap<const char*, ModelEntity::JointAttachData> JointAttachMap;
		typedef StaticArray<const FileResource*> JointAttachArray;
		typedef HashMap<const char*, ModelEntity::AnimData> AnimMap;
		typedef HashMap<const char*, ModelEntity::SoundData> SoundMap;

		ModelEntity();
		ModelEntity(const ModelEntity& entity);
		~ModelEntity();
		ModelEntity& operator = (const ModelEntity& entity);

		virtual EntityType GetType() const
			{ return ENTITY_TYPE_MODEL; }

		bool Load(const tchar* file_name);
		void SetupModelData(); // call after model resources have been loaded
		bool Save(const tchar* file_name);
		void Unload();

		const StaticArray<math3d::mat4f>& GetJointTransforms() const
			{ return _jointMatPalette; }
		void MaterialChanged(const Material* material);
		void UpdateGraphics(int frame_time);
		bool SetActiveAnimation(const char* anim_name);
		void PlayAnimation()
			{ _animPlaying = true; }
		void PauseAnimation()
			{ _animPlaying = false; }
		bool IsAnimationPlaying() const
			{ return _animPlaying; }

		const ModelRes* GetModelRes() const
			{ return _model; }
		const AIScriptRes* GetAIScriptRes() const
			{ return _aiScript; }
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
		bool IsDropped() const
			{ return _dropped; }
		void SetDropped(bool dropped)
			{ _dropped = dropped; }

		bool SetModel(const tchar* file_name);
		bool SetAIScript(const tchar* file_name);
		bool SetMaterial(const char* mat_name, const tchar* file_name);
		bool SetJointAttachment(const char* joint_name, const tchar* file_name);
		bool RemoveJointAttachment(const char* joint_name);
		bool AddAnimation(const char* anim_name, const tchar* file_name);
		bool RemoveAnimation(const char* anim_name);
		bool AddSound(const char* snd_name, const tchar* file_name);
		bool RemoveSound(const char* snd_name);

		ModelClass GetClass() const
			{ return _class; }
		void SetClass(ModelClass cls)
			{ _class = cls; }
		const char* GetName() const
			{ return _name; }
		void SetName(const char* name)
		{
			strncpy(_name, name, MAX_NAME_LENGTH);
			_name[MAX_NAME_LENGTH - 1] = '\0';
		}
		bool GetClip() const
			{ return _clip; }
		void SetClip(bool clip)
			{ _clip = clip; }
		int GetLifePoints() const
			{ return _lifePoints; }
		void SetLifePoints(int pts)
			{ _lifePoints = pts; }

	private:
		static JointAttachType GetJointAttachTypeByExt(const tchar* file_name);
		void ClearModelData();
		void CalcWorldBBox();
		ModelClass GetClassFromString(const char* name);
		const char* GetClassString(ModelClass c);
		void BindPoseTransforms();
		int GetShaderIndex(uint vert_flags, const Material* material);

		StaticArray<math3d::mat4f> _jointMatPalette;
		float _animTime;
		AnimData* _curAnim;
		bool _animPlaying;
		bool _dropped; // true if object is on terrain

		const ModelRes* _model;
		const AIScriptRes* _aiScript;
		MaterialMap _materials;
		MeshDataArray _meshDataArray;
		JointAttachMap _jointAttachments;
		JointAttachArray _jointAttachArray;
		AnimMap _animations;
		SoundMap _sounds;

		ModelClass _class;
		static const int MAX_NAME_LENGTH = 64;
		char _name[MAX_NAME_LENGTH];
		bool _clip;
		int _lifePoints;

		static const char* _classNames[MODEL_CLASS_COUNT];
	};

}


#endif // _ENGINE_MODEL_ENTITY_H_
