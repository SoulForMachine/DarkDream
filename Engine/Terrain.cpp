
#include "BaseLib/Memory.h"
#include "EngineInternal.h"
#include "Engine/Common.h"
#include "Engine/RenderSystem.h"
#include "Terrain.h"

using namespace Memory;
using namespace math3d;


namespace Engine
{

	Terrain::Terrain()
		: _patches(mapPool)
	{
	}

	bool Terrain::Init()
	{
		_renderer = engineAPI.renderSystem->GetRenderer();

		_patchIndexCount = PATCH_WIDTH * PATCH_HEIGHT * 6;
		_patchIndexBuf = _renderer->CreateBuffer(GL::OBJ_INDEX_BUFFER, _patchIndexCount * sizeof(ushort), 0, GL::USAGE_STATIC_DRAW);
		if(!_patchIndexBuf)
			return false;

		ushort* indices = (ushort*)_patchIndexBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(!indices)
		{
			Deinit();
			return false;
		}

		int i = 0;
		for(int h = 0; h < PATCH_HEIGHT; ++h)
		{
			for(int w = 0; w < PATCH_WIDTH; ++w)
			{
				indices[0] = i + 0;
				indices[1] = i + 1;
				indices[2] = i + PATCH_WIDTH + 1;

				indices[3] = i + 1;
				indices[4] = i + PATCH_WIDTH + 2;
				indices[5] = i + PATCH_WIDTH + 1;

				indices += 6;
				i++;
			}

			i++;
		}

		if(!_patchIndexBuf->UnmapBuffer())
		{
			Deinit();
			return false;
		}

		return true;
	}

	void Terrain::Deinit()
	{
		if(_patchIndexBuf)
		{
			_renderer->DestroyBuffer(_patchIndexBuf);
			_patchIndexBuf = 0;
		}

		for(TerrainPatchList::Iterator it = _patches.Begin(); it != _patches.End(); ++it)
		{
			_renderer->DestroyBuffer(it->vertBuf);
		}
		_patches.Clear();

		_renderer = 0;
	}

	bool Terrain::AddPatch(float* heights)
	{
		if(_patches.GetCount() == MAX_PATCHES)
			return false;

		TerrainPatch patch;
		patch.vertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, (PATCH_WIDTH + 1) * (PATCH_HEIGHT + 1) * sizeof(PatchVertex), 0, GL::USAGE_STATIC_DRAW);
		if(!patch.vertBuf)
			return false;

		PatchVertex* vertices = (PatchVertex*)patch.vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(!vertices)
		{
			_renderer->DestroyBuffer(patch.vertBuf);
			return false;
		}

		for(int h = 0; h < PATCH_HEIGHT + 1; ++h)
		{
			for(int w = 0; w < PATCH_WIDTH + 1; ++w)
			{
				vertices->position.x = (float)w;
				vertices->position.y = heights? heights[h * (PATCH_WIDTH + 1) + w]: 0.0f;
				vertices->position.z = (float)h;
				vertices->position.w = 1.0f;

				vertices->normal = vec3f::y_axis;

				++vertices;
			}
		}

		if(!patch.vertBuf->UnmapBuffer())
		{
			_renderer->DestroyBuffer(patch.vertBuf);
			return false;
		}

		patch.xPos = (float)_patches.GetCount() * PATCH_WIDTH;
		_patches.PushBack(patch);

		return true;
	}

}
