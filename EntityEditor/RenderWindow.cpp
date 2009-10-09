
#include "stdafx.h"
#include "RenderWindow.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))


using namespace Memory;
using namespace Engine;
using namespace GL;
using namespace math3d;


namespace EntityEditor
{

	struct LineVert3D
	{
		vec3f position;
	};


	static math3d::mat4f s_worldMat;


	RenderWindow::RenderWindow(Form^ parent)
	{
		_renderSystem = 0;
		_renderer = 0;
		_entity = 0;
		_font = 0;
		_gridVertBuf = 0;
		_lightVertBuf = 0;
		_jointVertBuf = 0;
		_lineVertFmt = 0;
		_vpSimple = 0;
		_fpConstClr = 0;

		_gridVertCount = 20;
		_gridHorizCount = 20;
		_gridVertSpacing = 1.0f;
		_gridHorizSpacing = 1.0f;

		_jointRadius = 0.025f;

		_rotX = 45.0f;
		_rotY = 45.0f;
		_panX = 0.0f;
		_panY = 0.0f;
		_zoom = 15.0f;
		_rotLightX = -20.0f;
		_rotLightY = 20.0f;

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
		_modelStats = false;
		_skelet = true;

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

	void RenderWindow::SetEntity(Engine::ModelEntity* entity)
	{
		engineAPI->world->RemoveAllEntities();
		if(entity)
			engineAPI->world->AddEntity(entity);
		_entity = entity;
		ModelChanged();
		InvalidateRect((HWND)Handle.ToPointer(), 0, FALSE);
	}

	void RenderWindow::ModelChanged()
	{
		if(_entity && _entity->GetModelRes())
		{
			const AABBox& bbox = _entity->GetModelRes()->GetModel()->GetBoundingBox();
			float max_dim = Max(
				bbox.maxPt.x - bbox.minPt.x,
				Max(bbox.maxPt.y - bbox.minPt.y,
				bbox.maxPt.z - bbox.minPt.z));
			_jointRadius = max_dim * 0.025f;
		}
	}

	bool RenderWindow::CreateResources()
	{
		_font = new(mainPool) Font;
		_font->Create(_renderer, _t("Verdana"), 11);

		size_t size = (_gridVertCount + _gridHorizCount) * sizeof(LineVert3D) * 2;
		_gridVertBuf = _renderer->CreateBuffer(OBJ_VERTEX_BUFFER, size, 0, USAGE_STATIC_DRAW);
		if(!_gridVertBuf)
		{
			::Console::PrintError("Failed to create grid vertex buffer.");
			return false;
		}

		LineVert3D* verts = (LineVert3D*)_gridVertBuf->MapBuffer(ACCESS_WRITE_ONLY, false);
		if(!verts)
		{
			::Console::PrintError("Failed to map grid vertex buffer.");
			return false;
		}

		float h = (_gridHorizCount - 1) * _gridHorizSpacing / 2.0f;
		float v = (_gridVertCount - 1) * _gridVertSpacing / 2.0f;
		float x1 = -h;
		float x2 = h;
		float y1 = -v;
		float y2 = v;

		// vertical lines
		float pos = x1;
		for(int i = 0; i < _gridVertCount; ++i)
		{
			verts[0].position.x = pos;
			verts[0].position.y = 0.0f;
			verts[0].position.z = y1;
			verts[1].position.x = pos;
			verts[1].position.y = 0.0f;
			verts[1].position.z = y2;
			verts += 2;
			pos += _gridVertSpacing;
		}

		// horizontal lines
		pos = y1;
		for(int i = 0; i < _gridHorizCount; ++i)
		{
			verts[0].position.x = x1;
			verts[0].position.y = 0.0f;
			verts[0].position.z = pos;
			verts[1].position.x = x2;
			verts[1].position.y = 0.0f;
			verts[1].position.z = pos;
			verts += 2;
			pos += _gridHorizSpacing;
		}

		_gridVertBuf->UnmapBuffer();

		// light vert buffer
		LineVert3D light_lines[] =
		{
			// arrow point
			{ vec3f(0.0f, 0.0f, 0.0f) }, { vec3f(0.5f, 0.0f, 0.5f) },
			{ vec3f(0.0f, 0.0f, 0.0f) }, { vec3f(0.5f, 0.0f, -0.5f) },
			{ vec3f(0.0f, 0.0f, 0.0f) }, { vec3f(0.5f, 0.5f, 0.0f) },
			{ vec3f(0.0f, 0.0f, 0.0f) }, { vec3f(0.5f, -0.5f, 0.0f) },

			// arrow body
			{ vec3f(0.5f, 0.0f, 0.5f) }, { vec3f(0.5f, 0.0f, -0.5f) },
			{ vec3f(0.5f, 0.0f, -0.5f) }, { vec3f(3.0f, 0.0f, -0.5f) },
			{ vec3f(3.0f, 0.0f, -0.5f) }, { vec3f(3.0f, 0.0f, 0.5f) },
			{ vec3f(3.0f, 0.0f, 0.5f) }, { vec3f(0.5f, 0.0f, 0.5f) },

			{ vec3f(0.5f, 0.5f, 0.0f) }, { vec3f(0.5f, -0.5f, 0.0f) },
			{ vec3f(0.5f, -0.5f, 0.0f) }, { vec3f(3.0f, -0.5f, 0.0f) },
			{ vec3f(3.0f, -0.5f, 0.0f) }, { vec3f(3.0f, 0.5f, 0.0f) },
			{ vec3f(3.0f, 0.5f, 0.0f) }, { vec3f(0.5f, 0.5f, 0.0f) },
		};
		_lightVertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, sizeof(light_lines), light_lines, GL::USAGE_STATIC_DRAW);
		if(!_lightVertBuf)
		{
			::Console::PrintError("Failed to create light vertex buffer.");
			return false;
		}

		// joint vertex buffer
		const int seg = 60;
		int count = seg * 3;
		_jointVertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, sizeof(LineVert3D) * count, 0, GL::USAGE_STATIC_DRAW);
		LineVert3D* joint_verts = (LineVert3D*)_jointVertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, false);
		if(joint_verts)
		{
			LineVert3D* p1 = joint_verts;
			LineVert3D* p2 = joint_verts + seg;
			LineVert3D* p3 = joint_verts + seg * 2;

			float step = TWO_PI / seg;
			float angle = 0.0f;
			for(int i = 0; i < seg; ++i)
			{
				float ca = cos(angle);
				float sa = sin(angle);
				p1->position.set(ca, sa, 0.0f);
				p2->position.set(ca, 0.0f, sa);
				p3->position.set(0.0f, ca, sa);
				++p1;
				++p2;
				++p3;
				angle += step;
			}
			_jointVertBuf->UnmapBuffer();
		}

		_vpSimple = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fpConstClr = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);

		VertexAttribDesc desc[] =
		{
			{ 0, 0, 3, TYPE_FLOAT, false, false, 0 }
		};
		_lineVertFmt = _renderer->CreateVertexFormat(desc, COUNTOF(desc));

		return true;
	}

	void RenderWindow::DestroyResources()
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

		_renderer->DestroyBuffer(_gridVertBuf);
		_gridVertBuf = 0;

		_renderer->DestroyBuffer(_lightVertBuf);
		_lightVertBuf = 0;

		_renderer->DestroyBuffer(_jointVertBuf);
		_jointVertBuf = 0;

		_renderer->DestroyVertexFormat(_lineVertFmt);
		_lineVertFmt = 0;
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

	void RenderWindow::OnPaint()
	{
		// prevent OnPaint re-entry
		static int in_during = 0;
		if(++in_during > 1)
		{
			in_during = 1;
			return;
		}
		//------------------------

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


		if(!_renderer)
			return;

		s_worldMat.set_rotation_y(_rotY);
		s_worldMat.rotate_x(_rotX);

		engineAPI->world->GetCamera().LookAt(
			vec3f(_panX, _panY, _zoom),
			vec3f(_panX, _panY, 0.0f),
			vec3f::y_axis);

		_renderer->SwapInterval(1);
		_renderer->ClearColorBuffer(0.5f, 0.5f, 0.5f, 1.0f);
		_renderer->ClearDepthStencilBuffer(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT, 1.0f, 0);
		_renderer->EnableDepthTest(true);

		RenderGrid();
		RenderEntity();
		if(_skelet)
			RenderSkelet(vec3f(0.0f, 1.0f, 0.0f));
		if(_modelStats)
			RenderStats();

		_renderer->Finish();
		_renderer->SwapBuffers();

		in_during = 0;
	}

	void RenderWindow::OnSize(int width, int height)
	{
		if(_renderer)
		{
			_width = width;
			_height = height;
			_renderer->Viewport(0, 0, width, height);
			engineAPI->world->GetCamera().Perspective(60.0f, float(width) / height, 0.1f, 10000.0f);
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
			if(modifiers & MK_SHIFT)
			{
				_rotLightX += (y - _prevY);
				_rotLightY += (x - _prevX);

				if(_rotLightX > 360.0f)
					_rotLightX -= 360.0f;

				if(_rotLightY > 360.0f)
					_rotLightY -= 360.0f;
			}
			else
			{
				_rotX += (y - _prevY);
				_rotY += (x - _prevX);

				if(_rotX > 360.0f)
					_rotX -= 360.0f;

				if(_rotY > 360.0f)
					_rotY -= 360.0f;
			}

			OnPaint();
		}
		else if(_middleBtnDown && (modifiers & MK_MBUTTON))
		{
			_panX -= (x - _prevX) * 0.001f * _zoom;
			_panY += (y - _prevY) * 0.001f * _zoom;

			OnPaint();
		}
		else if(_rightBtnDown && (modifiers & MK_RBUTTON))
		{
			_zoom += (x - _prevX) * 0.05f;
			float z = _zoom;
			clamp(z, 1.0f, 100.0f);
			_zoom = z;

			OnPaint();
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

		OnPaint();
	}

	void RenderWindow::RenderGrid()
	{
		if(_gridVertBuf)
		{
			_renderer->ActiveVertexFormat(_lineVertFmt);
			_renderer->VertexSource(0, _gridVertBuf, sizeof(LineVert3D), 0);
			_renderer->IndexSource(0, TYPE_VOID);

			_renderer->ActiveVertexASMProgram(_vpSimple->GetASMProgram());
			_renderer->ActiveFragmentASMProgram(_fpConstClr->GetASMProgram());
			const mat4f& view_proj = engineAPI->world->GetCamera().GetViewProjectionTransform();
			mat4f wvp;
			mul(wvp, s_worldMat, view_proj);
			_vpSimple->GetASMProgram()->LocalMatrix4x4(0, wvp);
			float diff_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			_fpConstClr->GetASMProgram()->LocalParameter(0, diff_color);

			_renderer->Draw(PRIM_LINES, 0, (_gridVertCount + _gridHorizCount) * 2);
		}
	}

	void RenderWindow::RenderEntity()
	{
		if(_entity && _entity->GetModelRes())
		{
			if(_wireframe)
				_renderer->RasterizationMode(GL::RASTER_LINE);

			_renderer->FrontFace(GL::ORIENT_CCW);
			_renderer->CullFace(GL::FACE_BACK);
			_renderer->EnableFaceCulling(true);
			_entity->SetWorldTransform(s_worldMat);
			_renderSystem->RenderEntities(_animate? _frameTime: 0);

			if(_wireframe)
				_renderer->RasterizationMode(GL::RASTER_FILL);
		}
	}

	void RenderWindow::RenderStats()
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

		if(_entity && _entity->GetModelRes() && _entity->GetModelRes()->GetModel())
		{
			const Model* model = _entity->GetModelRes()->GetModel();

			// mesh count
			width = _font->GetTextWidth("Meshes: ");
			sprintf(buf, "%d", model->GetMeshCount());
			_renderSystem->GetRender2D()->DrawText("Meshes: ", x, y, *_font, color);
			_renderSystem->GetRender2D()->DrawText(buf, x + width, y, *_font, color);
			y -= height;

			// triangle count
			width = _font->GetTextWidth("Triangles: ");
			sprintf(buf, "%d", model->GetIndexCount() / 3);
			_renderSystem->GetRender2D()->DrawText("Triangles: ", x, y, *_font, color);
			_renderSystem->GetRender2D()->DrawText(buf, x + width, y, *_font, color);
			y -= height;

			// vertex count
			width = _font->GetTextWidth("Vertices: ");
			sprintf(buf, "%d", model->GetVertexCount());
			_renderSystem->GetRender2D()->DrawText("Vertices: ", x, y, *_font, color);
			_renderSystem->GetRender2D()->DrawText(buf, x + width, y, *_font, color);
			y -= height;
		}

		_renderSystem->GetRender2D()->FlushText();
		_renderer->EnableDepthTest(true);
	}

	void RenderWindow::RenderSkelet(const vec3f& color)
	{
		if(!_entity->GetModelRes())
			return;

		const Model& model = *_entity->GetModelRes()->GetModel();
		if(!model.GetRootJoint())
			return;

		const Animation* animation = _entity->GetCurrentAnimation()? _entity->GetCurrentAnimation()->animation->GetAnimation(): 0;

		mat4f wvp, matrix;
		const mat4f& view_proj = engineAPI->world->GetCamera().GetViewProjectionTransform();
		mul(wvp, s_worldMat, view_proj);

		_renderer->ActiveVertexFormat(_lineVertFmt);
		_renderer->VertexSource(0, _gridVertBuf, sizeof(LineVert3D), 0);
		_renderer->IndexSource(0, TYPE_VOID);

		_renderer->ActiveVertexASMProgram(_vpSimple->GetASMProgram());
		_renderer->ActiveFragmentASMProgram(_fpConstClr->GetASMProgram());

		_vpSimple->GetASMProgram()->LocalMatrix4x4(0, wvp);
		_fpConstClr->GetASMProgram()->LocalParameter(0, vec4f(color));

		const StaticArray<Joint>& joints = model.GetJoints();
		const mat4f* joint_transforms = _entity->GetJointTransforms().GetData();
		for(size_t i = 0; i < joints.GetCount(); ++i)
		{
			if(animation)
			{
				mul(matrix, joint_transforms[i], wvp);
				RenderBone(&joints[i], matrix);
			}
			else
			{
				RenderBone(&joints[i], wvp);
			}
		}
	}

	void RenderWindow::RenderBone(const Joint* joint, const mat4f& matrix)
	{
		if(!joint)
			return;

		mat4f mat;

		if(joint->child)
		{
			Joint* ptr = joint->child;
			while(ptr)
			{
				mat4f temp = joint->offsetMatrix, temp2 = ptr->offsetMatrix;
				temp.inverse();
				temp2.inverse();

				vec3f vec1 = temp.row3.rvec3;
				vec3f vec2 = temp2.row3.rvec3;
				vec3f v = vec2 - vec1;
				float len = v.length();
				v.normalize();
				quatf q(acos(v.x), acos(v.y), acos(v.z));
				mat4f scale, rot;
				scale.set_scale(len - 2 * _jointRadius, _jointRadius, _jointRadius);
				scale.row3.set(joint->jointMatrix.row3.rvec3);
				scale.row3.x += _jointRadius;
				q.to_matrix(rot);
				rot.set_identity();
				mul(temp, scale, matrix);
				temp = matrix;
				_vpSimple->GetASMProgram()->LocalMatrix4x4(0, temp);

				glBegin(GL_LINES);
					glVertex3fv(vec1);
					glVertex3fv(vec2);
				glEnd();

				ptr = ptr->sibling;
			}
		}

		// draw joint
		mat4f inv_bind;
		joint->offsetMatrix.get_inverse(inv_bind);
		mul(mat, inv_bind, matrix);
		mat4f temp;
		mat4f scale;
		scale.set_scale(_jointRadius, _jointRadius, _jointRadius);
		mul(temp, scale, mat);
		_vpSimple->GetASMProgram()->LocalMatrix4x4(0, temp);

		_renderer->VertexSource(0, _jointVertBuf, sizeof(LineVert3D), 0);
		_renderer->IndexSource(0, GL::TYPE_VOID);
		_renderer->Draw(GL::PRIM_LINE_LOOP, 0, 60);
		_renderer->Draw(GL::PRIM_LINE_LOOP, 60, 60);
		_renderer->Draw(GL::PRIM_LINE_LOOP, 120, 60);
	}

}
