
#ifndef _ENGINE_DEBUG_RENDERER_H_
#define _ENGINE_DEBUG_RENDERER_H_

#include "BaseLib/Math/math3d.h"
#include "Engine/Common.h"

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

	private:
		void CreateBBoxRes();
		void DestroyBBoxRes();
		void RenderBoundingBox(const math3d::vec3f points[8]);

		GL::Renderer* _renderer;
		const ASMProgRes* _vpSimple;
		const ASMProgRes* _fpConstColor;
		GL::VertexFormat* _vertFmtLines;
		GL::Buffer* _vertBufBBox;
		GL::Buffer* _indBufBBox;
	};

}


#endif // _ENGINE_DEBUG_RENDERER_H_
