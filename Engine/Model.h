
#ifndef _ENGINE_MODEL_H_
#define _ENGINE_MODEL_H_

#include "BaseLib/Math/math3d.h"
#include "BaseLib/GL/GLRenderer.h"
#include "BaseLib/StaticArray.h"
#include "BaseLib/Bounds.h"
#include "BaseLib/FileSys.h"
#include "Engine/Common.h"


namespace Engine
{


	struct Mesh
	{
		enum
		{
			FLAG_UVS = 1,
			FLAG_TANGENTS = 2,
			FLAG_SKIN_DATA = 4
		};

		enum VertexLayout
		{
			VERT_LAYOUT_POS_NRM_UV_TAN_SKIN,
			VERT_LAYOUT_POS_NRM_UV_TAN,
			VERT_LAYOUT_POS_NRM_UV_SKIN,
			VERT_LAYOUT_POS_NRM_UV,
			VERT_LAYOUT_POS_NRM_SKIN,
			VERT_LAYOUT_POS_NRM,
		};

		size_t numVertices;
		size_t numIndices;
		size_t vertexSize; // size of one vertex in bytes
		GL::Buffer* vertBuf;
		GL::Buffer* indexBuf;
		const char* material;
		uint flags;
		VertexLayout vertLayout;
		AABBox bbox;
		math3d::vec3f center; // bounding box center point
	};

	struct Joint
	{
		const char* name;
		int index;
		int parentIndex;
		Joint* child;
		Joint* sibling;
		Joint* parent;
		math3d::mat4f offsetMatrix; // bind space -> bone space
		math3d::mat4f invOffsetMatrix; // bone space -> bind space
		math3d::mat4f jointMatrix; // bone space -> parent bone space
	};


	class ENGINE_API Model
	{
	public:
		struct MeshVert_UV_Tan_Skin
		{
			math3d::vec4f position;
			math3d::vec3f normal;
			math3d::vec4f tangent;
			math3d::vec2f texCoord;
			math3d::vec4f skinData;
		};

		struct MeshVert_UV_Tan
		{
			math3d::vec4f position;
			math3d::vec3f normal;
			math3d::vec4f tangent;
			math3d::vec2f texCoord;
		};

		struct MeshVert_UV_Skin
		{
			math3d::vec4f position;
			math3d::vec3f normal;
			math3d::vec2f texCoord;
			math3d::vec4f skinData;
		};

		struct MeshVert_UV
		{
			math3d::vec4f position;
			math3d::vec3f normal;
			math3d::vec2f texCoord;
		};

		struct MeshVert_Skin
		{
			math3d::vec4f position;
			math3d::vec3f normal;
			math3d::vec4f skinData;
		};

		struct MeshVert
		{
			math3d::vec4f position;
			math3d::vec3f normal;
		};


		Model();
		~Model();

		bool Load(const tchar* file_name);
		void Unload();

		const StaticArray<Mesh>& GetMeshes() const
			{ return _meshes; }
		size_t GetMeshCount() const
			{ return _meshes.GetCount(); }
		const StaticArray<Joint>& GetJoints() const
			{ return _joints; }
		size_t GetJointCount() const
			{ return _joints.GetCount(); }
		const Joint* GetRootJoint() const
			{ return _rootJoint; }
		bool HasSkelet() const
			{ return (_rootJoint != 0); }
		const AABBox& GetBoundingBox() const
			{ return _bbox; }
		int GetVertexCount() const
			{ return _numVertices; }
		int GetIndexCount() const
			{ return _numIndices; }

	private:
		bool ReadMesh(FileSys::File* file, Mesh& mesh, const tchar* file_name, int mesh_index);
		void CreateJointHierarchy();

		StaticArray<Mesh> _meshes;
		StaticArray<Joint> _joints;
		Joint* _rootJoint;
		AABBox _bbox;
		int _numVertices;
		int _numIndices;
	};
}


#endif // _ENGINE_MODEL_H_
