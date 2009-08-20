
#pragma once

#include "EM_TerrainEdit.h"


namespace MapEditor
{

	ref class TerrainBrush
	{
	public:
		void Draw(Engine::Terrain::TerrainPatch* patches, int patch_count);
		void SetParameters(EM_TerrainEdit::Parameters^ params)
			{ _parameters = params; }

	private:
		EM_TerrainEdit::Parameters^ _parameters;
	};

}
