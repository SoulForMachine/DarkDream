
#ifndef _ENGINE_FONT_H_
#define _ENGINE_FONT_H_

#include "Engine/Common.h"


namespace GL
{
	class Texture2D;
	class Renderer;
}

namespace Engine
{

	class ENGINE_API Font
	{
	public:
		struct TexCoords
		{
			float u1, v1; // top-left u & v
			float u2, v2; // bottom-right u & v
		};

		Font();
		~Font();

		bool Create(GL::Renderer* renderer, const tchar* font_name, int size, bool bold = false, bool italic = false);
		void Destroy();
		const GL::Texture2D* GetTexture() const
			{ return _texture; }
		const TexCoords* GetTexCoords() const
			{ return _texCoords; }
		const TexCoords* GetCharTexCoords(char c) const
			{ return &_texCoords[c - START_CHAR]; }
		int GetCharWidth(char c) const
			{ return _widths[c - START_CHAR]; }
		int GetFontHeight() const
			{ return _fontHeight; }
		int GetTextWidth(const char* text);

	private:
		static const int START_CHAR = 32;
		static const int END_CHAR = 255;
		GL::Texture2D* _texture;
		GL::Renderer* _renderer;
		TexCoords* _texCoords;
		int* _widths;
		int _fontHeight;
	};

}

#endif // _ENGINE_FONT_H_
