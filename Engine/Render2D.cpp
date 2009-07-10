
#include <cassert>
#include "BaseLib/FreeStackPool.h"
#include "BaseLib/Console.h"
#include "BaseLib/GL/GLRenderer.h"
#include "Engine/EngineInternal.h"
#include "RenderSystem.h"
#include "Render2D.h"

using namespace Memory;
using namespace GL;
using namespace math3d;


namespace Engine
{


	Render2D::Render2D():
		_textBatchPool(128, mainPool),
		_textDataPool(256, mainPool)
	{
		Clear();
	}

	bool Render2D::Init(RenderSystem* render_system)
	{
		_renderSystem = render_system;
		assert(_renderSystem);
		_renderer = _renderSystem->GetRenderer();
		assert(_renderer);

		// create vertex buffers
		_textVertBuffer = _renderer->CreateBuffer(OBJ_VERTEX_BUFFER, TEXT_VERTEX_BUFFER_SIZE, 0, USAGE_STREAM_DRAW);
		if(!_textVertBuffer)
		{
			Deinit();
			return false;
		}

		_rectVertBuffer = _renderer->CreateBuffer(OBJ_VERTEX_BUFFER, RECT_VERTEX_BUFFER_SIZE, 0, USAGE_STREAM_DRAW);
		if(!_rectVertBuffer)
		{
			Deinit();
			return false;
		}

		// load shaders
		CreateShaders();

		// create font
		if(!_consoleFont.Create(_renderer, _t("Courier New"), 12))
		{
			Deinit();
			return false;
		}

		// create vertex formats
		VertexAttribDesc text_vert_desc[] =
		{
			{ 0, 0, 2, TYPE_FLOAT, false, false, 0 },
			{ 0, 1, 2, TYPE_FLOAT, false, false, 8 },
			{ 0, 2, 4, TYPE_FLOAT, false, false, 16 },
		};
		_textVertFmt = _renderer->CreateVertexFormat(text_vert_desc, COUNTOF(text_vert_desc));
		if(!_textVertFmt)
		{
			Deinit();
			return false;
		}

		VertexAttribDesc rect_vert_desc[] =
		{
			{ 0, 0, 2, TYPE_FLOAT, false, false, 0 },
		};
		_rectVertFmt = _renderer->CreateVertexFormat(rect_vert_desc, COUNTOF(rect_vert_desc));
		if(!_rectVertFmt)
		{
			Deinit();
			return false;
		}

		// create sampler state
		SamplerStateDesc sampler_desc = GLState::defaultSamplerState;
		sampler_desc.addressU = TEX_ADDRESS_CLAMP;
		sampler_desc.addressV = TEX_ADDRESS_CLAMP;
		sampler_desc.addressW = TEX_ADDRESS_CLAMP;
		sampler_desc.minFilter = TEX_FILTER_NEAREST;
		sampler_desc.magFilter = TEX_FILTER_NEAREST;
		_textSamplerState = _renderer->CreateSamplerState(sampler_desc);
		if(!_textSamplerState)
		{
			Deinit();
			return false;
		}

		return true;
	}

