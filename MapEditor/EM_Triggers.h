#pragma once

#include "EditMode.h"
#include "UndoManager.h"



namespace MapEditor
{

	ref class TriggerPanel;


	public ref class EM_Triggers: public EditMode, public UndoEventListener
	{
	public:
		EM_Triggers(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager);
		~EM_Triggers();

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		virtual void Activate() override;
		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void LeftButtonUp(int x, int y) override;
		virtual void KeyDown(int key) override;
		virtual void Update(float dt) override;
		virtual void Render() override;

		virtual void UndoEvent(UndoEventListener::EventType type, Action^ action);

		void ClearSelection();
		void DropSelected();

	private:
		TriggerPanel^ _panel;
		UndoManager^ _undoManager;
	};

}
