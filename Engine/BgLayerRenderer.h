
#ifndef _ENGINE_BG_LAYER_RENDERER_H_
#define _ENGINE_BG_LAYER_RENDERER_H_

#include "Engine/Common.h"
#include "BgLayer.h"


namespace GL
{
	class Renderer;
	class Buffer;
	class VertexFormat;
}

namespace Engine
{

	class ENGINE_API BgLayerRenderer
	{
	public:
		BgLayerRenderer();
		~BgLayerRenderer();

		bool Init();
		void Deinit();

		void Render(const BgLayer::Sprite** sprites, int count);

	private:
		void RenderSpriteBatch(const BgLayer::Sprite** sprites, int count);
		void Clear();

		GL::Renderer* _renderer;
		GL::Buffer* _spriteVertexBuf;
		GL::Buffer* _spriteIndexBuf;
		GL::VertexFormat* _spriteVertFmt;
		GL::SamplerState* _spriteSampler;
		GL::SamplerState* _spriteSamplerTile;
		VertexASMProgResPtr _spriteVertProg;
		FragmentASMProgResPtr _spriteFragProg;
	};

}


#endif // _ENGINE_BG_LAYER_RENDERER_H_
