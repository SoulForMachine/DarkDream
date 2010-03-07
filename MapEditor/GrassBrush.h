
#pragma once

#include "EM_PaintGrass.h"


namespace GL
{
	class Renderer;
	class Buffer;
	class VertexFormat;
}


namespace MapEditor
{

	ref class GrassBrush
	{
	public:
		GrassBrush(EM_PaintGrass::Parameters^ params);

		bool Init();
		void Deinit();
		void Draw();
		void SetParameters(EM_PaintGrass::Parameters^ params)
			{ _parameters = params; }

	private:
		bool UpdateVertices();

		EM_PaintGrass::Parameters^ _parameters;

		GL::Renderer* _renderer;
		GL::Buffer* _vertBuf;
		GL::VertexFormat* _vertFmt;
		Engine::VertexASMProgResPtr& _vertpBrush;
		Engine::FragmentASMProgResPtr& _fragpBrush;
	};

}
