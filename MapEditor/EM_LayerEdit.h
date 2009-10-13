
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class UndoManager;
	ref class LayersPanel;


	public ref class EM_LayerEdit: public EditMode
	{
	public:
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

	private:
		LayersPanel^ _panel;
		UndoManager^ _undoManager;
	};

}
