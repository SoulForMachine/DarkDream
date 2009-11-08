
#ifndef _ENGINE_BG_LAYER_H_
#define _ENGINE_BG_LAYER_H_

#include "BaseLib/Math/math3d.h"
#include "BaseLib/Rect.h"
#include "BaseLib/List.h"
#include "Engine/Common.h"


namespace Engine
{

	class TextureRes;


	class ENGINE_API BgLayer
	{
	public:
		struct Sprite
		{
			enum
			{
				FLAG_TILE_U = 1,
				FLAG_TILE_V = 2,
				FLAG_ADDITIVE_BLEND = 4,
			};

			const TextureRes* texture;
			RectFloat rect; // absolute layer-space coords: [x1, y1] - lower left, [x2, y2] upper right
			math3d::vec2f uvScale;
			int layerIndex;
			uint flags;
		};

		BgLayer();
		~BgLayer();

		bool Init(float scroll_factor);
		void Deinit();

		void SetParameters(float screen_width, float cam_dist);
		Sprite& AddSprite(const Sprite& sprite);
		void RemoveSprite(Sprite& sprite);
		void MoveToFront(Sprite& sprite);
		void MoveToBack(Sprite& sprite);
		bool PickLayerPoint(int screen_x, int screen_y, math3d::vec2f& point);
		Sprite* PickSprite(int screen_x, int screen_y, math3d::vec2f& point);
		List<Sprite>& GetSprites()
			{ return _sprites; }
		int GetTextureCount() const
			{ return _sprites.GetCount(); }
		float GetScrollFactor() const
			{ return _scrollFactor; }
		void SetScrollFactor(float factor);
		float GetWidth() const
			{ return _width; }
		float GetScreenWidth() const
			{ return _screenWidth; }
		float GetCameraDistance() const
			{ return _cameraDistance; }
		float GetXPos() const;	// layer world-space x position
		float WorldXToLayerX(float x);
		float LayerXToWorldX(float x);

	private:
		List<Sprite>::Iterator FindSprite(Sprite* sprite);

		List<Sprite> _sprites;
		float _width;
		float _screenWidth; // width of a visible portion
		float _scrollFactor;
		float _cameraDistance;
	};


	class ENGINE_API BgLayerManager
	{
	public:
		BgLayerManager();
		~BgLayerManager();

		bool Init();
		void Deinit();

		void RecalculateSizes(); // must be called every time camera projection transform changes
		BgLayer& GetLayer(int index)
			{ return _layers[index]; }
		int GetVisibleSprites(const BgLayer::Sprite** sprites_buf, int max_count);
		BgLayer::Sprite* PickSprite(int screen_x, int screen_y, math3d::vec2f& point);

	private:
		BgLayer _layers[4];
	};

}


#endif // _ENGINE_BG_LAYER_H_
