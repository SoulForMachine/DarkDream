
#pragma once

#include "EditMode.h"


namespace MapEditor
{

	ref class UndoManager;
	ref class TerrainEditPanel;
	ref class ActionTerrainEdit;
	ref class TerrainBrush;


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
			float strength; // strength with which brush pulls vertices
			float height;
			// world-space position of brush center
			float posX;
			float posY;
			float posZ;
			// brush center start position (used for ramp brush only)
			float startPosX;
			float startPosY;
			float startPosZ;
			bool executing;
		};

		EM_TerrainEdit(UndoManager^ undo_manager);
		~EM_TerrainEdit();

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void LeftButtonUp(int x, int y) override;
		virtual void KeyDown(int key) override;
		virtual void Update(float dt) override;
		virtual void Render() override;

	private:
		TerrainEditPanel^ _panel;
		ActionTerrainEdit^ _action;
		Parameters^ _parameters;
		UndoManager^ _undoManager;
		TerrainBrush^ _brush;
		bool _overTerrain;
	};

}
