
#ifndef _TERRAIN_RENDERER_H_
#define _TERRAIN_RENDERER_H_

#include "Engine/Common.h"
#include "Engine/Terrain.h"


namespace GL
{
	class Renderer;
	class VertexFormat;
}

namespace Engine
{

	class ASMProgRes;
	class Camera;


	class ENGINE_API TerrainRenderer
	{
	public:
		TerrainRenderer();

		bool Init();
		void Deinit();
		void RenderTerrainPatch(const Camera& camera, const Terrain* terrain, Terrain::TerrainPatch** patches, int count);

	private:
		GL::Renderer* _renderer;
		const ASMProgRes* _vpTerrain;
		const ASMProgRes* _fpTerrain;
		GL::VertexFormat* _vertFmtTerrain;

		void Clear();
	};

}


#endif // _TERRAIN_RENDERER_H_
