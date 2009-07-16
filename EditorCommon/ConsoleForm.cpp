#include "StdAfx.h"
#include "FormDirector.h"
#include "ConsoleForm.h"

using namespace Memory;
using namespace math3d;

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))



namespace EditorCommon
{

	private ref class ConsoleWindow: public NativeWindow
	{
	public:
		ConsoleWindow(Form^ parent);
		~ConsoleWindow();

		void CreateCaret();

	protected:
		virtual void WndProc(Message% msg) override;

	private:
		void DrawConsoleText(HDC hdc, const RECT& rect);
		void OnConsoleChar(int c);
		void OnConsoleKeyDown(int key);
		void OnConsoleKeyUp(int key);
		void OnMouseWheel(int delta, int x, int y);
		void OnResize(int width, int height);
		void RedrawConsole();

		HWND _handle;
		HFONT _font;
		HPEN _greenPen;
		int _clWidth;
		int _clHeight;
		int _fontWidth;
		int _fontHeight;
	};


	inline
	COLORREF PackGDIColor(const vec4f& color)
	{
		return RGB(int(color.r * 255), int(color.g * 255), int(color.b * 255));
	}
	
	ConsoleWindow::ConsoleWindow(Form^ parent)
	{
		System::Windows::Forms::CreateParams^ cp = gcnew System::Windows::Forms::CreateParams;
		cp->Caption = "";
		cp->ClassName = nullptr;
		cp->ClassStyle = 0;
		cp->Style = WS_CHILDWINDOW | WS_VISIBLE;
		cp->ExStyle = 0;
		cp->X = 0;
		cp->Y = 0;
		cp->Width = parent->ClientRectangle.Width;
		cp->Height = parent->ClientRectangle.Height;
		cp->Param = nullptr;
		cp->Parent = parent->Handle;

		try
		{
			CreateHandle(cp);
		}
		catch(Exception^)
		{
			throw "Failed to create handle.";
		}

		_handle = (HWND)Handle.ToPointer();
		_font = (HFONT)GetStockObject(ANSI_FIXED_FONT);
		_greenPen = CreatePen(PS_SOLID, 1, RGB(0, 127, 0));
		RECT client_rect;
		GetClientRect(_handle, &client_rect);
		_clWidth = client_rect.right;
		_clHeight = client_rect.bottom;

		// get font character width and height
		HDC hdc = GetDC(_handle);
		if(!hdc)
		{
			DestroyHandle();
			throw "Failed to get device context.";
		}

		HFONT old_font = (HFONT)SelectObject(hdc, _font);
		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		_fontWidth = tm.tmMaxCharWidth; // fixed width font
		_fontHeight = tm.tmHeight;
		SelectObject(hdc, old_font);
		ReleaseDC(_handle, hdc);

		// create and show the caret
		::CreateCaret(_handle, 0, _fontWidth, 2);
		SetCaretPos(3 + _fontWidth, _clHeight - 5);
		SetCaretBlinkTime(500);
		ShowCaret(_handle);
	}

	ConsoleWindow::~ConsoleWindow()
	{
		DeleteObject(_greenPen);
	}

	void ConsoleWindow::CreateCaret()
	{
		if(::Console::GetInputMode() == ::Console::INM_OVERWRITE)
		{
			::CreateCaret(_handle, 0, _fontWidth, _fontHeight);
			SetCaretPos(3 + (::Console::GetInputPos() + 1) * _fontWidth, _clHeight - 3 - _fontHeight);
			SetCaretBlinkTime(500);
		}
		else
		{
			::CreateCaret(_handle, 0, _fontWidth, 2);
			SetCaretPos(3 + (::Console::GetInputPos() + 1) * _fontWidth, _clHeight - 5);
			SetCaretBlinkTime(500);
		}

		SetCaretBlinkTime(500);
		ShowCaret(_handle);
	}

