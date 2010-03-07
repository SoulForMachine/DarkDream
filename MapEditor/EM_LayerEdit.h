
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class UndoManager;
	ref class LayersPanel;


	public ref class EM_LayerEdit: public EditMode
	{
	public:
		value struct Parameters
		{
			int activeLayer;
		};

		EM_LayerEdit(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager);
		~EM_LayerEdit();

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void LeftButtonUp(int x, int y) override;
		virtual void KeyDown(int key) override;
		virtual void Update(float dt) override;
		virtual void Render() override;

		int GetActiveLayer()
			{ return _activeLayer; }
		void SetActiveLayer(int index)
			{ _activeLayer = index; _selectedSprite = 0; }
		void SelectSprite(Engine::BgLayer::Sprite* sprite);

	private:
		LayersPanel^ _panel;
		UndoManager^ _undoManager;
		int _activeLayer;
		math3d::vec2f* _moveStartPoint;
		bool _moving;
		Engine::BgLayer::Sprite* _selectedSprite;
		GL::Renderer* _renderer;
		GL::Buffer* _vertBufSelRect;
		GL::VertexFormat* _vertFmtPos;
		Engine::VertexASMProgResPtr& _vertpSimple;
		Engine::FragmentASMProgResPtr& _fragpConstColor;
	};

}
