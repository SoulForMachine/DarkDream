
#ifndef _ENGINE_RENDER_SYSTEM_H_
#define _ENGINE_RENDER_SYSTEM_H_

#include "BaseLib/GL/GLRenderer.h"
#include "Engine/Common.h"
#include "Engine/ResourceManager.h"
#include "Render2D.h"
#include "EntityRenderer.h"


namespace Engine
{

	class World;
	class ModelEntity;


	class ENGINE_API RenderSystem
	{
	public:
		RenderSystem();

		bool Init(void* instance_handle, void* window_handle);
		void Deinit();

		void RenderFrame(int frame_time);
		void RenderEntities(int frame_time);
		void ReloadShaders();

		GL::Renderer* GetRenderer()
			{ return _renderer; }
		Render2D* GetRender2D()
			{ return _render2D; }
		int GetFrameTime() const
			{ return _frameTime; }
		float GetFrameTimeSec() const
			{ return _frameTime * 0.001f; }

	private:
		static const int MAX_NUM_ENTITIES = 4 * 1024;
		static const int MAX_NUM_MESHES = 16 * 1024;
		static const int MAX_NUM_TRANSP_MESHES = 8 * 1024;

		void Clear();

		GL::Renderer* _renderer;
		Render2D* _render2D;
		EntityRenderer* _entityRenderer;
		ModelEntity** _entityBuf;
		EntityRenderer::MeshRenderData* _meshBuf;
		EntityRenderer::MeshRenderData* _transpMeshBuf;
		int _frameTime;

		int _smapWidth;
		int _smapHeight;
	};

}

#endif // _ENGINE_RENDER_SYSTEM_H_
