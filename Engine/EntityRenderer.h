
#ifndef _ENGINE_RENDER_ENTITY_H_
#define _ENGINE_RENDER_ENTITY_H_

#include "BaseLib/Math/math3d.h"
#include "BaseLib/GL/GLRenderer.h"


namespace Engine
{

	class Camera;
	struct Mesh;


	class EntityRenderer
	{
	public:
		struct MeshRenderData
		{
			const Mesh* mesh;
			const math3d::mat4f* worldMat;
			const math3d::mat4f* jointMatPalette;
			size_t jointCount;
			float eyeDistSq;
		};

		EntityRenderer();
		bool Init();
		void Deinit();
		void Render(const Camera& camera);
		void ReloadShaders();

	private:
		RenderSystem* _renderSystem;
		GL::Renderer* _renderer;
	};

}

#endif // _ENGINE_RENDER_ENTITY_H_