	void ConsoleWindow::WndProc(Message% msg)
	{
		switch(msg.Msg)
		{
		case WM_PAINT:
		{
			RECT rect;
			if(GetUpdateRect(_handle, &rect, FALSE))
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(_handle, &ps);
				if(hdc)
					DrawConsoleText(hdc, ps.rcPaint);
				EndPaint(_handle, &ps);
			}
			break;
		}
		case WM_ERASEBKGND:
		{
			RECT rect;
			GetClientRect(_handle, &rect);
			FillRect((HDC)msg.WParam.ToPointer(), &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
			msg.Result = IntPtr(1);
			return;
		}
		case WM_MOUSEMOVE:
			SetCursor(LoadCursor(0, IDC_ARROW));
			break;
		case WM_CHAR:
			OnConsoleChar(msg.WParam.ToInt32());
			break;
		case WM_KEYDOWN:
			OnConsoleKeyDown(msg.WParam.ToInt32());
			break;
		case WM_KEYUP:
			OnConsoleKeyUp(msg.WParam.ToInt32());
			break;
		case WM_MOUSEWHEEL:
			OnMouseWheel(
				GET_WHEEL_DELTA_WPARAM(msg.WParam.ToInt32()),
				GET_X_LPARAM(msg.LParam.ToInt32()),
				GET_Y_LPARAM(msg.LParam.ToInt32()));
			msg.Result = IntPtr(0);
			return;
		case WM_SIZE:
			OnResize(
				GET_X_LPARAM(msg.LParam.ToInt32()),
				GET_Y_LPARAM(msg.LParam.ToInt32()));
			break;
		}

		NativeWindow::WndProc(msg);
	}

	void ConsoleWindow::DrawConsoleText(HDC hdc, const RECT& rect)
	{
		SetBkMode(hdc, TRANSPARENT);
		HFONT old_font = (HFONT)SelectObject(hdc, _font);

		int ypos = _clHeight - 3 - _fontHeight * 2 - 5;
		int max_lines = ypos / _fontHeight + (ypos % _fontHeight? 1: 0);
		int lines_printed = 0;
		const short* line = ::Console::GetBufferCurrentLine();
		int max_line_size = ::Console::GetBufferLineSize();
		char* text = new(tempPool) char[max_line_size + 1];

		// draw console input
		SetTextColor(hdc, PackGDIColor(::Console::GetDefaultTextColor()));
		TextOutA(hdc, 3, _clHeight - 3 - _fontHeight, "#", 1);
		TextOutA(
			hdc, 3 + _fontWidth, _clHeight - 3 - _fontHeight,
			::Console::GetInputBuffer(), ::Console::GetInputSize());
		HPEN old_pen =(HPEN) SelectObject(hdc, _greenPen);
		MoveToEx(hdc, 0, _clHeight - 3 - _fontHeight - 2, 0);
		LineTo(hdc, _clWidth, _clHeight - 3 - _fontHeight - 2);
		SelectObject(hdc, old_pen);

		// draw console buffer
		while(line && lines_printed < max_lines)
		{
			if(*line & 0xFF)
			{
				const short* src = line;
				int xpos = 3;
				int num_chars = max_line_size;
				while(num_chars && *src)
				{
					char* dest = text;
					::Console::Color color = ::Console::Color((*src >> 8) & 0xFF);
					while(num_chars && *src && (color == ((*src >> 8) & 0xFF)))
					{
						*dest++ = char(*src++ & 0xFF);
						--num_chars;
					}
					*dest = '\0';
					SetTextColor(hdc, PackGDIColor(::Console::GetColor(color)));
					int text_len = strlen(text);
					TextOutA(hdc, xpos, ypos, text, text_len);
					xpos += _fontWidth * text_len;
				}
				ypos -= _fontHeight;
				++lines_printed;
			}

			line = ::Console::GetBufferNextLine(line);
		}

		SelectObject(hdc, old_font);

		delete[] text;
	}

	void ConsoleWindow::OnConsoleChar(int c)
	{
		if(c >= 32 && c <= 127)
		{
			::Console::InputChar((char)c);
			RedrawConsole();
		}
	}

	void ConsoleWindow::OnConsoleKeyDown(int key)
	{
		switch(key)
		{
		case VK_TAB:
			::Console::AutoCompleteInput();
			RedrawConsole();
			break;
		case VK_RETURN:
			::Console::ExecuteStatement();
			RedrawConsole();
			break;
		case VK_BACK:
			::Console::DeletePrevChar();
			RedrawConsole();
			break;
		case VK_DELETE:
			::Console::DeleteChar();
			RedrawConsole();
			break;
		case VK_NEXT:
			::Console::Scroll(1);
			RedrawConsole();
			break;
		case VK_PRIOR:
			::Console::Scroll(-1);
			RedrawConsole();
			break;
		case VK_HOME:
			if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
				::Console::ScrollToBeginning();
			else
				::Console::MoveInputPosBeg();
			RedrawConsole();
			break;
		case VK_END:
			if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
				::Console::ScrollToEnd();
			else
				::Console::MoveInputPosEnd();
			RedrawConsole();
			break;
		case VK_LEFT:
			if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
				::Console::MoveInputPosPrevWord();
			else
				::Console::MoveInputPosLeft();
			RedrawConsole();
			break;
		case VK_RIGHT:
			if(GetAsyncKeyState(VK_CONTROL) & 0x8000)
				::Console::MoveInputPosNextWord();
			else
				::Console::MoveInputPosRight();
			RedrawConsole();
			break;
		case VK_UP:
			::Console::CmdHistoryBack();
			RedrawConsole();
			break;
		case VK_DOWN:
			::Console::CmdHistoryForward();
			RedrawConsole();
			break;
		}
	}

