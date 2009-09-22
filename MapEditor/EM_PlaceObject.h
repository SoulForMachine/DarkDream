#pragma once

#include "EditMode.h"



namespace MapEditor
{
	ref class PlaceObjectPanel;


	public ref class EM_PlaceObject: public EditMode
	{
	public:
		EM_PlaceObject(EditModeEventListener^ listener, bool persistent);
		~EM_PlaceObject();

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		virtual void Activate() override;
		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;

	private:
		PlaceObjectPanel^ _panel;
	};

}
