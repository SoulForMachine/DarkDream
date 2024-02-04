
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
			int_t jointCount;
			const Material* material;
			int shaderIndex;
			float eyeDistSq;
		};

		EntityRenderer();
		bool Init();
		void Deinit();
		void Render(const Camera& camera, MeshRenderData** meshes, int count);
		void ReloadShaders();

	private:
		struct Shader
		{
			const GL::ASMProgram* vertProg;
			const GL::ASMProgram* fragProg;
		};

		void RenderMesh(const Camera& camera, const MeshRenderData* mesh_data);
		void Clear();

		GL::Renderer* _renderer;
		VertexASMProgResPtr _vpMesh;
		VertexASMProgResPtr _vpMeshNrm;
		VertexASMProgResPtr _vpMeshSkin;
		VertexASMProgResPtr _vpMeshNrmSkin;
		FragmentASMProgResPtr _fpMesh;
		FragmentASMProgResPtr _fpMeshNrm;
		FragmentASMProgResPtr _fpLambert;
		FragmentASMProgResPtr _fpLambertNrm;
		GL::VertexFormat* _vertFmtMesh;
		GL::VertexFormat* _vertFmtSkinnedMesh;
		GL::SamplerState* _diffuseSampler;
		GL::SamplerState* _emissionSampler;
		GL::SamplerState* _transpSampler;
		GL::SamplerState* _normalSampler;

		Shader _shaders[4];
		Shader _editorShaders[4];
	};

}

#endif // _ENGINE_RENDER_ENTITY_H_
