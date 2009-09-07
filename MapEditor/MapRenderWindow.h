#pragma once


namespace MapEditor
{

	using namespace System;
	using namespace System::Windows;
	using namespace System::Windows::Forms;

	ref class EditMode;


	ref class MapRenderWindow: public NativeWindow
	{
	public:
		MapRenderWindow(Form^ parent);

		void SetEditMode(EditMode^ mode)
			{ _editMode = mode; }
		void Animate(bool anim)
			{ _animate = anim; }
		void Wireframe(bool wireframe)
			{ _wireframe = wireframe; }
		void ShowStats(bool stats)
			{ _stats = stats; }
		void Draw()
			{ OnPaint(); }
		void UpdateFrame();

	protected:
		virtual void WndProc(Message% msg) override;

	private:
		void OnPaint();
		void OnDestroy();
		void OnSize(int width, int height);
		void OnMouseMove(int modifiers, int x, int y);
		void OnLButtonDown(int x, int y);
		void OnMButtonDown(int x, int y);
		void OnRButtonDown(int x, int y);
		void OnLButtonUp(int x, int y);
		void OnMButtonUp(int x, int y);
		void OnRButtonUp(int x, int y);
		void OnMouseWheel(int delta, int x, int y);
		void OnKeyDown(int key);
		void RenderStats();
		bool CreateResources();
		void DestroyResources();

		Engine::RenderSystem* _renderSystem;
		GL::Renderer* _renderer;
		EditMode^ _editMode;

		// drawing stuff
		Engine::Font* _font;
		GL::VertexFormat* _lineVertFmt;
		const Engine::ASMProgRes* _vpSimple;
		const Engine::ASMProgRes* _fpConstClr;

		int _width;
		int _height;
		int _prevX;
		int _prevY;
		bool _leftBtnDown;
		bool _middleBtnDown;
		bool _rightBtnDown;
		float _rotX;
		float _rotY;
		float _panX;
		float _panY;
		float _zoom;
		uint _prevTime;
		int _frameTime;
		int _fpsTime;
		int _frameCount;
		float _fps;
		bool _animate;
		bool _wireframe;
		bool _stats;
	};

}
