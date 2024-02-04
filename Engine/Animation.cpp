
#include "BaseLib/FileUtil.h"
#include "BaseLib/SSE.h"
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Console.h"
#include "Engine/EngineInternal.h"
#include "Engine/FileSystem.h"
#include "Model.h"
#include "Animation.h"


#define FILE_MAGIC ('A' | ('N' << 8) | ('M' << 16) | ('1' << 24))
#define FILE_VERSION	1


using namespace math3d;


namespace Engine
{

	const char* Animation::_commonAnimNames[ANIM_TYPE_COUNT] =
	{
		"Idle",
		"Walk",
		"Run",
		"Jump",
		"Alarm",
		"Pain",
		"Death_1",
		"Death_2",
		"Attack_1",
		"Attack_2",
	};

	Animation::Animation()
	{
	}

	Animation::~Animation()
	{
		Unload();
	}

	bool Animation::Load(const tchar* file_name)
	{
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(file_name, _t("rb"));

		if(!file)
		{
			Console::PrintError("Failed to open animation file: %ls", file_name);
			return false;
		}

		int magic = file->ReadInt();
		if(magic != FILE_MAGIC)
		{
			Console::PrintError("Not a valid animation file: %ls", file_name);
			return false;
		}

		int version = file->ReadInt();
		if(version != FILE_VERSION)
		{
			Console::PrintError("Wrong animation file version: %d, for file: %ls", version, file_name);
			return false;
		}

		_animationLength = file->ReadFloat();
		_framesPerSec = file->ReadInt();
		_frameCount = file->ReadInt();
		int track_count = file->ReadInt();
		_tracks.SetCount(track_count);
		_combinedTransforms.SetCount(track_count);

		for(int i = 0; i < track_count; ++i)
		{
			int len = file->ReadInt() + 1;
			_tracks[i].name = new char[len];
			file->ReadString(_tracks[i].name, len);
			_tracks[i].parentIndex = file->ReadInt();
			int chan_count = file->ReadInt();
			_tracks[i].channels.SetCount(chan_count);
			for(int j = 0; j < chan_count; ++j)
			{
				_tracks[i].channels[j].channel = (AnimChannelType)file->ReadUnsignedChar();
				_tracks[i].channels[j].values.SetCount(_frameCount);
				file->Read(_tracks[i].channels[j].values.GetData(), sizeof(float) * _frameCount);
			}
		}

		return true;
	}

	void Animation::Unload()
	{
		for(size_t i = 0; i < _tracks.GetCount(); ++i)
			Memory::Delete(_tracks[i].name);
		_tracks.Clear();
		_combinedTransforms.Clear();
	}

	void Animation::GetJointTransforms(float time, math3d::mat4f* transforms, const Joint* root_joint, const Joint* joint_array) const
	{
		JointCombinedMatrix(time, root_joint);
		size_t count = _tracks.GetCount();
		for(size_t i = 0; i < count; ++i)
			SSE_MultMatrices4x4(transforms[i], joint_array[i].offsetMatrix, _combinedTransforms[i]);
	}

	void Animation::JointCombinedMatrix(float time, const Joint* joint) const
	{
		if(!joint)
			return;

		const Joint* ptr = joint;
		while(ptr)
		{
			if(ptr->parent)
			{
				SSE_ALIGN mat4f local;
				EvalTime(time, local, ptr);
				SSE_MultMatrices4x4(_combinedTransforms[ptr->index], local, _combinedTransforms[ptr->parent->index]);
			}
			else
			{
				EvalTime(time, _combinedTransforms[ptr->index], ptr);
			}
			JointCombinedMatrix(time, ptr->child);

			ptr = ptr->sibling;
		}
	}

	bool Animation::IsSkeletCompatible(const StaticArray<Joint>& joint_array) const
	{
		if(joint_array.GetCount() != _tracks.GetCount())
			return false;

		for(size_t i = 0; i < joint_array.GetCount(); ++i)
		{
			if(joint_array[i].parentIndex != _tracks[i].parentIndex)
				return false;
		}

		return true;
	}

	const char* Animation::GetAnimTypeName(AnimType anim)
	{
		if(anim < ANIM_TYPE_COUNT)
			return _commonAnimNames[anim];
		else
			return "";
	}

	Animation::AnimType Animation::GetAnimTypeByName(const char* name)
	{
		for(int i = 0; i < ANIM_TYPE_COUNT; ++i)
		{
			if(!strcmp(name, _commonAnimNames[i]))
				return (AnimType)i;
		}
		return ANIM_TYPE_CUSTOM;
	}

	void Animation::EvalTime(float time, math3d::mat4f& transform, const Joint* joint) const
	{
		const AnimTrack& track = _tracks[joint->index];
		if(track.channels.GetCount() == 0)
		{
			transform = joint->jointMatrix;
			return;
		}

		clamp(time, 0.0f, _animationLength);
		float frame = time / _animationLength * (_frameCount - 1);
		int key1 = (int)frame;
		int key2 = (key1 == (_frameCount - 1))? 0: key1 + 1;
		float frac = frame - floor(frame);
		float results[ANIM_CH_COUNT];
		bool valid[ANIM_CH_COUNT] = { false };

		for(size_t chan_i = 0; chan_i < track.channels.GetCount(); ++chan_i)
		{
			valid[track.channels[chan_i].channel] = true;
			const float* values = track.channels[chan_i].values.GetData();
			results[track.channels[chan_i].channel] = lerp(values[key1], values[key2], frac);
		}

		// build the matrix

		float cr = cos(results[ANIM_CH_ROTATE_X]);
		float sr = sin(results[ANIM_CH_ROTATE_X]);
		float cp = cos(results[ANIM_CH_ROTATE_Y]);
		float sp = sin(results[ANIM_CH_ROTATE_Y]);
		float cy = cos(results[ANIM_CH_ROTATE_Z]);
		float sy = sin(results[ANIM_CH_ROTATE_Z]);

		transform(0) = cp * cy;
		transform(1) = cp * sy;
		transform(2) = -sp;
		transform(3) = 0.0f;

		float srsp = sr * sp;
		float crsp = cr * sp;

		transform(4) = srsp * cy - cr * sy;
		transform(5) = srsp * sy + cr * cy;
		transform(6) = sr * cp;
		transform(7) = 0.0f;

		transform(8) = crsp * cy + sr * sy;
		transform(9) = crsp * sy - sr * cy;
		transform(10) = cr * cp;
		transform(11) = 0.0f;

		transform(12) = results[ANIM_CH_TRANSLATE_X];
		transform(13) = results[ANIM_CH_TRANSLATE_Y];
		transform(14) = results[ANIM_CH_TRANSLATE_Z];
		transform(15) = 1.0f;

		mat3f t1(transform), t2(joint->jointMatrix), t3;
		mul(t3, t1, t2);
		transform.set3x3(t3);
	}

}
