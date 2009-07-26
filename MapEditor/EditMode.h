
#pragma once


namespace MapEditor
{

	public ref class EditMode abstract
	{
	public:
		enum class EditModeEnum
		{
			VIEW,
			TERRAIN_EDIT,
		};

		EditMode()
			{ _isExecuting = false; }

		virtual System::Windows::Forms::UserControl^ GetPanel() = 0;
		virtual EditModeEnum GetModeEnum() = 0;

		virtual void MouseMove(int modifiers, int x, int y) {}
		virtual void LeftButtonDown(int x, int y) {}
		virtual void RightButtonDown(int x, int y) {}
		virtual void MiddleButtonDown(int x, int y) {}
		virtual void LeftButtonUp(int x, int y) {}
		virtual void RightButtonUp(int x, int y) {}
		virtual void MiddleButtonUp(int x, int y) {}
		virtual void MouseWheel(int delta, int x, int y) {}
		virtual void KeyDown(int key) {}
		virtual void KeyUp(int key) {}
		virtual void Update() {}
		virtual void Render() {}
		
		bool IsExecuting()
			{ return _isExecuting; }

	protected:
		bool _isExecuting;
	};

}
