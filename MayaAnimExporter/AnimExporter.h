
#ifndef _ANIM_EXPORTER_H_
#define _ANIM_EXPORTER_H_

#include <list>
#include <maya/MPxFileTranslator.h>
#include <maya/MTime.h>
#include "BaseLib/Math/math3d.h"

#pragma warning(disable: 4996)

class MSelectionList;
class MDagPath;


enum
{
	TRANSLATE_X,
	TRANSLATE_Y,
	TRANSLATE_Z,
	ROTATE_X,
	ROTATE_Y,
	ROTATE_Z,
	SCALE_X,
	SCALE_Y,
	SCALE_Z,
};

struct JointAnim
{
	MString name;
	MString parentName;
	int parentIndex;
	std::list<float> channels[9];
};

class AnimExporter: public MPxFileTranslator
{
public:
	AnimExporter();

	static void* creator();

	virtual MStatus writer(const MFileObject& file_path, const MString& optionsString, MPxFileTranslator::FileAccessMode mode);
	virtual bool haveWriteMethod() const;
	virtual bool haveReadMethod() const;
	virtual	bool canBeOpened() const;
	virtual MString defaultExtension() const;
	virtual MFileKind identifyFile(const MFileObject &file, const char* buffer, short size) const;

private:
	MStatus WriteToFile(const char* file_name);
	bool ExtractAnimations(FileAccessMode mode);
	bool GetSelectionList(MSelectionList& list, FileAccessMode mode);
	void ProcessPath(const MDagPath &path);
	void ProcessAnimatedPlugs(const MPlugArray &animated_plugs, const MObject &node);
	void ProcessKeyframes(std::list<float>& value_list, int channel, const MObject& anim_node);
	void Cleanup();
	static int ChannelNameToIndex(const MString& name);
	static const char* ChannelIndexToName(int index);
#ifdef _DEBUG
	void _debug(const char* file_name);
#endif

	std::list<JointAnim> _jointAnimations;
	MTime _animationStart;
	MTime _animationEnd;
	float _animationLength; // in seconds
	int _frameCount;
	static const char* _channelNames[9];
	static const int _sampleRate = 24; // frames per second
};


#endif // _ANIM_EXPORTER_H_
