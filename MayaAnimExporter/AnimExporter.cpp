
#include <cassert>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MObjectArray.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnSet.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MAnimUtil.h>
#include <maya/MFnIkJoint.h>
#include <maya/MAnimControl.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MDistance.h>

#include "AnimExporter.h"


#define FILE_MAGIC	('A' | ('N' << 8) | ('M' << 16) | ('1' << 24))
#define FILE_VERSION	1

using namespace std;
using namespace math3d;


#ifdef _DEBUG

void AnimExporter::_debug(const char* file_name)
{
	FILE* file = fopen(file_name, "wt");
	if(!file)
		return;

	fprintf(file, "Animation start time: %.2f\n", _animationStart.value());
	fprintf(file, "Animation end time: %.2f\n\n", _animationEnd.value());
	for(list<JointAnim>::const_iterator it = _jointAnimations.begin(); it != _jointAnimations.end(); ++it)
	{
		const JointAnim& anim = *it;
		fprintf(file, "Joint name: %s\n", anim.name);
		fprintf(file, "Joint parent's name: %s\n", anim.parentName);
		fprintf(file, "Joint parent's index: %d\n", anim.parentIndex);
		fprintf(file, "Keyframes:\n");
		for(int i = 0; i < sizeof(_channelNames) / sizeof(*_channelNames); ++i)
		{
			if(!anim.channels[i].empty())
			{
				fprintf(file, "\t%s\n", ChannelIndexToName(i));
				int k = (int)_animationStart.value();
				for(list<float>::const_iterator it = anim.channels[i].begin(); it != anim.channels[i].end(); ++it)
				{
					float val;
					if(i >= 3 && i <= 5)
						val = rad2deg(*it);
					else
						val = *it;
					fprintf(file, "\t\t%d:\t %.4f\n", k++, val);
				}
				fprintf(file, "\n\n");
			}
		}
	}

	fclose(file);
}

#endif

const char* AnimExporter::_channelNames[9] =
{
	"translateX",
	"translateY",
	"translateZ",
	"rotateX",
	"rotateY",
	"rotateZ",
	"scaleX",
	"scaleY",
	"scaleZ"
};

AnimExporter::AnimExporter()
{
}

void* AnimExporter::creator()
{
	return new AnimExporter;
}

MStatus AnimExporter::writer(const MFileObject& file_path, const MString& optionsString, FileAccessMode mode)
{
	MStatus result = MStatus::kFailure;
	if(ExtractAnimations(mode))
	{
	//	_debug(file_path.fullName().asChar());
	//	result = MStatus::kSuccess;
		result = WriteToFile(file_path.fullName().asChar());
	}
	Cleanup();
	return result;
}

bool AnimExporter::ExtractAnimations(FileAccessMode mode)
{
	MSelectionList list;
	if(!GetSelectionList(list, mode))
	{
		MGlobal::displayError("Failed to get the list of objects for export.");
		return false;
	}

	// get animation start and end time
	_animationStart = MAnimControl::animationStartTime();
	_animationEnd = MAnimControl::animationEndTime();
	/*if(_animationStart.unit() != MTime::kFilm || _animationEnd.unit() != MTime::kFilm)
	{
		MGlobal::displayError("Only Film unit supported for animation timeline.");
		return false;
	}*/
	if(_animationStart.unit() != MTime::kFilm)
		_animationStart.setUnit(MTime::kFilm);
	if(_animationEnd.unit() != MTime::kFilm)
		_animationEnd.setUnit(MTime::kFilm);
	_frameCount = (int)_animationEnd.value() - (int)_animationStart.value() + 1;
	_animationLength = (float)(_frameCount - 1) / (float)_sampleRate;

	// Now extract the animation for each object on the selection list
	//
	unsigned int num_objects = list.length();
	for(unsigned int i = 0; i < num_objects; i++)
	{
		MDagPath path;
		MObject node;
		if(list.getDagPath (i, path) == MS::kSuccess)
		{
			ProcessPath(path);
		}
	}

	// find parent index for each joint
	for(std::list<JointAnim>::iterator it = _jointAnimations.begin(); it != _jointAnimations.end(); ++it)
	{
		int index = 0;
		for(std::list<JointAnim>::iterator it2 = _jointAnimations.begin(); it2 != _jointAnimations.end(); ++it2)
		{
			if(it->parentName == it2->name)
			{
				it->parentIndex = index;
				break;
			}
			++index;
		}
	}

	return true;
}

