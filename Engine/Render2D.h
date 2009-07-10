
#ifndef _RENDER_2D_
#define _RENDER_2D_

#include "BaseLib/Math/vec4.h"
#include "BaseLib/GL/GLRenderer.h"
#include "BaseLib/FreeStackPool.h"
#include "Engine/Common.h"
#include "Engine/FileResource.h"
#include "Engine/Font.h"

#undef DrawText

namespace Engine
{

	class RenderSystem;

	class ENGINE_API Render2D
	{
	public:
		Render2D();

		bool Init(RenderSystem* render_system);
		void Deinit();

		void FlushText();
		void DrawText(const char* text, int x, int y, const Font& font, const math3d::vec4f& color);
		void DrawTextImediate(const char* text, int x, int y, const Font& font, const math3d::vec4f& color);
		void DrawConsole(int frame_time);
		void DrawColoredRect(int x1, int y1, int x2, int y2, const math3d::vec4f& color);
		void ReloadShaders();

	private:
#pragma pack(push, 1)
		struct TextVertex
		{
			math3d::vec2f position;
			math3d::vec2f uv;
			math3d::vec4f color;
		};

		struct RectVertex
		{
			math3d::vec2f position;
		};
#pragma pack(pop)

		struct TextData
		{
			char* text;
			math3d::vec4f color;
			int x, y;
			TextData* next;
		};

		struct TextBatch
		{
			const Font* font;
			TextData* textDataList;
			TextBatch* next;
			int vertexOffset;
			int vertexCount;
		};

		static const int TEXT_VERTEX_BUFFER_SIZE = 512 * 1024;
		static const int MAX_NUM_TEXT_VERTICES = TEXT_VERTEX_BUFFER_SIZE / sizeof(TextVertex);
		static const int NUM_RECT_VERTICES = 6;
		static const int RECT_VERTEX_BUFFER_SIZE = NUM_RECT_VERTICES * sizeof(RectVertex);

		void CreateShaders();
		void DestroyShaders();
		bool FillVertexBuffer(TextBatch* start_batch, TextBatch*& end_batch);
		void Clear();

		RenderSystem* _renderSystem;
		GL::Renderer* _renderer;
		const ASMProgRes* _vpText;
		const ASMProgRes* _fpText;
		GL::Buffer* _textVertBuffer;
		GL::VertexFormat* _textVertFmt;
		GL::SamplerState* _textSamplerState;
		FreeStackPool<TextBatch> _textBatchPool;
		FreeStackPool<TextData> _textDataPool;
		TextBatch* _textBatchList;

		const ASMProgRes* _vpRect2D;
		const ASMProgRes* _fpRect2D;
		GL::Buffer* _rectVertBuffer;
		GL::VertexFormat* _rectVertFmt;
		Font _consoleFont;
		unsigned int _consoleCursorBlinkTime;
		int _consoleLineSize;
		char* _consoleLineText;
	};

}


#endif // _RENDER_2D_
