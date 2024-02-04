
#include "Win32Common.h"
#include "Memory.h"
#include "Console.h"
#include "Input.h"

#ifdef _MSC_VER
	#pragma comment(lib, "dinput8.lib")
	#pragma comment(lib, "dxguid.lib")
#endif

#define SAFE_RELEASE(p) {if(p){p->Release(); p = 0;}}


using namespace Memory;


struct KeyNames
{
	uchar code;
	const char* name;
};

KeyNames keyList[] = 
{
	DIK_ESCAPE, "Escape",
	DIK_1, "1",
	DIK_2, "2",
	DIK_3, "3",
	DIK_4, "4",
	DIK_5, "5",
	DIK_6, "6",
	DIK_7, "7",
	DIK_8, "8",
	DIK_9, "9",
	DIK_0, "0",
	DIK_MINUS, "-",
	DIK_EQUALS, "=",
	DIK_BACK, "Backspace",
	DIK_TAB, "Tab",
	DIK_Q, "Q",
	DIK_W, "W",
	DIK_E, "E",
	DIK_R, "R",
	DIK_T, "T",
	DIK_Y, "Y",
	DIK_U, "U",
	DIK_I, "I",
	DIK_O, "O",
	DIK_P, "P",
	DIK_LBRACKET, "[",
	DIK_RBRACKET, "]",
	DIK_RETURN, "Enter",
	DIK_LCONTROL, "Left control",
	DIK_A, "A",
	DIK_S, "S",
	DIK_D, "D",
	DIK_F, "F",
	DIK_G, "G",
	DIK_H, "H",
	DIK_J, "J",
	DIK_K, "K",
	DIK_L, "L",
	DIK_SEMICOLON, ";",
	DIK_APOSTROPHE, "'",
	DIK_GRAVE, "`",
	DIK_LSHIFT, "Left shift",
	DIK_BACKSLASH, "\\",
	DIK_Z, "Z",
	DIK_X, "X",
	DIK_C, "C",
	DIK_V, "V",
	DIK_B, "B",
	DIK_N, "N",
	DIK_M, "M",
	DIK_COMMA, ",",
	DIK_PERIOD, ".",
	DIK_SLASH, "/",
	DIK_RSHIFT, "Right shift",
	DIK_MULTIPLY, "Num. multiply",
	DIK_LMENU, "Left alt",
	DIK_SPACE, "Space",
	DIK_CAPITAL, "Caps lock",
	DIK_F1, "F1",
	DIK_F2, "F2",
	DIK_F3, "F3",
	DIK_F4, "F4",
	DIK_F5, "F5",
	DIK_F6, "F6",
	DIK_F7, "F7",
	DIK_F8, "F8",
	DIK_F9, "F9",
	DIK_F10, "F10",
	DIK_NUMLOCK, "Num. lock",
	DIK_SCROLL, "Scroll lock",
	DIK_NUMPAD7, "Num. 7",
	DIK_NUMPAD8, "Num. 8",
	DIK_NUMPAD9, "Num. 9",
	DIK_SUBTRACT, "Num. minus",
	DIK_NUMPAD4, "Num. 4",
	DIK_NUMPAD5, "Num. 5",
	DIK_NUMPAD6, "Num. 6",
	DIK_ADD, "Num. plus",
	DIK_NUMPAD1, "Num. 1",
	DIK_NUMPAD2, "Num. 2",
	DIK_NUMPAD3, "Num. 3",
	DIK_NUMPAD0, "Num. 0",
	DIK_DECIMAL, "Num. decimal",
	DIK_F11, "F11",
	DIK_F12, "F12",
	DIK_F13, "F13",
	DIK_F14, "F14",
	DIK_F15, "F15",
	DIK_NUMPADENTER, "Num. enter",
	DIK_RCONTROL, "Right control",
	DIK_DIVIDE, "Num. divide",
	DIK_SYSRQ, "SysRq",
	DIK_RMENU, "Right alt",
	DIK_HOME, "Home",
	DIK_UP, "Up arrow",
	DIK_PRIOR, "Page up",
	DIK_LEFT, "Left arrow",
	DIK_RIGHT, "Right arrow",
	DIK_END, "End",
	DIK_DOWN, "Down arrow",
	DIK_NEXT, "Page down",
	DIK_INSERT, "Insert",
	DIK_DELETE, "Delete",
	DIK_LWIN, "Left win",
	DIK_RWIN, "Right win",
	DIK_APPS, "Apps",
	DIK_PAUSE, "Pause"
};


Input::Keyboard* Input::_keyboard;
Input::Mouse* Input::_mouse;
Input::GameController* Input::_gameControllers;
int Input::_numGameControllers;
LPDIRECTINPUT8 Input::_directInput;
HWND Input::_windowHandle;

