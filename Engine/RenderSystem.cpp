
#include "BaseLib/Memory.h"
#include "BaseLib/Console.h"
#include "Engine/World.h"
#include "Engine/EngineInternal.h"
#include "Model.h"
#include "Material.h"
#include "RenderSystem.h"


using namespace Memory;
using namespace GL;
using namespace math3d;


namespace Engine
{

	RenderSystem::RenderSystem()
	{
		Clear();
	}

	bool RenderSystem::Init(void* instance_handle, void* window_handle)
	{
		if(!instance_handle || !window_handle)
			return false;

		Console::PrintLn("----- Initializing rendering system -----");

		// initialize rendering API
		FramebufferFormat fbuf_fmt;
		fbuf_fmt.colorBufferType = COLOR_RGBA;
		fbuf_fmt.sRGB = false;
		fbuf_fmt.doubleBuffer = true;
		fbuf_fmt.colorBits = 32;
		fbuf_fmt.depthBits = 24;
		fbuf_fmt.stencilBits = 8;
		fbuf_fmt.multisampleSamples = 0;
		fbuf_fmt.swapMethod = SWAP_EXCHANGE;

		_renderer = GL::CreateRenderer(210, instance_handle, window_handle, fbuf_fmt);
		if(!_renderer)
		{
			Console::PrintError("Failed to initialize renderer.");
			Deinit();
			return false;
		}
		bool result = _renderer->SetCurrentContext();
		if(!result)
		{
			Console::PrintError("Failed to set rendering context.");
			Deinit();
			return false;
		}

		// print some API info
		const ContextInfo& info = _renderer->GetInfo();
		Console::PrintLn(
			"OpenGL version: %d.%d\n"
			"Shading language version: %s\n"
			"Renderer: %s\n"
			"Vendor: %s",
			info.versionMajor, info.versionMinor, info.glslVersion, info.renderer, info.vendor);

		// shadowmap render target
		_smapWidth = 2048;
		_smapHeight = 2048;
		Light::UpdateShadowMapScaleBiasMat(_smapWidth, _smapHeight);
		_shadowClrTex = _renderer->CreateTexture2D();
		_shadowClrTex->TexImage(0, GL::PIXEL_FORMAT_RGBA8, _smapWidth, _smapHeight, GL::IMAGE_FORMAT_BGRA, GL::TYPE_UNSIGNED_BYTE, 0, 0);
		_shadowDepthTex = _renderer->CreateTexture2D();
		_shadowDepthTex->TexImage(0, GL::PIXEL_FORMAT_DEPTH32F, _smapWidth, _smapHeight, GL::IMAGE_FORMAT_DEPTH, GL::TYPE_UNSIGNED_INT, 0, 0);

		_shadowFbuf = _renderer->CreateFramebuffer(OBJ_DRAW_FRAMEBUFFER);
		_shadowFbuf->AttachTexture(GL::BUFFER_COLOR0, _shadowClrTex, 0);
		_shadowFbuf->AttachTexture(GL::BUFFER_DEPTH, _shadowDepthTex, 0);
		GL::RenderbufferType draw_buf = GL::BUFFER_COLOR0;
		_shadowFbuf->ActiveDrawBuffers(1, &draw_buf);
		GL::FramebufferStatus status = _shadowFbuf->CheckStatus();

		// init 2D rendering
		_render2D = new(mainPool) Render2D;
		result = _render2D->Init(this);
		if(!result)
		{
			Console::PrintError("Failed to initialize 2D rendering.");
			Deinit();
			return false;
		}

		// init entity rendering
		_entityRenderer = new(mainPool) EntityRenderer_GL30;
		result = _entityRenderer->Init();
		if(!result)
		{
			Console::PrintError("Failed to initialize entity rendering.");
			Deinit();
			return false;
		}

		_entityBuf = new(mainPool) ModelEntity*[MAX_NUM_ENTITIES];
		_meshBuf = new(mainPool) EntityRenderer::MeshRenderData[MAX_NUM_MESHES];
		_transpMeshBuf = new(mainPool) EntityRenderer::MeshRenderData[MAX_NUM_TRANSP_MESHES];

		return true;
	}

