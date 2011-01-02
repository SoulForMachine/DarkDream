
#ifndef _ENGINE_TERRAIN_H_
#define _ENGINE_TERRAIN_H_

#include "BaseLib/List.h"
#include "BaseLib/Math/math3d.h"
#include "BaseLib/Bounds.h"
#include "BaseLib/HashMap.h"
#include "ResourceManager.h"
#include "Engine/Common.h"


namespace GL
{
	class Buffer;
	class Renderer;
}


namespace Engine
{

	class TextureRes;


	class ENGINE_API Terrain
	{
	public:
		static const int PATCH_WIDTH = 128;
		static const int PATCH_HEIGHT = 64;
		static const int MAX_PATCHES = 32;
		static const int GRASS_SEGMENTS = 2;
		static const int GRASS_TEX_SIZE = 128;

		struct GrassBlade
		{
			float size;
			ushort texIndex;
		};

		struct GrassSegment
		{
			GL::Buffer* grassVertBuf;
			int grassVertCount;
			int grassIndexCount;
		};

		struct TerrainPatch
		{
			GL::Buffer* vertBuf;
			float* elevation;
			AABBox boundBox;
			GrassSegment grassSegments[GRASS_SEGMENTS];
			GrassBlade* grassData;
		};

		struct PatchVertex
		{
			math3d::vec4f position;
			math3d::vec4f normal;
		};

		struct GrassVertex
		{
			math3d::vec4f position;
			math3d::vec2f uv;
		};


		Terrain();

		bool Init();
		void Deinit();
		int AddPatch(int index = -1, float* heights = 0);
		void RemovePatch(int index);
		const GL::Buffer* GetPatchIndexBuffer() const
			{ return _patchIndexBuf; }
		int GetPatchIndexCount() const
			{ return _patchIndexCount; }
		const TerrainPatch* GetPatches()
			{ return _patches; }
		int GetPatchCount() const
			{ return _patchCount; }
		int PickTerrainPoint(int screen_x, int screen_y, math3d::vec3f& point);
		bool ElevationFromPoint(const math3d::vec2f& point, float& elevation);
		void SetElevation(int start_x, int start_y, int end_x, int end_y, const float* elevation);
		void OffsetElevation(int start_x, int start_y, int end_x, int end_y, const float* offsets);
		void GetElevation(int start_x, int start_y, int end_x, int end_y, float* elevation);
		void HighlightPatch(int index)
			{ _hlightPatch = &_patches[index]; }
		const TerrainPatch* GetHighlightPatch() const
			{ return _hlightPatch; }
		void SetTexture(Texture2DResPtr texture);
		Texture2DResPtr GetTexture() const
			{ return _texture; }
		void SetTextureTile(float tile)
			{ _texTile = tile; }
		float GetTexureTile() const
			{ return _texTile; }
		void SetGrassTexture(Texture2DResPtr texture);
		Texture2DResPtr GetGrassTexture() const
			{ return _grassTexture; }
		void SetGrassBlades(int start_x, int start_y, int end_x, int end_y, const GrassBlade* grass_data);
		void GetGrassBlades(int start_x, int start_y, int end_x, int end_y, GrassBlade* grass_data);
		void OptimizeGrassEdit(bool on)
			{ _optimizeGrassEdit = on; }
		const GL::Buffer* GetGrassIndexBuffer() const
			{ return _grassIndexBuf; }

	private:
		bool IntersectPatch(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, math3d::vec3f& point);
		bool IntersectPatchCell(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, int cell_x, int cell_y, math3d::vec3f& point);
		void UpdatePatchNormals(int patch_index, PatchVertex* vertices, int start_x, int start_y, int end_x, int end_y);
		void UpdatePatchGrassGeometry(int patch_index);

		GL::Renderer* _renderer;
		GL::Buffer* _patchIndexBuf;
		GL::Buffer* _grassIndexBuf;
		TerrainPatch _patches[MAX_PATCHES];
		int _patchCount;
		int _patchIndexCount;
		TerrainPatch* _hlightPatch; // used by map editor
		Texture2DResPtr _texture;
		float _texTile;
		Texture2DResPtr _grassTexture;
		bool _optimizeGrassEdit;
	};

}


#endif // _ENGINE_TERRAIN_H_
