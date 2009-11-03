
#include "stdafx.h"
#include "RenderWindow.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))


using namespace Memory;
using namespace Engine;
using namespace GL;
using namespace math3d;


namespace ParticleEditor
{

	RenderWindow::RenderWindow(Form^ parent)
	{
		_renderSystem = 0;
		_renderer = 0;
		_font = 0;
		_axisVertBuf = 0;
		_lineVertFmt = 0;
		_vertProgSimple = 0;
		_fragProgConst = 0;

		_rotX = 45.0f;
		_rotY = 45.0f;
		_panX = 0.0f;
		_panY = 0.0f;
		_zoom = 5.0f;

		_leftBtnDown = false;
		_middleBtnDown = false;
		_rightBtnDown = false;

		::Timer::Init();
		_prevTime = ::Timer::GetTime();
		_fpsTime = 0;
		_frameCount = 0;
		_fps = 0;
		_stats = true;

		System::Windows::Forms::CreateParams^ cp = gcnew System::Windows::Forms::CreateParams;
		cp->Caption = "";
		cp->ClassName = nullptr;
		cp->ClassStyle = CS_OWNDC;
		cp->Style = WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		cp->ExStyle = 0;
		cp->X = 0;
		cp->Y = 0;
		cp->Width = parent->ClientRectangle.Width;
		cp->Height = parent->ClientRectangle.Height;
		cp->Param = nullptr;
		cp->Parent = parent->Handle;

		try
		{
			CreateHandle(cp);
		}
		catch(Exception^)
		{
			throw "Failed to create handle.";
		}

		IntPtr hinstance = System::Runtime::InteropServices::Marshal::GetHINSTANCE(this->GetType()->Module);
		_renderSystem = engineAPI->renderSystem;
		bool result = _renderSystem->Init(hinstance.ToPointer(), Handle.ToPointer());
		if(result)
		{
			_renderer = _renderSystem->GetRenderer();
			engineAPI->world->Init();
		}
		else
		{
			_renderSystem = 0;
			DestroyHandle();
			throw "Failed to initialize renderer.";
		}

		// create grid vertex buffer and shaders
		CreateResources();
	}

	bool RenderWindow::CreateResources()
	{
		_font = new(mainPool) Font;
		_font->Create(_renderer, _t("Verdana"), 11);

		vec3f vertices[] =
		{
			vec3f::null, vec3f::x_axis * 10.0f,
			vec3f::null, vec3f::y_axis * 10.0f,
			vec3f::null, vec3f::z_axis * 10.0f,
		};
		_axisVertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, 6 * sizeof(vec3f), vertices, GL::USAGE_STATIC_DRAW);

		GL::VertexAttribDesc vert_desc[] =
		{
			{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 }
		};
		_lineVertFmt = _renderer->CreateVertexFormat(vert_desc, COUNTOF(vert_desc));

