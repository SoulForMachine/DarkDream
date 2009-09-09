
#ifndef _TERRAIN_RENDERER_H_
#define _TERRAIN_RENDERER_H_

#include "BaseLib/Console.h"
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
		enum RenderStyle
		{
			RENDER_STYLE_GAME,
			RENDER_STYLE_EDITOR,
		};

		TerrainRenderer();

		bool Init();
		void Deinit();
		void SetRenderStyle(RenderStyle style)
			{ _renderStyle = style; }
		RenderStyle GetRenderStyle() const
			{ return _renderStyle; }
		void RenderTerrainPatch(const Camera& camera, const Terrain* terrain, const Terrain::TerrainPatch** patches, int count);
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
		RenderStyle _renderStyle;

		void Clear();
	};


	extern Console::BoolVar g_cvarDrawTerrainNormals;

}


#endif // _TERRAIN_RENDERER_H_
