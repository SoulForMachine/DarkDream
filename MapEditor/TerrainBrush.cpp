
#include "stdafx.h"
#include "TerrainBrush.h"

using namespace math3d;


namespace MapEditor
{

	struct BrushVertex
	{
		vec3f position;
	};


	bool TerrainBrush::Init()
	{
		_renderer = engineAPI->renderSystem->GetRenderer();

		BrushVertex vertices[] =
		{
			{ vec3f(-1.0f, 0.0f, 0.0f) },
			{ vec3f(1.0f, 0.0f, 0.0f) },
			{ vec3f(0.0f, 0.0f, -1.0f) },
			{ vec3f(0.0f, 0.0f, 1.0f) },
			{ vec3f(0.0f, 0.0f, 0.0f) },
			{ vec3f(0.0f, 0.0f, 0.0f) },
		};
		_vertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, sizeof(BrushVertex) * 6, vertices, GL::USAGE_DYNAMIC_DRAW);
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
		BrushVertex vertices[] =
		{
			engineAPI->world->GetTerrain()._dbgLinePoints[0],
			engineAPI->world->GetTerrain()._dbgLinePoints[1],
		};
		_vertBuf->BufferSubData(4 * sizeof(BrushVertex), 2 * sizeof(BrushVertex), vertices);

		_renderer->ActiveVertexFormat(_vertFmt);
		_renderer->IndexSource(0, GL::TYPE_VOID);
		_renderer->VertexSource(0, _vertBuf, sizeof(BrushVertex), 0);
		_renderer->ActiveVertexASMProgram(_vertpBrush->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fragpBrush->GetASMProgram());

		mat4f world, wvp;
		world.set_translation(_parameters->posX, _parameters->posY, _parameters->posZ);
		mul(wvp, world, engineAPI->world->GetCamera().GetViewProjectionTransform());
		_vertpBrush->GetASMProgram()->LocalMatrix4x4(0, wvp);

		vec4f red(1.0f, 0.0f, 0.0f, 1.0f);
		_fragpBrush->GetASMProgram()->LocalParameter(0, red);

		_renderer->EnableDepthTest(false);

		_renderer->Draw(GL::PRIM_LINES, 0, 4);
		_vertpBrush->GetASMProgram()->LocalMatrix4x4(0, engineAPI->world->GetCamera().GetViewProjectionTransform());
		_renderer->Draw(GL::PRIM_LINES, 4, 2);

		_renderer->EnableDepthTest(true);
	}

}
