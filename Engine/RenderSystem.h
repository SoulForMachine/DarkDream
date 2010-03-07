
#ifndef _ENGINE_RENDER_SYSTEM_H_
#define _ENGINE_RENDER_SYSTEM_H_

#include "BaseLib/GL/GLRenderer.h"
#include "BaseLib/FreeStackPool.h"
#include "Engine/Common.h"
//#include "Engine/ResourceManager.h"
#include "Render2D.h"
#include "TerrainRenderer.h"
#include "EntityRenderer.h"
#include "BgLayerRenderer.h"
#include "ParticleRenderer.h"
#include "DebugRenderer.h"


namespace Engine
{

	class World;
	class ModelEntity;


	class ENGINE_API RenderSystem
	{
	public:
		enum RenderStyle
		{
			RENDER_STYLE_GAME,
			RENDER_STYLE_EDITOR,
		};

		RenderSystem();

		bool Init(void* instance_handle, void* window_handle);
		void Deinit();

		void Update(int frame_time);
		void RenderFrame();
		void RenderEntities();
		void RenderTerrain();
		void RenderGrass();
		void RenderBgLayers();
		void RenderParticles();
		void ReloadShaders();

		GL::Renderer* GetRenderer()
			{ return _renderer; }
		Render2D* GetRender2D()
			{ return _render2D; }
		TerrainRenderer* GetTerrainRenderer()
			{ return _terrainRenderer; }
		EntityRenderer* GetEntityRenderer()
			{ return _entityRenderer; }
		BgLayerRenderer* GetBgLayerRenderer()
			{ return _bgLayerRenderer; }
		ParticleRenderer* GetParticleRenderer()
			{ return _particleRenderer; }
		DebugRenderer* GetDebugRenderer()
			{ return _debugRenderer; }

		const math3d::vec4f& GetMainColor() const
			{ return _mainColor; }
		void SetMainColor(const math3d::vec4f& color)
			{ _mainColor = color; }
		const math3d::vec4f& GetEditorColor() const
			{ return _editorColor; }
		void SetEditorColor(const math3d::vec4f& color)
			{ _editorColor = color; }
		const math3d::vec4f& GetRenderColor() const
			{ return (_renderStyle == RENDER_STYLE_GAME)? _mainColor: _editorColor; }
		void SetRenderStyle(RenderStyle style)
			{ _renderStyle = style; }
		RenderStyle GetRenderStyle() const
			{ return _renderStyle; }
		const GL::Texture2D* GetWhiteTexture() const
			{ return _texWhite; }
		const GL::Texture2D* GetBlackTexture() const
			{ return _texBlack; }

	private:
		static const int MAX_NUM_ENTITIES = 4 * 1024;
		static const int MAX_NUM_MESHES = 16 * 1024;
		static const int MAX_NUM_TRANSP_MESHES = 8 * 1024;
		static const int MAX_SPRITES = 128;
		static const int MAX_PARTICLE_SYSTEMS = 128;
		static const int MAX_EMITTERS = MAX_PARTICLE_SYSTEMS * 8;

		void Clear();
		void AddEntityToDrawArray(ModelEntity* entity, const Camera& camera);
		void AddPartSysToDrawArray(ParticleSystem* part_sys);
		void UpdateEntityTime(Entity* entity);
		bool CreateNullResources();
		void DestroyNullResources();

		GL::Renderer* _renderer;
		Render2D* _render2D;
		EntityRenderer* _entityRenderer;
		ModelEntity** _entityBuf;
		EntityRenderer::MeshRenderData** _meshBuf;
		EntityRenderer::MeshRenderData** _transpMeshBuf;
		const Terrain::TerrainPatch* _terrainPatches[Terrain::MAX_PATCHES];
		const BgLayer::Sprite* _sprites[MAX_SPRITES];
		ParticleSystem* _particleSystems[MAX_PARTICLE_SYSTEMS];
		ParticleSystem::Emitter* _emitters[MAX_EMITTERS];
		TerrainRenderer* _terrainRenderer;
		BgLayerRenderer* _bgLayerRenderer;
		ParticleRenderer* _particleRenderer;
		DebugRenderer* _debugRenderer;
		GL::Texture2D* _texWhite;
		GL::Texture2D* _texBlack;
		float _frameTime;
		int _frameTimeMsec;
		math3d::vec4f _mainColor;
		math3d::vec4f _editorColor;
		RenderStyle _renderStyle;
		FreeStackPool<EntityRenderer::MeshRenderData> _meshDataPool;

		int _smapWidth;
		int _smapHeight;
		int _visEntityCount;
		int _visTerrPatchesCount;
		int _visSpriteCount;
		int _visPartSysCount;
		int _visEmitterCount;
		int _meshCount;
		int _transpMeshCount;
	};

}

#endif // _ENGINE_RENDER_SYSTEM_H_
