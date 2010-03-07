
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
	Console::BoolVar g_cvarDrawModelSkelet("r_drawModelSkelet", false);


	static
	int MeshCmpFunc(const void* el1, const void* el2)
	{
		const EntityRenderer::MeshRenderData* mesh1 = *(EntityRenderer::MeshRenderData**)el1;
		const EntityRenderer::MeshRenderData* mesh2 = *(EntityRenderer::MeshRenderData**)el2;

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
		const EntityRenderer::MeshRenderData* mesh1 = *(EntityRenderer::MeshRenderData**)el1;
		const EntityRenderer::MeshRenderData* mesh2 = *(EntityRenderer::MeshRenderData**)el2;

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



	RenderSystem::RenderSystem()
		: _meshDataPool(MAX_NUM_MESHES + MAX_NUM_TRANSP_MESHES)
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

		// set some default states
		_renderer->FrontFace(GL::ORIENT_CCW);
		_renderer->CullFace(GL::FACE_BACK);
		_renderer->EnableFaceCulling(true);
		_renderer->EnableDepthTest(true);

		// create graphics related null resources
		if(!CreateNullResources())
		{
			Deinit();
			return false;
		}

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
		_meshBuf = new(mainPool) EntityRenderer::MeshRenderData*[MAX_NUM_MESHES];
		_transpMeshBuf = new(mainPool) EntityRenderer::MeshRenderData*[MAX_NUM_TRANSP_MESHES];

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

		pixel = 255 << 24;
		_texBlack = _renderer->CreateTexture2D();
		_texBlack->TexImage(0, GL::PIXEL_FORMAT_RGBA8, 1, 1, GL::IMAGE_FORMAT_BGRA, GL::TYPE_UNSIGNED_BYTE, 0, &pixel);

		return true;
	}

	void RenderSystem::Deinit()
	{
		if(!_renderer)
			return;

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

		DestroyNullResources();

		GL::DestroyRenderer(_renderer);

		delete[] _entityBuf;
		delete[] _meshBuf;
		delete[] _transpMeshBuf;
		_meshDataPool.Reset();

		Clear();
	}

	void RenderSystem::Update(int frame_time)
	{
		_frameTime = frame_time * 0.001f;
		_frameTimeMsec = frame_time;

		// model entities
		EntityHashMap& entities = engineAPI.world->GetEntities();
		for(EntityHashMap::Iterator it = entities.Begin(); it != entities.End(); ++it)
		{
			UpdateEntityTime(*it);
		}

		_visEntityCount = engineAPI.world->GetVisibleModelEntities(_entityBuf, MAX_NUM_ENTITIES);

		_meshCount = 0;
		_transpMeshCount = 0;
		_meshDataPool.Reset();
		_visEmitterCount = 0;
		_visPartSysCount = 0;

		for(int ent_i = 0; ent_i < _visEntityCount; ++ent_i)
		{
			if(_entityBuf[ent_i]->GetModelRes())
			{
				_entityBuf[ent_i]->UpdateGraphics();
				AddEntityToDrawArray(_entityBuf[ent_i], engineAPI.world->GetCamera());
			}
		}

		qsort(_meshBuf, _meshCount, sizeof(EntityRenderer::MeshRenderData*), MeshCmpFunc);
		qsort(_transpMeshBuf, _transpMeshCount, sizeof(EntityRenderer::MeshRenderData*), TranspMeshCmpFunc);

		// particle systems
		ParticleSystem** ps = &_particleSystems[_visPartSysCount];
		int ps_count = engineAPI.world->GetVisibleParticleSystems(ps, MAX_PARTICLE_SYSTEMS - _visPartSysCount);

		for(int i = 0; i < ps_count; ++i)
		{
			ps[i]->UpdateGraphics();
			AddPartSysToDrawArray(ps[i]);

			if(_visEmitterCount == MAX_EMITTERS)
				break;
		}

		qsort(_emitters, _visEmitterCount, sizeof(ParticleSystem::Emitter*), EmitterCmpFunc);

		// terrain patches
		_visTerrPatchesCount = engineAPI.world->GetVisibleTerrainPatches(_terrainPatches, Terrain::MAX_PATCHES);

		// sprites
		_visSpriteCount = engineAPI.world->GetVisibleLayerSprites(_sprites, MAX_SPRITES);
	}

	void RenderSystem::UpdateEntityTime(Entity* entity)
	{
		entity->UpdateTime(_frameTime);
		if(entity->GetType() == ENTITY_TYPE_MODEL)
		{
			const ModelEntity::JointAttachMap& attachments = ((ModelEntity*)entity)->GetJointAttachments();
			for(ModelEntity::JointAttachMap::ConstIterator it = attachments.Begin(); it != attachments.End(); ++it)
			{
				if(it->type == ModelEntity::JOINT_ATTACH_MODEL)
				{
					ModelEntity* att_ent = (ModelEntity*) ((ModelEntityRes*)it->attachment)->GetResource();
					if(att_ent)
					{
						UpdateEntityTime(att_ent);
					}
				}
				else if(it->type == ModelEntity::JOINT_ATTACH_PARTICLE_SYSTEM)
				{
					ParticleSystem* ps = (ParticleSystem*) ((PartSysRes*)it->attachment)->GetResource();
					if(ps)
					{
						ps->UpdateTime(_frameTime);
					}
				}
			}
		}
	}

	bool RenderSystem::CreateNullResources()
	{
		if(!Texture2DRes::CreateNull())
		{
			Console::PrintError("Failed to create null 2D texture.");
			Deinit();
			return false;
		}

		if(!Texture3DRes::CreateNull())
		{
			Console::PrintError("Failed to create null 3D texture.");
			Deinit();
			return false;
		}

		if(!TextureCubeRes::CreateNull())
		{
			Console::PrintError("Failed to create null cube texture.");
			Deinit();
			return false;
		}

		if(!VertexShaderRes::CreateNull())
		{
			Console::PrintError("Failed to create null vertex shader.");
			Deinit();
			return false;
		}

		if(!FragmentShaderRes::CreateNull())
		{
			Console::PrintError("Failed to create null fragment shader.");
			Deinit();
			return false;
		}
		if(!GeometryShaderRes::CreateNull())
		{
			Console::PrintError("Failed to create null geometry shader.");
			Deinit();
			return false;
		}

		if(!VertexASMProgRes::CreateNull())
		{
			Console::PrintError("Failed to create null vertex asm program.");
			Deinit();
			return false;
		}

		if(!FragmentASMProgRes::CreateNull())
		{
			Console::PrintError("Failed to create null fragment asm program.");
			Deinit();
			return false;
		}

		if(!GeometryASMProgRes::CreateNull())
		{
			Console::PrintError("Failed to create null geometry asm program.");
			Deinit();
			return false;
		}

		if(!ModelRes::CreateNull())
		{
			Console::PrintError("Failed to create null model.");
			Deinit();
			return false;
		}

		if(!MaterialRes::CreateNull())
		{
			Console::PrintError("Failed to create null material.");
			Deinit();
			return false;
		}

		if(!ModelEntityRes::CreateNull())
		{
			Console::PrintError("Failed to create null model entity.");
			Deinit();
			return false;
		}

		if(!AnimationRes::CreateNull())
		{
			Console::PrintError("Failed to create null animation.");
			Deinit();
			return false;
		}

		if(!PartSysRes::CreateNull())
		{
			Console::PrintError("Failed to create null particle system.");
			Deinit();
			return false;
		}

		return true;
	}

	void RenderSystem::DestroyNullResources()
	{
		Texture2DRes::DestroyNull();
		Texture3DRes::DestroyNull();
		TextureCubeRes::DestroyNull();
		VertexShaderRes::DestroyNull();
		FragmentShaderRes::DestroyNull();
		GeometryShaderRes::DestroyNull();
		VertexASMProgRes::DestroyNull();
		FragmentASMProgRes::DestroyNull();
		GeometryASMProgRes::DestroyNull();
		ModelRes::DestroyNull();
		MaterialRes::DestroyNull();
		ModelEntityRes::DestroyNull();
		AnimationRes::DestroyNull();
		PartSysRes::DestroyNull();
	}

	void RenderSystem::RenderFrame()
	{
		_renderer->SwapInterval(0);
		_renderer->ClearColorBuffer(0.0f, 0.0f, 0.0f, 0.0f);
		_renderer->ClearDepthStencilBuffer(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT, 1.0f, 0);

		RenderBgLayers();
		RenderTerrain();
		RenderEntities();
		RenderGrass();
		RenderParticles();

		_renderer->Flush();
		_renderer->SwapBuffers();
	}

	void RenderSystem::RenderEntities()
	{
		_entityRenderer->Render(engineAPI.world->GetCamera(), _meshBuf, _meshCount);
		_entityRenderer->Render(engineAPI.world->GetCamera(), _transpMeshBuf, _transpMeshCount);

		if(g_cvarDrawEntityBBoxes)
		{
			for(int ent_i = 0; ent_i < _visEntityCount; ++ent_i)
			{
				_debugRenderer->RenderBoundingBox(_entityBuf[ent_i]->GetWorldBoundingBox());
			}
		}

		if(g_cvarDrawModelSkelet)
		{
			for(int ent_i = 0; ent_i < _visEntityCount; ++ent_i)
			{
				_debugRenderer->RenderModelSkelet(*_entityBuf[ent_i], vec3f(0.0f, 1.0f, 0.0f));
			}
		}
	}

	void RenderSystem::AddEntityToDrawArray(ModelEntity* entity, const Camera& camera)
	{
		const Model* model = entity->GetModelRes();
		const StaticArray<Mesh>& meshes = model->GetMeshes();
		const ModelEntity::MeshDataArray& mesh_data = entity->GetMeshDataArray(); 
		const mat4f& world_mat = entity->GetWorldTransform();
		const StaticArray<mat4f>& joint_mat_palette = entity->GetJointTransforms();

		for(size_t mesh_i = 0; mesh_i < meshes.GetCount(); ++mesh_i)
		{
			if(	mesh_data[mesh_i].materialData->materialRes->HasTransparency() &&
				_transpMeshCount < MAX_NUM_TRANSP_MESHES )
			{
				_transpMeshBuf[_transpMeshCount] = _meshDataPool.New();
				_transpMeshBuf[_transpMeshCount]->mesh = &meshes[mesh_i];
				_transpMeshBuf[_transpMeshCount]->worldMat = &world_mat;
				_transpMeshBuf[_transpMeshCount]->jointMatPalette = joint_mat_palette.GetData();
				_transpMeshBuf[_transpMeshCount]->jointCount = joint_mat_palette.GetCount();
				_transpMeshBuf[_transpMeshCount]->material = mesh_data[mesh_i].materialData->materialRes;
				_transpMeshBuf[_transpMeshCount]->shaderIndex = mesh_data[mesh_i].shaderIndex;

				vec3f wcenter;
				transform(wcenter, meshes[mesh_i].center, world_mat);
				float dist_sq = (camera.GetPosition() - wcenter).length_sq();
				_transpMeshBuf[_transpMeshCount]->eyeDistSq = dist_sq;

				_transpMeshCount++;
			}
			else if(_meshCount < MAX_NUM_MESHES)
			{
				_meshBuf[_meshCount] = _meshDataPool.New();
				_meshBuf[_meshCount]->mesh = &meshes[mesh_i];
				_meshBuf[_meshCount]->worldMat = &world_mat;
				_meshBuf[_meshCount]->jointMatPalette = joint_mat_palette.GetData();
				_meshBuf[_meshCount]->jointCount = joint_mat_palette.GetCount();
				_meshBuf[_meshCount]->material = mesh_data[mesh_i].materialData->materialRes;
				_meshBuf[_meshCount]->shaderIndex = mesh_data[mesh_i].shaderIndex;
				_meshBuf[_meshCount]->eyeDistSq = 0.0f;
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
				ModelEntity* att_ent = (ModelEntity*) ((ModelEntityRes*)it->attachment)->GetResource();
				if(att_ent)
				{
					att_ent->UpdateGraphics();
					AddEntityToDrawArray(att_ent, camera);
				}
			}
			else if(it->type == ModelEntity::JOINT_ATTACH_PARTICLE_SYSTEM)
			{
				ParticleSystem* ps = (ParticleSystem*) ((PartSysRes*)it->attachment)->GetResource();
				if(ps)
				{
					ps->UpdateGraphics();
					AddPartSysToDrawArray(ps);
				}
			}
		}
	}

	void RenderSystem::AddPartSysToDrawArray(ParticleSystem* part_sys)
	{
		if(_visPartSysCount == MAX_PARTICLE_SYSTEMS)
			return;

		const List<ParticleSystem::Emitter*>& em_list = part_sys->GetEmitterList();
		_visPartSysCount++;

		for(List<ParticleSystem::Emitter*>::ConstIterator it = em_list.Begin(); it != em_list.End(); ++it)
		{
			if((*it)->IsEnabled())
			{
				if(_visEmitterCount == MAX_EMITTERS)
					break;
				_emitters[_visEmitterCount++] = *it;
			}
		}
	}

	void RenderSystem::RenderTerrain()
	{
		_terrainRenderer->RenderTerrainPatches(engineAPI.world->GetCamera(), &engineAPI.world->GetTerrain(), _terrainPatches, _visTerrPatchesCount);

		if(g_cvarDrawTerrainNormals)
		{
			for(int i = 0; i < _visTerrPatchesCount; ++i)
			{
				_debugRenderer->RenderTerrainPatchNormals(*_terrainPatches[i], vec3f(1.0f, 0.0f, 0.0f));
			}
		}
	}

	void RenderSystem::RenderGrass()
	{
		_terrainRenderer->RenderTerrainPatchGrass(engineAPI.world->GetCamera(), &engineAPI.world->GetTerrain(), _terrainPatches, _visTerrPatchesCount);
	}

	void RenderSystem::RenderBgLayers()
	{
		_bgLayerRenderer->Render(_sprites, _visSpriteCount);
	}

	void RenderSystem::RenderParticles()
	{
		_particleRenderer->Render(engineAPI.world->GetCamera(), _emitters, _visEmitterCount);

		if(g_cvarDrawEntityBBoxes)
		{
			for(int i = 0; i < _visPartSysCount; ++i)
			{
				_debugRenderer->RenderBoundingBox(_particleSystems[i]->GetWorldBoundingBox());
			}
		}

		if(g_cvarDrawParticleEmitter)
		{
			for(int i = 0; i < _visEmitterCount; ++i)
			{
				_debugRenderer->RenderParticleEmitter(*_emitters[i]);
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
		_visEntityCount = 0;
		_visTerrPatchesCount = 0;
		_visSpriteCount = 0;
		_visPartSysCount = 0;
		_visEmitterCount = 0;
	}

}
