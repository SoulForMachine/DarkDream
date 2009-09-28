
#include "BaseLib/Memory.h"
#include "BaseLib/Console.h"
#include "Engine/World.h"
#include "Engine/EngineInternal.h"
#include "Engine/TerrainRenderer.h"
#include "Model.h"
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
		_entityRenderer = new(mainPool) EntityRenderer;
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

		// init terrain rendering
		_terrainRenderer = new(mainPool) TerrainRenderer;
		result = _terrainRenderer->Init();
		if(!result)
		{
			Console::PrintError("Failed to initialize terrain rendering.");
			Deinit();
			return false;
		}

		_mainColor.set(1.0f, 1.0f, 1.0f, 1.0f);
		_editorColor.set(0.31f, 0.67f, 0.79f, 1.0f);
		_renderStyle = RENDER_STYLE_GAME;

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

		if(_terrainRenderer)
		{
			_terrainRenderer->Deinit();
			delete _terrainRenderer;
		}

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
				return 0;
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
					return 0;
				}
			}
		}
	}

	void RenderSystem::RenderEntities(int frame_time)
	{
		int ent_count = engineAPI.world->GetVisibleEntities(_entityBuf, MAX_NUM_ENTITIES);
		RenderEntities(frame_time, engineAPI.world->GetCamera(), _entityBuf, ent_count);
	}

	void RenderSystem::RenderEntities(int frame_time, const Camera& camera, ModelEntity** entities, int ent_count)
	{
		int mesh_count = 0;
		int transp_mesh_count = 0;
		for(int ent_i = 0; ent_i < ent_count; ++ent_i)
		{
			if(entities[ent_i]->GetModelRes())
			{
				//! this should be somewhere else
				entities[ent_i]->UpdateGraphics(frame_time);

				const Model* model = entities[ent_i]->GetModelRes()->GetModel();
				const StaticArray<Mesh>& meshes = model->GetMeshes();
				const mat4f& world_mat = entities[ent_i]->GetWorldTransform();
				const StaticArray<mat4f>& joint_mat_palette = entities[ent_i]->GetJointTransforms();

				for(size_t mesh_i = 0; mesh_i < meshes.GetCount(); ++mesh_i)
				{
					if(entities[ent_i]->HasTransparency())
					{
						_transpMeshBuf[transp_mesh_count].mesh = &meshes[mesh_i];
						_transpMeshBuf[transp_mesh_count].worldMat = &world_mat;
						_transpMeshBuf[transp_mesh_count].jointMatPalette = joint_mat_palette.GetData();
						_transpMeshBuf[transp_mesh_count].jointCount = joint_mat_palette.GetCount();

						vec3f wcenter;
						transform(wcenter, meshes[mesh_i].center, world_mat);
						float dist_sq = (camera.GetPosition() - wcenter).length_sq();
						_transpMeshBuf[transp_mesh_count].eyeDistSq = dist_sq;

						transp_mesh_count++;
					}
					else
					{
						_meshBuf[mesh_count].mesh = &meshes[mesh_i];
						_meshBuf[mesh_count].worldMat = &world_mat;
						_meshBuf[mesh_count].jointMatPalette = joint_mat_palette.GetData();
						_meshBuf[mesh_count].jointCount = joint_mat_palette.GetCount();
						_meshBuf[mesh_count].eyeDistSq = 0.0f;
						mesh_count++;
					}
				}
			}
		}

		qsort(_meshBuf, mesh_count, sizeof(EntityRenderer::MeshRenderData), MeshCmpFunc);
		qsort(_transpMeshBuf, transp_mesh_count, sizeof(EntityRenderer::MeshRenderData), TranspMeshCmpFunc);

		_renderer->CullFace(GL::FACE_BACK);
		_entityRenderer->Render(camera, _meshBuf, mesh_count);
		_entityRenderer->Render(camera, _transpMeshBuf, transp_mesh_count);
	}

	void RenderSystem::RenderTerrain(int frame_time)
	{
		const Terrain::TerrainPatch* patches[Terrain::MAX_PATCHES];
		int count = engineAPI.world->GetVisibleTerrainPatches(patches, Terrain::MAX_PATCHES);
		_renderer->CullFace(GL::FACE_BACK);
		_terrainRenderer->RenderTerrainPatch(engineAPI.world->GetCamera(), &engineAPI.world->GetTerrain(), patches, count);

		if(g_cvarDrawTerrainNormals)
			_terrainRenderer->RenderTerrainPatchNormals(engineAPI.world->GetCamera(), &engineAPI.world->GetTerrain(), patches, count);
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
		_terrainRenderer = 0;
	}

}
