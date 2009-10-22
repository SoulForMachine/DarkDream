
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class UndoManager;
	ref class GrassPanel;
	ref class ActionPaintGrass;
	ref class GrassBrush;


	public ref class EM_PaintGrass: public EditMode
	{
	public:
		ref struct Parameters
		{
			float radius;
			float hardness;
			float strength; // strength with which brush raises grass
			float scale;
			// world-space position of brush center
			float posX;
			float posY;
			float posZ;
			bool executing;
		};

		EM_PaintGrass(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager);
		~EM_PaintGrass();

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void LeftButtonUp(int x, int y) override;
		virtual void KeyDown(int key) override;
		virtual void Update(float dt) override;
		virtual void Render() override;

	private:
		GrassPanel^ _panel;
		ActionPaintGrass^ _action;
		Parameters^ _parameters;
		UndoManager^ _undoManager;
		GrassBrush^ _brush;
		bool _overTerrain;
	};

}
