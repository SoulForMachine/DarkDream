
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
		_vertBuf = 0;
		_circleVertBuf = 0;
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
		DestroyResources();
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
		CreateResources();

		vec3f* vertices = (vec3f*)_vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
		if(vertices)
		{
			memcpy(vertices, points, 8 * sizeof(vec3f));

			if(_vertBuf->UnmapBuffer())
			{
				_renderer->ActiveVertexFormat(_vertFmtLines);
				_renderer->VertexSource(0, _vertBuf, sizeof(vec3f), 0);
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

	void DebugRenderer::RenderCircle(const math3d::mat4f& world_mat, float radius, const vec3f& color)
	{
		CreateResources();

		_renderer->ActiveVertexFormat(_vertFmtLines);
		_renderer->VertexSource(0, _circleVertBuf, sizeof(vec3f), 0);
		_renderer->IndexSource(0, GL::TYPE_VOID);
		_renderer->ActiveVertexASMProgram(_vpSimple->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fpConstColor->GetASMProgram());

		const Camera& camera = engineAPI.world->GetCamera();
		mat4f wvp, scale, temp;
		scale.set_scale(radius, radius, radius);
		mul(temp, world_mat, camera.GetViewProjectionTransform());
		mul(wvp, scale, temp);
		_vpSimple->GetASMProgram()->LocalMatrix4x4(0, wvp);
		_fpConstColor->GetASMProgram()->LocalParameter(0, vec4f(color));

		_renderer->Draw(GL::PRIM_LINE_LOOP, 60, 60);
	}

	void DebugRenderer::RenderSphere(const math3d::mat4f& world_mat, float radius, const vec3f& color)
	{
		CreateResources();

		_renderer->ActiveVertexFormat(_vertFmtLines);
		_renderer->VertexSource(0, _circleVertBuf, sizeof(vec3f), 0);
		_renderer->IndexSource(0, GL::TYPE_VOID);
		_renderer->ActiveVertexASMProgram(_vpSimple->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fpConstColor->GetASMProgram());

		const Camera& camera = engineAPI.world->GetCamera();
		mat4f wvp, scale, temp;
		scale.set_scale(radius, radius, radius);
		mul(temp, world_mat, camera.GetViewProjectionTransform());
		mul(wvp, scale, temp);
		_vpSimple->GetASMProgram()->LocalMatrix4x4(0, wvp);
		_fpConstColor->GetASMProgram()->LocalParameter(0, vec4f(color));

		_renderer->Draw(GL::PRIM_LINE_LOOP, 0, 60);
		_renderer->Draw(GL::PRIM_LINE_LOOP, 60, 60);
		_renderer->Draw(GL::PRIM_LINE_LOOP, 120, 60);
	}

	void DebugRenderer::RenderRectangle(const math3d::mat4f& world_mat, float width, float height, const vec3f& color)
	{
		vec3f* vertices = (vec3f*)_vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
		if(vertices)
		{
			vertices[0].set(-width * 0.5f, 0.0f, -height * 0.5f);
			vertices[1].set(width * 0.5f, 0.0f, -height * 0.5f);
			vertices[2].set(width * 0.5f, 0.0f, height * 0.5f);
			vertices[3].set(-width * 0.5f, 0.0f, height * 0.5f);

			if(_vertBuf->UnmapBuffer())
			{
				_renderer->ActiveVertexFormat(_vertFmtLines);
				_renderer->VertexSource(0, _vertBuf, sizeof(vec3f), 0);
				_renderer->IndexSource(0, GL::TYPE_VOID);
				_renderer->ActiveVertexASMProgram(_vpSimple->GetASMProgram());
				_renderer->ActiveFragmentASMProgram(_fpConstColor->GetASMProgram());

				const Camera& camera = engineAPI.world->GetCamera();
				mat4f wvp;
				mul(wvp, world_mat, camera.GetViewProjectionTransform());
				_vpSimple->GetASMProgram()->LocalMatrix4x4(0, wvp);
				_fpConstColor->GetASMProgram()->LocalParameter(0, vec4f(color));

				_renderer->Draw(GL::PRIM_LINE_LOOP, 0, 4);
			}
		}
	}

	void DebugRenderer::RenderLines(const math3d::mat4f& world_mat, const math3d::vec3f* points, int point_count, const vec3f& color)
	{
		if(point_count > MAX_VERTICES)
			return;

		vec3f* vertices = (vec3f*)_vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
		if(vertices)
		{
			memcpy(vertices, points, point_count * sizeof(vec3f));

			if(_vertBuf->UnmapBuffer())
			{
				_renderer->ActiveVertexFormat(_vertFmtLines);
				_renderer->VertexSource(0, _vertBuf, sizeof(vec3f), 0);
				_renderer->IndexSource(0, GL::TYPE_VOID);
				_renderer->ActiveVertexASMProgram(_vpSimple->GetASMProgram());
				_renderer->ActiveFragmentASMProgram(_fpConstColor->GetASMProgram());

				const Camera& camera = engineAPI.world->GetCamera();
				mat4f wvp;
				mul(wvp, world_mat, camera.GetViewProjectionTransform());
				_vpSimple->GetASMProgram()->LocalMatrix4x4(0, wvp);
				_fpConstColor->GetASMProgram()->LocalParameter(0, vec4f(color));

				_renderer->Draw(GL::PRIM_LINES, 0, point_count);
			}
		}
	}

	void DebugRenderer::RenderParticleEmitter(const ParticleSystem::Emitter& emitter)
	{
		_renderer->EnableDepthTest(false);
		vec3f color(1.0f, 1.0f, 1.0f);

		switch(emitter.GetEmitterType())
		{
		case ParticleSystem::Emitter::EMITTER_TYPE_SPHERE:
			RenderSphere(emitter.GetWorldTransform(), emitter.GetCurrentSize() * 0.5f, color);
			break;
		case ParticleSystem::Emitter::EMITTER_TYPE_PLANE:
			RenderRectangle(emitter.GetWorldTransform(), emitter.GetCurrentSize(), emitter.GetCurrentSize(), color);
			break;
		case ParticleSystem::Emitter::EMITTER_TYPE_CIRCLE:
			RenderCircle(emitter.GetWorldTransform(), emitter.GetCurrentSize() * 0.5f, color);
			break;
		case ParticleSystem::Emitter::EMITTER_TYPE_LINE:
			{
				float w = emitter.GetCurrentSize() * 0.5f;
				vec3f line[2] = { vec3f(-w, 0.0f, 0.0f), vec3f(w, 0.0f, 0.0f) };
				RenderLines(emitter.GetWorldTransform(), line, 2, color);
			}
			break;
		}

		vec3f line[2] = { vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, emitter.GetCurrentSize(), 0.0f) };
		RenderLines(emitter.GetWorldTransform(), line, 2, vec3f(1.0f, 1.0f, 0.0f));
		_renderer->EnableDepthTest(true);
	}

	void DebugRenderer::CreateResources()
	{
		if(!_vertBuf)
		{
			_vertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, MAX_VERTICES * sizeof(vec3f), 0, GL::USAGE_DYNAMIC_DRAW);
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

		if(!_circleVertBuf)
		{
			const int seg = 60;
			int count = seg * 3;
			_circleVertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, sizeof(vec3f) * count, 0, GL::USAGE_STATIC_DRAW);
			vec3f* vertices = (vec3f*)_circleVertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
			if(vertices)
			{
				vec3f* p1 = vertices;
				vec3f* p2 = vertices + seg;
				vec3f* p3 = vertices + seg * 2;

				float step = TWO_PI / seg;
				float angle = 0.0f;
				for(int i = 0; i < seg; ++i)
				{
					float ca = cos(angle);
					float sa = sin(angle);
					p1->set(ca, sa, 0.0f);
					p2->set(ca, 0.0f, sa);
					p3->set(0.0f, ca, sa);
					++p1;
					++p2;
					++p3;
					angle += step;
				}
				_circleVertBuf->UnmapBuffer();
			}
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

	void DebugRenderer::DestroyResources()
	{
		if(_vertBuf)
		{
			_renderer->DestroyBuffer(_vertBuf);
			_vertBuf = 0;
		}

		if(_indBufBBox)
		{
			_renderer->DestroyBuffer(_indBufBBox);
			_indBufBBox = 0;
		}

		if(_circleVertBuf)
		{
			_renderer->DestroyBuffer(_circleVertBuf);
			_circleVertBuf = 0;
		}

		if(_vertFmtLines)
		{
			_renderer->DestroyVertexFormat(_vertFmtLines);
			_vertFmtLines = 0;
		}
	}

}