	void RenderSystem::Deinit()
	{
		Console::PrintLn("----- Deinitializing rendering system -----");

		if(_render2D)
		{
			_render2D->Deinit();
			delete _render2D;
		}

		if(_entityRenderer)
		{
			_entityRenderer->Deinit();
			delete _entityRenderer;
		}

		_renderer->DestroyFramebuffer(_shadowFbuf);
		_renderer->DestroyTexture(_shadowClrTex);
		_renderer->DestroyTexture(_shadowDepthTex);

		GL::DestroyRenderer(_renderer);

		delete[] _entityBuf;
		delete[] _meshBuf;
		delete[] _transpMeshBuf;

		Clear();
	}

	void RenderSystem::RenderFrame(int frame_time)
	{
		_frameTime = frame_time;
		_renderer->SwapInterval(0);
		_renderer->ClearColorBuffer(0.0f, 0.0f, 0.0f, 0.0f);
		_renderer->ClearDepthStencilBuffer(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT, 1.0f, 0);
	//	_renderer->RasterizationMode(RASTER_LINE);
		_render2D->DrawConsole(frame_time);
		_renderer->Flush();
		_renderer->SwapBuffers();
	}

	static
	int MeshCmpFunc(const void* el1, const void* el2)
	{
		const EntityRenderer::MeshRenderData* mesh1 = (EntityRenderer::MeshRenderData*)el1;
		const EntityRenderer::MeshRenderData* mesh2 = (EntityRenderer::MeshRenderData*)el2;

		if(mesh1->mesh > mesh2->mesh)
		{
			return 1;
		}
		else if(mesh1->mesh < mesh2->mesh)
		{
			return -1;
		}
		else
		{
			if(mesh1->mesh->vertLayout > mesh2->mesh->vertLayout)
			{
				return 1;
			}
			else if(mesh1->mesh->vertLayout < mesh2->mesh->vertLayout)
			{
				return -1;
			}
			else
			{
				if(mesh1->material > mesh2->material)
				{
					return 1;
				}
				else if(mesh1->material < mesh2->material)
				{
					return -1;
				}
				else
				{
					return 0;
				}
			}
		}
	}

