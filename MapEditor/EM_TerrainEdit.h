
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class UndoManager;
	ref class TerrainEditPanel;
	ref class ActionTerrainEdit;


	public ref class EM_TerrainEdit: public EditMode
	{
	public:
		enum class EditType
		{
			RAISE_LOWER,
			SMOOTH,
			NOISE,
			PLATEAU,
			RELATIVE_PLATEAU,
			RAMP,
		};

		ref struct Parameters
		{
			EditType editType;
			float radius;
			float hardness;
			float strength;
			float height;
		};

		EM_TerrainEdit(UndoManager^ undo_manager);

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void LeftButtonUp(int x, int y) override;
		virtual void KeyDown(int key) override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		TerrainEditPanel^ _panel;
		ActionTerrainEdit^ _action;
		Parameters^ _parameters;
		UndoManager^ _undoManager;
	};

}