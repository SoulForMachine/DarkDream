
#ifndef _ENGINE_TERRAIN_H_
#define _ENGINE_TERRAIN_H_

#include "BaseLib/List.h"
#include "BaseLib/Math/math3d.h"
#include "BaseLib/AABBox.h"
#include "Engine/Common.h"


namespace GL
{
	class Buffer;
	class Renderer;
}


namespace Engine
{

	class ENGINE_API Terrain
	{
	public:
		struct TerrainPatch
		{
			GL::Buffer* vertBuf;
			float* elevation;
			AABBox boundBox;
		};

		struct PatchVertex
		{
			math3d::vec4f position;
			math3d::vec3f normal;
		};

		static const int PATCH_WIDTH = 128;
		static const int PATCH_HEIGHT = 32;
		static const int MAX_PATCHES = 32;

		Terrain();

		bool Init();
		void Deinit();
		bool AddPatch(float* heights = 0);
		void RemovePatch(int index);
		const GL::Buffer* GetPatchIndexBuffer() const
			{ return _patchIndexBuf; }
		int GetPatchIndexCount() const
			{ return _patchIndexCount; }
		const TerrainPatch* GetPatches()
			{ return _patches; }
		int GetPatchCount() const
			{ return _patchCount; }
		bool PickTerrainPoint(int screen_x, int screen_y, math3d::vec3f& point);
		bool ElevationFromPoint(const math3d::vec2f& point, float& elevation);
		void SetElevation(int start_x, int start_y, int end_x, int end_y, const float* elevation);
		void OffsetElevation(int start_x, int start_y, int end_x, int end_y, const float* offsets);
		void GetElevation(int start_x, int start_y, int end_x, int end_y, float* elevation);

		math3d::vec2i _dbgCells[4096];
		int _dbgCellCount;
		math3d::vec3f _dbgLinePoints[2];

	private:
		bool IntersectPatch(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, math3d::vec3f& point);
		bool IntersectPatchCell(const math3d::vec3f& ray_pt, const math3d::vec3f& ray_dir, const TerrainPatch& patch, int cell_x, int cell_y, math3d::vec3f& point);
		void UpdateNormals(PatchVertex* vertices, int start_x, int start_y, int end_x, int end_y);

		GL::Renderer* _renderer;
		GL::Buffer* _patchIndexBuf;
		TerrainPatch _patches[MAX_PATCHES];
		int _patchCount;
		int _patchIndexCount;
	};

}


#endif // _ENGINE_TERRAIN_H_
