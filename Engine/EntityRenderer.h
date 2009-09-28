
#ifndef _ENGINE_RENDER_ENTITY_H_
#define _ENGINE_RENDER_ENTITY_H_

#include "BaseLib/Math/math3d.h"
#include "BaseLib/GL/GLRenderer.h"


namespace Engine
{

	class Camera;
	struct Mesh;


	class EntityRenderer
	{
	public:
		struct MeshRenderData
		{
			const Mesh* mesh;
			const math3d::mat4f* worldMat;
			const math3d::mat4f* jointMatPalette;
			size_t jointCount;
			float eyeDistSq;
		};

		EntityRenderer();
		bool Init();
		void Deinit();
		void Render(const Camera& camera, const MeshRenderData* meshes, int count);
		void ReloadShaders();

	private:
		GL::Renderer* _renderer;
		const ASMProgRes* _vpMesh;
		const ASMProgRes* _vpMeshSkin;
		const ASMProgRes* _fpMesh;
		const ASMProgRes* _fpLambert;
		GL::VertexFormat* _vertFmtMesh;
		GL::VertexFormat* _vertFmtSkinnedMesh;

		void RenderMesh(const Camera& camera, const MeshRenderData* mesh_data);
		void Clear();
	};

}

#endif // _ENGINE_RENDER_ENTITY_H_