/* 
Initializes input devices, assumes there is always a keyboard and a mouse,
enumerates all joysticks
*/
bool Input::Init(void* instance_handle, void* window_handle)
{
	HRESULT result;

	result = DirectInput8Create((HINSTANCE)instance_handle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, 0);

	if(FAILED(result))
	{
		Console::PrintError("Failed to create direct input interface.");
		return false;
	}

	_windowHandle = (HWND)window_handle;

	// create devices
	_keyboard = New<Keyboard>(mainPool);
	if(!_keyboard->Init(_directInput, _windowHandle))
	{
		Console::PrintError("Failed to initialize the keyboard.");
		Deinit();
		return false;
	}

	_mouse = New<Mouse>(mainPool);
	if(!_mouse->Init(_directInput, _windowHandle))
	{
		Console::PrintError("Failed to initialize the mouse.");
		Deinit();
		return false;
	}

	// enumerate and create joystick devices
	_gameControllers = NewArray<GameController>(mainPool, MAX_NUM_GAME_CONTROLLERS);
	result = _directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, InitGameControllersCallback, 0, DIEDFL_ATTACHEDONLY);

	return true;
}

void Input::Deinit()
{
	if(_keyboard)
		_keyboard->Deinit();
	if(_mouse)
		_mouse->Deinit();
	if(_gameControllers)
		for(int i = 0; i < _numGameControllers; ++i)
			_gameControllers[i].Deinit();

	Delete(_keyboard);
	Delete(_mouse);
	Memory::Delete(_gameControllers);

	SAFE_RELEASE(_directInput);
}

Input::Keyboard* Input::GetKeyboard()
{
	return _keyboard;
}

Input::Mouse* Input::GetMouse()
{
	return _mouse;
}

Input::GameController* Input::GetGameController(int index)
{
	return &_gameControllers[index];
}

int Input::GetGameControllerCount()
{
	return _numGameControllers;
}

void Input::AcquireDevices()
{
	_keyboard->Acquire();
	_mouse->Acquire();
	for(int i = 0; i < _numGameControllers; ++i)
		_gameControllers[i].Acquire();
}

const char* Input::GetKeyName(uchar code)
{
	for(int i = 0; i < COUNTOF(keyList); ++i)
	{
		if(code == keyList[i].code)
			return keyList[i].name;
	}

	return 0;
}

uchar Input::GetKeyCode(const char* name)
{
	for(int i = 0; i < COUNTOF(keyList); ++i)
	{
		if(stricmp(name, keyList[i].name) == 0)
			return keyList[i].code;
	}

	return 0;
}

BOOL CALLBACK Input::InitGameControllersCallback(const DIDEVICEINSTANCE* instance, void* data)
{
	GameController& gc = _gameControllers[_numGameControllers];
	if(gc.Init(_directInput, _windowHandle, instance->guidInstance))
	{
		++_numGameControllers;
	}
	else
	{
		Console::PrintError("Failed to initialize game controler: %ls.", instance->tszProductName);
	}

	return (_numGameControllers == MAX_NUM_GAME_CONTROLLERS)? DIENUM_STOP: DIENUM_CONTINUE;
}


Input::Device::Device()
{
	_device = 0;
	_directInput = 0;
	_instanceName = 0;
	_productName = 0;
}

bool Input::Device::Init(LPDIRECTINPUT8 dinput, HWND window_handle, REFGUID rguid, LPCDIDATAFORMAT data_format, DWORD coop_flags)
{
	if(_device)
	{
		return true;
	}

	_directInput = dinput;
	HRESULT result;
	result = _directInput->CreateDevice(rguid, &_device, 0);

	if(FAILED(result))
	{
		return false;
	}

	result = _device->SetDataFormat(data_format);
	if(FAILED(result))
	{
		SAFE_RELEASE(_device);
		return false;
	}

	result = _device->SetCooperativeLevel(window_handle, coop_flags);
	if(FAILED(result))
	{
		SAFE_RELEASE(_device);
		return false;
	}

	DIDEVICEINSTANCE di;
	di.dwSize = sizeof(di);
	result = _device->GetDeviceInfo(&di);
	if(SUCCEEDED(result))
	{
		size_t len = tstrlen(di.tszInstanceName);
		if(len)
		{
			_instanceName = NewArray<tchar>(mainPool, len + 1);
			tstrcpy(_instanceName, di.tszInstanceName);
		}
		len = tstrlen(di.tszProductName);
		if(len)
		{
			_productName = NewArray<tchar>(mainPool, len + 1);
			tstrcpy(_productName, di.tszProductName);
		}
	}

	return true;
}

void Input::Device::Deinit()
{
	if(_device)
	{
		_device->Unacquire();
		SAFE_RELEASE(_device);
		Memory::Delete(_instanceName);
		Memory::Delete(_productName);
		_instanceName = 0;
		_productName = 0;
	}
}

