
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include "Common.h"
#include "Types.h"


class BASELIB_API Input
{
public:
	class Device;
	class Keyboard;
	class Mouse;
	class GameController;

	enum KeyCode;

	static bool Init(void* instance_handle, void* window_handle); // initialize input
	static void Deinit();
	static void AcquireDevices();

	static const char* GetKeyName(uchar code);
	static uchar GetKeyCode(const char* name);

	static Keyboard* GetKeyboard();
	static Mouse* GetMouse();
	static GameController* GetGameController(int index);
	static int GetGameControllerCount();

private:
	Input(){}
	~Input(){}
	Input(const Input&);
	static BOOL CALLBACK InitGameControllersCallback(const DIDEVICEINSTANCE* instance, void* data);

	static Keyboard* _keyboard;
	static Mouse* _mouse;
	static GameController* _gameControllers;
	static const int MAX_NUM_GAME_CONTROLLERS = 4;
	static int _numGameControllers;

	static LPDIRECTINPUT8 _directInput;
	static HWND _windowHandle;
};


enum Input::KeyCode
{
	KEY_ESCAPE		= DIK_ESCAPE,
	KEY_1			= DIK_1,
	KEY_2			= DIK_2,
	KEY_3			= DIK_3,
	KEY_4			= DIK_4,
	KEY_5			= DIK_5,
	KEY_6			= DIK_6,
	KEY_7			= DIK_7,
	KEY_8			= DIK_8,
	KEY_9			= DIK_9,
	KEY_0			= DIK_0,
	KEY_MINUS		= DIK_MINUS,
	KEY_EQUALS		= DIK_EQUALS,
	KEY_BACK		= DIK_BACK,
	KEY_TAB			= DIK_TAB,
	KEY_Q			= DIK_Q,
	KEY_W			= DIK_W,
	KEY_E			= DIK_E,
	KEY_R			= DIK_R,
	KEY_T			= DIK_T,
	KEY_Y			= DIK_Y,
	KEY_U			= DIK_U,
	KEY_I			= DIK_I,
	KEY_O			= DIK_O,
	KEY_P			= DIK_P,
	KEY_LBRACKET	= DIK_LBRACKET,
	KEY_RBRACKET	= DIK_RBRACKET,
	KEY_RETURN		= DIK_RETURN,
	KEY_LCONTROL	= DIK_LCONTROL,
	KEY_A			= DIK_A,
	KEY_S			= DIK_S,
	KEY_D			= DIK_D,
	KEY_F			= DIK_F,
	KEY_G			= DIK_G,
	KEY_H			= DIK_H,
	KEY_J			= DIK_J,
	KEY_K			= DIK_K,
	KEY_L			= DIK_L,
	KEY_SEMICOLON	= DIK_SEMICOLON,
	KEY_APOSTROPHE	= DIK_APOSTROPHE,
	KEY_GRAVE		= DIK_GRAVE,
	KEY_LSHIFT		= DIK_LSHIFT,
	KEY_BACKSLASH	= DIK_BACKSLASH,
	KEY_Z			= DIK_Z,
	KEY_X			= DIK_X,
	KEY_C			= DIK_C,
	KEY_V			= DIK_V,
	KEY_B			= DIK_B,
	KEY_N			= DIK_N,
	KEY_M			= DIK_M,
	KEY_COMMA		= DIK_COMMA,
	KEY_PERIOD		= DIK_PERIOD,
	KEY_SLASH		= DIK_SLASH,
	KEY_RSHIFT		= DIK_RSHIFT,
	KEY_MULTIPLY	= DIK_MULTIPLY,
	KEY_LMENU		= DIK_LMENU,
	KEY_SPACE		= DIK_SPACE,
	KEY_CAPITAL		= DIK_CAPITAL,
	KEY_F1			= DIK_F1,
	KEY_F2			= DIK_F2,
	KEY_F3			= DIK_F3,
	KEY_F4			= DIK_F4,
	KEY_F5			= DIK_F5,
	KEY_F6			= DIK_F6,
	KEY_F7			= DIK_F7,
	KEY_F8			= DIK_F8,
	KEY_F9			= DIK_F9,
	KEY_F10			= DIK_F10,
	KEY_NUMLOCK		= DIK_NUMLOCK,
	KEY_SCROLL		= DIK_SCROLL,
	KEY_NUMPAD7		= DIK_NUMPAD7,
	KEY_NUMPAD8		= DIK_NUMPAD8,
	KEY_NUMPAD9		= DIK_NUMPAD9,
	KEY_SUBTRACT	= DIK_SUBTRACT,
	KEY_NUMPAD4		= DIK_NUMPAD4,
	KEY_NUMPAD5		= DIK_NUMPAD5,
	KEY_NUMPAD6		= DIK_NUMPAD6,
	KEY_ADD			= DIK_ADD,
	KEY_NUMPAD1		= DIK_NUMPAD1,
	KEY_NUMPAD2		= DIK_NUMPAD2,
	KEY_NUMPAD3		= DIK_NUMPAD3,
	KEY_NUMPAD0		= DIK_NUMPAD0,
	KEY_DECIMAL		= DIK_DECIMAL,
	KEY_F11			= DIK_F11,
	KEY_F12			= DIK_F12,
	KEY_F13			= DIK_F13,
	KEY_F14			= DIK_F14,
	KEY_F15			= DIK_F15,
	KEY_NUMPADENTER	= DIK_NUMPADENTER,
	KEY_RCONTROL	= DIK_RCONTROL,
	KEY_DIVIDE		= DIK_DIVIDE,
	KEY_SYSRQ		= DIK_SYSRQ,
	KEY_RMENU		= DIK_RMENU,
	KEY_HOME		= DIK_HOME,
	KEY_UP			= DIK_UP,
	KEY_PRIOR		= DIK_PRIOR,
	KEY_LEFT		= DIK_LEFT,
	KEY_RIGHT		= DIK_RIGHT,
	KEY_END			= DIK_END,
	KEY_DOWN		= DIK_DOWN,
	KEY_NEXT		= DIK_NEXT,
	KEY_INSERT		= DIK_INSERT,
	KEY_DELETE		= DIK_DELETE,
	KEY_LWIN		= DIK_LWIN,
	KEY_RWIN		= DIK_RWIN,
	KEY_APPS		= DIK_APPS,
	KEY_PAUSE		= DIK_PAUSE,
};

