#pragma once

#include "EM_View.h"


namespace MapEditor
{

	using namespace System;
	using namespace System::Windows;
	using namespace System::Windows::Forms;

	ref class EditMode;


	public ref class MapRenderWindow: public NativeWindow
	{
	public:
		enum class ViewMode
		{
			GAME,
			EDITOR,
		};

		MapRenderWindow(Form^ parent, EditorCommon::FormDirector^ director);

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
		void SetViewMode(ViewMode mode);
		ViewMode GetViewMode()
			{ return _viewMode; }
		void SetViewParameters(EM_View::Parameters^ params)
			{ _parameters = params; }
		void SetCamX(float x);
		float GetCamX();

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
		void UpdateEditorCam();
		void UpdateGameCam();

		Engine::RenderSystem* _renderSystem;
		GL::Renderer* _renderer;
		EditMode^ _editMode;

		// drawing stuff
		Engine::Font* _font;
		GL::VertexFormat* _lineVertFmt;
		Engine::VertexASMProgResPtr& _vpSimple;
		Engine::FragmentASMProgResPtr& _fpConstClr;

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
		float _frameTimeSec;
		int _fpsTime;
		int _frameCount;
		float _fps;
		bool _animate;
		bool _wireframe;
		bool _stats;
		ViewMode _viewMode;
		EM_View::Parameters^ _parameters;
		EditorCommon::FormDirector^ _director;
		Form^ _parent;
	};

}
