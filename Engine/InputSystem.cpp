
#include "BaseLib/Input.h"
#include "BaseLib/Console.h"
#include "InputSystem.h"

using namespace Memory;

namespace Engine
{

	InputSystem::InputSystem():
		_inputEventPool(MAX_INPUT_EVENT_QUEUE_SIZE),
		_inputEventQueue(_inputEventPool)
	{
		
	}

	bool InputSystem::Init(void* instance_handle, void* window_handle)
	{
		Console::PrintLn("----- Initializing input system -----");

		if(!Input::Init(instance_handle, window_handle))
		{
			Console::PrintError("Failed to initialize input system.");
			return false;
		}

		return true;
	}
	
	void InputSystem::Deinit()
	{
		Console::PrintLn("----- Deinitializing input system -----");

		Input::Deinit();
	}

	void InputSystem::AddInputEvent(const InputEvent& in_event)
	{
		if(_inputEventQueue.GetCount() < MAX_INPUT_EVENT_QUEUE_SIZE)
			_inputEventQueue.PushBack(in_event);
	}

	bool InputSystem::GetInputEvent(InputEvent& in_event)
	{
		bool result = false;
		if(!_inputEventQueue.IsEmpty())
		{
			in_event = _inputEventQueue.GetHead();
			_inputEventQueue.PopFront();
			result = true;
		}
		return result;
	}

	void InputSystem::ClearInputEventQueue()
	{
		_inputEventQueue.Clear();
	}

	void InputSystem::AcquireDevices()
	{
		Input::AcquireDevices();
	}

	void InputSystem::DoConsoleInput()
	{
		Input::GetKeyboard()->UpdateState();

		InputEvent ev;
		while(GetInputEvent(ev))
		{
			if(ev.ch)
			{
				if(ev.ch >= 32 && ev.ch <= 126)
					Console::InputChar((char)ev.ch);
			}
			else
			{
				if(ev.pressed)
				{
					switch(ev.code)
					{
					case Input::KEY_TAB:
						Console::AutoCompleteInput();
						break;
					case Input::KEY_RETURN:
						Console::ExecuteStatement();
						break;
					case Input::KEY_BACK:
						Console::DeletePrevChar();
						break;
					case Input::KEY_DELETE:
						Console::DeleteChar();
						break;
					case Input::KEY_NEXT:
						if(Input::GetKeyboard()->GetKeyState(Input::KEY_LSHIFT) || Input::GetKeyboard()->GetKeyState(Input::KEY_RSHIFT))
							Console::Scroll(3);
						else
							Console::Scroll(1);
						break;
					case Input::KEY_PRIOR:
						if(Input::GetKeyboard()->GetKeyState(Input::KEY_LSHIFT) || Input::GetKeyboard()->GetKeyState(Input::KEY_RSHIFT))
							Console::Scroll(-3);
						else
							Console::Scroll(-1);
						break;
					case Input::KEY_HOME:
						if(Input::GetKeyboard()->GetKeyState(Input::KEY_LCONTROL) || Input::GetKeyboard()->GetKeyState(Input::KEY_RCONTROL))
							Console::ScrollToBeginning();
						else
							Console::MoveInputPosBeg();
						break;
					case Input::KEY_END:
						if(Input::GetKeyboard()->GetKeyState(Input::KEY_LCONTROL) || Input::GetKeyboard()->GetKeyState(Input::KEY_RCONTROL))
							Console::ScrollToEnd();
						else
							Console::MoveInputPosEnd();
						break;
					case Input::KEY_LEFT:
						if(Input::GetKeyboard()->GetKeyState(Input::KEY_LCONTROL) || Input::GetKeyboard()->GetKeyState(Input::KEY_RCONTROL))
							Console::MoveInputPosPrevWord();
						else
							Console::MoveInputPosLeft();
						break;
					case Input::KEY_RIGHT:
						if(Input::GetKeyboard()->GetKeyState(Input::KEY_LCONTROL) || Input::GetKeyboard()->GetKeyState(Input::KEY_RCONTROL))
							Console::MoveInputPosNextWord();
						else
							Console::MoveInputPosRight();
						break;
					case Input::KEY_UP:
						Console::CmdHistoryBack();
						break;
					case Input::KEY_DOWN:
						Console::CmdHistoryForward();
						break;
					}
				}
				else
				{
					switch(ev.code)
					{
					case Input::KEY_INSERT:
						Console::ToggleInputMode();
						break;
					}
				}
			}
		}
	}

}
