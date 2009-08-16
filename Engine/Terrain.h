
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
			float xPos;
			AABBox boundBox;
		};

		struct PatchVertex
		{
			math3d::vec4f position;
			math3d::vec3f normal;
		};

		typedef List<TerrainPatch> TerrainPatchList;

		static const int PATCH_WIDTH = 128;
		static const int PATCH_HEIGHT = 32;
		static const int MAX_PATCHES = 32;

		Terrain();

		bool Init();
		void Deinit();
		bool AddPatch(float* heights = 0);
		const GL::Buffer* GetPatchIndexBuffer() const
			{ return _patchIndexBuf; }
		int GetPatchIndexCount() const
			{ return _patchIndexCount; }
		TerrainPatchList& GetPatches()
			{ return _patches; }

	private:
		GL::Renderer* _renderer;
		GL::Buffer* _patchIndexBuf;
		TerrainPatchList _patches;
		int _patchIndexCount;
	};

}


#endif // _ENGINE_TERRAIN_H_
