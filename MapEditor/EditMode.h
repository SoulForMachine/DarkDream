
#pragma once


namespace MapEditor
{

	public interface class EditModeEventListener
	{
		enum class EMEvent
		{
			EDIT_COMPLETE,
			EDIT_CANCELED,
			EDIT_ERROR,
		};

		void EditModeEvent(EMEvent ev);
	};


	public ref class EditMode abstract
	{
	public:
		enum class EditModeEnum
		{
			VIEW,
			TERRAIN_EDIT,
			ADD_PATCH,
			REMOVE_PATCH,

			EDIT_MODE_COUNT
		};

		EditMode(EditModeEventListener^ listener, bool persistent)
		{
			_isExecuting = false;
			_evListener = listener;
			_persistent = persistent;
		}

		virtual System::Windows::Forms::UserControl^ GetPanel() { return nullptr; }
		virtual EditModeEnum GetModeEnum() = 0;

		virtual void Activate() {}
		virtual void MouseMove(int modifiers, int x, int y) { SetCursor(LoadCursor(0, IDC_ARROW)); }
		virtual void LeftButtonDown(int x, int y) {}
		virtual void RightButtonDown(int x, int y) {}
		virtual void MiddleButtonDown(int x, int y) {}
		virtual void LeftButtonUp(int x, int y) {}
		virtual void RightButtonUp(int x, int y) {}
		virtual void MiddleButtonUp(int x, int y) {}
		virtual void MouseWheel(int delta, int x, int y) {}
		virtual void KeyDown(int key) {}
		virtual void KeyUp(int key) {}
		virtual void Update(float dt) {}
		virtual void Render() {}

		bool IsExecuting()
			{ return _isExecuting; }
		bool IsPersistent()
			{ return _persistent; }

	protected:
		/*
			Set this flag to true if an edit operation is currently beeing performed
			and it lasts over some period of time, typically with user interaction.
			If the operation is instant and doesn't require user interaction, don't
			set this flag.
		*/
		bool _isExecuting;
		bool _persistent;
		EditModeEventListener^ _evListener;
	};

}
