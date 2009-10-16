
#include "BaseLib/GL/GLRenderer.h"
#include "EngineInternal.h"
#include "RenderSystem.h"
#include "BgLayerRenderer.h"

using namespace math3d;


namespace Engine
{

	BgLayerRenderer::BgLayerRenderer()
	{
		Clear();
	}

	BgLayerRenderer::~BgLayerRenderer()
	{
	}

	bool BgLayerRenderer::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();

		float vertices[64]; // 16 quads with 4 vertices
		for(int i = 0; i < 64; ++i)
			vertices[i] = (float)i;

		_spriteVertexBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, 64 * sizeof(float), vertices, GL::USAGE_STATIC_DRAW);

		ushort indices[96];
		for(int i = 0; i < 16; ++i)
		{
			int n = i * 6;
			int m = i * 4;
			indices[n] = m;
			indices[n + 1] = m + 1;
			indices[n + 2] = m + 2;
			indices[n + 3] = m;
			indices[n + 4] = m + 2;
			indices[n + 5] = m + 3;
		}

		_spriteIndexBuf = _renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, 96 * sizeof(float), indices, GL::USAGE_STATIC_DRAW);

		GL::VertexAttribDesc vert_fmt[] =
		{
			{ 0, 0, 1, GL::TYPE_FLOAT, false, false, 0 },
		};
		_spriteVertFmt = _renderer->CreateVertexFormat(vert_fmt, COUNTOF(vert_fmt));

		_spriteVertProg = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Sprite.vp"), true);
		_spriteFragProg = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Sprite.fp"), true);

		return true;
	}

	void BgLayerRenderer::Deinit()
	{
		_renderer->DestroyBuffer(_spriteVertexBuf);
		_renderer->DestroyBuffer(_spriteIndexBuf);
		_renderer->DestroyVertexFormat(_spriteVertFmt);
		engineAPI.asmProgManager->ReleaseASMProgram(_spriteVertProg);
		engineAPI.asmProgManager->ReleaseASMProgram(_spriteFragProg);

		Clear();
	}

	void BgLayerRenderer::Render(const BgLayer::Sprite** sprites, int count)
	{
		if(!sprites || count < 1)
			return;

		_renderer->VertexSource(0, _spriteVertexBuf, sizeof(float), 0);
		_renderer->IndexSource(_spriteIndexBuf, GL::TYPE_UNSIGNED_SHORT);
		_renderer->ActiveVertexFormat(_spriteVertFmt);
		_renderer->ActiveVertexASMProgram(_spriteVertProg->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_spriteFragProg->GetASMProgram());

		// first sprite doesn't need blending
		int start = 0;
		if(sprites[0]->flags & BgLayer::Sprite::FLAG_TILED)
		{
			RenderSprites(&sprites[0], 1);
			start = 1;
		}

		_renderer->EnableBlending(true);
		_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);

		for(int i = start; i < count; ++i)
		{
			start = i;
			TextureRes* last_tex = sprites[i]->texture;

			while(i < count &&
				sprites[i]->texture == last_tex)
			{
				++i;
			}

			RenderSprites(&sprites[start], i - start);
		}

		_renderer->EnableBlending(false);
	}

	void BgLayerRenderer::RenderSpriteBatch(const BgLayer::Sprite** sprites, int count)
	{
	}

	void BgLayerRenderer::Clear()
	{
		_renderer = 0;
		_spriteVertexBuf = 0;
		_spriteIndexBuf = 0;
		_spriteVertFmt = 0;
		_spriteVertProg = 0;
		_spriteFragProg = 0;
	}

}
