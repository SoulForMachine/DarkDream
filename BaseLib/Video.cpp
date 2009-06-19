
#include "Video.h"


namespace Video
{

	Window::Window()
	{
		_windowHandle = 0;
	}

	Window::~Window()
	{
		Destroy();
	}

	bool Window::Create(int x, int y, int width, int height, const tchar* title, const WindowData& data)
	{
		_data = data;

		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.hInstance = data.instanceHandle;
		wc.lpfnWndProc = data.wndProc;
		wc.lpszClassName = data.wndClassName;
		wc.style = CS_OWNDC;

		ATOM result = RegisterClass(&wc);
		if(!result)
		{
			return false;
		}

		DWORD style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | (WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME));
		DWORD ex_style = WS_EX_APPWINDOW;
		RECT rect = { x, y, x + width, y + height };
		if(!AdjustWindowRectEx(&rect, style, FALSE, ex_style))
		{
			UnregisterClass(_data.wndClassName, _data.instanceHandle);
			return false;
		}

		_windowHandle = CreateWindowEx(
			ex_style, data.wndClassName, title? title: _t(""), style,
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
			0, 0, data.instanceHandle, 0);

		if(!_windowHandle)
		{
			UnregisterClass(_data.wndClassName, _data.instanceHandle);
			return false;
		}

		return true;
	}

	void Window::Destroy()
	{
		if(_windowHandle)
		{
			DestroyWindow(_windowHandle);
			UnregisterClass(_data.wndClassName, _data.instanceHandle);
			_windowHandle = 0;
		}
	}

	void Window::Show()
	{
		if(_windowHandle)
			ShowWindow(_windowHandle, SW_NORMAL);
	}

}
