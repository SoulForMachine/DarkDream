
#include "BaseLib/GL/GLRenderer.h"
#include "EngineInternal.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "World.h"
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

		vec2f uv[4] = {
			vec2f(0.0f, 1.0f),
			vec2f(1.0f, 1.0f),
			vec2f(1.0f, 0.0f),
			vec2f(0.0f, 0.0f),
		};
		vec4f vertices[64]; // 16 quads with 4 vertices
		for(int i = 0; i < 64; ++i)
			vertices[i].set((float)i, uv[i % 4].x, uv[i % 4].y, 0.0f);

		_spriteVertexBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, 64 * sizeof(vec4f), vertices, GL::USAGE_STATIC_DRAW);

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

		_spriteIndexBuf = _renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, 96 * sizeof(ushort), indices, GL::USAGE_STATIC_DRAW);

		GL::VertexAttribDesc vert_fmt[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 },
		};
		_spriteVertFmt = _renderer->CreateVertexFormat(vert_fmt, COUNTOF(vert_fmt));

		GL::SamplerStateDesc sampl_desc = GL::GLState::defaultSamplerState;
		sampl_desc.minFilter = GL::TEX_FILTER_LINEAR_MIPMAP_LINEAR;
		sampl_desc.magFilter = GL::TEX_FILTER_LINEAR;
		sampl_desc.addressU = GL::TEX_ADDRESS_CLAMP_TO_EDGE;
		sampl_desc.addressV = GL::TEX_ADDRESS_CLAMP_TO_EDGE;
		_spriteSampler = _renderer->CreateSamplerState(sampl_desc);

		sampl_desc.addressU = GL::TEX_ADDRESS_REPEAT;
		sampl_desc.addressV = GL::TEX_ADDRESS_REPEAT;
		_spriteSamplerTile = _renderer->CreateSamplerState(sampl_desc);

		_spriteVertProg = engineAPI.asmProgManager->CreateVertexASMProgram(_t("Programs/Sprite.vp"), true);
		_spriteFragProg = engineAPI.asmProgManager->CreateFragmentASMProgram(_t("Programs/Sprite.fp"), true);

		return true;
	}

	void BgLayerRenderer::Deinit()
	{
		_renderer->DestroyBuffer(_spriteVertexBuf);
		_renderer->DestroyBuffer(_spriteIndexBuf);
		_renderer->DestroyVertexFormat(_spriteVertFmt);
		_renderer->DestroySamplerState(_spriteSampler);
		_renderer->DestroySamplerState(_spriteSamplerTile);
		engineAPI.asmProgManager->ReleaseASMProgram(_spriteVertProg);
		engineAPI.asmProgManager->ReleaseASMProgram(_spriteFragProg);

		Clear();
	}

	void BgLayerRenderer::Render(const BgLayer::Sprite** sprites, int count)
	{
		if(!sprites || count < 1)
			return;

		_renderer->VertexSource(0, _spriteVertexBuf, sizeof(vec4f), 0);
		_renderer->IndexSource(_spriteIndexBuf, GL::TYPE_UNSIGNED_SHORT);
		_renderer->ActiveVertexFormat(_spriteVertFmt);
		_renderer->ActiveVertexASMProgram(_spriteVertProg);
		_renderer->ActiveFragmentASMProgram(_spriteFragProg);

		// first sprite doesn't need blending if it's tiled
		int start = 0;
		if(	(sprites[0]->flags & BgLayer::Sprite::FLAG_TILE_U) &&
			(sprites[0]->flags & BgLayer::Sprite::FLAG_TILE_V) )
		{
			RenderSpriteBatch(&sprites[0], 1);
			start = 1;
		}

		_renderer->EnableBlending(true);
		_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
		_renderer->EnableDepthTest(false);

		int i = start;
		while(i < count)
		{
			Texture2DResPtr last_tex = sprites[i]->texture;
			int last_flags = sprites[i]->flags;

			start = i++;
			while(i < count &&
				sprites[i]->texture == last_tex &&
				sprites[i]->flags == last_flags)
			{
				++i;
			}

			RenderSpriteBatch(&sprites[start], i - start);
		}

		_renderer->EnableBlending(false);
		_renderer->EnableDepthTest(true);
	}

	void BgLayerRenderer::RenderSpriteBatch(const BgLayer::Sprite** sprites, int count)
	{
		_spriteVertProg->LocalMatrix4x4(0, engineAPI.world->GetCamera().GetViewProjectionTransform());
		const vec3f& cam_pos = engineAPI.world->GetCamera().GetPosition();
		BgLayerManager& layer_mgr = engineAPI.world->GetLayerManager();

		for(int i = 0; i < count; ++i)
		{
			BgLayer& layer = layer_mgr.GetLayer(sprites[i]->layerIndex);
			float x1 = layer.LayerXToWorldX(sprites[i]->rect.x1);
			float y1 = sprites[i]->rect.y1;
			float x2 = layer.LayerXToWorldX(sprites[i]->rect.x2);
			float y2 = sprites[i]->rect.y2;
			float z = - layer.GetCameraDistance() + cam_pos.z;

			_spriteVertProg->LocalParameter(4, vec4f(sprites[i]->uvScale, 0.0f, 0.0f));

			_spriteVertProg->LocalParameter(5 + i * 4, vec4f(x1, y1, z, 1.0f));
			_spriteVertProg->LocalParameter(5 + i * 4 + 1, vec4f(x2, y1, z, 1.0f));
			_spriteVertProg->LocalParameter(5 + i * 4 + 2, vec4f(x2, y2, z, 1.0f));
			_spriteVertProg->LocalParameter(5 + i * 4 + 3, vec4f(x1, y2, z, 1.0f));
		}

		_renderer->SetSamplerState(0, _spriteSamplerTile);
		_renderer->SetSamplerTexture(0, sprites[0]->texture.operator const GL::Texture2D*());

		_renderer->DrawIndexed(GL::PRIM_TRIANGLES, 0, count * 6);

		_renderer->SetSamplerTexture(0, 0);
	}

	void BgLayerRenderer::Clear()
	{
		_renderer = 0;
		_spriteVertexBuf = 0;
		_spriteIndexBuf = 0;
		_spriteVertFmt = 0;
		_spriteSampler = 0;
		_spriteSamplerTile = 0;
		_spriteVertProg = VertexASMProgResPtr::null;
		_spriteFragProg = FragmentASMProgResPtr::null;
	}

}
