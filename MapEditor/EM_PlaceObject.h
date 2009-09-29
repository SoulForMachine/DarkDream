#pragma once

#include "EditMode.h"
#include "UndoManager.h"



namespace MapEditor
{
	ref class PlaceObjectPanel;


	public ref class EM_PlaceObject: public EditMode, public UndoEventListener
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

		virtual void UndoEvent(UndoEventListener::EventType type);

		void ClearSelection();

	private:
		enum class SelectMode
		{
			NEW_SELECTION,
			ADD_TO_SELECTION,
			INVERT_SELECTION,
		};

		void AddObject(int x, int y);
		void SelectObjects();
		void DeleteObjects();
		bool BBoxInSelRect(const AABBox& bbox, const math3d::vec4f planes[4]);
		void UpdateSelectionRect(int x, int y);
		void SelectEntity(Engine::ModelEntity* entity, SelectMode mode);

		PlaceObjectPanel^ _panel;
		UndoManager^ _undoManager;
		List<Engine::ModelEntity*>* _selectedEntities;
		System::Drawing::Rectangle _selectionRect;
		System::Drawing::Point _selStartPoint;
		bool _selecting;
		GL::Renderer* _renderer;
		GL::Buffer* _vertBufSelRect;
		GL::Buffer* _indBufSelRect;
		GL::Buffer* _vertBufSelMark;
		GL::VertexFormat* _vertFmtPos;
		const Engine::ASMProgRes* _vertpSimple2D;
		const Engine::ASMProgRes* _vertpSimple;
		const Engine::ASMProgRes* _fragpConstColor;
	};

}
