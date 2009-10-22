
#pragma once

#include "EM_PaintGrass.h"

namespace Engine
{
	class ASMProgRes;
}

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
		GrassBrush(EM_PaintGrass::Parameters^ params)
			{ _parameters = params; }

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
		const Engine::ASMProgRes* _vertpBrush;
		const Engine::ASMProgRes* _fragpBrush;
	};

}
