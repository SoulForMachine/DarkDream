
#include "BaseLib/Console.h"
#include "Engine/EngineInternal.h"
#include "Engine/RenderSystem.h"
#include "Engine/Camera.h"
#include "TerrainRenderer.h"


using namespace math3d;

namespace Engine
{

	Console::BoolVar g_cvarDrawTerrainNormals("r_drawTerrainNormals", false);


	TerrainRenderer::TerrainRenderer()
	{
		Clear();
	}

	bool TerrainRenderer::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();
		_vpTerrain = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Terrain.vp"), true);
		_fpTerrain = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Mesh.fp"), true);
		_fpLambert = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Lambert.fp"), true);

		_vpDbgLine = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fpDbgLine = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		GL::VertexAttribDesc vert_desc[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },
			{ 0, 1, 3, GL::TYPE_FLOAT, false, false, 16 },
		};
		_vertFmtTerrain = _renderer->CreateVertexFormat(vert_desc, COUNTOF(vert_desc));

		GL::VertexAttribDesc vert_desc2[] =
		{
			{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 },
		};
		_vertFmtDbgLine = _renderer->CreateVertexFormat(vert_desc2, COUNTOF(vert_desc2));

		_renderStyle = RENDER_STYLE_GAME;

		return true;
	}

	void TerrainRenderer::Deinit()
	{
		engineAPI.asmProgManager->ReleaseASMProgram(_vpTerrain);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpTerrain);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpLambert);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpDbgLine);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpDbgLine);
		_renderer->DestroyVertexFormat(_vertFmtTerrain);
		_renderer->DestroyVertexFormat(_vertFmtDbgLine);

		Clear();
	}

	void TerrainRenderer::RenderTerrainPatch(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count)
	{
		const GL::ASMProgram* frag_prog = (_renderStyle == RENDER_STYLE_GAME)? _fpTerrain->GetASMProgram(): _fpLambert->GetASMProgram();
		_renderer->IndexSource(terrain->GetPatchIndexBuffer(), GL::TYPE_UNSIGNED_SHORT);
		_renderer->ActiveVertexASMProgram(_vpTerrain->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(frag_prog);
		_renderer->ActiveVertexFormat(_vertFmtTerrain);
		_vpTerrain->GetASMProgram()->LocalMatrix4x4(1, camera.GetViewProjectionTransform());
		const float* color = engineAPI.renderSystem->GetMainColor();
		frag_prog->LocalParameter(0, color);
		const Terrain::TerrainPatch* hlight = terrain->GetHighlightPatch();

		for(int i = 0; i < count; ++i)
		{
			_renderer->VertexSource(0, patches[i]->vertBuf, sizeof(Terrain::PatchVertex), 0);

			float transl[] = { patches[i]->boundBox.minPt.x, 0.0f, 0.0f, 0.0f };
			_vpTerrain->GetASMProgram()->LocalParameter(0, transl);

			if(patches[i] == hlight)
			{
				float hlcolor[] = { 0.9215f, 0.5529f, 0.4470f, 1.0f };
				frag_prog->LocalParameter(0, hlcolor);
			}

			_renderer->DrawIndexed(GL::PRIM_TRIANGLES, 0, terrain->GetPatchIndexCount());

			if(patches[i] == hlight)
			{
				frag_prog->LocalParameter(0, color);
			}
		}
	}

	void TerrainRenderer::RenderTerrainPatchNormals(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count)
	{
		_renderer->IndexSource(0, GL::TYPE_VOID);
		_renderer->ActiveVertexASMProgram(_vpDbgLine->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fpDbgLine->GetASMProgram());
		_renderer->ActiveVertexFormat(_vertFmtDbgLine);

		float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		_fpDbgLine->GetASMProgram()->LocalParameter(0, color);

		for(int i = 0; i < count; ++i)
		{
			if(patches[i]->normalBuf)
			{
				_renderer->VertexSource(0, patches[i]->normalBuf, sizeof(vec3f), 0);

				mat4f transl, wvp;
				transl.set_translation(patches[i]->boundBox.minPt.x, 0.0f, 0.0f);
				mul(wvp, transl, camera.GetViewProjectionTransform());
				_vpDbgLine->GetASMProgram()->LocalMatrix4x4(0, wvp);

				_renderer->Draw(GL::PRIM_LINES, 0, terrain->GetPatchIndexCount() * 2);
			}
		}
	}

	void TerrainRenderer::Clear()
	{
		_renderer = 0;
		_vpTerrain = 0;
		_fpTerrain = 0;
		_fpLambert = 0;
		_vpDbgLine = 0;
		_fpDbgLine = 0;
		_vertFmtTerrain = 0;
	}

}