	void Render2D::CreateShaders()
	{
		_vpText = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Text.vp"), true);
		_fpText = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Text.fp"), true);
		_vpRect2D = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/Simple2D.vp"), true);
		_fpRect2D = engineAPI.asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);
	}

	void Render2D::DestroyShaders()
	{
		engineAPI.asmProgManager->ReleaseASMProgram(_vpText);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpText);
		engineAPI.asmProgManager->ReleaseASMProgram(_vpRect2D);
		engineAPI.asmProgManager->ReleaseASMProgram(_fpRect2D);
	}

	void Render2D::Deinit()
	{
		DestroyShaders();
		_consoleFont.Destroy();
		_renderer->DestroyBuffer(_textVertBuffer);
		_renderer->DestroyBuffer(_rectVertBuffer);
		_renderer->DestroyVertexFormat(_textVertFmt);
		_renderer->DestroyVertexFormat(_rectVertFmt);
		_renderer->DestroySamplerState(_textSamplerState);
		delete[] _consoleLineText;

		Clear();
	}

	void Render2D::FlushText()
	{
		TextBatch* start_batch = _textBatchList;
		TextBatch* end_batch;

		float viewport[4];
		_renderer->GetViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		_vpText->GetASMProgram()->LocalParameter(0, viewport);

		_renderer->ActiveVertexASMProgram(_vpText->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fpText->GetASMProgram());

		_renderer->ActiveVertexFormat(_textVertFmt);
		_renderer->VertexSource(0, _textVertBuffer, sizeof(TextVertex), 0);

		_renderer->BlendingFunc(BLEND_FUNC_SRC_ALPHA, BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
		_renderer->EnableBlending(true);

		_renderer->SetSamplerState(0, _textSamplerState);

		do
		{
			if(FillVertexBuffer(start_batch, end_batch))
			{
				TextBatch* batch = start_batch;

				while(batch != end_batch)
				{
					_renderer->SetSamplerTexture(0, batch->font->GetTexture());
					_renderer->Draw(PRIM_TRIANGLES, batch->vertexOffset, batch->vertexCount);

					batch = batch->next;
				}
			}

			start_batch = end_batch;
		}
		while(end_batch);

		_renderer->SetSamplerTexture(0, 0);
		_renderer->EnableBlending(false);

		_textBatchPool.Reset();
		_textBatchList = 0;
		_textDataPool.Reset();
	}

	/*
		This function merely batches text draw requests.
		Text is not drawn until FlushText() is called.
	*/
	void Render2D::DrawText(const char* text, int x, int y, const Font& font, const vec4f& color)
	{
		if(!text || *text == '\0')
			return;

		// try to find a batch with same texture
		TextBatch* batch = _textBatchList;
		bool batch_added = false;
		int text_len = strlen(text);
		int num_vertices = text_len * 6;

		if(num_vertices > MAX_NUM_TEXT_VERTICES)
			return;

		while(batch)
		{
			if(batch->font->GetTexture() == font.GetTexture())
			{
				if(batch->vertexCount + num_vertices <= MAX_NUM_TEXT_VERTICES)
				{
					batch_added = true;
					TextData* text_data = _textDataPool.New();

					if(text_data)
					{
						text_data->text = new(tempPool) char[text_len + 1];
						strcpy(text_data->text, text);
						text_data->x = x;
						text_data->y = y;
						text_data->color = color;
						text_data->next = batch->textDataList;
						
						batch->textDataList = text_data;
						batch->vertexCount += num_vertices;
						break;
					}
					else
					{
						// no more empty slots
						return;
					}
				}
			}

			batch = batch->next;
		}

		if(!batch_added)
		{
			// create a new batch
			TextBatch* batch = _textBatchPool.New();

			if(batch)
			{
				TextData* text_data = _textDataPool.New();

				if(text_data)
				{
					text_data->text = new(tempPool) char[text_len + 1];
					strcpy(text_data->text, text);
					text_data->x = x;
					text_data->y = y;
					text_data->color = color;
					text_data->next = 0;
				}
				else
				{
					// no more empty slots
					return;
				}

				batch->font = &font;
				batch->vertexCount = num_vertices;
				batch->vertexOffset = 0;
				batch->textDataList = text_data;
				batch->next = 0;
				_textBatchList = batch;
			}
			else
			{
				// no more empty slots
				return;
			}
		}
	}

	/*
		Doesn't use batching, draws text imediately
	*/
	void Render2D::DrawTextImediate(const char* text, int x, int y, const Font& font, const vec4f& color)
	{
		DrawText(text, x, y, font, color);
		FlushText();
	}

	void Render2D::DrawConsole(int frame_time)
	{
		// draw console background and bottom line
		int x, y, width, height;
		_renderer->GetViewport(x, y, width, height);
		int x1 = 0;
		int x2 = width;
		int y1 = 0;//height / 2;
		int y2 = height;
		DrawColoredRect(x1, y1, x2, y2, vec4f(0.0f, 0.0f, 0.0f, 1.0f));
		DrawColoredRect(x1, y1, x2, y1 + 3, vec4f(0.0f, 0.8f, 0.0f, 1.0f));

		// draw console text

		_renderer->Scissor(x1, y1, x2, y2);
		_renderer->EnableScissorTest(true);
		int font_height = _consoleFont.GetFontHeight();
		int font_width = _consoleFont.GetCharWidth('X'); // console should use fixed width font
		int ypos = y1 + 3 + font_height;
		int text_space = y2 - ypos;
		int max_lines = text_space / font_height + (text_space % font_height? 1: 0);
		int lines_printed = 0;
		const short* line = Console::GetBufferCurrentLine();
		int max_line_size = Console::GetBufferLineSize();
		if(max_line_size > _consoleLineSize)
		{
			_consoleLineSize = max_line_size;
			delete[] _consoleLineText;
			_consoleLineText = new(mainPool) char[max_line_size + 1];
		}

		// draw console input
		DrawText("#", 3, y1 + 3, _consoleFont, Console::GetDefaultTextColor());
		DrawText(Console::GetInputBuffer(), 3 + font_width, y1 + 3, _consoleFont, Console::GetDefaultTextColor());

		// draw console buffer
		while(line && lines_printed < max_lines)
		{
			if(*line & 0xFF)
			{
				const short* src = line;
				int xpos = 3;
				int num_chars = max_line_size;
				while(num_chars && *src)
				{
					char* dest = _consoleLineText;
					Console::Color color = Console::Color((*src >> 8) & 0xFF);
					while(num_chars && *src && (color == ((*src >> 8) & 0xFF)))
					{
						*dest++ = char(*src++ & 0xFF);
						--num_chars;
					}
					*dest = '\0';
					DrawText(_consoleLineText, xpos, ypos, _consoleFont, Console::GetColor(color));
					xpos += font_width * strlen(_consoleLineText); //_consoleFont.GetTextWidth(_consoleLineText);
				}
				ypos += font_height;
				++lines_printed;
			}

			line = Console::GetBufferNextLine(line);
		}

		FlushText();

		// draw cursor
		_consoleCursorBlinkTime += frame_time;
		_consoleCursorBlinkTime %= 1000;
		if(_consoleCursorBlinkTime <= 500)
		{
			_renderer->BlendingFunc(BLEND_FUNC_SRC_ALPHA, BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
			_renderer->EnableBlending(true);

			int cursor_x = 3 + (Console::GetInputPos() + 1) * font_width;
			if(Console::GetInputMode() == Console::INM_INSERT)
				DrawColoredRect(cursor_x, y1 + 3, cursor_x + font_width, y1 + 6, vec4f(1.0f, 1.0f, 1.0f, 1.0f));
			else
				DrawColoredRect(cursor_x, y1 + 3, cursor_x + font_width, y1 + 3 + font_height, vec4f(1.0f, 1.0f, 1.0f, 1.0f));

			_renderer->EnableBlending(false);
		}

		_renderer->Scissor(x, y, width, height);
		_renderer->EnableScissorTest(false);
	}

	void Render2D::DrawColoredRect(int x1, int y1, int x2, int y2, const math3d::vec4f& color)
	{
		_renderer->ActiveVertexASMProgram(_vpRect2D->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fpRect2D->GetASMProgram());
		float viewport[4];
		_renderer->GetViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		_vpRect2D->GetASMProgram()->LocalParameter(0, viewport);
		_fpRect2D->GetASMProgram()->LocalParameter(0, color);

		RectVertex* buffer = (RectVertex*)_rectVertBuffer->MapBuffer(ACCESS_WRITE_ONLY, false);

		if(!buffer)
			return;

		buffer[0].position = vec2f((float)x1, (float)y1);
		buffer[1].position = vec2f((float)x1, (float)y2);
		buffer[2].position = vec2f((float)x2, (float)y1);
		buffer[3].position = vec2f((float)x1, (float)y2);
		buffer[4].position = vec2f((float)x2, (float)y2);
		buffer[5].position = vec2f((float)x2, (float)y1);

		if(!_rectVertBuffer->UnmapBuffer())
			return;

		_renderer->ActiveVertexFormat(_rectVertFmt);
		_renderer->VertexSource(0, _rectVertBuffer, sizeof(RectVertex), 0);

		_renderer->Draw(PRIM_TRIANGLES, 0, NUM_RECT_VERTICES);
	}

	void Render2D::ReloadShaders()
	{
		DestroyShaders();
		CreateShaders();
	}

	bool Render2D::FillVertexBuffer(TextBatch* start_batch, TextBatch*& end_batch)
	{
		TextVertex* vertex_buf = (TextVertex*)_textVertBuffer->MapBuffer(ACCESS_WRITE_ONLY, false);

		if(!vertex_buf)
			return false;

		int num_vertices = 0;
		TextBatch* batch = start_batch;

		while(batch)
		{
			batch->vertexOffset = num_vertices;
			num_vertices += batch->vertexCount;

			if(num_vertices > MAX_NUM_TEXT_VERTICES)
			{
				batch = batch->next;
				break;
			}

			TextData* text_data = batch->textDataList;

			while(text_data)
			{
				int x1 = text_data->x;
				int y1 = text_data->y;
				int y2 = y1 + batch->font->GetFontHeight();
				const char* text = text_data->text;
				const vec4f& color = text_data->color;

				while(*text)
				{
					const Font::TexCoords* tex_coords = batch->font->GetCharTexCoords(*text);
					int x2 = x1 + batch->font->GetCharWidth(*text);

					vertex_buf[0].position.x = (float)x1;
					vertex_buf[0].position.y = (float)y1;
					vertex_buf[0].uv.s = tex_coords->u1;
					vertex_buf[0].uv.t = tex_coords->v2;
					vertex_buf[0].color = color;

					vertex_buf[1].position.x = (float)x1;
					vertex_buf[1].position.y = (float)y2;
					vertex_buf[1].uv.s = tex_coords->u1;
					vertex_buf[1].uv.t = tex_coords->v1;
					vertex_buf[1].color = color;

					vertex_buf[2].position.x = (float)x2;
					vertex_buf[2].position.y = (float)y1;
					vertex_buf[2].uv.s = tex_coords->u2;
					vertex_buf[2].uv.t = tex_coords->v2;
					vertex_buf[2].color = color;

					vertex_buf[3].position.x = (float)x1;
					vertex_buf[3].position.y = (float)y2;
					vertex_buf[3].uv.s = tex_coords->u1;
					vertex_buf[3].uv.t = tex_coords->v1;
					vertex_buf[3].color = color;

					vertex_buf[4].position.x = (float)x2;
					vertex_buf[4].position.y = (float)y2;
					vertex_buf[4].uv.s = tex_coords->u2;
					vertex_buf[4].uv.t = tex_coords->v1;
					vertex_buf[4].color = color;

					vertex_buf[5].position.x = (float)x2;
					vertex_buf[5].position.y = (float)y1;
					vertex_buf[5].uv.s = tex_coords->u2;
					vertex_buf[5].uv.t = tex_coords->v2;
					vertex_buf[5].color = color;

					vertex_buf += 6;
					++text;
					x1 = x2;
				}

				// delete text buffer
				delete[] text_data->text;

				text_data = text_data->next;
			}

			batch = batch->next;
		}

		if(!_textVertBuffer->UnmapBuffer())
			return false;

		end_batch = batch;
		return true;
	}

	void Render2D::Clear()
	{
		_renderSystem = 0;
		_renderer = 0;
		_vpText = 0;
		_fpText = 0;
		_textBatchList = 0;
		_vpRect2D = 0;
		_fpRect2D = 0;
		_textVertBuffer = 0;
		_rectVertBuffer = 0;
		_textVertFmt = 0;
		_rectVertFmt = 0;
		_textSamplerState = 0;
		_consoleCursorBlinkTime = 0;
		_consoleLineSize = 0;
		_consoleLineText = 0;
	}

}
