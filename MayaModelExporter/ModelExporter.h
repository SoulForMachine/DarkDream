
#ifndef _MODEL_EXPORTER_H_
#define _MODEL_EXPORTER_H_

#include <list>
#include <vector>
#include <string>
#include <maya/MPxFileTranslator.h>
#include <maya/MFnDagNode.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MObjectArray.h>
#include <maya/MStringArray.h>
#include <maya/MMatrix.h>
#include "BaseLib/Math/math3d.h"

#pragma warning(disable: 4996)


struct BoundingBox
{
	math3d::vec3f minPoint;
	math3d::vec3f maxPoint;
};

struct Material
{
	std::string name;
	int index; // index of a material in the output list
};

struct Mesh
{
	Mesh()
	{
		positions = 0;
		normals = 0;
		tangents = 0;
		binormals = 0;
		uvs = 0;
		weights = 0;
		boneIndices = 0;
		indices = 0;
		vertexCount = 0;
		indexCount = 0;
	}

	~Mesh()
	{
		delete[] positions;
		delete[] normals;
		delete[] tangents;
		delete[] binormals;
		delete[] uvs;
		delete[] weights;
		delete[] boneIndices;
		delete[] indices;
	}

	int vertexCount;
	int indexCount;
	int materialIndex;
	int flags;
	math3d::vec3f* positions;
	math3d::vec3f* normals;
	math3d::vec3f* tangents;
	math3d::vec3f* binormals;
	math3d::vec2f* uvs;
	math3d::vec2f* weights;
	math3d::vec2ub* boneIndices;
	unsigned short* indices;
	BoundingBox bbox;
};

struct TempMesh
{
	MString				dagPath;
	MFloatPointArray	vertexArray;
	MFloatVectorArray	normalArray;
	MString				uvSetName;
	MFloatArray			uArray;
	MFloatArray			vArray;
	MDoubleArray		weights;
	MIntArray			bones;
	std::vector<Material>		shaders; // an array of shaders for this mesh
	MIntArray			shaderIndices; // an index into shaders array for each polygon
	MIntArray			vertexIndices;
	MIntArray			normalIndices;
	MIntArray			uvIndices;
};

struct Joint
{
	MString name;
	MString pathName;
	MMatrix bindMatrix;
	MMatrix localMatrix;
	int index;
	int parentIndex;
	MString parentName;
};

class ModelExporter: public MPxFileTranslator
{
public:
	ModelExporter();

	static void* creator();

	virtual MStatus writer(const MFileObject& file_path, const MString& optionsString, MPxFileTranslator::FileAccessMode mode);
	virtual bool haveWriteMethod() const;
	virtual bool haveReadMethod() const;
	virtual	bool canBeOpened() const;
	virtual MString defaultExtension() const;
	virtual MFileKind identifyFile(const MFileObject &file, const char* buffer, short size) const;

private:
	bool IsVisible(MFnDagNode& fnDag, MStatus& status);
	MStatus ExtractMeshes(MPxFileTranslator::FileAccessMode mode);
	MStatus ProcessMesh(const MDagPath& dagPath);
	MStatus ProcessSkinningData();
	MStatus ProcessBones();
	bool GetJointBindMatrix(const MObject& joint, MMatrix& world_matrix, MMatrix& local_matrix);
	bool CreateOutputMeshes();
	void CalculateTangentsAndBinormals();
	MStatus WriteToFile(const char* file_name);
	void Cleanup();
	TempMesh* GetTempMesh(const MString& dag_path);
	Joint* FindJointByPath(const MString& dag_path);
	int AddMaterial(const Material& material);
#ifdef _DEBUG
	void _debug(const char* file_name);
	void _debug_out_bones(const char* file_name);
#endif

	std::list<TempMesh*> _tempMeshes;
	std::list<Joint*> _joints;

	// for output
	std::list<Mesh*> _meshes;
	std::list<Material> _materials;
	BoundingBox _bbox;
};


#endif // _MODEL_EXPORTER_H_
