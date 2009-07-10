
#ifndef _ENGINE_MODEL_ENTITY_H_
#define _ENGINE_MODEL_ENTITY_H_

#include "BaseLib/AABBox.h"
#include "BaseLib/HashMap.h"
#include "BaseLib/Math/mat4.h"
#include "Animation.h"
#include "RenderableEntity.h"
#include "Entity.h"



namespace Engine
{

	class FileResource;
	class ModelRes;
	class AIScriptRes;
	class ModelEntityRes;
	class AnimationRes;
	class SoundRes;



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

		struct JointAttachData
		{
			char* name;
			JointAttachType type;
			const FileResource* attachment;
		};

		struct AnimData
		{
			char* name;
			AnimType type;
			const AnimationRes* animation;
		};

		struct SoundData
		{
			char* name;
			const SoundRes* sound;
		};

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
		void SetupModelData(); // call after model has been loaded
		bool Save(const tchar* file_name);
		void Unload();

		const math3d::mat4f& GetWorldTransform() const
			{ return _worldMat; }
		void SetWorldTransform(const math3d::mat4f& transform);
		const StaticArray<math3d::mat4f>& GetJointTransforms() const
			{ return _jointMatPalette; }
		const AABBox& GetWorldBoundingBox() const
			{ return _worldBBox; }
		void UpdateGraphics(int frame_time);
		void ActiveAnimation(const char* anim_name);

		const ModelRes* GetModelRes() const
			{ return _model; }
		const AIScriptRes* GetAIScriptRes() const
			{ return _aiScript; }
		const JointAttachMap& GetJointAttachments() const
			{ return _jointAttachments; }
		const AnimMap& GetAnimations() const
			{ return _animations; }
		const SoundMap& GetSounds() const
			{ return _sounds; }

		bool SetModel(const tchar* file_name);
		bool SetAIScript(const tchar* file_name);
		bool SetJointAttachment(const char* joint_name, const tchar* file_name);
		bool RemoveJointAttachment(const char* joint_name);
		bool AddAnimation(const char* anim_name, const tchar* file_name);
		bool RemoveAnimation(const char* anim_name);
		bool AddSound(const char* snd_name, const tchar* file_name);
		bool RemoveSound(const char* snd_name);

		EntityClass GetClass() const
			{ return _class; }
		void SetClass(EntityClass cls)
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
		float GetTransparency() const
			{ return _transparency; }
		void SetTransparency(float transp)
			{ _transparency = transp; }
		bool HasTransparency() const
			{ return (_transparency > 0.001f); }

	private:
		static JointAttachType GetJointAttachTypeByExt(const tchar* file_name);
		void ClearModelData();
		void CalcWorldBBox();

		AABBox _worldBBox; // model's world space bounding box
		math3d::mat4f _worldMat;
		StaticArray<math3d::mat4f> _jointMatPalette;
		float _animTime;
		AnimData* _curAnim;

		const ModelRes* _model;
		const AIScriptRes* _aiScript;
		JointAttachMap _jointAttachments;
		JointAttachArray _jointAttachArray;
		AnimMap _animations;
		SoundMap _sounds;

		EntityClass _class;
		static const int MAX_NAME_LENGTH = 64;
		char _name[MAX_NAME_LENGTH];
		bool _clip;
		int _lifePoints;
		float _transparency;
	};

}


#endif // _ENGINE_MODEL_ENTITY_H_
