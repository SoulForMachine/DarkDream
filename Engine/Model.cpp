
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Console.h"
#include "BaseLib/GL/GLCommon.h"
#include "Engine/FileSystem.h"
#include "Engine/RenderSystem.h"
#include "Engine/EngineInternal.h"
#include "Model.h"
#include "BaseLib/Memory.h"


#define FILE_MAGIC ('M' | ('D' << 8) | ('L' << 16) | ('1' << 24))
#define FILE_VERSION	1

using namespace Memory;
using namespace math3d;
using namespace GL;


namespace Engine
{

	Model::Model()
	{
		_rootJoint = 0;
	}

	Model::~Model()
	{
		Unload();
	}

	bool Model::Load(const tchar* file_name)
	{
		if(!file_name || !*file_name)
			return false;

		Unload();

		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(file_name, _t("rb"));
		if(!file)
			return false;

		uint magic = file->ReadUnsignedInt();
		if(magic != FILE_MAGIC)
			return false;
		uint version = file->ReadUnsignedInt();
		if(version != FILE_VERSION)
			return false;

		int mesh_count = file->ReadInt();
		int material_count = file->ReadInt();
		int joint_count = file->ReadInt();

		// bounding box
		file->ReadVec3(_bbox.minPt);
		file->ReadVec3(_bbox.maxPt);

		_numVertices = 0;
		_numIndices = 0;

		// read meshes
		_meshes.SetCount(mesh_count);
		for(int i = 0; i < mesh_count; ++i)
		{
			Mesh& mesh = _meshes[i];
			if(!ReadMesh(file, mesh, file_name, i))
			{
				Unload();
				return false;
			}
		}

		// read materials
		StaticArray<const char*> mat_names(material_count, tempPool);
		for(int i = 0; i < material_count; ++i)
		{
			int len = file->ReadInt() + 1;
			char* str = new(tempPool) char[len];
			file->ReadString(str, len);
			mat_names[i] = str;
		}

		// assign each mesh it's material name
		for(int i = 0; i < mesh_count; ++i)
		{
			int index = (int)_meshes[i].material;
			_meshes[i].material = StringDup(mat_names[index]);
		}

		// delete temporary array with material names
		for(int i = 0; i < material_count; ++i)
			delete[] mat_names[i];
		mat_names.Clear();

		// read skeleton joints
		_joints.SetCount(joint_count);
		for(int i = 0; i < joint_count; ++i)
		{
			int len = file->ReadInt() + 1;
			char* str = new(stringPool) char[len];
			file->ReadString(str, len);
			_joints[i].name = str;

			_joints[i].index = i;
			_joints[i].parentIndex = file->ReadInt();
			file->ReadMat4(_joints[i].offsetMatrix);
			file->ReadMat4(_joints[i].jointMatrix);
			_joints[i].invOffsetMatrix = _joints[i].offsetMatrix;
			_joints[i].invOffsetMatrix.inverse();

			_joints[i].parent = 0;
			_joints[i].child = 0;
			_joints[i].sibling = 0;
		}

		CreateJointHierarchy();

		return true;
	}

	void Model::Unload()
	{
		GL::Renderer* renderer = engineAPI.renderSystem->GetRenderer();

		for(size_t i = 0; i < _meshes.GetCount(); ++i)
		{
			renderer->DestroyBuffer(_meshes[i].vertBuf);
			renderer->DestroyBuffer(_meshes[i].indexBuf);
			delete[] _meshes[i].material;
		}
		_meshes.Clear();

		for(size_t i = 0; i < _joints.GetCount(); ++i)
		{
			delete[] _joints[i].name;
		}
		_joints.Clear();
		_rootJoint = 0;

		_numVertices = 0;
		_numIndices = 0;
	}

