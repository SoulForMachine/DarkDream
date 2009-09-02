
#include "Engine/EngineInternal.h"
#include "Engine/RenderSystem.h"
#include "Engine/Camera.h"
#include "TerrainRenderer.h"


using namespace math3d;

namespace Engine
{

	TerrainRenderer::TerrainRenderer()
	{
		Clear();
	}

	bool TerrainRenderer::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();
		_vpTerrain = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Terrain.vp"), true);
		_fpTerrain = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		GL::VertexAttribDesc vert_desc[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },
			{ 0, 1, 3, GL::TYPE_FLOAT, false, false, 16 },
		};
		_vertFmtTerrain = _renderer->CreateVertexFormat(vert_desc, COUNTOF(vert_desc));

		return true;
	}

	void TerrainRenderer::Deinit()
	{
		engineAPI.asmProgManager->ReleaseASMProgram(_vpTerrain);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpTerrain);
		_renderer->DestroyVertexFormat(_vertFmtTerrain);

		Clear();
	}

	void TerrainRenderer::RenderTerrainPatch(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count)
	{
		_renderer->IndexSource(terrain->GetPatchIndexBuffer(), GL::TYPE_UNSIGNED_SHORT);
		_renderer->ActiveVertexASMProgram(_vpTerrain->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fpTerrain->GetASMProgram());
		_renderer->ActiveVertexFormat(_vertFmtTerrain);
		_vpTerrain->GetASMProgram()->LocalMatrix4x4(1, camera.GetViewProjectionTransform());
		float color[] = { 0.31f, 0.67f, 0.79f, 1.0f };
		_fpTerrain->GetASMProgram()->LocalParameter(0, color);

		for(int i = 0; i < count; ++i)
		{
			_renderer->VertexSource(0, patches[i]->vertBuf, sizeof(Terrain::PatchVertex), 0);

			float transl[] = { patches[i]->boundBox.minPt.x, 0.0f, 0.0f, 0.0f };
			_vpTerrain->GetASMProgram()->LocalParameter(0, transl);

			_renderer->DrawIndexed(GL::PRIM_TRIANGLES, 0, terrain->GetPatchIndexCount());
		}

		/*_renderer->EnableDepthTest(false);
		float dbg_color[] = { 0.7f, 0.2f, 0.2f, 1.0f };
		_fpTerrain->GetASMProgram()->LocalParameter(0, dbg_color);
		for(int i = 0; i < terrain->_dbgCellCount; ++i)
		{
			assert(terrain->_dbgCells[i].x >= 0);
			assert(terrain->_dbgCells[i].y >= 0);
			assert(terrain->_dbgCells[i].x < Terrain::PATCH_WIDTH);
			assert(terrain->_dbgCells[i].y < Terrain::PATCH_HEIGHT);
			size_t start = (size_t)(terrain->_dbgCells[i].y * Terrain::PATCH_WIDTH + terrain->_dbgCells[i].x) * 6 * sizeof(ushort);
			_renderer->DrawIndexed(GL::PRIM_TRIANGLES, start, 6);
		}
		_renderer->EnableDepthTest(true);*/
	}

	void TerrainRenderer::Clear()
	{
		_renderer = 0;
		_vpTerrain = 0;
		_fpTerrain = 0;
		_vertFmtTerrain = 0;
	}

}