	void ConsoleWindow::OnConsoleKeyUp(int key)
	{
		switch(key)
		{
		case VK_INSERT:
			::Console::ToggleInputMode();
			DestroyCaret();
			CreateCaret();
			break;
		}
	}

	void ConsoleWindow::OnMouseWheel(int delta, int x, int y)
	{
		int ticks = delta / WHEEL_DELTA;
		::Console::Scroll(-ticks);
		RedrawConsole();
	}

	void ConsoleWindow::OnResize(int width, int height)
	{
		_clWidth = width;
		_clHeight = height;
		RedrawConsole();
	}

	void ConsoleWindow::RedrawConsole()
	{
		if(::Console::GetInputMode() == ::Console::INM_OVERWRITE)
			SetCaretPos(3 + (::Console::GetInputPos() + 1) * _fontWidth, _clHeight - 3 - _fontHeight);
		else
			SetCaretPos(3 + (::Console::GetInputPos() + 1) * _fontWidth, _clHeight - 5);
		InvalidateRect(_handle, 0, TRUE);
	}



	// ======================================================================

	ConsoleForm::ConsoleForm(FormDirector^ director)
	{
		InitializeComponent();
		_director = director;
	}

	void ConsoleForm::RedrawAsync()
	{
		InvalidateRect((HWND)_consoleWnd->Handle.ToPointer(), 0, TRUE);
	}

	System::Void ConsoleForm::ConsoleForm_HandleCreated(System::Object^ sender, System::EventArgs^ e)
	{
		try
		{
			_consoleWnd = gcnew ConsoleWindow(this);
		}
		catch(...)
		{
			_consoleWnd = nullptr;
		}
	}

	System::Void ConsoleForm::ConsoleForm_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		if(_consoleWnd)
		{
			HWND hwnd = (HWND)_consoleWnd->Handle.ToPointer();
			MoveWindow(hwnd, 0, 0, ClientRectangle.Width, ClientRectangle.Height, TRUE);
		}
		else
		{
			Refresh();
		}
	}

	System::Void ConsoleForm::ConsoleForm_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(_consoleWnd)
		{
			::SendMessage((HWND)_consoleWnd->Handle.ToPointer(), WM_MOUSEWHEEL, MAKEWPARAM(0, e->Delta), MAKELPARAM(e->X, e->Y));
		}
	}

	System::Void ConsoleForm::ConsoleForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		if(!_consoleWnd)
		{
			Drawing::Font^ font = gcnew Drawing::Font("Arial", 14);
			StringFormat^ fmt = gcnew StringFormat;
			fmt->Alignment = StringAlignment::Center;
			fmt->LineAlignment = StringAlignment::Center;

			e->Graphics->DrawString("Failed to initialize console rendering!", font, Brushes::Red, ClientRectangle, fmt);
		}
	}

	System::Void ConsoleForm::ConsoleForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if(_consoleWnd)
		{
			::SendMessage((HWND)_consoleWnd->Handle.ToPointer(), WM_KEYDOWN, (WPARAM)e->KeyCode, 0);
		}
	}

	System::Void ConsoleForm::ConsoleForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
	{
		if(_consoleWnd)
		{
			::SendMessage((HWND)_consoleWnd->Handle.ToPointer(), WM_CHAR, (WPARAM)e->KeyChar, 0);
		}
	}

	System::Void ConsoleForm::ConsoleForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if(_consoleWnd)
		{
			::SendMessage((HWND)_consoleWnd->Handle.ToPointer(), WM_KEYUP, (WPARAM)e->KeyCode, 0);
		}
	}

	System::Void ConsoleForm::ConsoleForm_Enter(System::Object^  sender, System::EventArgs^  e)
	{
		_consoleWnd->CreateCaret();
	}

	System::Void ConsoleForm::ConsoleForm_Leave(System::Object^  sender, System::EventArgs^  e)
	{
		DestroyCaret();
	}

}