	bool Model::ReadMesh(FileUtil::File* file, Mesh& mesh, const tchar* file_name, int mesh_index)
	{
		mesh.numVertices = file->ReadInt();
		mesh.numIndices = file->ReadInt();
		mesh.material = (const char*)file->ReadInt(); // use material name pointer to store index temporarily

		_numVertices += mesh.numVertices;
		_numIndices += mesh.numIndices;

		mesh.flags = file->ReadInt();

		file->ReadVec3(mesh.bbox.minPt);
		file->ReadVec3(mesh.bbox.maxPt);
		mesh.center = (mesh.bbox.minPt + mesh.bbox.maxPt) / 2.0f;

		// read vertex attributes

		vec3f* positions = 0;
		vec3f* normals = 0;
		vec3f* tangents = 0;
		vec3f* binormals = 0;
		vec2f* uvs = 0;
		vec2f* weights = 0;
		vec2ub* joint_inds = 0;

		positions = new(tempPool) vec3f[mesh.numVertices];
		file->Read(positions, sizeof(vec3f) * mesh.numVertices);

		normals = new(tempPool) vec3f[mesh.numVertices];
		file->Read(normals, sizeof(vec3f) * mesh.numVertices);

		if(mesh.flags & Mesh::FLAG_TANGENTS)
		{
			tangents = new(tempPool) vec3f[mesh.numVertices];
			file->Read(tangents, sizeof(vec3f) * mesh.numVertices);
			binormals = new(tempPool) vec3f[mesh.numVertices];
			file->Read(binormals, sizeof(vec3f) * mesh.numVertices);
		}

		if(mesh.flags & Mesh::FLAG_UVS)
		{
			uvs = new(tempPool) vec2f[mesh.numVertices];
			file->Read(uvs, sizeof(vec2f) * mesh.numVertices);
		}

		if(mesh.flags & Mesh::FLAG_SKIN_DATA)
		{
			weights = new(tempPool) vec2f[mesh.numVertices];
			file->Read(weights, sizeof(vec2f) * mesh.numVertices);

			joint_inds = new(tempPool) vec2ub[mesh.numVertices];
			file->Read(joint_inds, sizeof(vec2ub) * mesh.numVertices);

			//for(size_t i = 0; i < mesh.numVertices; ++i)
			//{
			//	assert(joint_inds[i].x < 38);
			//	assert(joint_inds[i].y < 38);
			//	/*if(fcmp_lt(weights[i].x + weights[i].y, 0.9f))
			//	{
			//		weights[i].x = 0.5f;
			//		weights[i].y = 0.5f;
			//	}*/
			////	assert(fcmp_gt(weights[i].x + weights[i].y, 0.1f));
			//}
		}

		GL::Renderer* renderer = engineAPI.renderSystem->GetRenderer();

		if(mesh.flags == (Mesh::FLAG_SKIN_DATA | Mesh::FLAG_TANGENTS | Mesh::FLAG_UVS))
		{
			mesh.vertexSize = sizeof(MeshVert_UV_Tan_Skin);
			mesh.vertLayout = Mesh::VERT_LAYOUT_POS_NRM_UV_TAN_SKIN;

			mesh.vertBuf = renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, mesh.numVertices * sizeof(MeshVert_UV_Tan_Skin), 0, GL::USAGE_STATIC_DRAW);
			if(!mesh.vertBuf)
				return false;
			MeshVert_UV_Tan_Skin* buf = (MeshVert_UV_Tan_Skin*)mesh.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
			if(!buf)
			{
				renderer->DestroyBuffer(mesh.vertBuf);
				return false;
			}

			for(size_t vert_i = 0; vert_i < mesh.numVertices; ++vert_i)
			{
				buf[vert_i].position = vec4f(positions[vert_i], 1.0f);
				buf[vert_i].normal = normals[vert_i];
				buf[vert_i].tangent.rvec3 = tangents[vert_i];
				vec3f b;
				cross(b, normals[vert_i], tangents[vert_i]);
				buf[vert_i].tangent.w = (dot(b, binormals[vert_i]) > 0.0)? 1.0f: -1.0f;
				buf[vert_i].texCoord = uvs[vert_i];
				buf[vert_i].skinData = vec4f(weights[vert_i].x, weights[vert_i].y, (float)joint_inds[vert_i].x, (float)joint_inds[vert_i].y);
			}

			if(!mesh.vertBuf->UnmapBuffer())
				return false;
		}
		else if(mesh.flags == (Mesh::FLAG_SKIN_DATA | Mesh::FLAG_UVS))
		{
			mesh.vertexSize = sizeof(MeshVert_UV_Skin);
			mesh.vertLayout = Mesh::VERT_LAYOUT_POS_NRM_UV_SKIN;

			mesh.vertBuf = renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, mesh.numVertices * sizeof(MeshVert_UV_Skin), 0, GL::USAGE_STATIC_DRAW);
			if(!mesh.vertBuf)
				return false;
			MeshVert_UV_Skin* buf = (MeshVert_UV_Skin*)mesh.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
			if(!buf)
			{
				renderer->DestroyBuffer(mesh.vertBuf);
				return false;
			}

			for(size_t vert_i = 0; vert_i < mesh.numVertices; ++vert_i)
			{
				buf[vert_i].position = vec4f(positions[vert_i], 1.0f);
				buf[vert_i].normal = normals[vert_i];
				buf[vert_i].texCoord = uvs[vert_i];
				buf[vert_i].skinData = vec4f(weights[vert_i].x, weights[vert_i].y, (float)joint_inds[vert_i].x, (float)joint_inds[vert_i].y);
			}