bool AnimExporter::GetSelectionList(MSelectionList& list, FileAccessMode mode)
{
	MStatus status;
	if(mode == kExportAccessMode)
	{
		// Add the joint objects
		MItDag dag_it(MItDag::kDepthFirst, MFn::kJoint, &status);
		if(status != MStatus::kSuccess)
			return false;
		for(; !dag_it.isDone(); dag_it.next())
		{
			MDagPath path;
			if(dag_it.getPath(path) == MS::kSuccess)
			{
				list.add(path);
			}
		}
	}
	else if(mode == kExportActiveAccessMode)
	{
		status = MGlobal::getActiveSelectionList(list);
		if(status != MStatus::kSuccess)
			return false;
	}

	return true;
}

//void AnimExporter::ProcessPath(const MDagPath &path)
//{
//	MPlugArray animated_plugs;
//	MObject node = path.node();
//	MAnimUtil::findAnimatedPlugs(path, animated_plugs);
//	ProcessAnimatedPlugs(animated_plugs, node);
//}

void AnimExporter::ProcessPath(const MDagPath &path)
{
	// support animation of joints only
	MObject node = path.node();
	if(node.apiType() != MFn::kJoint)
		return;

	JointAnim joint_anim;
	MFnIkJoint joint(node);
	joint_anim.name = joint.name();
	MObject parent = joint.parent(0);
	if(parent.apiType() == MFn::kJoint)
	{
		MFnIkJoint parent_joint(parent);
		joint_anim.parentName = parent_joint.name();
	}
	else
	{
		joint_anim.parentName = "";
	}
	joint_anim.parentIndex = -1;

	int start_frame = (int)_animationStart.value();
	int end_frame = (int)_animationEnd.value();
	MDistance dist(0.0, MDistance::uiUnit());
	for(int i = start_frame; i <= end_frame; ++i)
	{
		// set the current frame
		MAnimControl::setCurrentTime(MTime(i, MTime::kFilm));

		// attach the function set to the object
		MFnTransform fn(path);

		// get the rotation for the bone this frame
		MEulerRotation rot;
		fn.getRotation(rot);
		joint_anim.channels[ROTATE_X].push_back((float)rot.x);
		joint_anim.channels[ROTATE_Y].push_back((float)rot.y);
		joint_anim.channels[ROTATE_Z].push_back((float)rot.z);

		// get translation
		MVector transl = fn.getTranslation(MSpace::kTransform);
		dist.setValue(transl.x);
		joint_anim.channels[TRANSLATE_X].push_back((float)dist.asMeters());
		dist.setValue(transl.y);
		joint_anim.channels[TRANSLATE_Y].push_back((float)dist.asMeters());
		dist.setValue(transl.z);
		joint_anim.channels[TRANSLATE_Z].push_back((float)dist.asMeters());

		// get scale
		double scale[3];
		fn.getScale(scale);
		joint_anim.channels[SCALE_X].push_back((float)scale[0]);
		joint_anim.channels[SCALE_Y].push_back((float)scale[1]);
		joint_anim.channels[SCALE_Z].push_back((float)scale[2]);
	}

	_jointAnimations.push_back(joint_anim);
}

void AnimExporter::ProcessAnimatedPlugs(const MPlugArray &animated_plugs, const MObject &node)
{
	// support animation of joints only
	if(node.apiType() != MFn::kJoint)
		return;

	JointAnim joint_anim;
	MFnIkJoint joint(node);
	joint_anim.name = joint.name();
	MObject parent = joint.parent(0);
	if(parent.apiType() == MFn::kJoint)
	{
		MFnIkJoint parent_joint(parent);
		joint_anim.parentName = parent_joint.name();
	}
	else
	{
		joint_anim.parentName = "";
	}
	joint_anim.parentIndex = -1;

	// Walk through each animated plug and write out the animation curve(s)
	unsigned int num_plugs = animated_plugs.length();
	for(unsigned int i = 0; i < num_plugs; i++)
	{
		MPlug plug = animated_plugs[i];
		MObjectArray animation;
		if(!MAnimUtil::findAnimation(plug, animation))
			continue;

		MObject attr_obj = plug.attribute();
		MFnAttribute leaf_attr(attr_obj);
		int ch_index = ChannelNameToIndex(leaf_attr.name());
		if(ch_index == -1)
			continue;

		list<float>& value_list = joint_anim.channels[ch_index];
		unsigned int num_curves = animation.length();
		// Write out each animation curve that animates the plug
		for(unsigned int j = 0; j < num_curves; ++j)
		{
			MObject anim_curve_node = animation[j];
			if(anim_curve_node.hasFn(MFn::kAnimCurve))
			{
				ProcessKeyframes(value_list, ch_index, anim_curve_node);
				break; // Take only 1st curve
			}
		}
	}

	_jointAnimations.push_back(joint_anim);
}

