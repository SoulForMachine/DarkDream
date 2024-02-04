
#include "BaseLib/Common.h"
#include "BaseLib/Memory.h"
#include "BaseLib/Win32Common.h"
#include "BaseLib/GL/GLRenderer.h"
#include "Font.h"

using namespace Memory;
using namespace GL;


namespace Engine
{

	Font::Font()
	{
		_texCoords = 0;
		_texture = 0;
	}

	Font::~Font()
	{
	//	Destroy();
	}

	bool Font::Create(GL::Renderer* renderer, const tchar* font_name, int size, bool bold, bool italic)
	{
		_renderer = renderer;

		// create memory DC
		HDC sdc = GetDC(0);
		HDC dc = CreateCompatibleDC(sdc);
		if(!dc)
			return false;
		SetMapMode(dc, MM_TEXT);

		// create font
		HFONT font = 0;
		if(font_name == 0 || font_name[0] == _t('\0'))
		{
			font = (HFONT)GetStockObject(DEVICE_DEFAULT_FONT);
		}
		else
		{
			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = -MulDiv(size, GetDeviceCaps(dc, LOGPIXELSY), 72);
			lf.lfWeight = bold? FW_BOLD: FW_REGULAR;
			lf.lfItalic = italic;
			lf.lfCharSet = ANSI_CHARSET;
			lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			lf.lfQuality = ANTIALIASED_QUALITY;
			lf.lfPitchAndFamily = DEFAULT_PITCH;
			tstrncpy(lf.lfFaceName, font_name, LF_FACESIZE);
			lf.lfFaceName[LF_FACESIZE - 1] = _t('\0'); // just to be safe
			font = CreateFontIndirect(&lf);
		}

		if(!font)
		{
			DeleteDC(dc);
			return false;
		}

		HGDIOBJ old_font = SelectObject(dc, font);

		// calculate needed size of the bitmap
		TEXTMETRIC tm;
		GetTextMetrics(dc, &tm);
		_fontHeight = tm.tmHeight;
		const int CHAR_COUNT = END_CHAR - START_CHAR + 1;
		const int CHAR_SLACK = 2;
		const int CHAR_HALF_SLACK = CHAR_SLACK / 2;
		_widths = NewArray<int>(mainPool, CHAR_COUNT);
		GetCharWidth32(dc, START_CHAR, END_CHAR, _widths);

		int total_width = CHAR_COUNT * CHAR_SLACK;
		for(int i = 0; i < CHAR_COUNT; ++i)
			total_width += _widths[i];

		int tex_width = 128;
		int tex_height = 128;
		int dim_toggle = 0;
		while((total_width / tex_width) * (_fontHeight + CHAR_SLACK) > tex_height)
		{
			(dim_toggle? tex_height: tex_width) <<= 1;
			dim_toggle = (dim_toggle + 1) & 1;

			int max_texture_size = _renderer->GetInfo().maxTextureSize;
			if(tex_width > max_texture_size)
				tex_width = max_texture_size;
			if(tex_height > max_texture_size)
				tex_height = max_texture_size;

			if(tex_width == max_texture_size && tex_height == max_texture_size)
				break;
		}

		// create bitmap
		HBITMAP bitmap = CreateCompatibleBitmap(sdc, tex_width, tex_height);

		if(!bitmap)
		{
			Memory::Delete(_widths);
			DeleteDC(dc);
			DeleteObject(font);
			return false;
		}

		HGDIOBJ old_bmp = SelectObject(dc, bitmap);

		// clear bitmap to black
		RECT rect = { 0, 0, tex_width, tex_height};
		FillRect(dc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

		// white text
		SetBkMode(dc, TRANSPARENT);
		SetTextColor(dc, RGB(255, 255, 255));

		// render font into bitmap
		tchar c[2] = { (tchar)START_CHAR, _t('\0') };
		int x = 0, y = 0;
		_texCoords = NewArray<TexCoords>(mainPool, CHAR_COUNT);
		float xscale = 1.0f / tex_width;
		float yscale = 1.0f / tex_height;

		for(int i = 0; i < CHAR_COUNT; ++i)
		{
			int next_x = x + _widths[i] + CHAR_SLACK;
			if(next_x > tex_width)
			{
				x = 0;
				y += _fontHeight + CHAR_SLACK;
				next_x = _widths[i] + CHAR_SLACK;
			}

			if(y + _fontHeight + CHAR_SLACK > tex_height)
				break;

			// calculate texture coordinates for this character
			_texCoords[i].u1 = float(x + CHAR_HALF_SLACK) * xscale;
			_texCoords[i].v1 = float(y + CHAR_HALF_SLACK) * yscale;
			_texCoords[i].u2 = float(x + CHAR_HALF_SLACK + _widths[i]) * xscale;
			_texCoords[i].v2 = float(y + CHAR_HALF_SLACK + _fontHeight) * yscale;

			// draw character
			TextOut(dc, x + CHAR_HALF_SLACK, y + CHAR_HALF_SLACK, c, 1);

			x = next_x;
			++c[0];
		}

		// get the pixels and the create texture

		SelectObject(dc, old_bmp);
		SelectObject(dc, old_font);

		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		GetDIBits(dc, bitmap, 0, tex_height, 0, &bmi, DIB_RGB_COLORS);
		int pixel_bytes = bmi.bmiHeader.biBitCount / 8;
		size_t image_size = RoundUp(bmi.bmiHeader.biWidth * pixel_bytes, 4) * bmi.bmiHeader.biHeight;
		ubyte* pixels = NewArray<ubyte>(tempPool, image_size);
		GetBitmapBits(bitmap, static_cast<LONG>(image_size), pixels);

		DeleteObject(font);
		DeleteObject(bitmap);
		DeleteDC(dc);

		ImageFormat format = (pixel_bytes == 4)? IMAGE_FORMAT_BGRA: IMAGE_FORMAT_BGR;
		_texture = _renderer->CreateTexture2D();
		bool result = (_texture != 0);
		result = result && _texture->TexImage(0, PIXEL_FORMAT_R8, tex_width, tex_height, format, TYPE_UNSIGNED_BYTE, 0, pixels);

		Memory::Delete(pixels);

		if(!result)
		{
			Memory::Delete(_texCoords);
			Memory::Delete(_widths);
		}

		return result;
	}

	void Font::Destroy()
	{
		_renderer->DestroyTexture(_texture);
		Memory::Delete(_texCoords);
		Memory::Delete(_widths);
	}

	/*
		gets text width in pixels for this font
	*/
	int Font::GetTextWidth(const char* text)
	{
		int width = 0;
		while(*text)
			width += GetCharWidth(*text++);
		return width;
	}

}
