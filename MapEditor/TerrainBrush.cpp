
#include "stdafx.h"
#include "TerrainBrush.h"

using namespace math3d;
using namespace Engine;

#define CIRCLE_VERTEX_COUNT		64


namespace MapEditor
{

	struct BrushVertex
	{
		vec3f position;
	};


	bool TerrainBrush::Init()
	{
		_renderer = engineAPI->renderSystem->GetRenderer();

		// allocate vertex memory for:
		// - 2 lines that represent brush center
		// - 2 circles with 64 vertices each
		// - for ramp brush, 1 circle with 64, 2 lines, and a quad with 6 vertices
		_vertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, sizeof(BrushVertex) * (4 + CIRCLE_VERTEX_COUNT * 3 + 4 + 6), 0, GL::USAGE_DYNAMIC_DRAW);
		if(!_vertBuf)
			return false;

		GL::VertexAttribDesc vert_fmt[] =
		{
			{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 }
		};
		_vertFmt = _renderer->CreateVertexFormat(vert_fmt, COUNTOF(vert_fmt));

		_vertpBrush = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fragpBrush = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		return true;
	}

	void TerrainBrush::Deinit()
	{
		_renderer->DestroyBuffer(_vertBuf);
		_renderer->DestroyVertexFormat(_vertFmt);
		engineAPI->asmProgManager->ReleaseASMProgram(_vertpBrush);
		engineAPI->asmProgManager->ReleaseASMProgram(_fragpBrush);
	}

	void TerrainBrush::Draw()
	{
		// check if brush center is on the terrain
		const Terrain::TerrainPatch* patches = engineAPI->world->GetTerrain().GetPatches();
		int patch_count = engineAPI->world->GetTerrain().GetPatchCount();
		if(	_parameters->posX < 0.0f || _parameters->posX > patch_count * Terrain::PATCH_WIDTH ||
			_parameters->posZ < 0.0f || _parameters->posZ > Terrain::PATCH_HEIGHT)
		{
			return;
		}

		if(UpdateVertices())
		{
			_renderer->ActiveVertexFormat(_vertFmt);
			_renderer->IndexSource(0, GL::TYPE_VOID);
			_renderer->VertexSource(0, _vertBuf, sizeof(BrushVertex), 0);
			_renderer->ActiveVertexASMProgram(_vertpBrush->GetASMProgram());
			_renderer->ActiveFragmentASMProgram(_fragpBrush->GetASMProgram());

			_vertpBrush->GetASMProgram()->LocalMatrix4x4(0, engineAPI->world->GetCamera().GetViewProjectionTransform());

			vec4f red(1.0f, 0.0f, 0.0f, 1.0f);
			vec4f orange(1.0f, 0.5f, 0.0f, 1.0f);
			_fragpBrush->GetASMProgram()->LocalParameter(0, red);

			_renderer->EnableDepthTest(false);
			_renderer->Draw(GL::PRIM_LINES, 0, 4);
			_renderer->EnableDepthTest(true);
			if(_parameters->hardness < 1.0f)
				_renderer->Draw(GL::PRIM_LINE_LOOP, 4, CIRCLE_VERTEX_COUNT);
			_fragpBrush->GetASMProgram()->LocalParameter(0, orange);
			_renderer->Draw(GL::PRIM_LINE_LOOP, 4 + CIRCLE_VERTEX_COUNT, CIRCLE_VERTEX_COUNT);

			if( _parameters->editType == EM_TerrainEdit::EditType::RAMP &&
				_parameters->executing)
			{
				vec4f green(0.0f, 1.0f, 0.0f, 0.3f);
				_fragpBrush->GetASMProgram()->LocalParameter(0, red);
				_renderer->Draw(GL::PRIM_LINES, 4 + CIRCLE_VERTEX_COUNT * 2, CIRCLE_VERTEX_COUNT);
				_renderer->Draw(GL::PRIM_LINE_LOOP, 4 + CIRCLE_VERTEX_COUNT * 2 + 4, CIRCLE_VERTEX_COUNT);
				_fragpBrush->GetASMProgram()->LocalParameter(0, green);
				_renderer->EnableBlending(true);
				_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
				_renderer->Draw(GL::PRIM_TRIANGLES, 4 + CIRCLE_VERTEX_COUNT * 3 + 4, 6);
				_renderer->EnableBlending(false);
			}
		}
	}

	bool TerrainBrush::UpdateVertices()
	{
		vec3f center(_parameters->posX, _parameters->posY, _parameters->posZ); // brush center

		BrushVertex* vertices = (BrushVertex*)_vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
		if(!vertices)
			return false;

		const float VERT_OFFSET = 0.1f;

		// center cross
		vertices[0].position.set(center.x - 1.0f, center.y + VERT_OFFSET, center.z);
		vertices[1].position.set(center.x + 1.0f, center.y + VERT_OFFSET, center.z);
		vertices[2].position.set(center.x, center.y + VERT_OFFSET, center.z - 1.0f);
		vertices[3].position.set(center.x, center.y + VERT_OFFSET, center.z + 1.0f);
		vertices += 4;

		float d = TWO_PI / CIRCLE_VERTEX_COUNT;
		float radius[] = { _parameters->radius, _parameters->radius * _parameters->hardness };

		for(int i = 0; i < 2; ++i)
		{
			float angle = 0.0f;
			for(int j = 0; j < CIRCLE_VERTEX_COUNT; ++j)
			{
				float elev;
				vec2f pt(cos(angle) * radius[i] + center.x, sin(angle) * radius[i] + center.z);
				if(!engineAPI->world->GetTerrain().ElevationFromPoint(pt, elev))
					elev = 0.0f;
				vertices->position.set(pt.x, elev + VERT_OFFSET, pt.y);
				vertices++;
				angle += d;
			}
		}

		// ramp
		if(	_parameters->editType == EM_TerrainEdit::EditType::RAMP &&
			_parameters->executing )
		{
			// start point marker
			vec3f start_pt(_parameters->startPosX, _parameters->startPosY, _parameters->startPosZ);
			vertices[0].position.set(start_pt.x - 1.0f, start_pt.y + VERT_OFFSET, start_pt.z);
			vertices[1].position.set(start_pt.x + 1.0f, start_pt.y + VERT_OFFSET, start_pt.z);
			vertices[2].position.set(start_pt.x, start_pt.y + VERT_OFFSET, start_pt.z - 1.0f);
			vertices[3].position.set(start_pt.x, start_pt.y + VERT_OFFSET, start_pt.z + 1.0f);
			vertices += 4;

			float angle = 0.0f;
			const float radius = 2.0f;
			for(int j = 0; j < CIRCLE_VERTEX_COUNT; ++j)
			{
				float elev;
				vec2f pt(cos(angle) * radius + start_pt.x, sin(angle) * radius + start_pt.z);
				if(!engineAPI->world->GetTerrain().ElevationFromPoint(pt, elev))
					elev = 0.0f;
				vertices->position.set(pt.x, elev + VERT_OFFSET, pt.y);
				vertices++;
				angle += d;
			}

			// ramp quad
			vec3f dir;
			cross(dir, vec3f::y_axis, center - start_pt);
			dir.normalize();
			dir *= _parameters->radius;

			vertices[0].position = start_pt + dir;
			vertices[0].position.y += VERT_OFFSET;
			vertices[1].position = start_pt - dir;
			vertices[1].position.y += VERT_OFFSET;
			vertices[2].position = center - dir;
			vertices[2].position.y += VERT_OFFSET;
			vertices[3].position = start_pt + dir;
			vertices[3].position.y += VERT_OFFSET;
			vertices[4].position = center - dir;
			vertices[4].position.y += VERT_OFFSET;
			vertices[5].position = center + dir;
			vertices[5].position.y += VERT_OFFSET;
		}

		return _vertBuf->UnmapBuffer();
	}

}
