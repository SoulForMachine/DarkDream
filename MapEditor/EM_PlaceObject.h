#pragma once

#include "EditMode.h"



namespace MapEditor
{
	ref class PlaceObjectPanel;
	ref class UndoManager;


	public ref class EM_PlaceObject: public EditMode
	{
	public:
		EM_PlaceObject(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager);
		~EM_PlaceObject();

		virtual System::Windows::Forms::UserControl^ GetPanel() override;
		virtual EditModeEnum GetModeEnum() override;

		virtual void Activate() override;
		virtual void MouseMove(int modifiers, int x, int y) override;
		virtual void LeftButtonDown(int x, int y) override;
		virtual void LeftButtonUp(int x, int y) override;
		virtual void KeyDown(int key) override;
		virtual void Render() override;

		void ClearSelection();

	private:
		void AddObject(int x, int y);
		void SelectObjects();
		void DeleteObjects();
		bool BBoxInSelRect(const AABBox& bbox, const math3d::vec4f planes[4]);

		PlaceObjectPanel^ _panel;
		UndoManager^ _undoManager;
		List<Engine::ModelEntity*>* _selectedEntities;
		System::Drawing::Rectangle _selectionRect;
		bool _selecting;
		GL::Renderer* _renderer;
	};

}
