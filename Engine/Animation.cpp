
#include "BaseLib/FileSys.h"
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

	const char* Animation::_commonAnimNames[ANIM_COUNT] =
	{
		"Idle",
		"Walk",
		"Run",
		"Pain",
		"Death 1",
		"Death 2",
		"Death 3",
		"Attack",
		"Melee Attack",
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
		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(file_name, _t("rb"));

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
			delete[] _tracks[i].name;
		_tracks.Clear();
		_combinedTransforms.Clear();
	}

	void Animation::GetJointTransforms(float time, math3d::mat4f* transforms, const Joint* root_joint, const Joint* joint_array) const
	{
		JointCombinedMatrix(time, root_joint);
		size_t count = _tracks.GetCount();
		for(size_t i = 0; i < count; ++i)
			mul(transforms[i], joint_array[i].offsetMatrix, _combinedTransforms[i]);
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
				mat4f local;
				EvalTime(time, local, ptr);
				mul(_combinedTransforms[ptr->index], local, _combinedTransforms[ptr->parent->index]);
			}
			else
			{
				EvalTime(time, _combinedTransforms[ptr->index], ptr);
			}
			JointCombinedMatrix(time, ptr->child);

			ptr = ptr->sibling;
		}
	}

	const char* Animation::GetAnimTypeName(AnimType anim)
	{
		if(anim < ANIM_COUNT)
			return _commonAnimNames[anim];
		else
			return "";
	}

	AnimType Animation::GetAnimTypeByName(const char* name)
	{
		for(int i = 0; i < ANIM_COUNT; ++i)
		{
			if(!strcmp(name, _commonAnimNames[i]))
				return (AnimType)i;
		}
		return ANIM_CUSTOM;
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

		transform.set_identity();
		if(valid[ANIM_CH_SCALE_X])
			transform(0, 0) = results[ANIM_CH_SCALE_X];
		if(valid[ANIM_CH_SCALE_Y])
			transform(1, 1) = results[ANIM_CH_SCALE_Y];
		if(valid[ANIM_CH_SCALE_Z])
			transform(2, 2) = results[ANIM_CH_SCALE_X];
		if(valid[ANIM_CH_ROTATE_X])
			transform.rotate_x(rad2deg(results[ANIM_CH_ROTATE_X]));
		if(valid[ANIM_CH_ROTATE_Y])
			transform.rotate_y(rad2deg(results[ANIM_CH_ROTATE_Y]));
		if(valid[ANIM_CH_ROTATE_Z])
			transform.rotate_z(rad2deg(results[ANIM_CH_ROTATE_Z]));
		if(valid[ANIM_CH_TRANSLATE_X])
			transform.row3.x = results[ANIM_CH_TRANSLATE_X];
		else
			transform.row3.x = joint->jointMatrix.row3.x;
		if(valid[ANIM_CH_TRANSLATE_Y])
			transform.row3.y = results[ANIM_CH_TRANSLATE_Y];
		else
			transform.row3.y = joint->jointMatrix.row3.y;
		if(valid[ANIM_CH_TRANSLATE_Z])
			transform.row3.z = results[ANIM_CH_TRANSLATE_Z];
		else
			transform.row3.z = joint->jointMatrix.row3.z;


		mat3f t1(transform), t2(joint->jointMatrix), t3;
		mul(t3, t1, t2);
		transform.set3x3(t3);
	}

}
