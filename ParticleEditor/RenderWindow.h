#pragma once


namespace ParticleEditor
{

	using namespace System;
	using namespace System::Windows;
	using namespace System::Windows::Forms;


	ref class RenderWindow: public NativeWindow
	{
	public:
		RenderWindow(Form^ parent);

		void ShowStats(bool stats)
			{ _stats = stats; }
		bool StatsVisible()
			{ return _stats; }
		void Draw()
			{ OnPaint(); }
		void UpdateFrame();
		void SetParticleSystem(Engine::ParticleSystem* part_sys);

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
		void RenderStats();
		void RenderCoordSys();
		bool CreateResources();
		void DestroyResources();

		Engine::RenderSystem* _renderSystem;
		GL::Renderer* _renderer;
		GL::Buffer* _axisVertBuf;
		GL::VertexFormat* _lineVertFmt;
		const Engine::ASMProgRes* _vertProgSimple;
		const Engine::ASMProgRes* _fragProgConst;
		Engine::ParticleSystem* _particleSystem;

		// drawing stuff
		Engine::Font* _font;

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
		bool _stats;
	};

}