class BASELIB_API Input::Device
{
public:
	void Acquire()
	{
		if(_device)
			_device->Acquire();
	}

	void Unacquire()
	{
		if(_device)
			_device->Unacquire();
	}

	const tchar* GetInstanceName()
		{ return _instanceName; }
	const tchar* GetProductName()
		{ return _productName; }

protected:
	Device();
	bool Init(LPDIRECTINPUT8 dinput, HWND window_handle, REFGUID rguid, LPCDIDATAFORMAT data_format, DWORD coop_flags);
	void Deinit();

	LPDIRECTINPUT8 _directInput;
	LPDIRECTINPUTDEVICE8 _device;
	tchar* _instanceName;
	tchar* _productName;
};

class BASELIB_API Input::Keyboard: public Input::Device
{
public:
	Keyboard() {}

	void UpdateState();

	// return current state of a key
	bool GetKeyState(uchar key)
	{
		return (_state[key] & 0x80) != 0;
	}

	// return previous state of a key
	bool GetKeyPrevState(uchar key)
	{
		return (_prevState[key] & 0x80) != 0;
	}

	// return true if key state changed to down since last call to Update()
	bool StateChangedToDown(uchar key)
	{
		return GetKeyState(key) && !GetKeyPrevState(key);
	}

	// return true if key state changed to up since last call to Update()
	bool StateChangedToUp(uchar key)
	{
		return !GetKeyState(key) && GetKeyPrevState(key);
	}

private:
	Keyboard(const Keyboard&);
	bool Init(LPDIRECTINPUT8 dinput, HWND window_handle);

	static const int STATE_BUFFER_SIZE = 256;
	uchar _state[STATE_BUFFER_SIZE];
	uchar _prevState[STATE_BUFFER_SIZE];

	friend class Input;
};

class BASELIB_API Input::Mouse: public Input::Device
{
public:
	Mouse() {}

	void UpdateState();

	int GetNumButtons()
	{
		return _numButtons;
	}
	bool HasWheel()
	{
		return _hasWheel;
	}

	// get current state of a button
	bool GetButtonState(int button)
	{
		return (_state.rgbButtons[button] & 0x80) != 0;
	}

	// get previous state of a button
	bool GetButtonPrevState(int button)
	{
		return (_prevState.rgbButtons[button] & 0x80) != 0;
	}

	// return true if button state chaged to down since last call to Update()
	bool StateChangedToDown(int button)
	{
		return GetButtonState(button) && !GetButtonPrevState(button);
	}

	// return true if button state chaged to up since last call to Update()
	bool StateChangedToUp(int button)
	{
		return !GetButtonState(button) && GetButtonPrevState(button);
	}

	// get mouse relative position
	void GetPosition(int& x, int& y)
	{
		x = (int)_state.lX;
		y = (int)_state.lY;
	}

	// get wheel relative offset
	int GetWheelChange()
	{
		return (int)_state.lZ;
	}

private:
	Mouse(const Mouse&);
	bool Init(LPDIRECTINPUT8 dinput, HWND window_handle);

	int _numButtons;
	bool _hasWheel;
	DIMOUSESTATE _state;
	DIMOUSESTATE _prevState;

	friend class Input;
};

class BASELIB_API Input::GameController: public Input::Device
{
public:
	GameController() {}

	void UpdateState();

	int GetNumButtons()
	{
		return _numButtons;
	}

	int GetNumAxes()
	{
		return _numAxes;
	}

	int GetNumPOVs()
	{
		return _numPOVs;
	}

	bool HasForceFeedback()
	{
		return _hasForceFeedback;
	}

	bool GetButtonState(int button)
	{
		return (_state.rgbButtons[button] & 0x80) != 0;
	}

	bool GetButtonPrevState(int button)
	{
		return (_prevState.rgbButtons[button] & 0x80) != 0;
	}

	int GetXAxis()
	{
		return (int)_state.lX;
	}

	int GetYAxis()
	{
		return (int)_state.lY;
	}

	int GetZAxis()
	{
		return (int)_state.lZ;
	}

	int GetXAxisRotation()
	{
		return (int)_state.lRx;
	}

	int GetYAxisRotation()
	{
		return (int)_state.lRy;
	}

	int GetZAxisRotation()
	{
		return (int)_state.lRz;
	}

	float GetPOVAngle(int pov)
	{
		return (LOWORD(_state.rgdwPOV[pov]) == 0xFFFF)? -1.0f: _state.rgdwPOV[pov] / 100.0f;
	}

private:
	GameController(const GameController&);
	bool Init(LPDIRECTINPUT8 dinput, HWND window_handle, REFGUID rguid);
	static BOOL CALLBACK EnumGameControllerAxesCallback(LPCDIDEVICEOBJECTINSTANCE instance, LPVOID ref);

	int _numButtons;
	int _numAxes;
	int _numPOVs;
	bool _hasForceFeedback;
	DIJOYSTATE2 _state;
	DIJOYSTATE2 _prevState;

	friend class Input;
};

#endif // _INPUT_H_
