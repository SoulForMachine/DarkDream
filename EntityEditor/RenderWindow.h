#pragma once


namespace EntityEditor
{

	using namespace System;
	using namespace System::Windows;
	using namespace System::Windows::Forms;


	ref class RenderWindow: public NativeWindow
	{
	public:
		RenderWindow(Form^ parent);

		void SetEntity(Engine::ModelEntity* entity);
		void Animate(bool anim)
			{ _animate = anim; }
		void Wireframe(bool wireframe)
			{ _wireframe = wireframe; }
		void ShowStats(bool stats)
			{ _modelStats = stats; }
		void ShowSkelet(bool show)
			{ _skelet = show; }
		void Draw()
			{ OnPaint(); }
		void ModelChanged();

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
		void RenderGrid();
		void RenderEntity();
		void RenderStats();
		void RenderSkelet(const math3d::vec3f& color);
		void RenderBone(const Engine::Joint* joint, const math3d::mat4f& matrix);
		bool CreateResources();
		void DestroyResources();

		Engine::RenderSystem* _renderSystem;
		Engine::ModelEntity* _entity;
		GL::Renderer* _renderer;

		// drawing stuff
		Engine::Font* _font;
		GL::Buffer* _gridVertBuf;
		GL::Buffer* _lightVertBuf;
		GL::Buffer* _jointVertBuf;
		GL::VertexFormat* _lineVertFmt;
		const Engine::ASMProgRes* _vpSimple;
		const Engine::ASMProgRes* _fpConstClr;
		int _gridVertCount;
		int _gridHorizCount;
		float _gridVertSpacing;
		float _gridHorizSpacing;
		float _jointRadius;

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
		float _rotLightX;
		float _rotLightY;
		uint _prevTime;
		int _frameTime;
		int _fpsTime;
		int _frameCount;
		float _fps;
		bool _animate;
		bool _wireframe;
		bool _modelStats;
		bool _skelet;
	};

}
