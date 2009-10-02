
#include "BaseLib/Bounds.h"
#include "EngineInternal.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "World.h"
#include "DebugRenderer.h"

using namespace math3d;


namespace Engine
{

	DebugRenderer::DebugRenderer()
	{
		_vpSimple = 0;
		_fpConstColor = 0;
		_vertFmtLines = 0;
		_vertBufBBox = 0;
		_indBufBBox = 0;
		_renderer = 0;
	}

	bool DebugRenderer::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();

		_vpSimple = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fpConstColor = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		return true;
	}

	void DebugRenderer::Deinit()
	{
		engineAPI.asmProgManager->ReleaseASMProgram(_vpSimple);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpConstColor);
		DestroyBBoxRes();
	}

	void DebugRenderer::RenderBoundingBox(const AABBox& bbox)
	{
		vec3f points[8];
		bbox.GetPoints(points);
		RenderBoundingBox(points);
	}

	void DebugRenderer::RenderBoundingBox(const OBBox& bbox)
	{
		RenderBoundingBox(bbox.points);
	}

	void DebugRenderer::RenderBoundingBox(const vec3f points[8])
	{
		CreateBBoxRes();

		vec3f* vertices = (vec3f*)_vertBufBBox->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
		if(vertices)
		{
			memcpy(vertices, points, 8 * sizeof(vec3f));

			if(_vertBufBBox->UnmapBuffer())
			{
				_renderer->ActiveVertexFormat(_vertFmtLines);
				_renderer->VertexSource(0, _vertBufBBox, sizeof(vec3f), 0);
				_renderer->IndexSource(_indBufBBox, GL::TYPE_UNSIGNED_SHORT);
				_renderer->ActiveVertexASMProgram(_vpSimple->GetASMProgram());
				_renderer->ActiveFragmentASMProgram(_fpConstColor->GetASMProgram());

				const Camera& camera = engineAPI.world->GetCamera();
				_vpSimple->GetASMProgram()->LocalMatrix4x4(0, camera.GetViewProjectionTransform());
				_fpConstColor->GetASMProgram()->LocalParameter(0, vec4f(1.0f, 0.0f, 0.0f, 1.0f));

				_renderer->DrawIndexed(GL::PRIM_LINES, 0, 24);
			}
		}
	}

	void DebugRenderer::CreateBBoxRes()
	{
		if(!_vertBufBBox)
		{
			_vertBufBBox = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, 8 * sizeof(vec3f), 0, GL::USAGE_DYNAMIC_DRAW);
		}

		if(!_indBufBBox)
		{
			ushort indices[] = 
			{
				0, 1,	1, 5,	5, 4,	4, 0,
				2, 3,	3, 7,	7, 6,	6, 2,
				0, 2,	1, 3,	4, 6,	5, 7
			};
			_indBufBBox = _renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, 24 * sizeof(ushort), indices, GL::USAGE_STATIC_DRAW);
		}

		if(!_vertFmtLines)
		{
			GL::VertexAttribDesc vfmt[] =
			{
				{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 }
			};
			_vertFmtLines = _renderer->CreateVertexFormat(vfmt, COUNTOF(vfmt));
		}
	}

	void DebugRenderer::DestroyBBoxRes()
	{
		if(_vertBufBBox)
		{
			_renderer->DestroyBuffer(_vertBufBBox);
			_vertBufBBox = 0;
		}

		if(_indBufBBox)
		{
			_renderer->DestroyBuffer(_indBufBBox);
			_indBufBBox = 0;
		}

		if(_vertFmtLines)
		{
			_renderer->DestroyVertexFormat(_vertFmtLines);
			_vertFmtLines = 0;
		}
	}


}
