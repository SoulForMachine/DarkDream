
#include "BaseLib/Console.h"
#include "Engine/EngineInternal.h"
#include "Engine/RenderSystem.h"
#include "Engine/Camera.h"
#include "ResourceManager.h"
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
		_vpTerrain = engineAPI.asmProgManager->CreateVertexASMProgram(_t("Programs/Terrain.vp"), true);
		_fpTerrain = engineAPI.asmProgManager->CreateFragmentASMProgram(_t("Programs/Terrain.fp"), true);
		_fpLambert = engineAPI.asmProgManager->CreateFragmentASMProgram(_t("Programs/Lambert.fp"), true);

		_vpGrass = engineAPI.asmProgManager->CreateVertexASMProgram(_t("Programs/Grass.vp"), true);
		_fpGrass = engineAPI.asmProgManager->CreateFragmentASMProgram(_t("Programs/Grass.fp"), true);

		_vpDbgLine = engineAPI.asmProgManager->CreateVertexASMProgram(_t("Programs/Simple.vp"), true);
		_fpDbgLine = engineAPI.asmProgManager->CreateFragmentASMProgram(_t("Programs/ConstColor.fp"), true);

		_gradTex = engineAPI.textureManager->CreateTexture2D(_t("Textures/terrain_grad.dds"), true);

		GL::SamplerStateDesc terr_sampler = GL::GLState::defaultSamplerState;
		terr_sampler.addressU = GL::TEX_ADDRESS_REPEAT;
		terr_sampler.addressV = GL::TEX_ADDRESS_REPEAT;
		terr_sampler.minFilter = GL::TEX_FILTER_LINEAR_MIPMAP_LINEAR;
		terr_sampler.magFilter = GL::TEX_FILTER_LINEAR;
		terr_sampler.maxAnisotropy = _renderer->GetInfo().maxTextureAnisotropy;
		_terrainSampler = _renderer->CreateSamplerState(terr_sampler);

		GL::SamplerStateDesc grad_sampler = GL::GLState::defaultSamplerState;
		grad_sampler.addressU = GL::TEX_ADDRESS_CLAMP_TO_EDGE;
		grad_sampler.addressV = GL::TEX_ADDRESS_CLAMP_TO_EDGE;
		grad_sampler.minFilter = GL::TEX_FILTER_LINEAR;
		grad_sampler.magFilter = GL::TEX_FILTER_LINEAR;
		_gradSampler = _renderer->CreateSamplerState(grad_sampler);

		GL::SamplerStateDesc grass_sampler = GL::GLState::defaultSamplerState;
		grass_sampler.addressU = GL::TEX_ADDRESS_CLAMP_TO_EDGE;
		grass_sampler.addressV = GL::TEX_ADDRESS_CLAMP_TO_EDGE;
		grass_sampler.minFilter = GL::TEX_FILTER_LINEAR_MIPMAP_LINEAR;
		grass_sampler.magFilter = GL::TEX_FILTER_LINEAR;
		_grassSampler = _renderer->CreateSamplerState(grass_sampler);

		{
			GL::VertexAttribDesc vert_desc[] =
			{
				{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },
				{ 0, 1, 4, GL::TYPE_FLOAT, false, false, 16 },
			};
			_vertFmtTerrain = _renderer->CreateVertexFormat(vert_desc, COUNTOF(vert_desc));
		}

		{
			GL::VertexAttribDesc vert_desc[] =
			{
				{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 },
			};
			_vertFmtDbgLine = _renderer->CreateVertexFormat(vert_desc, COUNTOF(vert_desc));
		}

		{
			GL::VertexAttribDesc vert_desc[] =
			{
				{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },
				{ 0, 1, 2, GL::TYPE_FLOAT, false, false, 16 },
			};
			_vertFmtGrass = _renderer->CreateVertexFormat(vert_desc, COUNTOF(vert_desc));
		}

		return true;
	}

	void TerrainRenderer::Deinit()
	{
		engineAPI.asmProgManager->ReleaseASMProgram(_vpTerrain);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpTerrain);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpLambert);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpGrass);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpGrass);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpDbgLine);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpDbgLine);
		engineAPI.textureManager->ReleaseTexture(_gradTex);
		_renderer->DestroySamplerState(_terrainSampler);
		_renderer->DestroySamplerState(_gradSampler);
		_renderer->DestroySamplerState(_grassSampler);
		_renderer->DestroyVertexFormat(_vertFmtTerrain);
		_renderer->DestroyVertexFormat(_vertFmtDbgLine);
		_renderer->DestroyVertexFormat(_vertFmtGrass);

		Clear();
	}

	void TerrainRenderer::RenderTerrainPatches(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count)
	{
	//	const GL::ASMProgram* frag_prog = 
	//		(engineAPI.renderSystem->GetRenderStyle() == RenderSystem::RENDER_STYLE_GAME)?
	//		_fpTerrain: _fpLambert;
		_renderer->IndexSource(terrain->GetPatchIndexBuffer(), GL::TYPE_UNSIGNED_SHORT);
		_renderer->ActiveVertexASMProgram(_vpTerrain);
		_renderer->ActiveFragmentASMProgram(_fpTerrain);
		_renderer->ActiveVertexFormat(_vertFmtTerrain);
		_vpTerrain->LocalMatrix4x4(1, camera.GetViewProjectionTransform());
		_vpTerrain->LocalParameter(5, vec4f(camera.GetPosition()));
		float tile = terrain->GetTexureTile();
		_vpTerrain->LocalParameter(6, vec4f(tile / Terrain::PATCH_WIDTH, tile / Terrain::PATCH_WIDTH, 0.0f, 0.0f));
		const float* color = engineAPI.renderSystem->GetMainColor();
		_fpTerrain->LocalParameter(0, color);
		const Terrain::TerrainPatch* hlight = terrain->GetHighlightPatch();

		Texture2DResPtr tex_res = terrain->GetTexture();
		const GL::Texture* tex;
		if(tex_res)
			tex = tex_res;
		else
			tex = engineAPI.renderSystem->GetWhiteTexture();

		_renderer->SetSamplerState(0, _terrainSampler);
		_renderer->SetSamplerState(1, _gradSampler);
		_renderer->SetSamplerTexture(0, tex);
		_renderer->SetSamplerTexture(1, _gradTex);

		for(int i = 0; i < count; ++i)
		{
			_renderer->VertexSource(0, patches[i]->vertBuf, sizeof(Terrain::PatchVertex), 0);

			float transl[] = { patches[i]->boundBox.minPt.x, 0.0f, 0.0f, 0.0f };
			_vpTerrain->LocalParameter(0, transl);

			if(patches[i] == hlight)
			{
				float hlcolor[] = { 0.9215f, 0.5529f, 0.4470f, 1.0f };
				_fpTerrain->LocalParameter(0, hlcolor);
			}

			_renderer->DrawIndexed(GL::PRIM_TRIANGLES, 0, terrain->GetPatchIndexCount());

			if(patches[i] == hlight)
			{
				_fpTerrain->LocalParameter(0, color);
			}
		}

		_renderer->SetSamplerTexture(0, 0);
		_renderer->SetSamplerTexture(1, 0);
	}

	void TerrainRenderer::RenderTerrainPatchGrass(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count)
	{
		_renderer->IndexSource(terrain->GetGrassIndexBuffer(), GL::TYPE_UNSIGNED_SHORT);
		_renderer->ActiveVertexASMProgram(_vpGrass);
		_renderer->ActiveFragmentASMProgram(_fpGrass);
		_renderer->ActiveVertexFormat(_vertFmtGrass);

		_vpGrass->LocalMatrix4x4(1, camera.GetViewProjectionTransform());

		_renderer->SetSamplerState(0, _grassSampler);
		_renderer->SetSamplerTexture(0, terrain->GetGrassTexture()? (const GL::Texture2D*)terrain->GetGrassTexture(): 0);

		_renderer->EnableBlending(true);
		_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
		_renderer->EnableFaceCulling(false);

		for(int i = 0; i < count; ++i)
		{
			vec4f transl(patches[i]->boundBox.minPt.x, 0.0f, 0.0f, 0.0f);
			_vpGrass->LocalParameter(0, transl);

			for(int j = 0; j < Terrain::GRASS_SEGMENTS; ++j)
			{
				if(patches[i]->grassSegments[j].grassVertBuf)
				{
					_renderer->VertexSource(0, patches[i]->grassSegments[j].grassVertBuf, sizeof(Terrain::GrassVertex), 0);

					_renderer->DrawIndexed(GL::PRIM_TRIANGLES, 0, patches[i]->grassSegments[j].grassIndexCount);
				}
			}
		}

		_renderer->SetSamplerTexture(0, 0);
		_renderer->EnableBlending(false);
		_renderer->EnableFaceCulling(true);
	}

	void TerrainRenderer::Clear()
	{
		_renderer = 0;
		_vpTerrain = VertexASMProgResPtr::null;
		_fpTerrain = FragmentASMProgResPtr::null;
		_fpLambert = FragmentASMProgResPtr::null;
		_vpDbgLine = VertexASMProgResPtr::null;
		_fpDbgLine = FragmentASMProgResPtr::null;
		_vpGrass = VertexASMProgResPtr::null;
		_fpGrass = FragmentASMProgResPtr::null;
		_vertFmtTerrain = 0;
		_vertFmtDbgLine = 0;
		_vertFmtGrass = 0;
		_gradSampler = 0;
		_terrainSampler = 0;
		_grassSampler = 0;
		_gradTex = Texture2DResPtr::null;
	}

}
