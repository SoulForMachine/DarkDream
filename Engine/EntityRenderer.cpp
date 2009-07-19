
#include "Engine/ModelEntity.h"
#include "Engine/Model.h"
#include "Engine/World.h"
#include "Engine/EngineInternal.h"
#include "RenderSystem.h"
#include "EntityRenderer.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	EntityRenderer::EntityRenderer()
	{
		Clear();
	}

	bool EntityRenderer::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();

		_vpMesh = engineAPI.asmProgManager->CreateASMProgram("Programs/Mesh.vp", true);
		_vpMeshSkin = engineAPI.asmProgManager->CreateASMProgram("Programs/Mesh_Skin.vp", true);
		_fpMesh = engineAPI.asmProgManager->CreateASMProgram("Programs/Mesh.fp", true);

		GL::VertexAttribDesc* vfmt[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },
			{ 0, 1, 3, GL::TYPE_FLOAT, false, false, 16 },
		};
		_vertFmtMesh = _renderer->CreateVertexFormat(vfmt, COUNTOF(vfmt));

		GL::VertexAttribDesc* skin_vfmt[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },
			{ 0, 1, 3, GL::TYPE_FLOAT, false, false, 16 },
			{ 0, 2, 4, GL::TYPE_FLOAT, false, false, 28 },
		};
		_vertFmtSkinnedMesh = _renderer->CreateVertexFormat(skin_vfmt, COUNTOF(skin_vfmt));

		return true;
	}

	void EntityRenderer::Deinit()
	{
		engineAPI.asmProgManager->ReleaseASMProgram(_vpMesh);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpMeshSkin);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpMesh);

		_renderer->DestroyVertexFormat(_vertFmtMesh);
		_renderer->DestroyVertexFormat(_vertFmtSkinnedMesh);

		Clear();
	}

	void EntityRenderer::Render(const Camera& camera, const MeshRenderData* meshes, int count)
	{
		for(int i = 0; i < count; ++i)
		{
			RenderMesh(camera, &meshes[i]);
		}
	}

	void EntityRenderer::ReloadShaders()
	{
	}

	void EntityRenderer::RenderMesh(const Camera& camera, const MeshRenderData* mesh)
	{

	}

	void EntityRenderer::Clear()
	{
		_vpMesh = 0;
		_vpMeshSkin = 0;
		_fpMesh = 0;
		_vertFmtMesh = 0;
		_vertFmtSkinnedMesh = 0;
		_renderer = 0;
	}

}