	static
	int TranspMeshCmpFunc(const void* el1, const void* el2)
	{
		const EntityRenderer::MeshRenderData* mesh1 = (EntityRenderer::MeshRenderData*)el1;
		const EntityRenderer::MeshRenderData* mesh2 = (EntityRenderer::MeshRenderData*)el2;

		if(mesh1->eyeDistSq > mesh2->eyeDistSq)
		{
			return 1;
		}
		else if(mesh1->eyeDistSq < mesh2->eyeDistSq)
		{
			return -1;
		}
		else
		{
			if(mesh1->mesh > mesh2->mesh)
			{
				return 1;
			}
			else if(mesh1->mesh < mesh2->mesh)
			{
				return -1;
			}
			else
			{
				if(mesh1->mesh->vertLayout > mesh2->mesh->vertLayout)
				{
					return 1;
				}
				else if(mesh1->mesh->vertLayout < mesh2->mesh->vertLayout)
				{
					return -1;
				}
				else
				{
					if(mesh1->material > mesh2->material)
					{
						return 1;
					}
					else if(mesh1->material < mesh2->material)
					{
						return -1;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}

	void RenderSystem::RenderEntities(int frame_time)
	{
		_frameTime = frame_time;
		int ent_count = engineAPI.world->GetVisibleEntities(_entityBuf, MAX_NUM_ENTITIES);
		int mesh_count = 0;
		int transp_mesh_count = 0;
		for(int ent_i = 0; ent_i < ent_count; ++ent_i)
		{
			if(_entityBuf[ent_i]->GetModelRes())
			{
				//! this should be somewhere else
				_entityBuf[ent_i]->UpdateGraphics(frame_time);

				const Model* model = _entityBuf[ent_i]->GetModelRes()->GetModel();
				const StaticArray<Mesh>& meshes = model->GetMeshes();
				const ModelEntity::MeshDataArray& mesh_data = _entityBuf[ent_i]->GetMeshDataArray();
				const mat4f& world_mat = _entityBuf[ent_i]->GetWorldTransform();
				const StaticArray<mat4f>& joint_mat_palette = _entityBuf[ent_i]->GetJointTransforms();

				for(size_t mesh_i = 0; mesh_i < meshes.GetCount(); ++mesh_i)
				{
					if(mesh_data[mesh_i].materialData->materialRes->GetMaterial()->HasTransparency())
					{
						_transpMeshBuf[transp_mesh_count].mesh = &meshes[mesh_i];
						_transpMeshBuf[transp_mesh_count].material = mesh_data[mesh_i].materialData->materialRes->GetMaterial();
						_transpMeshBuf[transp_mesh_count].worldMat = &world_mat;
						_transpMeshBuf[transp_mesh_count].jointMatPalette = joint_mat_palette.GetData();
						_transpMeshBuf[transp_mesh_count].jointCount = joint_mat_palette.GetCount();
						_transpMeshBuf[transp_mesh_count].programIndex = mesh_data[mesh_i].gpuProgIndex;
						_transpMeshBuf[transp_mesh_count].smapProgramIndex = mesh_data[mesh_i].smapGPUProgIndex;
						_transpMeshBuf[transp_mesh_count].uvAnimOffset = mesh_data[mesh_i].materialData->uvAnimOffset;

						vec3f wcenter;
						transform(wcenter, meshes[mesh_i].center, world_mat);
						float dist_sq = (engineAPI.world->GetCamera().GetPosition() - wcenter).length_sq();
						_transpMeshBuf[transp_mesh_count].eyeDistSq = dist_sq;

						transp_mesh_count++;
					}
					else
					{
						_meshBuf[mesh_count].mesh = &meshes[mesh_i];
						_meshBuf[mesh_count].material = mesh_data[mesh_i].materialData->materialRes->GetMaterial();
						_meshBuf[mesh_count].worldMat = &world_mat;
						_meshBuf[mesh_count].jointMatPalette = joint_mat_palette.GetData();
						_meshBuf[mesh_count].jointCount = joint_mat_palette.GetCount();
						_meshBuf[mesh_count].programIndex = mesh_data[mesh_i].gpuProgIndex;
						_meshBuf[mesh_count].smapProgramIndex = mesh_data[mesh_i].smapGPUProgIndex;
						_meshBuf[mesh_count].uvAnimOffset = mesh_data[mesh_i].materialData->uvAnimOffset;
						_meshBuf[mesh_count].eyeDistSq = 0.0f;
						mesh_count++;
					}
				}
			}
		}

		qsort(_meshBuf, mesh_count, sizeof(EntityRenderer::MeshRenderData), MeshCmpFunc);
		qsort(_transpMeshBuf, transp_mesh_count, sizeof(EntityRenderer::MeshRenderData), TranspMeshCmpFunc);

		_entityRenderer->BatchMeshes(false, _meshBuf, mesh_count);
		_entityRenderer->BatchMeshes(true, _transpMeshBuf, transp_mesh_count);
		_renderer->ActiveDrawFramebuffer(_shadowFbuf);
	//	_renderer->ClearColorBuffer(1.0f, 1.0f, 1.0f, 1.0f);
		_renderer->EnableColorWrite(false, false, false, false);
		_renderer->ClearDepthStencilBuffer(GL::DEPTH_BUFFER_BIT, 1.0f, 0);
		int vp[4];
		_renderer->GetViewport(vp[0], vp[1], vp[2], vp[3]);
		_renderer->Viewport(0, 0, _smapWidth, _smapHeight);
		_renderer->CullFace(GL::FACE_FRONT);
		_entityRenderer->RenderToShadowmap(engineAPI.world->GetMainLight());
		_renderer->ActiveDrawFramebuffer(0);
		_renderer->EnableColorWrite(true, true, true, true);
		_renderer->Viewport(vp[0], vp[1], vp[2], vp[3]);
		_renderer->CullFace(GL::FACE_BACK);
		_entityRenderer->Render(engineAPI.world->GetCamera(), engineAPI.world->GetMainLight());
		_entityRenderer->ClearBatches();
		_entityRenderer->InvalidateTransformCache();
	}

	void RenderSystem::InvalidateCache()
	{
		_entityRenderer->InvalidateGeometryCache();
		_entityRenderer->InvalidateTransformCache();
	}

	void RenderSystem::ReloadShaders()
	{
		_render2D->ReloadShaders();
		_entityRenderer->ReloadShaders();
	}

	void RenderSystem::Clear()
	{
		_renderer = 0;
		_render2D = 0;
		_entityRenderer = 0;
		_entityBuf = 0;
		_meshBuf = 0;
		_transpMeshBuf = 0;
	}

}
