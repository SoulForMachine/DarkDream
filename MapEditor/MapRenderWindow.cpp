
#include "stdafx.h"
#include "EditMode.h"
#include "MapRenderWindow.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))


using namespace Memory;
using namespace Engine;
using namespace GL;
using namespace math3d;


namespace MapEditor
{

	struct LineVert3D
	{
		vec3f position;
	};



	MapRenderWindow::MapRenderWindow(Form^ parent)
	{
		_renderSystem = 0;
		_renderer = 0;
		_font = 0;
		_lineVertFmt = 0;
		_vpSimple = 0;
		_fpConstClr = 0;

		_rotX = 45.0f;
		_rotY = 45.0f;
		_panX = (float)Terrain::PATCH_WIDTH / 2;
		_panY = 0.0f;
		_zoom = 150.0f;

		_leftBtnDown = false;
		_middleBtnDown = false;
		_rightBtnDown = false;

		::Timer::Init();
		_prevTime = ::Timer::GetTime();
		_fpsTime = 0;
		_frameCount = 0;
		_fps = 0;
		_animate = false;
		_wireframe = false;
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

		_editMode = nullptr;
	}

	bool MapRenderWindow::CreateResources()
	{
		_font = new(mainPool) Font;
		_font->Create(_renderer, _t("Verdana"), 11);

		_vpSimple = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fpConstClr = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		VertexAttribDesc desc[] =
		{
			{ 0, 0, 3, TYPE_FLOAT, false, false, 0 }
		};
		_lineVertFmt = _renderer->CreateVertexFormat(desc, COUNTOF(desc));

		return true;
	}

	void MapRenderWindow::DestroyResources()
	{
		if(_font)
			_font->Destroy();
		delete _font;

		if(_vpSimple)
		{
			engineAPI->asmProgManager->ReleaseASMProgram(_vpSimple);
			_vpSimple = 0;
		}

		if(_fpConstClr)
		{
			engineAPI->asmProgManager->ReleaseASMProgram(_fpConstClr);
			_fpConstClr = 0;
		}

		_renderer->DestroyVertexFormat(_lineVertFmt);
		_lineVertFmt = 0;
	}

	void MapRenderWindow::WndProc(Message% msg)
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

	void MapRenderWindow::OnPaint()
	{
		// prevent OnPaint re-entry
	/*	static int in_during = 0;
		if(++in_during > 1)
		{
			in_during = 1;
			return;
		}*/
		if(!System::Threading::Monitor::TryEnter(this))
			return;
		//------------------------

		if(!_renderer)
			return;

		if(!_renderer->SetCurrentContext())
			return;

		mat4f rot, look_at, cam;
		rot.set_translation(-_panX, 0.0f, -Terrain::PATCH_HEIGHT / 2);
		rot.rotate_y(_rotY);
		rot.rotate_x(_rotX);
		rot.translate(_panX, 0.0f, Terrain::PATCH_HEIGHT / 2);
		look_at.look_at(
			vec3f(_panX, _panY, _zoom),
			vec3f(_panX, _panY, 0.0f),
			vec3f::y_axis);
		mul(cam, rot, look_at);

		engineAPI->world->GetCamera().SetViewingTransform(cam);

		_renderer->SwapInterval(0);
		_renderer->ClearColorBuffer(0.5f, 0.5f, 0.5f, 1.0f);
		_renderer->ClearDepthStencilBuffer(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT, 1.0f, 0);
		_renderer->EnableDepthTest(true);
		_renderer->EnableFaceCulling(false);

		if(_wireframe)
			_renderer->RasterizationMode(GL::RASTER_LINE);

		engineAPI->renderSystem->RenderTerrain(_frameTime);

		if(_wireframe)
			_renderer->RasterizationMode(GL::RASTER_FILL);

		if(_editMode)
			_editMode->Render();

		if(_stats)
			RenderStats();

		_renderer->Finish();
		_renderer->SwapBuffers();

	//	in_during = 0;
		System::Threading::Monitor::Exit(this);
	}

	void MapRenderWindow::OnSize(int width, int height)
	{
		if(_renderer)
		{
			if(!_renderer->SetCurrentContext())
				return;

			_width = width;
			_height = height;
			_renderer->Viewport(0, 0, width, height);
			engineAPI->world->GetCamera().Perspective(60.0f, float(width) / height, 0.1f, 10000.0f);
		}
	}

