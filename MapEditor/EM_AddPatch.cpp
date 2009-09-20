
#include "stdafx.h"
#include "EM_AddPatch.h"

using namespace Engine;
using namespace math3d;
using namespace System::Windows::Forms;


namespace MapEditor
{

	EM_AddPatch::EM_AddPatch(EditModeEventListener^ listener, bool persistent)
			: EditMode(listener, persistent)
	{
		_isExecuting = false;

		_renderer = engineAPI->renderSystem->GetRenderer();
		_vertBuf = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, (Terrain::PATCH_HEIGHT + 1) * 2 * sizeof(vec3f), 0, GL::USAGE_DYNAMIC_DRAW);

		GL::VertexAttribDesc vert_fmt[] =
		{
			{ 0, 0, 3, GL::TYPE_FLOAT, false, false, 0 }
		};
		_vertFmt = _renderer->CreateVertexFormat(vert_fmt, COUNTOF(vert_fmt));

		_vertProg = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fragProg = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);
	}

	EM_AddPatch::~EM_AddPatch()
	{
		_renderer->DestroyBuffer(_vertBuf);
		_renderer->DestroyVertexFormat(_vertFmt);
		engineAPI->asmProgManager->ReleaseASMProgram(_vertProg);
		engineAPI->asmProgManager->ReleaseASMProgram(_fragProg);
	}

	void EM_AddPatch::Activate()
	{
		_patchIndex = -1;
	}

	void EM_AddPatch::MouseMove(int modifiers, int x, int y)
	{
		SetCursor(LoadCursor(0, IDC_ARROW));

		int vp_x, vp_y, vp_width, vp_height;
		engineAPI->renderSystem->GetRenderer()->GetViewport(vp_x, vp_y, vp_width, vp_height);
		vec3f point;
		_patchIndex = engineAPI->world->GetTerrain().PickTerrainPoint(x, vp_height - y, point);
		if(_patchIndex != -1)
		{
			float x = point.x - _patchIndex * Terrain::PATCH_WIDTH;
			if(x <= Terrain::PATCH_WIDTH / 4)
				;
			else if(x >= Terrain::PATCH_WIDTH * 3 / 4)
				_patchIndex++;
			else
				_patchIndex = -1;
		}
	}

	void EM_AddPatch::LeftButtonDown(int x, int y)
	{
		if(_patchIndex != -1)
		{
			if(engineAPI->world->GetTerrain().GetPatchCount() == Engine::Terrain::MAX_PATCHES)
			{
				MessageBox::Show(
					"Failed to add terrain patch - maximum of 32 patches reached.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);

				_evListener->EditModeEvent(EditModeEventListener::EMEvent::EDIT_ERROR);

				return;
			}

			if(engineAPI->world->GetTerrain().AddPatch(_patchIndex) == -1)
			{
				MessageBox::Show(
					"Failed to add terrain patch.", GetAppName(),
					MessageBoxButtons::OK, MessageBoxIcon::Error);

				_evListener->EditModeEvent(EditModeEventListener::EMEvent::EDIT_ERROR);

				return;
			}

			// if shift is not being held we exit this edit mode
			if(!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				_evListener->EditModeEvent(EditModeEventListener::EMEvent::EDIT_COMPLETE);
			}
		}
	}

	void EM_AddPatch::KeyDown(int key)
	{
		if(key == VK_ESCAPE)
		{
			_evListener->EditModeEvent(EditModeEventListener::EMEvent::EDIT_CANCELED);
		}
	}

	void EM_AddPatch::Render()
	{
		if(_patchIndex == -1)
			return;

		vec3f* vertices = (vec3f*)_vertBuf->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
		if(vertices)
		{
			const Terrain::TerrainPatch* patches = engineAPI->world->GetTerrain().GetPatches();
			if(_patchIndex == engineAPI->world->GetTerrain().GetPatchCount())
			{
				const Terrain::TerrainPatch& patch = patches[_patchIndex - 1];
				for(int i = 0; i <= Terrain::PATCH_HEIGHT; ++i)
				{
					float y = patch.elevation[i * (Terrain::PATCH_WIDTH + 1) + Terrain::PATCH_WIDTH];
					vertices[i * 2].set(patch.boundBox.maxPt.x, y - 15.0f, (float)i);
					vertices[i * 2 + 1].set(patch.boundBox.maxPt.x, y + 15.0f, (float)i);
				}
			}
			else
			{
				const Terrain::TerrainPatch& patch = patches[_patchIndex];
				for(int i = 0; i <= Terrain::PATCH_HEIGHT; ++i)
				{
					float y = patch.elevation[i * (Terrain::PATCH_WIDTH + 1)];
					vertices[i * 2].set(patch.boundBox.minPt.x, y - 15.0f, (float)i);
					vertices[i * 2 + 1].set(patch.boundBox.minPt.x, y + 15.0f, (float)i);
				}
			}

			if(_vertBuf->UnmapBuffer())
			{
				_renderer->ActiveVertexFormat(_vertFmt);
				_renderer->IndexSource(0, GL::TYPE_VOID);
				_renderer->VertexSource(0, _vertBuf, sizeof(vec3f), 0);
				_renderer->ActiveVertexASMProgram(_vertProg->GetASMProgram());
				_renderer->ActiveFragmentASMProgram(_fragProg->GetASMProgram());
				_vertProg->GetASMProgram()->LocalMatrix4x4(0, engineAPI->world->GetCamera().GetViewProjectionTransform());
				vec4f green(0.0f, 1.0f, 0.0f, 0.3f);
				_fragProg->GetASMProgram()->LocalParameter(0, green);

				_renderer->EnableBlending(true);
				_renderer->BlendingFunc(GL::BLEND_FUNC_SRC_ALPHA, GL::BLEND_FUNC_ONE_MINUS_SRC_ALPHA);
				_renderer->EnableFaceCulling(false);

				_renderer->Draw(GL::PRIM_TRIANGLE_STRIP, 0, (Terrain::PATCH_HEIGHT + 1) * 2);

				_renderer->EnableFaceCulling(true);
				_renderer->EnableBlending(false);
			}
		}
	}

}
