
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

	Console::BoolVar g_cvarDrawEntityBBoxes("r_drawEntityBBoxes", false);
	Console::BoolVar g_cvarDrawParticleEmitter("r_drawParticleEmitter", false);


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

		// particle system renderer
		_particleRenderer = new(mainPool) ParticleRenderer;
		result = _particleRenderer->Init();
		if(!result)
		{
			Console::PrintError("Failed to initialize particle system rendering.");
			Deinit();
			return false;
		}

		// layer renderer
		_bgLayerRenderer = new(mainPool) BgLayerRenderer;
		result = _bgLayerRenderer->Init();
		if(!result)
		{
			Console::PrintError("Failed to initialize background layer rendering.");
			Deinit();
			return false;
		}

		// init debug rendering
		_debugRenderer = new(mainPool) DebugRenderer;
		result = _debugRenderer->Init();

		_mainColor.set(1.0f, 1.0f, 1.0f, 1.0f);
		_editorColor.set(0.31f, 0.67f, 0.79f, 1.0f);
		_renderStyle = RENDER_STYLE_GAME;

		// white and black textures
		int pixel = -1;
		_texWhite = _renderer->CreateTexture2D();
		_texWhite->TexImage(0, GL::PIXEL_FORMAT_RGBA8, 1, 1, GL::IMAGE_FORMAT_BGRA, GL::TYPE_UNSIGNED_BYTE, 0, &pixel);
		_texWhite->GenerateMipmap();

		pixel = 255 << 24;
		_texBlack = _renderer->CreateTexture2D();
		_texBlack->TexImage(0, GL::PIXEL_FORMAT_RGBA8, 1, 1, GL::IMAGE_FORMAT_BGRA, GL::TYPE_UNSIGNED_BYTE, 0, &pixel);
		_texBlack->GenerateMipmap();

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

		if(_bgLayerRenderer)
		{
			_bgLayerRenderer->Deinit();
			delete _bgLayerRenderer;
		}

		if(_particleRenderer)
		{
			_particleRenderer->Deinit();
			delete _particleRenderer;
		}

		if(_debugRenderer)
		{
			_debugRenderer->Deinit();
			delete _debugRenderer;
		}

		_renderer->DestroyTexture(_texWhite);
		_renderer->DestroyTexture(_texBlack);

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
	int EmitterCmpFunc(const void* el1, const void* el2)
	{
		const ParticleSystem::Emitter* em1 = *(ParticleSystem::Emitter**)el1;
		const ParticleSystem::Emitter* em2 = *(ParticleSystem::Emitter**)el2;

		const vec3f& cam_pos = engineAPI.world->GetCamera().GetPosition();
		float dif = (em1->GetPosition() - cam_pos).length_sq() - (em2->GetPosition() - cam_pos).length_sq();
		return (dif > 0.0f)? -1: 1;
	}

	void RenderSystem::RenderEntities(int frame_time)
	{
		int ent_count = engineAPI.world->GetVisibleEntities(_entityBuf, MAX_NUM_ENTITIES);
		RenderEntities(frame_time, engineAPI.world->GetCamera(), _entityBuf, ent_count);
	}

	void RenderSystem::RenderEntities(int frame_time, const Camera& camera, ModelEntity** entities, int ent_count)
	{
		_meshCount = 0;
		_transpMeshCount = 0;
		for(int ent_i = 0; ent_i < ent_count; ++ent_i)
		{
			if(entities[ent_i]->GetModelRes())
			{
				//! this should be somewhere else
				entities[ent_i]->UpdateGraphics(frame_time);
				AddEntityToDrawArray(entities[ent_i], camera);
			}
		}

		qsort(_meshBuf, _meshCount, sizeof(EntityRenderer::MeshRenderData), MeshCmpFunc);
		qsort(_transpMeshBuf, _transpMeshCount, sizeof(EntityRenderer::MeshRenderData), TranspMeshCmpFunc);

		_renderer->CullFace(GL::FACE_BACK);
		_entityRenderer->Render(camera, _meshBuf, _meshCount);
		_entityRenderer->Render(camera, _transpMeshBuf, _transpMeshCount);

		if(g_cvarDrawEntityBBoxes)
		{
			for(int ent_i = 0; ent_i < ent_count; ++ent_i)
			{
				_debugRenderer->RenderBoundingBox(entities[ent_i]->GetWorldBoundingBox());
			}
		}
	}

	void RenderSystem::AddEntityToDrawArray(ModelEntity* entity, const Camera& camera)
	{
		const Model* model = entity->GetModelRes()->GetModel();
		const StaticArray<Mesh>& meshes = model->GetMeshes();
		const ModelEntity::MeshDataArray& mesh_data = entity->GetMeshDataArray(); 
		const mat4f& world_mat = entity->GetWorldTransform();
		const StaticArray<mat4f>& joint_mat_palette = entity->GetJointTransforms();

		for(size_t mesh_i = 0; mesh_i < meshes.GetCount(); ++mesh_i)
		{
			if(	mesh_data[mesh_i].materialData->materialRes->GetMaterial()->HasTransparency() &&
				_transpMeshCount < MAX_NUM_TRANSP_MESHES )
			{
				_transpMeshBuf[_transpMeshCount].mesh = &meshes[mesh_i];
				_transpMeshBuf[_transpMeshCount].worldMat = &world_mat;
				_transpMeshBuf[_transpMeshCount].jointMatPalette = joint_mat_palette.GetData();
				_transpMeshBuf[_transpMeshCount].jointCount = joint_mat_palette.GetCount();
				_transpMeshBuf[_transpMeshCount].material = mesh_data[mesh_i].materialData->materialRes->GetMaterial();
				_transpMeshBuf[_transpMeshCount].shaderIndex = mesh_data[mesh_i].shaderIndex;

				vec3f wcenter;
				transform(wcenter, meshes[mesh_i].center, world_mat);
				float dist_sq = (camera.GetPosition() - wcenter).length_sq();
				_transpMeshBuf[_transpMeshCount].eyeDistSq = dist_sq;

				_transpMeshCount++;
			}
			else if(_meshCount < MAX_NUM_MESHES)
			{
				_meshBuf[_meshCount].mesh = &meshes[mesh_i];
				_meshBuf[_meshCount].worldMat = &world_mat;
				_meshBuf[_meshCount].jointMatPalette = joint_mat_palette.GetData();
				_meshBuf[_meshCount].jointCount = joint_mat_palette.GetCount();
				_meshBuf[_meshCount].material = mesh_data[mesh_i].materialData->materialRes->GetMaterial();
				_meshBuf[_meshCount].shaderIndex = mesh_data[mesh_i].shaderIndex;
				_meshBuf[_meshCount].eyeDistSq = 0.0f;
				_meshCount++;
			}
			else
			{
				break;
			}
		}

		const ModelEntity::JointAttachMap& attachments = entity->GetJointAttachments();
		for(ModelEntity::JointAttachMap::ConstIterator it = attachments.Begin(); it != attachments.End(); ++it)
		{
			if(it->type == ModelEntity::JOINT_ATTACH_MODEL)
			{
				ModelEntity* att_ent = ((ModelEntityRes*)it->attachment)->GetEntity();
				if(att_ent)
					AddEntityToDrawArray(att_ent, camera);
			}
		}
	}

	void RenderSystem::RenderTerrain(int frame_time)
	{
		const Terrain::TerrainPatch* patches[Terrain::MAX_PATCHES];
		int count = engineAPI.world->GetVisibleTerrainPatches(patches, Terrain::MAX_PATCHES);
		_renderer->CullFace(GL::FACE_BACK);
		_terrainRenderer->RenderTerrainPatches(engineAPI.world->GetCamera(), &engineAPI.world->GetTerrain(), patches, count);

		if(g_cvarDrawTerrainNormals)
			_terrainRenderer->RenderTerrainPatchNormals(engineAPI.world->GetCamera(), &engineAPI.world->GetTerrain(), patches, count);
	}

	void RenderSystem::RenderGrass(int frame_time)
	{
		const Terrain::TerrainPatch* patches[Terrain::MAX_PATCHES];
		int count = engineAPI.world->GetVisibleTerrainPatches(patches, Terrain::MAX_PATCHES);
		_terrainRenderer->RenderTerrainPatchGrass(engineAPI.world->GetCamera(), &engineAPI.world->GetTerrain(), patches, count);
	}

	void RenderSystem::RenderBgLayers(int frame_time)
	{
		const int MAX_SPRITES = 128;
		const BgLayer::Sprite* sprites[MAX_SPRITES];
		int count = engineAPI.world->GetVisibleLayerSprites(sprites, MAX_SPRITES);
		_bgLayerRenderer->Render(sprites, count);
	}

	void RenderSystem::RenderParticles(int frame_time)
	{
		const int MAX_PARTICLE_SYSTEMS = 128;
		ParticleSystem* part_sys[MAX_PARTICLE_SYSTEMS];
		ParticleSystem::Emitter* emitters[MAX_PARTICLE_SYSTEMS * 8];
		int count = engineAPI.world->GetVisibleParticleSystems(part_sys, MAX_PARTICLE_SYSTEMS);
		int em_count = 0;

		for(int i = 0; i < count; ++i)
		{
			part_sys[i]->UpdateGraphics(frame_time);
			const List<ParticleSystem::Emitter*>& em_list = part_sys[i]->GetEmitterList();

			for(List<ParticleSystem::Emitter*>::ConstIterator it = em_list.Begin(); it != em_list.End(); ++it)
			{
				if((*it)->IsEnabled())
				{
					emitters[em_count++] = *it;
					if(em_count == MAX_PARTICLE_SYSTEMS * 8)
						break;
				}
			}

			if(em_count == MAX_PARTICLE_SYSTEMS * 8)
				break;
		}

		qsort(emitters, em_count, sizeof(ParticleSystem::Emitter*), EmitterCmpFunc);

		_particleRenderer->Render(engineAPI.world->GetCamera(), emitters, em_count);

		if(g_cvarDrawEntityBBoxes)
		{
			for(int i = 0; i < count; ++i)
			{
				_debugRenderer->RenderBoundingBox(part_sys[i]->GetWorldBoundingBox());
			}
		}

		if(g_cvarDrawParticleEmitter)
		{
			for(int i = 0; i < count; ++i)
			{
				const List<ParticleSystem::Emitter*>& em_list = part_sys[i]->GetEmitterList();
				for(List<ParticleSystem::Emitter*>::ConstIterator it = em_list.Begin(); it != em_list.End(); ++it)
				{
					if((*it)->IsEnabled())
					{
						_debugRenderer->RenderParticleEmitter(**it);
					}
				}
			}
		}
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
		_bgLayerRenderer = 0;
		_particleRenderer = 0;
		_debugRenderer = 0;
	}

}
