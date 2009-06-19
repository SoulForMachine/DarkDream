
#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "Common.h"
#include "Types.h"
#include "Win32Common.h"

namespace Video
{

	struct WindowData
	{
		HINSTANCE instanceHandle;
		LPCTSTR wndClassName;
		WNDPROC wndProc;
	};


	class BASELIB_API Window
	{
	public:
		Window();
		~Window();
		bool Create(int x, int y, int width, int height, const tchar* title, const WindowData& data);
		void Destroy();
		void Show();
		void* GetHandle() const
			{ return _windowHandle; }

	private:
		HWND _windowHandle;
		WindowData _data;
	};

}

#endif // _VIDEO_H_
