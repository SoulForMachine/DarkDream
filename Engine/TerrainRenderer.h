
#ifndef _TERRAIN_RENDERER_H_
#define _TERRAIN_RENDERER_H_

#include "BaseLib/Console.h"
#include "Engine/Common.h"
#include "Engine/Terrain.h"


namespace GL
{
	class Renderer;
	class VertexFormat;
	class SamplerState;
}

namespace Engine
{

	class ASMProgRes;
	class TextureRes;
	class Camera;


	class ENGINE_API TerrainRenderer
	{
	public:
		TerrainRenderer();

		bool Init();
		void Deinit();

		void RenderTerrainPatches(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count);
		void RenderTerrainPatchNormals(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count);

	private:
		GL::Renderer* _renderer;
		const ASMProgRes* _vpTerrain;
		const ASMProgRes* _fpTerrain;
		const ASMProgRes* _fpLambert;
		const ASMProgRes* _vpDbgLine;
		const ASMProgRes* _fpDbgLine;
		GL::VertexFormat* _vertFmtTerrain;
		GL::VertexFormat* _vertFmtDbgLine;
		const TextureRes* _gradTex;
		GL::SamplerState* _gradSampler;
		GL::SamplerState* _terrainSampler;

		void Clear();
	};


	extern Console::BoolVar g_cvarDrawTerrainNormals;

}


#endif // _TERRAIN_RENDERER_H_