	void MapRenderWindow::OnDestroy()
	{
		if(_renderSystem)
		{
			if(!_renderer->SetCurrentContext())
				return;

			DestroyResources();

			// release engine resources
		//!	Engine::ResourceManager::ReleaseAllResources();

			::Timer::Deinit();

			engineAPI->world->Deinit();
			_renderSystem->Deinit();
		}
	}

	void MapRenderWindow::OnMouseMove(int modifiers, int x, int y)
	{
		_editMode->MouseMove(modifiers, x, y);

		if(!_editMode->IsExecuting())
		{
			if(_leftBtnDown && (modifiers & MK_LBUTTON))
			{
				
			}
			else if(_middleBtnDown && (modifiers & MK_MBUTTON))
			{
				_rotX += (y - _prevY) * 0.5f;
				_rotY += (x - _prevX) * 0.5f;

				if(_rotX > 360.0f)
					_rotX -= 360.0f;

				if(_rotY > 360.0f)
					_rotY -= 360.0f;

				OnPaint();
			}
			else if(_rightBtnDown && (modifiers & MK_RBUTTON))
			{
				_panX -= (x - _prevX) * 0.001f * _zoom;
				_panY += (y - _prevY) * 0.001f * _zoom;

				OnPaint();
			}
		}

		_prevX = x;
		_prevY = y;
	}

	void MapRenderWindow::OnLButtonDown(int x, int y)
	{
		SetCapture((HWND)Handle.ToPointer());
		_prevX = x;
		_prevY = y;
		_leftBtnDown = true;

		_editMode->LeftButtonDown(x, y);
	}

	void MapRenderWindow::OnMButtonDown(int x, int y)
	{
		SetCapture((HWND)Handle.ToPointer());
		_prevX = x;
		_prevY = y;
		_middleBtnDown = true;

		_editMode->MiddleButtonDown(x, y);
	}

	void MapRenderWindow::OnRButtonDown(int x, int y)
	{
		SetCapture((HWND)Handle.ToPointer());
		_prevX = x;
		_prevY = y;
		_rightBtnDown = true;

		_editMode->RightButtonDown(x, y);
	}

	void MapRenderWindow::OnLButtonUp(int x, int y)
	{
		ReleaseCapture();
		_leftBtnDown = false;

		_editMode->LeftButtonUp(x, y);
	}

	void MapRenderWindow::OnMButtonUp(int x, int y)
	{
		ReleaseCapture();
		_middleBtnDown = false;

		_editMode->MiddleButtonUp(x, y);
	}

	void MapRenderWindow::OnRButtonUp(int x, int y)
	{
		ReleaseCapture();
		_rightBtnDown = false;

		_editMode->RightButtonUp(x, y);
	}

	void MapRenderWindow::OnMouseWheel(int delta, int x, int y)
	{
		_editMode->MouseWheel(delta, x, y);

		if(!_editMode->IsExecuting())
		{
			for(int i = 0; i < 6; ++i)
			{
				_zoom -= float(delta) / WHEEL_DELTA;
				float z = _zoom;
				clamp(z, 5.0f, 100.0f);
				_zoom = z;

				OnPaint();
			}
		}
	}

	void MapRenderWindow::RenderStats()
	{
		_renderer->EnableDepthTest(false);
		_renderer->FrontFace(GL::ORIENT_CW);

		int width, height;
		height = _font->GetFontHeight();
		int x = 5, y = _height - height - 5;
		vec4f color(1.0f, 1.0f, 1.0f, 1.0f);
		char buf[64];

		// FPS
		width = _font->GetTextWidth("FPS: ");
		sprintf(buf, "%d", (int)round(_fps));
		_renderSystem->GetRender2D()->DrawText("FPS: ", x, y, *_font, color);
		_renderSystem->GetRender2D()->DrawText(buf, x + width, y, *_font, color);
		y -= height;

		_renderSystem->GetRender2D()->FlushText();
		_renderer->EnableDepthTest(true);
		_renderer->FrontFace(GL::ORIENT_CCW);
	}

	void MapRenderWindow::UpdateFrame()
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

		if(_editMode->IsExecuting())
			_editMode->Update(_frameTime * 0.001f);
	}

}