			if(!mesh.vertBuf->UnmapBuffer())
				return false;
		}
		else if(mesh.flags == Mesh::FLAG_SKIN_DATA)
		{
			mesh.vertexSize = sizeof(MeshVert_Skin);
			mesh.vertLayout = Mesh::VERT_LAYOUT_POS_NRM_SKIN;

			mesh.vertBuf = renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, mesh.numVertices * sizeof(MeshVert_Skin), 0, GL::USAGE_STATIC_DRAW);
			if(!mesh.vertBuf)
				return false;
			MeshVert_Skin* buf = (MeshVert_Skin*)mesh.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
			if(!buf)
			{
				renderer->DestroyBuffer(mesh.vertBuf);
				return false;
			}

			for(size_t vert_i = 0; vert_i < mesh.numVertices; ++vert_i)
			{
				buf[vert_i].position = vec4f(positions[vert_i], 1.0f);
				buf[vert_i].normal = normals[vert_i];
				buf[vert_i].skinData = vec4f(weights[vert_i].x, weights[vert_i].y, (float)joint_inds[vert_i].x, (float)joint_inds[vert_i].y);
			}

			if(!mesh.vertBuf->UnmapBuffer())
				return false;
		}
		else if(mesh.flags == (Mesh::FLAG_TANGENTS | Mesh::FLAG_UVS))
		{
			mesh.vertexSize = sizeof(MeshVert_UV_Tan);
			mesh.vertLayout = Mesh::VERT_LAYOUT_POS_NRM_UV_TAN;

			mesh.vertBuf = renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, mesh.numVertices * sizeof(MeshVert_UV_Tan), 0, GL::USAGE_STATIC_DRAW);
			if(!mesh.vertBuf)
				return false;
			MeshVert_UV_Tan* buf = (MeshVert_UV_Tan*)mesh.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
			if(!buf)
			{
				renderer->DestroyBuffer(mesh.vertBuf);
				return false;
			}

			for(size_t vert_i = 0; vert_i < mesh.numVertices; ++vert_i)
			{
				buf[vert_i].position = vec4f(positions[vert_i], 1.0f);
				buf[vert_i].normal = normals[vert_i];
				buf[vert_i].tangent.rvec3 = tangents[vert_i];
				vec3f b;
				cross(b, normals[vert_i], tangents[vert_i]);
				buf[vert_i].tangent.w = (dot(b, binormals[vert_i]) > 0.0)? 1.0f: -1.0f;
				buf[vert_i].texCoord = uvs[vert_i];
			}

			if(!mesh.vertBuf->UnmapBuffer())
				return false;
		}
		else if(mesh.flags == (Mesh::FLAG_UVS))
		{
			mesh.vertexSize = sizeof(MeshVert_UV);
			mesh.vertLayout = Mesh::VERT_LAYOUT_POS_NRM_UV;

			mesh.vertBuf = renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, mesh.numVertices * sizeof(MeshVert_UV), 0, GL::USAGE_STATIC_DRAW);
			if(!mesh.vertBuf)
				return false;
			MeshVert_UV* buf = (MeshVert_UV*)mesh.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
			if(!buf)
			{
				renderer->DestroyBuffer(mesh.vertBuf);
				return false;
			}

			for(size_t vert_i = 0; vert_i < mesh.numVertices; ++vert_i)
			{
				buf[vert_i].position = vec4f(positions[vert_i], 1.0f);
				buf[vert_i].normal = normals[vert_i];
				buf[vert_i].texCoord = uvs[vert_i];
			}

			if(!mesh.vertBuf->UnmapBuffer())
				return false;
		}
		else
		{
			mesh.vertexSize = sizeof(MeshVert);
			mesh.vertLayout = Mesh::VERT_LAYOUT_POS_NRM;

			mesh.vertBuf = renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, mesh.numVertices * sizeof(MeshVert), 0, GL::USAGE_STATIC_DRAW);
			if(!mesh.vertBuf)
				return false;
			MeshVert* buf = (MeshVert*)mesh.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
			if(!buf)
			{
				renderer->DestroyBuffer(mesh.vertBuf);
				return false;
			}

			for(size_t vert_i = 0; vert_i < mesh.numVertices; ++vert_i)
			{
				buf[vert_i].position = vec4f(positions[vert_i], 1.0f);
				buf[vert_i].normal = normals[vert_i];
			}

			if(!mesh.vertBuf->UnmapBuffer())
				return false;
		}

		delete[] positions;
		delete[] normals;
		delete[] tangents;
		delete[] binormals;
		delete[] uvs;
		delete[] weights;
		delete[] joint_inds;

		// read indices
		ushort* indices = new(tempPool) ushort[mesh.numIndices];
		file->Read(indices, sizeof(ushort) * mesh.numIndices);
		mesh.indexBuf = renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, sizeof(ushort) * mesh.numIndices, indices, GL::USAGE_STATIC_DRAW);
		delete[] indices;
		if(!mesh.indexBuf)
		{
			renderer->DestroyBuffer(mesh.vertBuf);
			return false;
		}

		return true;
	}

	void Model::CreateJointHierarchy()
	{
		_rootJoint = 0;

		for(size_t i = 0; i < _joints.GetCount(); ++i)
		{
			if(_joints[i].parentIndex == -1)
			{
				_rootJoint = &_joints[i];
				_rootJoint->parent = 0;
			}
			else
			{
				Joint* parent = &_joints[_joints[i].parentIndex];
				_joints[i].parent = parent;
				if(parent->child)
				{
					_joints[i].sibling = parent->child->sibling;
					parent->child->sibling = &_joints[i];
				}
				else
				{
					parent->child = &_joints[i];
				}
			}
		}
	}

}
