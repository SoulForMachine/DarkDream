
#include "EngineInternal.h"
#include "RenderSystem.h"
#include "World.h"
#include "BgLayer.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	// ========== BgLayer ==========

	BgLayer::BgLayer()
		: _sprites(mapPool)
	{
	}

	BgLayer::~BgLayer()
	{
	}

	bool BgLayer::Init()
	{
		return true;
	}

	void BgLayer::Deinit()
	{
		for(List<Sprite>::Iterator it = _sprites.Begin(); it != _sprites.End(); ++it)
		{
			engineAPI.textureManager->ReleaseTexture(it->texture);
		}
	}

	void BgLayer::SetParameters(float screen_width, float scroll_factor, float cam_dist)
	{
		int patch_count = engineAPI.world->GetTerrain().GetPatchCount();
		_width = scroll_factor * screen_width * patch_count + screen_width;
		_screenWidth = screen_width;
		_scrollFactor = scroll_factor;
		_cameraDistance = cam_dist;
	}

	BgLayer::Sprite& BgLayer::AddSprite(const Sprite& sprite)
	{
		_sprites.PushBack(sprite);
		return _sprites.GetTail();
	}

	void BgLayer::RemoveSprite(Sprite& sprite)
	{
		engineAPI.textureManager->ReleaseTexture(sprite.texture);
		_sprites.Remove(sprite);
	}

	void BgLayer::MoveToFront(Sprite& sprite)
	{
		List<Sprite>::Iterator it = _sprites.Find(sprite);
		if(it != _sprites.End())
		{
			List<Sprite>::Iterator next_it = it;
			++next_it;
			if(next_it != _sprites.End())
			{
				++next_it;
				List<Sprite>::NodeType* node = _sprites.Unlink(it);
				_sprites.Link(next_it, node);
			}
		}
	}

	void BgLayer::MoveToBack(Sprite& sprite)
	{
		List<Sprite>::Iterator it = _sprites.Find(sprite);
		if(it != _sprites.End())
		{
			List<Sprite>::Iterator prev_it = it;
			--prev_it;
			if(prev_it != _sprites.End())
			{
				List<Sprite>::NodeType* node = _sprites.Unlink(it);
				_sprites.Link(prev_it, node);
			}
		}
	}

	/*
		Get layer-space point from screen-space point
	*/
	bool BgLayer::PickLayerPoint(int screen_x, int screen_y, vec2f& point)
	{
		mat4f inv_view_proj = engineAPI.world->GetCamera().GetViewProjectionTransform();
		inv_view_proj.inverse();
		float viewport[4];
		engineAPI.renderSystem->GetRenderer()->GetViewport(viewport);
		vec3f ray_pt;
		unproject(ray_pt, screen_x, screen_y, inv_view_proj, viewport);
		const vec3f& cam_pos = engineAPI.world->GetCamera().GetPosition();
		vec3f ray_dir = ray_pt - cam_pos;
		ray_dir.normalize();

		vec3f pt;
		vec4f plane(vec3f::z_axis, - (cam_pos.z - _cameraDistance));
		if(intersect_ray_plane(pt, ray_pt, ray_dir, plane))
		{
			point.x = WorldXToLayerX(pt.x);
			point.y = pt.y;
			return true;
		}
		else
		{
			return false;
		}
	}

	BgLayer::Sprite* BgLayer::PickSprite(int screen_x, int screen_y, vec2f& point)
	{
		if(PickLayerPoint(screen_x, screen_y, point))
		{
			List<Sprite>::Iterator it = _sprites.End();
			--it;
			for( ; it != _sprites.End(); --it)
			{
				vec4f rect(it->rect.x1, it->rect.y1, it->rect.x2, it->rect.y2);
				if(point_in_rectangle_2d(point, rect))
					return &(*it);
			}
		}

		return 0;
	}

	float BgLayer::GetXPos() const
	{
		float cam_x = engineAPI.world->GetCamera().GetPosition().x;
		return (cam_x - _screenWidth * 0.5f) - (cam_x / Terrain::PATCH_WIDTH * _scrollFactor * _screenWidth);
	}

	float BgLayer::WorldXToLayerX(float x)
	{
		return x - GetXPos();
	}

	float BgLayer::LayerXToWorldX(float x)
	{
		return x + GetXPos();
	}



	// ========== BgLayerManager ==========

	BgLayerManager::BgLayerManager()
	{
	}

	BgLayerManager::~BgLayerManager()
	{
	}

	bool BgLayerManager::Init()
	{
		for(int i = 0; i < 4; ++i)
			_layers[i].Init();
		RecalculateSizes();
		return true;
	}

	void BgLayerManager::Deinit()
	{
		for(int i = 0; i < 4; ++i)
			_layers[i].Deinit();
	}

	void BgLayerManager::RecalculateSizes()
	{
		float ldist = 512.0f;
		Camera& cam = engineAPI.world->GetCamera();
		float fov = cam.GetFOV();
		float aspect = cam.GetAspectRatio();

		float scroll_factors[] = { 0.0f, 0.2f, 0.4f, 0.8f };
		for(int i = 0; i < 4; ++i)
		{
			float scr_width = tan(fov * 0.5f) * ldist * 2.0f * aspect; // width of a layer covering one screen
			_layers[i].SetParameters(scr_width, scroll_factors[i], ldist);
			ldist -= 1.0f;
		}
	}

	int BgLayerManager::GetVisibleSprites(const BgLayer::Sprite** sprites_buf, int max_count)
	{
		int patch_count = engineAPI.world->GetTerrain().GetPatchCount();
		Camera& cam = engineAPI.world->GetCamera();
		float scrl = cam.GetPosition().x / Terrain::PATCH_WIDTH;
		clamp(scrl, 0.0f, (float)patch_count);
		int count = 0;

		for(int i = 0; i < 4; ++i)
		{
			float scrl_factor = _layers[i].GetScrollFactor();
			float scr_width = _layers[i].GetScreenWidth();
			const List<BgLayer::Sprite>& sprites = _layers[i].GetSprites();
			for(List<BgLayer::Sprite>::ConstIterator it = sprites.Begin(); it != sprites.End(); ++it)
			{
				float left = scrl * scrl_factor * scr_width;
				float right = left + scr_width;
				if(	(it->rect.x1 >= left && it->rect.x1 <= right) ||
					(it->rect.x2 >= left && it->rect.x2 <= right) )
				{
					sprites_buf[count++] = &(*it);
					if(count == max_count)
						return count;
				}
			}
		}

		return count;
	}

	BgLayer::Sprite* BgLayerManager::PickSprite(int screen_x, int screen_y, vec2f& point)
	{
		for(int i = 3; i >= 0; --i)
		{
			BgLayer::Sprite* sprite = _layers[i].PickSprite(screen_x, screen_y, point);
			if(sprite)
				return sprite;
		}

		return 0;
	}

}
