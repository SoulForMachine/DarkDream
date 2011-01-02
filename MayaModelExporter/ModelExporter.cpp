
#include <cstdio>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MPlug.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MDagPathArray.h>
#include <maya/MItGeometry.h>
#include <maya/MPlugArray.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnIkJoint.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFloatMatrix.h>
#include <maya/MDistance.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include "ModelExporter.h"

#define FILE_MAGIC	('M' | ('D' << 8) | ('L' << 16) | ('1' << 24))
#define FILE_VERSION	1

enum
{
	FLAG_UVS = 1,
	FLAG_TANGENTS = 2,
	FLAG_SKINDATA = 4
};

using namespace std;
using namespace math3d;


// extract file name with extension from absolute or relative file path
static
void ExtractFileNameExt(char* result, const char* file_path)
{
	const char* bslash = strrchr(file_path, '/');
	if(bslash)
		strcpy(result, bslash + 1);
	else
		strcpy(result, file_path);
}

static
void CalcTangent(vec3f& tgt, const vec3f& p0, const vec3f& p1, const vec3f& p2,
				 const vec2f& t0, const vec2f& t1, const vec2f& t2)
{
	//(v1 - v0)(P2 - P0) - (v2 - v0)(P1 - P0)
	//---------------------------------------
	//(v1 - v0)(u2 - u0) - (v2 - v0)(u1 - u0)

	vec3f tmp1 = (p2 - p0) * (t1.y - t0.y) - (p1 - p0) * (t2.y - t0.y);
	float tmp2 = (t1.y - t0.y) * (t2.x - t0.x) - (t2.y - t0.y) * (t1.x - t0.x);

	tgt = tmp1 / tmp2;
}

static
void CalcBinormal(vec3f& tgt, const vec3f& p0, const vec3f& p1, const vec3f& p2,
				 const vec2f& t0, const vec2f& t1, const vec2f& t2)
{
	//(u2 - u0)(P1 - P0) - (u1 - u0)(P2 - P0)
	//---------------------------------------
	//(v1 - v0)(u2 - u0) - (v2 - v0)(u1 - u0)

	vec3f tmp1 = (p1 - p0) * (t2.x - t0.x) - (p2 - p0) * (t1.x - t0.x);
	float tmp2 = (t1.y - t0.y) * (t2.x - t0.x) - (t2.y - t0.y) * (t1.x - t0.x);

	tgt = tmp1 / tmp2;
}

ModelExporter::ModelExporter()
{

}

void* ModelExporter::creator()
{
	return new ModelExporter;
}

#ifdef _DEBUG

#define SEPARATOR		fprintf(file, "---------------------------------------------------------------------\n")

void ModelExporter::_debug(const char* file_name)
{
	FILE* file = fopen(file_name, "wt");
	if(!file)
	{
		MGlobal::displayError(MString("Could not open file for writing: ") + file_name);
		return;
	}

	fprintf(file, "Number of meshes: %u\n\n", _tempMeshes.size());
	SEPARATOR;

	for(list<TempMesh*>::iterator it = _tempMeshes.begin(); it != _tempMeshes.end(); ++it)
	{
		TempMesh* mesh = *it;
		fprintf(file, "\n\nMesh: %s\n", mesh->dagPath.asChar());
		SEPARATOR;

		// points
		fprintf(file, "\nPoints count: %u\n", mesh->vertexArray.length());
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->vertexArray.length(); ++i)
		{
			fprintf(file, "\t%f, %f, %f\n", mesh->vertexArray[i].x, mesh->vertexArray[i].y, mesh->vertexArray[i].z);
		}

		// normals
		fprintf(file, "\nNormal count: %u\n", mesh->normalArray.length());
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->normalArray.length(); ++i)
		{
			fprintf(file, "\t%f, %f, %f\n", mesh->normalArray[i].x, mesh->normalArray[i].y, mesh->normalArray[i].z);
		}

		// tangents
		/*fprintf(file, "\nTangent count: %u\n", mesh->tangentArray.length());
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->tangentArray.length(); ++i)
		{
			fprintf(file, "\t%f, %f, %f\n", mesh->tangentArray[i].x, mesh->tangentArray[i].y, mesh->tangentArray[i].z);
		}*/

		// uvs
		fprintf(file, "\nUV set name: %s\nUV count: %u\n", mesh->uvSetName.asChar(), mesh->uArray.length());
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->uArray.length(); ++i)
		{
			fprintf(file, "\t%f, %f\n", mesh->uArray[i], mesh->vArray[i]);
		}

		// weights
		fprintf(file, "\nWeight count: %u\n", mesh->weights.length() / 2);
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->weights.length(); i += 2)
		{
			fprintf(file, "\t%f, %f\n", (float)mesh->weights[i], (float)mesh->weights[i + 1]);
		}

		// bone indices
		fprintf(file, "\nBone index count: %u\n", mesh->bones.length() / 2);
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->bones.length(); i += 2)
		{
			fprintf(file, "\t%d, %d\n", mesh->bones[i], mesh->bones[i + 1]);
		}

		// point indices
		fprintf(file, "\nPoint index count: %u\n", mesh->vertexIndices.length());
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->vertexIndices.length(); i += 3)
		{
			fprintf(file, "\t%d, %d, %d\n", mesh->vertexIndices[i], mesh->vertexIndices[i + 1], mesh->vertexIndices[i + 2]);
		}

		// normal indices
		fprintf(file, "\nNormal index count: %u\n", mesh->normalIndices.length());
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->normalIndices.length(); i += 3)
		{
			fprintf(file, "\t%d, %d, %d\n", mesh->normalIndices[i], mesh->normalIndices[i + 1], mesh->normalIndices[i + 2]);
		}

		// uv indices
		fprintf(file, "\nUV index count: %u\n", mesh->uvIndices.length());
		SEPARATOR;

		for(unsigned int i = 0; i < mesh->uvIndices.length(); i += 3)
		{
			fprintf(file, "\t%d, %d, %d\n", mesh->uvIndices[i], mesh->uvIndices[i + 1], mesh->uvIndices[i + 2]);
		}

		// shaders
		fprintf(file, "\nShaders\n");
		SEPARATOR;
		int i = 0;
		for(vector<Material>::iterator it = mesh->shaders.begin(); it != mesh->shaders.end(); ++it)
		{
			const Material& mat = *it;
			fprintf(file, "\t%s", mat.name.c_str());
			/*fprintf(
				file,
				"\t---Shader %d---\n\tdiffuse: %f\n\tspecular: %f, %f, %f\n\tspecular exp: %f\n\temission: %f, %f, %f\n\t"
				"transparency: %f\n\treflection: %f\n\tboth faces: %s\n\ttexture: %s\n\n",
				i++,
				mat.diffuse,
				mat.specularColor.r, mat.specularColor.g ,mat.specularColor.b,
				mat.specularExp,
				mat.emmission.r, mat.emmission.g, mat.emmission.b,
				mat.transparency,
				mat.reflection,
				mat.bothFaces? "yes": "no",
				mat.diffuseTexture);*/
		}

		// shader indices for every face
		fprintf(file, "\nShader index count: %u\n", mesh->shaderIndices.length());
		SEPARATOR;
		for(unsigned int i = 0; i < mesh->shaderIndices.length(); ++i)
		{
			fprintf(file, "\t%d\n", mesh->shaderIndices[i]);
		}
	}
	
	fclose(file);
}

