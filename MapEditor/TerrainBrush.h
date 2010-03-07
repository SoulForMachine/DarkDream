
#pragma once

#include "EM_TerrainEdit.h"


namespace GL
{
	class Renderer;
	class Buffer;
	class VertexFormat;
}


namespace MapEditor
{

	ref class TerrainBrush
	{
	public:
		TerrainBrush(EM_TerrainEdit::Parameters^ params);

		bool Init();
		void Deinit();
		void Draw();
		void SetParameters(EM_TerrainEdit::Parameters^ params)
			{ _parameters = params; }

	private:
		bool UpdateVertices();

		EM_TerrainEdit::Parameters^ _parameters;

		GL::Renderer* _renderer;
		GL::Buffer* _vertBuf;
		GL::VertexFormat* _vertFmt;
		Engine::VertexASMProgResPtr& _vertpBrush;
		Engine::FragmentASMProgResPtr& _fragpBrush;
	};

}
