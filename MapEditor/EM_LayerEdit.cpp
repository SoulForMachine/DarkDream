
#include "stdafx.h"
#include "LayersPanel.h"
#include "EM_LayerEdit.h"

using namespace math3d;
using namespace Memory;
using namespace Engine;


namespace MapEditor
{

	EM_LayerEdit::EM_LayerEdit(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager)
		: EditMode(listener, persistent)
	{
		_panel = gcnew LayersPanel(this);
		_undoManager = undo_manager;
		_activeLayer = 0;
		_moveStartPoint = new(mainPool) vec2f;
		_moving = false;
		_selectedSprite = 0;

		_renderer = engineAPI->renderSystem->GetRenderer();
		_vertBufSelRect = _renderer->CreateBuffer(GL::OBJ_VERTEX_BUFFER, 4 * sizeof(vec4f), 0, GL::USAGE_DYNAMIC_DRAW);

		GL::VertexAttribDesc desc[] =
		{
			{ 0, 0, 4, GL::TYPE_FLOAT, false, false, 0 }
		};
		_vertFmtPos = _renderer->CreateVertexFormat(desc, COUNTOF(desc));;

		_vertpSimple = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/Simple.vp"), true);
		_fragpConstColor = engineAPI->asmProgManager->CreateASMProgram(_t("Programs/ConstColor.fp"), true);
	}

	EM_LayerEdit::~EM_LayerEdit()
	{
		delete _moveStartPoint;

		_renderer->DestroyBuffer(_vertBufSelRect);
		_renderer->DestroyVertexFormat(_vertFmtPos);
		engineAPI->asmProgManager->ReleaseASMProgram(_vertpSimple);
		engineAPI->asmProgManager->ReleaseASMProgram(_fragpConstColor);
	}

	System::Windows::Forms::UserControl^ EM_LayerEdit::GetPanel()
	{
		return _panel;
	}

	EditMode::EditModeEnum EM_LayerEdit::GetModeEnum()
	{
		return EditMode::EditModeEnum::LAYER_EDIT;
	}

	void EM_LayerEdit::MouseMove(int modifiers, int x, int y)
	{
		SetCursor(LoadCursor(0, IDC_ARROW));

		if(_moving && _selectedSprite)
		{
			if(! ((_selectedSprite->flags & BgLayer::Sprite::FLAG_TILE_U) &&
				(_selectedSprite->flags & BgLayer::Sprite::FLAG_TILE_V)) )
			{
				int viewport[4];
				engineAPI->renderSystem->GetRenderer()->GetViewport(viewport);
				y = viewport[3] - y;
				vec2f point;
				if(engineAPI->world->GetLayerManager().GetLayer(_activeLayer).PickLayerPoint(x, y, point))
				{
					vec2f vec = point - *_moveStartPoint;
					if(_selectedSprite->flags & BgLayer::Sprite::FLAG_TILE_U)
						_selectedSprite->rect.Offset(0.0f, vec.y);
					else if(_selectedSprite->flags & BgLayer::Sprite::FLAG_TILE_V)
						_selectedSprite->rect.Offset(vec.x, 0.0f);
					else
						_selectedSprite->rect.Offset(vec.x, vec.y);
				}
				*_moveStartPoint = point;
			}
		}
	}

	void EM_LayerEdit::LeftButtonDown(int x, int y)
	{
		int viewport[4];
		engineAPI->renderSystem->GetRenderer()->GetViewport(viewport);
		y = viewport[3] - y;
		vec2f point;
		BgLayer::Sprite* sprite = engineAPI->world->GetLayerManager().GetLayer(_activeLayer).PickSprite(x, y, point);
		SelectSprite(sprite);

		if(sprite)
		{
			_moving = true;
			*_moveStartPoint = point;
		}
	}

	void EM_LayerEdit::LeftButtonUp(int x, int y)
	{
		_moving = false;
	}

	void EM_LayerEdit::KeyDown(int key)
	{
	}

	void EM_LayerEdit::Update(float dt)
	{
	}

	void EM_LayerEdit::Render()
	{
		if(_selectedSprite)
		{
			vec4f* vertices = (vec4f*)_vertBufSelRect->MapBuffer(GL::ACCESS_WRITE_ONLY, true);
			if(vertices)
			{
				BgLayer& layer = engineAPI->world->GetLayerManager().GetLayer(_selectedSprite->layerIndex);
				const vec3f& cam_pos = engineAPI->world->GetCamera().GetPosition();

				float x1 = layer.LayerXToWorldX(_selectedSprite->rect.x1);
				float y1 = _selectedSprite->rect.y1;
				float x2 = layer.LayerXToWorldX(_selectedSprite->rect.x2);
				float y2 = _selectedSprite->rect.y2;
				float z = - layer.GetCameraDistance() + cam_pos.z;

				vertices[0].set(x1, y1, z, 1.0f);
				vertices[1].set(x2, y1, z, 1.0f);
				vertices[2].set(x2, y2, z, 1.0f);
				vertices[3].set(x1, y2, z, 1.0f);

				if(_vertBufSelRect->UnmapBuffer())
				{
					_renderer->ActiveVertexFormat(_vertFmtPos);
					_renderer->VertexSource(0, _vertBufSelRect, sizeof(vec4f), 0);
					_renderer->IndexSource(0, GL::TYPE_VOID);
					_renderer->ActiveVertexASMProgram(_vertpSimple->GetASMProgram());
					_renderer->ActiveFragmentASMProgram(_fragpConstColor->GetASMProgram());

					_vertpSimple->GetASMProgram()->LocalMatrix4x4(0, engineAPI->world->GetCamera().GetViewProjectionTransform());

					float color[] = { 0.0f, 1.0f, 0.0f, 1.0f };
					_fragpConstColor->GetASMProgram()->LocalParameter(0, color);

					_renderer->Draw(GL::PRIM_LINE_LOOP, 0, 4);
				}
			}
		}
	}

	void EM_LayerEdit::SelectSprite(BgLayer::Sprite* sprite)
	{
		_selectedSprite = sprite;
		_panel->SelectSprite(sprite);
	}

}