void ModelExporter::_debug_out_bones(const char* file_name)
{
	FILE* file = fopen(file_name, "wt");

	for(list<Joint*>::iterator it = _joints.begin(); it != _joints.end(); ++it)
	{
		Joint& joint = **it;
		fprintf(file, "Name: %s\n\n", joint.name.asChar());
		
		fprintf(file, "World matrix:\n");
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				fprintf(file, "\t%.2f", joint.bindMatrix(i, j));
			}
			fprintf(file, "\n");
		}

		fprintf(file, "\nParent name: %s\nParent index: %d\n\n", joint.parentName.asChar(), joint.parentIndex);

		/*fprintf(file, "Transform matrix:\n");
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				fprintf(file, "\t%.2f", joint.xformMatrix(i, j));
			}
			fprintf(file, "\n");
		}*/
	}

	fclose(file);
}

#endif

MStatus ModelExporter::writer(const MFileObject& file_path, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
{
	MStatus result = MStatus::kFailure;

	if(ExtractMeshes(mode) == MStatus::kSuccess)
	{
	//	_debug(file_path.fullName().asChar());
	//	_debug_out_bones((file_path.fullName() + ".bones").asChar());
		if(CreateOutputMeshes())
			result = WriteToFile(file_path.fullName().asChar());
	}

	Cleanup();

	return result;
}

MStatus ModelExporter::ExtractMeshes(MPxFileTranslator::FileAccessMode mode)
{
	MStatus status;

	if(mode == MPxFileTranslator::kExportAccessMode)
	{
		// extract all meshes
		MItDag it_mesh(MItDag::kDepthFirst, MFn::kMesh, &status);

		if(status == MStatus::kFailure)
		{
			MGlobal::displayError("MItDag::MItDag");
			return MStatus::kFailure;
		}

		for( ; !it_mesh.isDone(); it_mesh.next())
		{
			MDagPath dagPath;
			if(it_mesh.getPath(dagPath) == MStatus::kFailure)
			{
				MGlobal::displayError("MDagPath::getPath");
				return MStatus::kFailure;
			}

			MFnDagNode node(dagPath);
			if(IsVisible(node, status) && status == MStatus::kSuccess)
			{
				if(ProcessMesh(dagPath) == MStatus::kFailure)
				{
					return MStatus::kFailure;
				}
			}
		}
	}
	else if(mode == MPxFileTranslator::kExportActiveAccessMode)
	{
		// extract selected meshes
		MSelectionList sel_list;
		if(MGlobal::getActiveSelectionList(sel_list) == MStatus::kFailure)
		{
			MGlobal::displayError("MGlobal::getActiveSelectionList");
			return MStatus::kFailure;
		}

		MItSelectionList sel_it(sel_list, MFn::kMesh, &status);
		if(status == MStatus::kFailure)
		{
			return MStatus::kFailure;
		}

		for(sel_it.reset(); !sel_it.isDone(); sel_it.next())
		{
			MDagPath dagPath;

			if(sel_it.getDagPath(dagPath) == MStatus::kFailure)
			{
				MGlobal::displayError("MItSelectionList::getDagPath");
				return MStatus::kFailure;
			}

			if(ProcessMesh(dagPath) == MStatus::kFailure)
			{
				return MStatus::kFailure;
			}
		}
	}
	else
	{
		return MStatus::kFailure;
	}

	if(ProcessBones() == MStatus::kFailure)
		return MStatus::kFailure;

	if(ProcessSkinningData() == MStatus::kFailure)
		return MStatus::kFailure;

	return MStatus::kSuccess;
}

MStatus ModelExporter::ProcessMesh(const MDagPath& dagPath)
{
	MStatus status;

	MFnMesh mesh(dagPath, &status);
	if(status == MStatus::kFailure)
		return MStatus::kFailure;

	// check if the mesh is triangulated
	int num_polys = mesh.numPolygons();
	for(int i = 0; i < num_polys; ++i)
	{
		if(mesh.polygonVertexCount(i) > 3)
		{
			MGlobal::displayError("All meshes must be triangulated.");
			return MStatus::kFailure;
		}
	}

	TempMesh* m = new TempMesh;
	m->dagPath = dagPath.fullPathName();

	// if mesh is skinned, get the mesh that is coming into the skinCluster
	MFnMesh mesh2;
	if(0){
		MObject inObj;
		MObject dataObj1;
		MFnDagNode dagNode(dagPath);

		// the deformed mesh comes into the visible mesh
		// through its "inmesh" plug
		MPlug inMeshPlug = dagNode.findPlug("inMesh", &status);

		if (status == MS::kSuccess && inMeshPlug.isConnected())
		{
			// walk the tree of stuff upstream from this plug
			MItDependencyGraph dgIt(inMeshPlug,
				MFn::kInvalid,
				MItDependencyGraph::kUpstream,
				MItDependencyGraph::kDepthFirst,
				MItDependencyGraph::kPlugLevel,
				&status);

			if (MS::kSuccess == status)
			{
				dgIt.disablePruningOnFilter();
				int count = 0;

				for ( ; ! dgIt.isDone(); dgIt.next() )
				{
					MObject thisNode = dgIt.thisNode();

					// go until we find a skinCluster

					if (thisNode.apiType() == MFn::kSkinClusterFilter)
					{
						MFnSkinCluster skinCluster(thisNode);

						// get the mesh coming into the skinCluster.  This
						// is the mesh before being deformed but after
						// being edited/tweaked/etc.

						MPlug inputPlug = skinCluster.findPlug("input", &status);
						if (status == MS::kSuccess)
						{
							MPlug childPlug = inputPlug.elementByLogicalIndex(0);
							MPlug geomPlug = childPlug.child(0);

							geomPlug.getValue(dataObj1);

							// let use this mesh instead of the visible one
							status = mesh2.setObject(dataObj1);
							break;
						}
					}
				}
			}
		}
	}

	// points, normals
	mesh.getPoints(m->vertexArray, MSpace::kWorld);
	// convert points to meters
	MDistance dist(0.0, MDistance::uiUnit());
	for(unsigned int i = 0; i < m->vertexArray.length(); ++i)
	{
		dist.setValue(m->vertexArray[i].x);
		m->vertexArray[i].x = (float)dist.asMeters();
		dist.setValue(m->vertexArray[i].y);
		m->vertexArray[i].y = (float)dist.asMeters();
		dist.setValue(m->vertexArray[i].z);
		m->vertexArray[i].z = (float)dist.asMeters();
	}
//	int len = m->vertexArray.length();
	mesh.getNormals(m->normalArray, MSpace::kWorld);

	// uvs
	MStringArray set_names;
	mesh.getUVSetNames(set_names);
	m->uvSetName = set_names[0];
	mesh.getUVs(m->uArray, m->vArray, &m->uvSetName);

	// shaders
	MObjectArray shaders;
	MIntArray indices;
	mesh.getConnectedShaders(0, shaders, m->shaderIndices);
	for(unsigned int i = 0; i < shaders.length(); ++i)
	{
		bool shader_added = false;
		MFnDependencyNode shader_node(shaders[i]);					
		MPlug shader_plug = shader_node.findPlug("surfaceShader");
		if(!shader_plug.isNull())
		{
			MPlugArray connected_plugs;
			shader_plug.connectedTo(connected_plugs, true, false);
			if(connected_plugs.length() == 1)
			{
				MObject shader = connected_plugs[0].node();
				Material mat;
				MFnDependencyNode dep_node(shader);
				mat.name = dep_node.name().asChar();
				m->shaders.push_back(mat);
				shader_added = true;
			}
		}

		if(!shader_added)
		{
			Material default_mat;
			default_mat.name = "Default";
			m->shaders.push_back(default_mat);
		}
	}

	// indices
	unsigned int poly_count = mesh.numPolygons();
	for(unsigned int poly_i = 0; poly_i < poly_count; ++poly_i)
	{
		MIntArray vert_indices, normal_indices, tangent_indices, uv_indices;
		mesh.getPolygonVertices(poly_i, vert_indices);
		mesh.getFaceNormalIds(poly_i, normal_indices);

		unsigned int vert_count = mesh.polygonVertexCount(poly_i);
		tangent_indices.setLength(vert_count);
		uv_indices.setLength(vert_count);
		for(unsigned int vert_i = 0; vert_i < vert_count; ++vert_i)
		{
			mesh.getPolygonUVid(poly_i, vert_i, uv_indices[vert_i], &m->uvSetName);
		}

		// copy indices for this polygon
		for(unsigned int i = 0; i < vert_indices.length(); ++i)
			m->vertexIndices.append(vert_indices[i]);
		for(unsigned int i = 0; i < normal_indices.length(); ++i)
			m->normalIndices.append(normal_indices[i]);
		for(unsigned int i = 0; i < uv_indices.length(); ++i)
			m->uvIndices.append(uv_indices[i]);
	}

	_tempMeshes.push_back(m);

	return MStatus::kSuccess;
}

MStatus ModelExporter::ProcessSkinningData()
{
	MStatus status;

	// extract skinning data
	MItDependencyNodes it(MFn::kSkinClusterFilter, &status);

	for( ; !it.isDone(); it.next())
	{
		MFnSkinCluster skin_cluster(it.item());
		MDagPathArray infl_objs;
		skin_cluster.influenceObjects(infl_objs, &status);
		unsigned int geom_count = skin_cluster.numOutputConnections();
		for(unsigned int i = 0; i < geom_count; ++i)
		{
			unsigned int index = skin_cluster.indexForOutputConnection(i);
			MDagPath skin_path;
			skin_cluster.getPathAtIndex(index, skin_path);
			MFnDagNode node(skin_path);
			if(IsVisible(node, status) && status == MStatus::kSuccess)
			{
				MItGeometry git(skin_path);
				TempMesh* mesh = GetTempMesh(skin_path.fullPathName());
				if(mesh)
				{
					// reserve and initialize memory for weights and bones
					int count = mesh->vertexArray.length() * 2;
					mesh->weights.setLength(count);
					mesh->bones.setLength(count);
					for(int k = 0; k < count; ++k)
					{
						mesh->weights[k] = 0.0;
						mesh->bones[k] = -1;
					}

					// for each vertex, get its weights for each influence object
					for(int vert_i = 0; !git.isDone(); git.next(), ++vert_i)
					{
						MObject component = git.component();
						unsigned int infl_count = infl_objs.length();
						for(unsigned int j = 0; j < infl_count; ++j)
						{
							Joint* joint = FindJointByPath(infl_objs[j].fullPathName());
							MDoubleArray weights;
							skin_cluster.getWeights(skin_path, component, j, weights);
							if(weights.length() > 0)
							{
								if(mesh->bones[vert_i * 2] == -1 && weights[0] != 0.0)
								{
									mesh->weights[vert_i * 2] = weights[0];
									mesh->bones[vert_i * 2] = joint->index;
								}
								else if(mesh->bones[vert_i * 2 + 1] == -1 && weights[0] != 0.0)
								{
									mesh->weights[vert_i * 2 + 1] = weights[0];
									mesh->bones[vert_i * 2 + 1] = joint->index;
								}
							}
						}
					}

					for(int k = 0; k < count; ++k)
						if(mesh->bones[k] == -1)
							mesh->bones[k] = 0;
				}
			}
		}
	}

	return MStatus::kSuccess;
}

MStatus ModelExporter::ProcessBones()
{
	MStatus status;
	MMatrix world_matrix, local_matrix;

	MItDag it_joints(MItDag::kDepthFirst, MFn::kJoint, &status);
	int joint_index = 0;

	for( ; !it_joints.isDone(); it_joints.next())
	{
	//	if(GetJointBindMatrix(it_joints.currentItem(), world_matrix, local_matrix))
		{
			MDagPath path;
			it_joints.getPath(path);
			MFnIkJoint joint(path);

			Joint* j = new Joint;
			j->name = joint.name();
			j->pathName = path.fullPathName();
			MObject parent = joint.parent(0);
			if(parent.apiType() == MFn::kJoint)
			{
				MFnIkJoint parent_joint(parent);
				j->parentName = parent_joint.name();
			}
			else
			{
				j->parentName = "";
			}
			j->index = joint_index++;
			j->parentIndex = -1;

			double scale[3];
			joint.getScale(scale);
			MMatrix scale_mat;
			scale_mat[0][0] = scale[0];
			scale_mat[1][1] = scale[1];
			scale_mat[2][2] = scale[2];
			MQuaternion rot_orient;
			joint.getScaleOrientation(rot_orient);
			MMatrix rot_orient_mat = rot_orient.asMatrix();
			MQuaternion rot;
			joint.getRotation(rot);
			MMatrix rot_mat = rot.asMatrix();
			MQuaternion orient;
			joint.getOrientation(orient);
			MMatrix orient_mat = orient.asMatrix();
			double pscale[3];
			MFnTransform(parent).transformation().getScale(pscale, MSpace::kTransform);
			MMatrix pscale_mat;
			pscale_mat[0][0] = pscale[0];
			pscale_mat[1][1] = pscale[1];
			pscale_mat[2][2] = pscale[2];
			pscale_mat = pscale_mat.inverse();
			MVector transl = joint.translation(MSpace::kTransform);
			MMatrix transl_mat;
			transl_mat[3][0] = transl.x;
			transl_mat[3][1] = transl.y;
			transl_mat[3][2] = transl.z;

			local_matrix = scale_mat * rot_orient_mat /** rot_mat*/ * orient_mat * pscale_mat * transl_mat;
			world_matrix = path.inclusiveMatrixInverse();

			j->bindMatrix = world_matrix;
			j->localMatrix = local_matrix;
			_joints.push_back(j);
		}
	}

	// find parent index for each joint
	for(list<Joint*>::iterator it = _joints.begin(); it != _joints.end(); ++it)
	{
		int index = 0;
		for(list<Joint*>::iterator it2 = _joints.begin(); it2 != _joints.end(); ++it2)
		{
			if((*it)->parentName == (*it2)->name)
			{
				(*it)->parentIndex = index;
				break;
			}
			++index;
		}
	}

	return MStatus::kSuccess;
}

bool ModelExporter::GetJointBindMatrix(const MObject& joint, MMatrix& world_matrix, MMatrix& local_matrix)
{
	MStatus status;
	MFnDependencyNode fn_joint(joint);
	MObject attr_bind_pose = fn_joint.attribute("bindPose", &status);

	if(status == MStatus::kSuccess)
	{
		MPlugArray conn_plugs;
		MPlug bind_pose(joint, attr_bind_pose);
		bind_pose.connectedTo(conn_plugs, false, true);
		for(unsigned int i = 0; i < conn_plugs.length(); ++i)
		{
			if(conn_plugs[i].node().apiType() == MFn::kDagPose)
			{
				MObject attr_member = conn_plugs[i].attribute();
				MFnAttribute fn_attr(attr_member);
				if(fn_attr.name() == "worldMatrix")
				{
					MObject joint_object = conn_plugs[i].node();
					unsigned int index = conn_plugs[i].logicalIndex();

					MFnDependencyNode dag_pose(joint_object);

					MObject wma = dag_pose.attribute("worldMatrix");
					MPlug wmp(joint_object, wma);
					wmp.selectAncestorLogicalIndex(index, wma);
					MObject wm;
					wmp.getValue(wm);
					world_matrix = MFnMatrixData(wm).matrix().inverse();

					MObject xma = dag_pose.attribute("xformMatrix");
					MPlug xmp(joint_object, xma);
					xmp.selectAncestorLogicalIndex(index, xma);
					MObject xm;
					xmp.getValue(xm);
					MFnMatrixData md(xm);
					if(md.isTransformation())
					{
						local_matrix = md.transformation().asMatrix();
					}
					else
					{
						local_matrix.setToIdentity();
					}

					return true;
				}
			}
		}
	}

	return false;
}



struct _Vertex
{
	vec3f position;
	vec3f normal;
	vec2f uv;
	vec2f weights;
	vec2ub boneIndices;
};

struct _Triangle
{
	int vertices[3];
};

struct _Mesh
{
	int flags;
	int material;
	vector<_Vertex> vertices;
	vector<_Triangle> triangles;
};

__forceinline
bool CompareVertices(const _Vertex& v1, const _Vertex& v2, int flags)
{
	return	(
		fcmp_eq(v1.position.x, v2.position.x) &&
		fcmp_eq(v1.position.y, v2.position.y) &&
		fcmp_eq(v1.position.z, v2.position.z) &&
		fcmp_eq(v1.normal.x, v2.normal.x) &&
		fcmp_eq(v1.normal.y, v2.normal.y) &&
		fcmp_eq(v1.normal.z, v2.normal.z) &&
		(!(flags & FLAG_UVS) ||
			(fcmp_eq(v1.uv.x, v2.uv.x) &&
			fcmp_eq(v1.uv.y, v2.uv.y))) &&
		(!(flags & FLAG_SKINDATA) ||
			(fcmp_eq(v1.weights.x, v2.weights.x) &&
			fcmp_eq(v1.weights.y, v2.weights.y) &&
			(v1.boneIndices.x == v2.boneIndices.x) &&
			(v1.boneIndices.y == v2.boneIndices.y))) );
}

static
_Mesh* GetMesh(int mat_index, int flags, list<_Mesh*>& mesh_list)
{
	for(list<_Mesh*>::iterator mesh_it = mesh_list.begin(); mesh_it != mesh_list.end(); ++mesh_it)
	{
		_Mesh* m = *mesh_it;
		if(m->material == mat_index && m->flags == flags)
		{
			return m;
		}
	}

	_Mesh* new_mesh = new _Mesh;
	new_mesh->flags = flags;
	new_mesh->material = mat_index;
	new_mesh->vertices.reserve(1 << 16);
	new_mesh->triangles.reserve(1 << 20);
	mesh_list.push_back(new_mesh);
	return new_mesh;
}

inline
int AddVertex(const _Vertex& vert, vector<_Vertex>& vert_list, int flags)
{
	for(int vert_i = vert_list.size() - 1; vert_i >= 0; vert_i--)
	{
		if(CompareVertices(vert_list[vert_i], vert, flags))
			return vert_i;
	}

	vert_list.push_back(vert);
	return vert_list.size() - 1;
}

bool ModelExporter::CreateOutputMeshes()
{
	// find unique materials from all used shaders
	for(list<TempMesh*>::iterator it_mesh = _tempMeshes.begin(); it_mesh != _tempMeshes.end(); ++it_mesh)
	{
		for(vector<Material>::iterator it_mat = (*it_mesh)->shaders.begin(); it_mat != (*it_mesh)->shaders.end(); ++it_mat)
		{
			it_mat->index = AddMaterial(*it_mat);
		}
	}

	// create a mesh for all polygons with same material and vertex format
	list<_Mesh*> meshes;
	for(list<TempMesh*>::iterator mesh_it = _tempMeshes.begin(); mesh_it != _tempMeshes.end(); ++mesh_it)
	{
		TempMesh& mesh = **mesh_it;
		int flags = 0;
		if(mesh.uArray.length() > 0 && mesh.vArray.length() > 0)
			flags |= FLAG_UVS;
		if(mesh.weights.length() > 0 && mesh.bones.length() > 0)
			flags |= FLAG_SKINDATA;

		unsigned int num_tris = mesh.vertexIndices.length() / 3;

		for(unsigned int tri_i = 0; tri_i < num_tris; ++tri_i)
		{
			_Mesh* m = GetMesh(mesh.shaders[mesh.shaderIndices[tri_i]].index, flags, meshes);
			_Triangle triangle;

			for(int vert_i = 0; vert_i < 3; ++vert_i)
			{
				_Vertex vertex;

				int i = tri_i * 3 + vert_i;
				vertex.position.x = mesh.vertexArray[mesh.vertexIndices[i]].x;
				vertex.position.y = mesh.vertexArray[mesh.vertexIndices[i]].y;
				vertex.position.z = mesh.vertexArray[mesh.vertexIndices[i]].z;
				vertex.normal.x = mesh.normalArray[mesh.normalIndices[i]].x;
				vertex.normal.y = mesh.normalArray[mesh.normalIndices[i]].y;
				vertex.normal.z = mesh.normalArray[mesh.normalIndices[i]].z;
				if(flags & FLAG_UVS)
				{
					vertex.uv.x = mesh.uArray[mesh.uvIndices[i]];
					vertex.uv.y = mesh.vArray[mesh.uvIndices[i]];
				}
				if(flags & FLAG_SKINDATA)
				{
					vertex.weights.x = (float)mesh.weights[mesh.vertexIndices[i] * 2];
					vertex.weights.y = (float)mesh.weights[mesh.vertexIndices[i] * 2 + 1];
					vertex.boneIndices.x = (unsigned char)mesh.bones[mesh.vertexIndices[i] * 2];
					vertex.boneIndices.y = (unsigned char)mesh.bones[mesh.vertexIndices[i] * 2 + 1];
				}

				triangle.vertices[vert_i] = AddVertex(vertex, m->vertices, flags);
			}

			m->triangles.push_back(triangle);
		}
	}

	// make output meshes
	for(list<_Mesh*>::iterator mesh_it = meshes.begin(); mesh_it != meshes.end(); ++mesh_it)
	{
		_Mesh& mesh = **mesh_it;

		// output this mesh (also check if we don't have more than 2^16 vertices)
		Mesh* out_mesh = new Mesh;
		out_mesh->vertexCount = mesh.vertices.size();
		out_mesh->indexCount = mesh.triangles.size() * 3;
		out_mesh->materialIndex = mesh.material;
		out_mesh->flags = mesh.flags;
		out_mesh->positions = new vec3f[out_mesh->vertexCount];
		out_mesh->normals = new vec3f[out_mesh->vertexCount];
		if(out_mesh->flags & FLAG_UVS)
			out_mesh->uvs = new vec2f[out_mesh->vertexCount];
		if(out_mesh->flags & FLAG_SKINDATA)
		{
			out_mesh->weights = new vec2f[out_mesh->vertexCount];
			out_mesh->boneIndices = new vec2ub[out_mesh->vertexCount];
		}
		out_mesh->indices = new unsigned short[out_mesh->indexCount];

		int index = 0;
		for(vector<_Vertex>::iterator vert_it = mesh.vertices.begin(); vert_it != mesh.vertices.end(); ++vert_it)
		{
			out_mesh->positions[index] = vert_it->position;
			out_mesh->normals[index] = vert_it->normal;
			if(out_mesh->flags & FLAG_UVS)
			{
				out_mesh->uvs[index].x = vert_it->uv.x;
				out_mesh->uvs[index].y = 1.0f - vert_it->uv.y; // flip v coordinate
			}
			if(out_mesh->flags & FLAG_SKINDATA)
			{
				out_mesh->weights[index] = vert_it->weights;
				out_mesh->boneIndices[index] = vert_it->boneIndices;
			}
			++index;
		}

		index = 0;
		for(vector<_Triangle>::iterator tri_it = mesh.triangles.begin(); tri_it != mesh.triangles.end(); ++tri_it)
		{
			out_mesh->indices[index++] = (unsigned short)tri_it->vertices[0];
			out_mesh->indices[index++] = (unsigned short)tri_it->vertices[1];
			out_mesh->indices[index++] = (unsigned short)tri_it->vertices[2];
		}

		// calculate bounding box
		out_mesh->bbox.minPoint = out_mesh->positions[0];
		out_mesh->bbox.maxPoint = out_mesh->positions[0];
		for(int vert_i = 1; vert_i < out_mesh->vertexCount; ++vert_i)
		{
			vec3f& pos = out_mesh->positions[vert_i];

			if(pos.x < out_mesh->bbox.minPoint.x)
				out_mesh->bbox.minPoint.x = pos.x;
			else if(pos.x > out_mesh->bbox.maxPoint.x)
				out_mesh->bbox.maxPoint.x = pos.x;

			if(pos.y < out_mesh->bbox.minPoint.y)
				out_mesh->bbox.minPoint.y = pos.y;
			else if(pos.y > out_mesh->bbox.maxPoint.y)
				out_mesh->bbox.maxPoint.y = pos.y;

			if(pos.z < out_mesh->bbox.minPoint.z)
				out_mesh->bbox.minPoint.z = pos.z;
			else if(pos.z > out_mesh->bbox.maxPoint.z)
				out_mesh->bbox.maxPoint.z = pos.z;
		}

		_meshes.push_back(out_mesh);
	}

	// calculate tangents and binormals
	CalculateTangentsAndBinormals();

	// calculate bounding box for whole model
	_bbox = _meshes.front()->bbox;
	for(list<Mesh*>::iterator mesh_it = _meshes.begin(); mesh_it != _meshes.end(); ++mesh_it)
	{
		Mesh& mesh = **mesh_it;

		if(mesh.bbox.minPoint.x < _bbox.minPoint.x)
			_bbox.minPoint.x = mesh.bbox.minPoint.x;
		if(mesh.bbox.maxPoint.x > _bbox.maxPoint.x)
			_bbox.maxPoint.x = mesh.bbox.maxPoint.x;

		if(mesh.bbox.minPoint.y < _bbox.minPoint.y)
			_bbox.minPoint.y = mesh.bbox.minPoint.y;
		if(mesh.bbox.maxPoint.y > _bbox.maxPoint.y)
			_bbox.maxPoint.y = mesh.bbox.maxPoint.y;

		if(mesh.bbox.minPoint.z < _bbox.minPoint.z)
			_bbox.minPoint.z = mesh.bbox.minPoint.z;
		if(mesh.bbox.maxPoint.z > _bbox.maxPoint.z)
			_bbox.maxPoint.z = mesh.bbox.maxPoint.z;
	}

	// cleanup
	for(list<_Mesh*>::iterator mesh_it = meshes.begin(); mesh_it != meshes.end(); ++mesh_it)
	{
		delete *mesh_it;
	}

	return true;
}

void ModelExporter::CalculateTangentsAndBinormals()
{
	for(list<Mesh*>::iterator mesh_it = _meshes.begin(); mesh_it != _meshes.end(); ++mesh_it)
	{
		Mesh& mesh = **mesh_it;
		if(mesh.uvs)
		{
			mesh.flags |= FLAG_TANGENTS;
			mesh.tangents = new vec3f[mesh.vertexCount];
			mesh.binormals = new vec3f[mesh.vertexCount];
			memset(mesh.tangents, 0, sizeof(vec3f) * mesh.vertexCount);
			memset(mesh.binormals, 0, sizeof(vec3f) * mesh.vertexCount);
			int num_tris = mesh.indexCount / 3;
			for(int tri_i = 0; tri_i < num_tris; ++tri_i)
			{
				int i0 = mesh.indices[tri_i * 3];
				int i1 = mesh.indices[tri_i * 3 + 1];
				int i2 = mesh.indices[tri_i * 3 + 2];
				vec3f tangent;
				vec3f binormal;

				CalcTangent(
					tangent,
					mesh.positions[i0],
					mesh.positions[i1],
					mesh.positions[i2],
					mesh.uvs[i0],
					mesh.uvs[i1],
					mesh.uvs[i2]);

				mesh.tangents[i0] += tangent;
				mesh.tangents[i1] += tangent;
				mesh.tangents[i2] += tangent;

				CalcBinormal(
					binormal,
					mesh.positions[i0],
					mesh.positions[i1],
					mesh.positions[i2],
					mesh.uvs[i0],
					mesh.uvs[i1],
					mesh.uvs[i2]);

				mesh.binormals[i0] += binormal;
				mesh.binormals[i1] += binormal;
				mesh.binormals[i2] += binormal;
			}

			for(int vert_i = 0; vert_i < mesh.vertexCount; ++vert_i)
			{
				const vec3f& normal = mesh.normals[vert_i];
				const vec3f& tangent = mesh.tangents[vert_i];
				const vec3f& binormal = mesh.binormals[vert_i];

				vec3f t = tangent - normal * dot(tangent, normal);
				t.normalize();
				vec3f b = binormal - normal * dot(binormal, normal) - t * dot(t, binormal);
				b.normalize();

				mesh.tangents[vert_i] = t;
				mesh.binormals[vert_i] = b;
			}
		}
	}
}

MStatus ModelExporter::WriteToFile(const char* file_name)
{
	FILE* file = fopen(file_name, "wb");
	if(!file)
	{
		MGlobal::displayError(MString("Could not open file for writing: ") + file_name);
		return MStatus::kFailure;
	}

	// write header
	int magic = FILE_MAGIC, version = FILE_VERSION;
	fwrite(&magic, sizeof(int), 1, file);
	fwrite(&version, sizeof(int), 1, file);
	int mesh_count = _meshes.size();
	fwrite(&mesh_count, sizeof(int), 1, file);
	int material_count = _materials.size();
	fwrite(&material_count, sizeof(int), 1, file);
	int bone_count = _joints.size();
	fwrite(&bone_count, sizeof(int), 1, file);
	fwrite(&_bbox.minPoint, sizeof(vec3f), 1, file);
	fwrite(&_bbox.maxPoint, sizeof(vec3f), 1, file);

	// write meshes
	for(list<Mesh*>::iterator it = _meshes.begin(); it != _meshes.end(); ++it)
	{
		Mesh& mesh = **it;
		fwrite(&mesh.vertexCount, sizeof(int), 1, file);
		fwrite(&mesh.indexCount, sizeof(int), 1, file);
		fwrite(&mesh.materialIndex, sizeof(int), 1, file);
		fwrite(&mesh.flags, sizeof(int), 1, file);
		fwrite(&mesh.bbox.minPoint, sizeof(vec3f), 1, file);
		fwrite(&mesh.bbox.maxPoint, sizeof(vec3f), 1, file);
		fwrite(mesh.positions, sizeof(vec3f), mesh.vertexCount, file);
		fwrite(mesh.normals, sizeof(vec3f), mesh.vertexCount, file); // normals
		if(mesh.flags & FLAG_TANGENTS)
		{
			fwrite(mesh.tangents, sizeof(vec3f), mesh.vertexCount, file); // tangents
			fwrite(mesh.binormals, sizeof(vec3f), mesh.vertexCount, file); // binormals
		}
		if(mesh.flags & FLAG_UVS)
			fwrite(mesh.uvs, sizeof(vec2f), mesh.vertexCount, file); // uvs
		if(mesh.flags & FLAG_SKINDATA)
		{
			fwrite(mesh.weights, sizeof(vec2f), mesh.vertexCount, file); // weights
			fwrite(mesh.boneIndices, sizeof(vec2ub), mesh.vertexCount, file); // bone indices
		}
		fwrite(mesh.indices, sizeof(unsigned short), mesh.indexCount, file);
	}

	// write materials
	for(list<Material>::iterator it = _materials.begin(); it != _materials.end(); ++it)
	{
		Material& material = *it;
		int len = (int)material.name.size();
		fwrite(&len, sizeof(int), 1, file);
		fwrite(material.name.c_str(), sizeof(char), len, file);
	}

	// write bones
	for(list<Joint*>::const_iterator it = _joints.begin(); it != _joints.end(); ++it)
	{
		Joint& joint = **it;
		int len = (int)joint.name.length();
		fwrite(&len, sizeof(int), 1, file);
		fwrite(joint.name.asChar(), sizeof(char), len, file);
		fwrite(&joint.parentIndex, sizeof(int), 1, file);

		MDistance dist(0.0, MDistance::uiUnit());
		MFloatMatrix m(joint.bindMatrix.matrix);
		dist.setValue(m[3][0]);
		m[3][0] = (float)dist.asMeters();
		dist.setValue(m[3][1]);
		m[3][1] = (float)dist.asMeters();
		dist.setValue(m[3][2]);
		m[3][2] = (float)dist.asMeters();
		fwrite(m.matrix, sizeof(float), 16, file);

		//! najverovatnije nece trebati!
		MFloatMatrix lm(joint.localMatrix.matrix);
		dist.setValue(lm[3][0]);
		lm[3][0] = (float)dist.asMeters();
		dist.setValue(lm[3][1]);
		lm[3][1] = (float)dist.asMeters();
		dist.setValue(lm[3][2]);
		lm[3][2] = (float)dist.asMeters();
		fwrite(lm.matrix, sizeof(float), 16, file);
	}

	fclose(file);

	return MStatus::kSuccess;
}

bool ModelExporter::haveWriteMethod() const
{
	return true;
}

bool ModelExporter::haveReadMethod() const
{
	return false;
}

bool ModelExporter::canBeOpened() const
{
	return true;
}

MString ModelExporter::defaultExtension() const
{
	return MString("model");
}

MPxFileTranslator::MFileKind ModelExporter::identifyFile(const MFileObject &file, const char* buffer, short size) const
{
	const char* name = file.name().asChar();
	int   nameLength = (int)strlen(name);

	if ((nameLength > 6) && !strcasecmp(name+nameLength-6, ".model"))
	{
		return kIsMyFileType;
	}

	return kNotMyFileType;
}

bool ModelExporter::IsVisible(MFnDagNode& fnDag, MStatus& status) 
{
	if(fnDag.isIntermediateObject())
		return false;

	MPlug visPlug = fnDag.findPlug("visibility", &status);
	if(MStatus::kFailure == status)
	{
		MGlobal::displayError("MPlug::findPlug");
		return false;
	}else
	{
		bool visible;
		status = visPlug.getValue(visible);
		if (MStatus::kFailure == status)
		{
			MGlobal::displayError("MPlug::getValue");
		}
		return visible;
	}
}

void ModelExporter::Cleanup()
{
	for(list<TempMesh*>::iterator it = _tempMeshes.begin(); it != _tempMeshes.end(); ++it)
	{
		delete *it;
	}
	_tempMeshes.clear();

	for(list<Mesh*>::iterator it = _meshes.begin(); it != _meshes.end(); ++it)
	{
		delete *it;
	}
	_meshes.clear();

	for(list<Joint*>::iterator it = _joints.begin(); it != _joints.end(); ++it)
	{
		delete *it;
	}
	_joints.clear();

	_materials.clear();
}

TempMesh* ModelExporter::GetTempMesh(const MString& dag_path)
{
	for(list<TempMesh*>::iterator it = _tempMeshes.begin(); it != _tempMeshes.end(); ++it)
	{
		if((*it)->dagPath == dag_path)
			return *it;
	}

	return 0;
}

Joint* ModelExporter::FindJointByPath(const MString& dag_path)
{
	for(list<Joint*>::iterator it = _joints.begin(); it != _joints.end(); ++it)
	{
		if((*it)->pathName == dag_path)
			return *it;
	}

	return 0;
}

int ModelExporter::AddMaterial(const Material& material)
{
	int index = 0;
	for(list<Material>::iterator it = _materials.begin(); it != _materials.end(); ++it)
	{
		if(it->name == material.name)
			return index;
		++index;
	}

	_materials.push_back(material);
	return _materials.size() - 1;
}
