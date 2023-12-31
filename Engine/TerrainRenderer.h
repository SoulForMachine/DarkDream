
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

	class Camera;


	class ENGINE_API TerrainRenderer
	{
	public:
		TerrainRenderer();

		bool Init();
		void Deinit();

		void RenderTerrainPatches(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count);
		void RenderTerrainPatchGrass(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count);

	private:
		GL::Renderer* _renderer;
		VertexASMProgResPtr _vpTerrain;
		FragmentASMProgResPtr _fpTerrain;
		FragmentASMProgResPtr _fpLambert;
		VertexASMProgResPtr _vpDbgLine;
		FragmentASMProgResPtr _fpDbgLine;
		VertexASMProgResPtr _vpGrass;
		FragmentASMProgResPtr _fpGrass;
		GL::VertexFormat* _vertFmtTerrain;
		GL::VertexFormat* _vertFmtDbgLine;
		GL::VertexFormat* _vertFmtGrass;
		Texture2DResPtr _gradTex;
		GL::SamplerState* _gradSampler;
		GL::SamplerState* _terrainSampler;
		GL::SamplerState* _grassSampler;

		void Clear();
	};


	extern Console::BoolVar g_cvarDrawTerrainNormals;

}


#endif // _TERRAIN_RENDERER_H_
