
#ifndef _ENGINE_ANIMATION_H_
#define _ENGINE_ANIMATION_H_

#include "BaseLib/Math/math3d.h"
#include "BaseLib/StaticArray.h"
#include "Engine/Common.h"


namespace Engine
{

	struct Joint;


	enum AnimType
	{
		ANIM_CUSTOM = -1,
		ANIM_IDLE,
		ANIM_WALK,
		ANIM_RUN,
		ANIM_PAIN,
		ANIM_DEATH1,
		ANIM_DEATH2,
		ANIM_DEATH3,
		ANIM_ATTACK,
		ANIM_MELEE_ATTACK,

		ANIM_COUNT,
	};

	enum AnimChannelType
	{
		ANIM_CH_TRANSLATE_X,
		ANIM_CH_TRANSLATE_Y,
		ANIM_CH_TRANSLATE_Z,
		ANIM_CH_ROTATE_X,
		ANIM_CH_ROTATE_Y,
		ANIM_CH_ROTATE_Z,
		ANIM_CH_SCALE_X,
		ANIM_CH_SCALE_Y,
		ANIM_CH_SCALE_Z,

		ANIM_CH_COUNT,
	};

	struct AnimChannel
	{
		AnimChannelType channel;
		StaticArray<float> values;
	};

	struct AnimTrack
	{
		char* name;
		int parentIndex;
		StaticArray<AnimChannel> channels;
	};


	class ENGINE_API Animation
	{
	public:
		Animation();
		~Animation();

		bool Load(const tchar* file_name);
		void Unload();
		void GetJointTransforms(float time, math3d::mat4f* transforms, const Joint* root_joint, const Joint* joint_array) const;
		float GetLength() const
			{ return _animationLength; }
		int GetFPS() const
			{ return _framesPerSec; }
		int GetFrameCount() const
			{ return _frameCount; }

		static const char* GetAnimTypeName(AnimType anim);
		static AnimType GetAnimTypeByName(const char* name);

	private:
		void JointCombinedMatrix(float time, const Joint* joint) const;
		void EvalTime(float time, math3d::mat4f& transform, const Joint* joint) const;

		float	_animationLength;
		int		_framesPerSec;
		int		_frameCount;
		StaticArray<AnimTrack> _tracks;
		mutable StaticArray<math3d::mat4f> _combinedTransforms;
		static const char* _commonAnimNames[ANIM_COUNT];
	};

}


#endif // _ENGINE_ANIMATION_H_