bool Input::Keyboard::Init(LPDIRECTINPUT8 dinput, HWND window_handle)
{
	bool bres;
	bres = Device::Init(dinput, window_handle, GUID_SysKeyboard, &c_dfDIKeyboard, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(!bres)
		return false;

	memset(_state, 0, STATE_BUFFER_SIZE);
	memset(_prevState, 0, STATE_BUFFER_SIZE);

	_device->Acquire();

	Console::PrintLn("Initialized keyboard");

	return true;
}

void Input::Keyboard::UpdateState()
{
	memcpy(_prevState, _state, STATE_BUFFER_SIZE);

	HRESULT result = _device->GetDeviceState(sizeof(_state), _state);

	if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
	{
		_device->Acquire();
		result = _device->GetDeviceState(sizeof(_state), _state);
		if(FAILED(result))
			memset(_state, 0, STATE_BUFFER_SIZE);
	}
}

bool Input::Mouse::Init(LPDIRECTINPUT8 dinput, HWND window_handle)
{
	bool bres;
	bres = Device::Init(dinput, window_handle, GUID_SysMouse, &c_dfDIMouse, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(!bres)
		return false;

	DIDEVCAPS dc;
	dc.dwSize = sizeof(dc);
	HRESULT result = _device->GetCapabilities(&dc);
	if(SUCCEEDED(result) && (dc.dwFlags & DIDC_ATTACHED))
	{
		_numButtons = dc.dwButtons;
		_hasWheel = dc.dwAxes > 2;

		_device->Acquire();

		memset(&_state, 0, sizeof(DIMOUSESTATE));
		memset(&_prevState, 0, sizeof(DIMOUSESTATE));

		Console::PrintLn("Initialized mouse");

		return true;
	}
	else
	{
		Deinit();
		return false;
	}
}

void Input::Mouse::UpdateState()
{
	_prevState = _state;

	HRESULT result = _device->GetDeviceState(sizeof(DIMOUSESTATE), &_state);
	if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
	{
		_device->Acquire();
		result = _device->GetDeviceState(sizeof(DIMOUSESTATE), &_state);
		if(FAILED(result))
			memset(&_state, 0, sizeof(DIMOUSESTATE));
	}
}


BOOL CALLBACK Input::GameController::EnumGameControllerAxesCallback(LPCDIDEVICEOBJECTINSTANCE instance, LPVOID ref)
{
	DIPROPRANGE range_prop;
	range_prop.diph.dwSize = sizeof(DIPROPRANGE);
	range_prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	range_prop.diph.dwHow = DIPH_BYID;
	range_prop.diph.dwObj = instance->dwType;
	range_prop.lMin = -1000;
	range_prop.lMax = 1000;

	LPDIRECTINPUTDEVICE8 joy = (LPDIRECTINPUTDEVICE8)ref;
	joy->SetProperty(DIPROP_RANGE, &range_prop.diph);

	return DIENUM_CONTINUE;
}

bool Input::GameController::Init(LPDIRECTINPUT8 dinput, HWND window_handle, REFGUID rguid)
{
	bool bres;
	bres = Device::Init(dinput, window_handle, rguid, &c_dfDIJoystick2, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(!bres)
		return false;

	// get joystick capabilities
	DIDEVCAPS caps;
	caps.dwSize = sizeof(DIDEVCAPS);

	HRESULT result;
	result = _device->GetCapabilities(&caps);

	if(FAILED(result))
	{
		Deinit();
		return false;
	}

	_numAxes = caps.dwAxes;
	_numButtons = caps.dwButtons;
	_numPOVs = caps.dwPOVs;
	_hasForceFeedback = (caps.dwFlags & DIDC_FORCEFEEDBACK) != 0;

	// enumerate axes and set their ranges
	result = _device->EnumObjects(&GameController::EnumGameControllerAxesCallback, (void*)_device, DIDFT_AXIS);

	if(FAILED(result))
	{
		Deinit();
		return false;
	}

	memset(&_state, 0, sizeof(DIJOYSTATE2));
	memset(&_prevState, 0, sizeof(DIJOYSTATE2));

	Console::PrintLn("Initialized game controller: %ls - %ls", _instanceName, _productName);

	return true;
}

void Input::GameController::UpdateState()
{
	HRESULT result;

	result = _device->Poll();

	if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
	{
		result = _device->Acquire();
		if(FAILED(result))
			return;

		result = _device->Poll();
		if(FAILED(result))
			return;
	}

	_prevState = _state;

	result = _device->GetDeviceState(sizeof(DIJOYSTATE2), &_state);

	if(FAILED(result))
		memset(&_state, 0, sizeof(DIJOYSTATE2));
}