		_vertProgSimple = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fragProgConst = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		return true;
	}

	void RenderWindow::DestroyResources()
	{
		if(_font)
			_font->Destroy();
		delete _font;

		_renderer->DestroyBuffer(_axisVertBuf);
		_axisVertBuf = 0;

		_renderer->DestroyVertexFormat(_lineVertFmt);
		_lineVertFmt = 0;

		engineAPI->asmProgManager->ReleaseASMProgram(_vertProgSimple);
		engineAPI->asmProgManager->ReleaseASMProgram(_fragProgConst);
	}

	void RenderWindow::WndProc(Message% msg)
	{
		int wparam = msg.WParam.ToInt32();
		int lparam = msg.LParam.ToInt32();

		switch(msg.Msg)
		{
		case WM_PAINT:
			OnPaint();
			break;
		case WM_ERASEBKGND:
			msg.Result = IntPtr(1);
			return;
		case WM_MOUSEMOVE:
			OnMouseMove(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_MBUTTONDOWN:
			OnMButtonDown(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_RBUTTONDOWN:
			OnRButtonDown(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_LBUTTONUP:
			OnLButtonUp(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_MBUTTONUP:
			OnMButtonUp(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_RBUTTONUP:
			OnRButtonUp(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_MOUSEWHEEL:
			OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wparam), GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			msg.Result = IntPtr(0);
			return;
		case WM_SIZE:
			OnSize(LOWORD(lparam), HIWORD(lparam));
			break;
		case WM_DESTROY:
			OnDestroy();
			break;
		};

		NativeWindow::WndProc(msg);
	}

	void RenderWindow::UpdateFrame()
	{
		uint cur_time = ::Timer::GetTime();
		_frameTime = cur_time - _prevTime;
		_prevTime = cur_time;
		_fpsTime += _frameTime;
		_frameCount++;
		if(_fpsTime > 200)
		{
			_fps = _frameCount / (_fpsTime * 0.001f);
			_fpsTime = 0;
			_frameCount = 0;
		}

		mat4f rot, look, view;
		rot.set_rotation_y(deg2rad(_rotY));
		rot.rotate_x(deg2rad(_rotX));
		look.look_at(
			vec3f(_panX, _panY, _zoom),
			vec3f(_panX, _panY, 0.0f),
			vec3f::y_axis);
		mul(view, rot, look);

		engineAPI->world->GetCamera().SetViewingTransform(view);
	}

	void RenderWindow::OnPaint()
	{
		// prevent OnPaint re-entry
		if(!System::Threading::Monitor::TryEnter(this))
			return;
		//------------------------

		if(!_renderer)
			return;

		_renderer->SwapInterval(1);
		_renderer->ClearColorBuffer(0.5f, 0.5f, 0.5f, 1.0f);
		_renderer->ClearDepthStencilBuffer(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT, 1.0f, 0);
		_renderer->EnableDepthTest(true);

		RenderCoordSys();
		RenderParticleSystem();
		if(_stats)
			RenderStats();

		_renderer->Finish();
		_renderer->SwapBuffers();

		System::Threading::Monitor::Exit(this);
	}

	void RenderWindow::OnSize(int width, int height)
	{
		if(_renderer)
		{
			_width = width;
			_height = height;
			_renderer->Viewport(0, 0, width, height);
			engineAPI->world->GetCamera().Perspective(deg2rad(60.0f), float(width) / height, 0.1f, 1000.0f);
		}
	}

	void RenderWindow::OnDestroy()
	{
		if(_renderSystem)
		{
			DestroyResources();

			// release engine resources
		//!	Engine::ResourceManager::ReleaseAllResources();

			::Timer::Deinit();

			engineAPI->world->Deinit();
			_renderSystem->Deinit();
		}
	}

	void RenderWindow::OnMouseMove(int modifiers, int x, int y)
	{
		SetCursor(LoadCursor(0, IDC_ARROW));

		if(_leftBtnDown && (modifiers & MK_LBUTTON))
		{
			_rotX += (y - _prevY);
			_rotY += (x - _prevX);

			if(_rotX > 360.0f)
				_rotX -= 360.0f;

			if(_rotY > 360.0f)
				_rotY -= 360.0f;
		}
		else if(_middleBtnDown && (modifiers & MK_MBUTTON))
		{
			_panX -= (x - _prevX) * 0.001f * _zoom;
			_panY += (y - _prevY) * 0.001f * _zoom;
		}
		else if(_rightBtnDown && (modifiers & MK_RBUTTON))
		{
			_zoom += (x - _prevX) * 0.05f;
			float z = _zoom;
			clamp(z, 1.0f, 100.0f);
			_zoom = z;
		}

		_prevX = x;
		_prevY = y;
	}

	void RenderWindow::OnLButtonDown(int x, int y)
	{
		SetCapture((HWND)Handle.ToPointer());
		_prevX = x;
		_prevY = y;
		_leftBtnDown = true;
	}

	void RenderWindow::OnMButtonDown(int x, int y)
	{
		SetCapture((HWND)Handle.ToPointer());
		_prevX = x;
		_prevY = y;
		_middleBtnDown = true;
	}

	void RenderWindow::OnRButtonDown(int x, int y)
	{
		SetCapture((HWND)Handle.ToPointer());
		_prevX = x;
		_prevY = y;
		_rightBtnDown = true;
	}

	void RenderWindow::OnLButtonUp(int x, int y)
	{
		ReleaseCapture();
		_leftBtnDown = false;
	}

	void RenderWindow::OnMButtonUp(int x, int y)
	{
		ReleaseCapture();
		_middleBtnDown = false;
	}

	void RenderWindow::OnRButtonUp(int x, int y)
	{
		ReleaseCapture();
		_rightBtnDown = false;
	}

	void RenderWindow::OnMouseWheel(int delta, int x, int y)
	{
		_zoom -= float(delta) / WHEEL_DELTA;
		float z = _zoom;
		clamp(z, 1.0f, 100.0f);
		_zoom = z;
	}

	void RenderWindow::RenderParticleSystem()
	{
		
	}

	void RenderWindow::RenderStats()
	{
		_renderer->EnableDepthTest(false);
		_renderer->FrontFace(GL::ORIENT_CW);

		int width, height;
		height = _font->GetFontHeight();
		int x = 5, y = _height - height - 5;
		vec4f white(1.0f, 1.0f, 1.0f, 1.0f);
		vec4f green(0.0f, 1.0f, 0.0f, 1.0f);
		char buf[64];

		// FPS
		width = _font->GetTextWidth("FPS: ");
		sprintf(buf, "%d", (int)round(_fps));
		_renderSystem->GetRender2D()->DrawText("FPS: ", x, y, *_font, white);
		_renderSystem->GetRender2D()->DrawText(buf, x + width, y, *_font, green);
		y -= height * 2;

		//if()
		//{
		//	// particle count
		//	width = _font->GetTextWidth("Particles: ");
		//	sprintf(buf, "%d", );
		//	_renderSystem->GetRender2D()->DrawText("Particles: ", x, y, *_font, white);
		//	_renderSystem->GetRender2D()->DrawText(buf, x + width, y, *_font, green);
		//	y -= height;
		//}

		_renderSystem->GetRender2D()->FlushText();
		_renderer->EnableDepthTest(true);
	}

	void RenderWindow::RenderCoordSys()
	{
		_renderer->VertexSource(0, _axisVertBuf, sizeof(vec3f), 0);
		_renderer->ActiveVertexFormat(_lineVertFmt);
		_renderer->ActiveVertexASMProgram(_vertProgSimple->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fragProgConst->GetASMProgram());

		_vertProgSimple->GetASMProgram()->LocalMatrix4x4(0, engineAPI->world->GetCamera().GetViewProjectionTransform());

		vec4f red(1.0f, 0.0f, 0.0f, 1.0f);
		vec4f green(0.0f, 1.0f, 0.0f, 1.0f);
		vec4f blue(0.0f, 0.0f, 1.0f, 1.0f);

		_fragProgConst->GetASMProgram()->LocalParameter(0, red);
		_renderer->Draw(GL::PRIM_LINES, 0, 2);

		_fragProgConst->GetASMProgram()->LocalParameter(0, green);
		_renderer->Draw(GL::PRIM_LINES, 2, 2);

		_fragProgConst->GetASMProgram()->LocalParameter(0, blue);
		_renderer->Draw(GL::PRIM_LINES, 4, 2);
	}

}
