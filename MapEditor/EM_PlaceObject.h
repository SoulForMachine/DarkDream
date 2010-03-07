#pragma once

#include "EditMode.h"
#include "UndoManager.h"



namespace MapEditor
{
	ref class PlaceObjectPanel;
	ref class ActionPlaceObjects;


	public ref class EM_PlaceObject: public EditMode, public UndoEventListener
	{
	public:
		enum class TransformType
		{
			MOVE_XZ,
			MOVE_Y,
			ROTATE,
			DROP,
		};

		value struct Parameters
		{
			TransformType transformType;
			float translX;
			float translY;
			float translZ;
			float rotateY;
			List<Engine::RenderableEntity*>* selectedEntities;
		};

		EM_PlaceObject(EditModeEventListener^ listener, bool persistent, UndoManager^ undo_manager);
		~EM_PlaceObject();

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
		enum class SelectMode
		{
			NEW_SELECTION,
			ADD_TO_SELECTION,
			INVERT_SELECTION,
		};

		enum class GizmoType
		{
			NONE,
			MOVE_XZ,
			MOVE_Y,
			ROTATE
		};

		void AddObject(int x, int y);
		void SelectObjects();
		void DeleteObjects();
		bool BBoxInSelRect(const OBBox& bbox, const math3d::vec4f planes[4]);
		void UpdateSelectionRect(int x, int y);
		void SelectEntity(Engine::RenderableEntity* entity, SelectMode mode);
		GizmoType MouseOverGizmo(int x, int y);

		PlaceObjectPanel^ _panel;
		UndoManager^ _undoManager;
		ActionPlaceObjects^ _actionPlaceObjs;
		Parameters^ _parameters;
		List<Engine::RenderableEntity*>* _selectedEntities;
		System::Drawing::Rectangle _selectionRect;
		System::Drawing::Point _mouseStartPoint;
		// used for moving on xz
		math3d::vec3f* _moveStartPoint;
		math3d::vec3f* _moveEndPoint;
		bool _selecting;
		bool _selectOne;
		bool _placing;
		GL::Renderer* _renderer;
		GL::Buffer* _vertBufSelRect;
		GL::Buffer* _indBufSelRect;
		GL::Buffer* _vertBufSelMark;
		GL::VertexFormat* _vertFmtPos;
		Engine::VertexASMProgResPtr& _vertpSimple2D;
		Engine::VertexASMProgResPtr& _vertpSimple;
		Engine::FragmentASMProgResPtr& _fragpConstColor;
		HCURSOR	_cursorMove;
		HCURSOR	_cursorMoveUD;
		HCURSOR	_cursorRotate;
	};

}
