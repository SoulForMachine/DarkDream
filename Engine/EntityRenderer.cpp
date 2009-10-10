
#include "Engine/ModelEntity.h"
#include "Engine/Model.h"
#include "Engine/Material.h"
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

		_vpMesh = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Mesh.vp"), true);
		_vpMeshNrm = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Mesh_Nrm.vp"), true);
		_vpMeshSkin = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Mesh_Skin.vp"), true);
		_vpMeshNrmSkin = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Mesh_NrmSkin.vp"), true);
		_fpMesh = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Mesh.fp"), true);
		_fpMeshNrm = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Mesh_Nrm.fp"), true);

		_fpLambert = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Lambert.fp"), true);
		_fpLambertNrm = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Lambert_Nrm.fp"), true);

		_shaders[0].vertProg = _vpMesh->GetASMProgram();
		_shaders[0].fragProg = _fpMesh->GetASMProgram();
		_shaders[1].vertProg = _vpMeshNrm->GetASMProgram();
		_shaders[1].fragProg = _fpMeshNrm->GetASMProgram();
		_shaders[2].vertProg = _vpMeshSkin->GetASMProgram();
		_shaders[2].fragProg = _fpMesh->GetASMProgram();
		_shaders[3].vertProg = _vpMeshNrmSkin->GetASMProgram();
		_shaders[3].fragProg = _fpMeshNrm->GetASMProgram();

		_editorShaders[0].vertProg = _vpMesh->GetASMProgram();
		_editorShaders[0].fragProg = _fpLambert->GetASMProgram();
		_editorShaders[1].vertProg = _vpMesh->GetASMProgram();
		_editorShaders[1].fragProg = _fpLambert->GetASMProgram();
		_editorShaders[2].vertProg = _vpMeshSkin->GetASMProgram();
		_editorShaders[2].fragProg = _fpLambert->GetASMProgram();
		_editorShaders[3].vertProg = _vpMeshSkin->GetASMProgram();
		_editorShaders[3].fragProg = _fpLambert->GetASMProgram();

		GL::VertexAttribDesc vfmt[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },	// position
			{ 0, 1, 3, GL::TYPE_FLOAT, false, false, 16 },	// normal
			{ 0, 2, 4, GL::TYPE_FLOAT, false, false, 28 },	// tangent
			{ 0, 3, 2, GL::TYPE_FLOAT, false, false, 44 },	// uv
		};
		_vertFmtMesh = _renderer->CreateVertexFormat(vfmt, COUNTOF(vfmt));

		GL::VertexAttribDesc skin_vfmt[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },	// position
			{ 0, 1, 3, GL::TYPE_FLOAT, false, false, 16 },	// normal
			{ 0, 2, 4, GL::TYPE_FLOAT, false, false, 28 },	// tangent
			{ 0, 3, 2, GL::TYPE_FLOAT, false, false, 44 },	// uv
			{ 0, 4, 4, GL::TYPE_FLOAT, false, false, 52 },	// skinning
		};
		_vertFmtSkinnedMesh = _renderer->CreateVertexFormat(skin_vfmt, COUNTOF(skin_vfmt));

		// samplers
		GL::SamplerStateDesc sampler_desc = GL::GLState::defaultSamplerState;
		sampler_desc.addressU = GL::TEX_ADDRESS_REPEAT;
		sampler_desc.addressV = GL::TEX_ADDRESS_REPEAT;
		sampler_desc.minFilter = GL::TEX_FILTER_LINEAR_MIPMAP_LINEAR;
		sampler_desc.magFilter = GL::TEX_FILTER_LINEAR;
		_diffuseSampler = _renderer->CreateSamplerState(sampler_desc);
		_emissionSampler = _renderer->CreateSamplerState(sampler_desc);
		_transpSampler = _renderer->CreateSamplerState(sampler_desc);
		_normalSampler = _renderer->CreateSamplerState(sampler_desc);

		return true;
	}

	void EntityRenderer::Deinit()
	{
		engineAPI.asmProgManager->ReleaseASMProgram(_vpMesh);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpMeshNrm);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpMeshSkin);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpMeshNrmSkin);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpMesh);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpMeshNrm);

		engineAPI.asmProgManager->ReleaseASMProgram(_fpLambert);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpLambertNrm);

		_renderer->DestroyVertexFormat(_vertFmtMesh);
		_renderer->DestroyVertexFormat(_vertFmtSkinnedMesh);

		_renderer->DestroySamplerState(_diffuseSampler);
		_renderer->DestroySamplerState(_emissionSampler);
		_renderer->DestroySamplerState(_transpSampler);
		_renderer->DestroySamplerState(_normalSampler);

		Clear();
	}

	void EntityRenderer::Render(const Camera& camera, const MeshRenderData* meshes, int count)
	{
		if(count > 0)
		{
			bool transp = meshes[0].material->HasTransparency();

			if(transp)
			{
				_renderer->EnableBlending(true);
				_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
			}

			for(int i = 0; i < count; ++i)
			{
				RenderMesh(camera, &meshes[i]);
			}

			if(transp)
			{
				_renderer->EnableBlending(false);
			}
		}
	}

	void EntityRenderer::ReloadShaders()
	{
	}

	void EntityRenderer::RenderMesh(const Camera& camera, const MeshRenderData* mesh_data)
	{
		_renderer->VertexSource(0, mesh_data->mesh->vertBuf, mesh_data->mesh->vertexSize, 0);
		_renderer->IndexSource(mesh_data->mesh->indexBuf, GL::TYPE_UNSIGNED_SHORT);

		bool animated = (mesh_data->mesh->flags & Mesh::FLAG_SKIN_DATA) != 0;
		bool nrm = (mesh_data->material->HasNormalMap() && (mesh_data->mesh->flags & Mesh::FLAG_TANGENTS));
		const GL::ASMProgram* vert_prog;
		const GL::ASMProgram* frag_prog;
		if(engineAPI.renderSystem->GetRenderStyle() == RenderSystem::RENDER_STYLE_GAME)
		{
			vert_prog = _shaders[mesh_data->shaderIndex].vertProg;
			frag_prog = _shaders[mesh_data->shaderIndex].fragProg;
		}
		else
		{
			vert_prog = _editorShaders[mesh_data->shaderIndex].vertProg;
			frag_prog = _editorShaders[mesh_data->shaderIndex].fragProg;
		}

		_renderer->ActiveVertexASMProgram(vert_prog);
		vert_prog->LocalMatrix4x4(0, *mesh_data->worldMat);
		vert_prog->LocalMatrix4x4(4, camera.GetViewProjectionTransform());
		vert_prog->LocalParameter(8, vec4f(camera.GetPosition()));
		if(animated)
		{
			for(size_t i = 0; i < mesh_data->jointCount; ++i)
				vert_prog->LocalMatrix4x4(9 + i * 3, mesh_data->jointMatPalette[i]);

			_renderer->ActiveVertexFormat(_vertFmtSkinnedMesh);
		}
		else
		{
			_renderer->ActiveVertexFormat(_vertFmtMesh);
		}

		_renderer->ActiveFragmentASMProgram(frag_prog);
		if(engineAPI.renderSystem->GetRenderStyle() == RenderSystem::RENDER_STYLE_GAME)
		{
			vec4f color(mesh_data->material->GetDiffuseColor(), mesh_data->material->GetOpacity());
			frag_prog->LocalParameter(0, color);
		}
		else
		{
			const vec4f& color = engineAPI.renderSystem->GetEditorColor();
			frag_prog->LocalParameter(0, color);
		}

		// set textures
		_renderer->SetSamplerState(0, _diffuseSampler);
		_renderer->SetSamplerState(1, _emissionSampler);
		_renderer->SetSamplerState(2, _transpSampler);
		_renderer->SetSamplerState(3, _normalSampler);

		const GL::Texture* tex = mesh_data->material->HasDiffuse()?
			mesh_data->material->GetDiffuseTexture()->GetTexture(): engineAPI.renderSystem->GetWhiteTexture();
		_renderer->SetSamplerTexture(0, tex);

		tex = mesh_data->material->HasEmission()?
			mesh_data->material->GetEmissionTexture()->GetTexture(): engineAPI.renderSystem->GetBlackTexture();
		_renderer->SetSamplerTexture(1, tex);

		tex = mesh_data->material->GetTransparencyTexture()?
			mesh_data->material->GetTransparencyTexture()->GetTexture(): engineAPI.renderSystem->GetWhiteTexture();
		_renderer->SetSamplerTexture(2, tex);

		if(nrm)
			_renderer->SetSamplerTexture(3, mesh_data->material->GetNormalMap()->GetTexture());

		_renderer->DrawIndexed(GL::PRIM_TRIANGLES, 0, mesh_data->mesh->numIndices);

		_renderer->SetSamplerTexture(0, 0);
		_renderer->SetSamplerTexture(1, 0);
		_renderer->SetSamplerTexture(2, 0);
		_renderer->SetSamplerTexture(3, 0);
	}

	void EntityRenderer::Clear()
	{
		_vpMesh = 0;
		_vpMeshNrm = 0;
		_vpMeshSkin = 0;
		_vpMeshNrmSkin = 0;
		_fpMesh = 0;
		_fpMeshNrm = 0;
		_fpLambert = 0;
		_fpLambertNrm = 0;
		_vertFmtMesh = 0;
		_vertFmtSkinnedMesh = 0;
		_diffuseSampler = 0;
		_emissionSampler = 0;
		_transpSampler = 0;
		_normalSampler = 0;
		_renderer = 0;
	}

}