void AnimExporter::ProcessKeyframes(list<float>& value_list, int channel, const MObject& anim_node)
{
	bool has_anim = false;
	MFnAnimCurve anim_curve(anim_node);
	unsigned int num_keys = anim_curve.numKeys();
	for(unsigned int key_i = 0; key_i < num_keys; ++key_i)
	{
		float val = (float)anim_curve.value(key_i);
		if(channel >= TRANSLATE_X && channel <= ROTATE_Z)
		{
			// we have animation if translation or rotation value is not 0
			if(fcmp_ne(val, 0.0f))
			{
				has_anim = true;
				break;
			}
		}
		else if(channel >= SCALE_X && channel <= SCALE_Z)
		{
			// we have animation if scale is not 1
			if(fcmp_ne(val, 1.0f))
			{
				has_anim = true;
				break;
			}
		}
	}

	if(has_anim)
	{
		int start = (int)_animationStart.value();
		int end = (int)_animationEnd.value();
		for(int i = start; i <= end; ++i)
		{
			float val = (float)anim_curve.evaluate(MTime((double)i, MTime::kFilm));
			value_list.push_back(val);
		}
	}
}

int AnimExporter::ChannelNameToIndex(const MString& name)
{
	for(int i = 0; i < sizeof(_channelNames) / sizeof(*_channelNames); ++i)
	{
		if(name == _channelNames[i])
			return i;
	}

	return -1;
}

const char* AnimExporter::ChannelIndexToName(int index)
{
	return _channelNames[index];
}

MStatus AnimExporter::WriteToFile(const char* file_name)
{
	if(_jointAnimations.empty())
	{
		MGlobal::displayError("No animations to export.");
		return MStatus::kFailure;
	}

	FILE* file = fopen(file_name, "wb");
	if(!file)
	{
		MGlobal::displayError(MString("Could not open file for writing: ") + file_name);
		return MStatus::kFailure;
	}

	// write header
	int magic = FILE_MAGIC;
	fwrite(&magic, sizeof(int), 1, file);
	int version = FILE_VERSION;
	fwrite(&version, sizeof(int), 1, file);
	fwrite(&_animationLength, sizeof(float), 1, file);
	fwrite(&_sampleRate, sizeof(int), 1, file);
	fwrite(&_frameCount, sizeof(int), 1, file);
	int joint_count = _jointAnimations.size();
	fwrite(&joint_count, sizeof(int), 1, file);

	// write joints and their keyframes
	for(list<JointAnim>::const_iterator joint_it = _jointAnimations.begin(); joint_it != _jointAnimations.end(); ++joint_it)
	{
		int name_len = (int)joint_it->name.length();
		fwrite(&name_len, sizeof(int), 1, file);
		fwrite(joint_it->name.asChar(), sizeof(char), name_len, file);
		fwrite(&joint_it->parentIndex, sizeof(int), 1, file);
		
		int num_channels = sizeof(_channelNames) / sizeof(*_channelNames);
		int ef_num_channels = 0;
		for(int i = 0; i < num_channels; ++i)
		{
			if(!joint_it->channels[i].empty())
				++ef_num_channels;
		}
		fwrite(&ef_num_channels, sizeof(int), 1, file);

		for(int i = 0; i < num_channels; ++i)
		{
			const list<float>& channel = joint_it->channels[i];
			if(channel.size())
			{
				assert(channel.size() == _frameCount);
				unsigned char ch = (unsigned char)i;
				fwrite(&ch, sizeof(unsigned char), 1, file);
				for(list<float>::const_iterator val_it = channel.begin(); val_it != channel.end(); ++val_it)
				{
					float val = *val_it;
					fwrite(&val, sizeof(float), 1, file);
				}
			}
		}
	}

	fclose(file);

	return MStatus::kSuccess;
}

void AnimExporter::Cleanup()
{
	_jointAnimations.clear();
}

bool AnimExporter::haveWriteMethod() const
{
	return true;
}

bool AnimExporter::haveReadMethod() const
{
	return false;
}

bool AnimExporter::canBeOpened() const
{
	return true;
}

MString AnimExporter::defaultExtension() const
{
	return MString("anima");
}

MPxFileTranslator::MFileKind AnimExporter::identifyFile(const MFileObject &file, const char* buffer, short size) const
{
	const char* name = file.name().asChar();
	int   nameLength = (int)strlen(name);

	if ((nameLength > 6) && !strcasecmp(name+nameLength-6, ".anima"))
	{
		return kIsMyFileType;
	}

	return kNotMyFileType;
}
