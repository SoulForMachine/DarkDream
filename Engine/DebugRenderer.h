
#ifndef _ENGINE_DEBUG_RENDERER_H_
#define _ENGINE_DEBUG_RENDERER_H_

#include "BaseLib/Math/math3d.h"
#include "Engine/Common.h"
#include "Engine/ParticleSystem.h"


class AABBox;
class OBBox;

namespace GL
{
	class Renderer;
	class Buffer;
	class VertexFormat;
}


namespace Engine
{

	class ASMProgRes;


	class ENGINE_API DebugRenderer
	{
	public:
		DebugRenderer();

		bool Init();
		void Deinit();

		void RenderBoundingBox(const AABBox& bbox);
		void RenderBoundingBox(const OBBox& bbox);
		void RenderCircle(const math3d::mat4f& world_mat, float radius, const math3d::vec3f& color);
		void RenderSphere(const math3d::mat4f& world_mat, float radius, const math3d::vec3f& color);
		void RenderRectangle(const math3d::mat4f& world_mat, float width, float height, const math3d::vec3f& color);
		void RenderLines(const math3d::mat4f& world_mat, const math3d::vec3f* points, int point_count, const math3d::vec3f& color);
		void RenderParticleEmitter(const ParticleSystem::Emitter& emitter);

	private:
		void CreateResources();
		void DestroyResources();
		void RenderBoundingBox(const math3d::vec3f points[8]);

		static const int MAX_VERTICES = 1024;

		GL::Renderer* _renderer;
		const ASMProgRes* _vpSimple;
		const ASMProgRes* _fpConstColor;
		GL::VertexFormat* _vertFmtLines;
		GL::Buffer* _vertBuf;
		GL::Buffer* _circleVertBuf;
		GL::Buffer* _indBufBBox;
	};

}


#endif // _ENGINE_DEBUG_RENDERER_H_
