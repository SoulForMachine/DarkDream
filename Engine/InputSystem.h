
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include "BaseLib/FreeList.h"
#include "Engine/Common.h"

template <class _Type> class FreePool;
template <class _Type> class FreeList;
template <class _Type> struct FreeListNode;
class Input;

namespace Engine
{

	class ENGINE_API InputSystem
	{
	public:
		struct InputEvent
		{
			uchar code; // key code or mouse button number
			char ch; // character code; 0 if it is not character key
			bool pressed; // true if pressed, false if released
		};

		InputSystem();

		bool Init(void* instance_handle, void* window_handle);
		void Deinit();
		void AddInputEvent(const InputEvent& in_event);
		bool GetInputEvent(InputEvent& in_event);
		void ClearInputEventQueue();
		void AcquireDevices();
		void DoConsoleInput();

	private:
		static const int MAX_INPUT_EVENT_QUEUE_SIZE = 32;
		FreePool<FreeListNode<InputEvent> > _inputEventPool;
		FreeList<InputEvent> _inputEventQueue;
	};

}

#endif // _INPUT_SYSTEM_H_
