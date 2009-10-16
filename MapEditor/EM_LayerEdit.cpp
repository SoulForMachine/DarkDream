
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
	}

	EM_LayerEdit::~EM_LayerEdit()
	{
		delete _moveStartPoint;
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
			int viewport[4];
			engineAPI->renderSystem->GetRenderer()->GetViewport(viewport);
			y = viewport[3] - y;
			vec2f point;
			if(engineAPI->world->GetLayerManager().GetLayer(_activeLayer).PickLayerPoint(x, y, point))
			{
				vec2f vec = point - *_moveStartPoint;
				_selectedSprite->rect.Offset(vec.x, vec.y);
			}
			*_moveStartPoint = point;
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
	}

	void EM_LayerEdit::SelectSprite(BgLayer::Sprite* sprite)
	{
		_selectedSprite = sprite;
	}

}
