
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <cstdio>
#include <cstdlib>
#include <limits>
#include "Common.h"
#include "Math/vec4.h"

#undef min
#undef max

#define ESC_WHITE		"^0"
#define ESC_LIGHTGRAY	"^1"
#define ESC_DARKGRAY	"^2"
#define ESC_RED			"^3"
#define ESC_GREEN		"^4"
#define ESC_BLUE		"^5"
#define ESC_YELLOW		"^6"
#define ESC_ORANGE		"^7"
#define ESC_CYAN		"^8"
#define ESC_MAGENTA		"^9"
#define ESC_DEFAULT		ESC_LIGHTGRAY


class BASELIB_API Console
{
public:
	enum Color
	{
		CLR_WHITE,
		CLR_LIGHTGRAY,
		CLR_DARKGRAY,
		CLR_RED,
		CLR_GREEN,
		CLR_BLUE,
		CLR_YELLOW,
		CLR_ORANGE,
		CLR_CYAN,
		CLR_MAGENTA,

		CLR_COUNT
	};

	enum InputMode
	{
		INM_INSERT,
		INM_OVERWRITE
	};

	class BASELIB_API ConsoleObject;
	class BASELIB_API Variable;
	class BASELIB_API IntVar;
	class BASELIB_API FloatVar;
	class BASELIB_API BoolVar;
	class BASELIB_API StringVar;
	class BASELIB_API Command;

	static bool Init(int line_size, int line_count);
	static void Deinit();
	static bool IsInitialized()
		{ return _initialized; }

	// --- general console functions ---
	static void Print(const char* str, ...);
	static void PrintLn(const char* str, ...);
	static void PrintWarning(const char* str, ...);
	static void PrintError(const char* str, ...);
	static void ExecuteStatement();
	static void ExecuteStatement(const char* statement);
	static void ClearBuffer();
	static void Scroll(int offset);
	static void ScrollToBeginning();
	static void ScrollToEnd();

	// --- console buffer functions ---
	static void SetBufferSize(int line_size, int line_count);
	static int GetBufferLineSize();
	static int GetBufferLineCount();
	static int GetBufferSize();
	static const short* GetBufferFirstLine();
	static const short* GetBufferCurrentLine();
	static const short* GetBufferNextLine(const short* line);
	static bool IsEmpty();
	static bool IsFull();

	// --- input buffer functions ---
	static void InputChar(char c);
	static void DeleteChar();
	static void DeletePrevChar();
	static void ClearInput();
	static void AutoCompleteInput();
	static void MoveInputPosLeft()
		{ if(_inputPos > 0) --_inputPos; }
	static void MoveInputPosRight()
		{ if(_inputPos < _inputSize) ++_inputPos; }
	static void MoveInputPosBeg()
		{_inputPos = 0; }
	static void MoveInputPosEnd()
		{_inputPos = _inputSize; }
	static void MoveInputPosNextWord();
	static void MoveInputPosPrevWord();
	static void SetInputMode(InputMode mode)
		{ _inputMode = mode; }
	static InputMode GetInputMode()
		{ return _inputMode; }
	static void ToggleInputMode()
		{ _inputMode = InputMode(_inputMode ^ 1); }
	static int GetInputSize()
		{ return _inputSize; }
	static int GetInputPos()
		{ return _inputPos; }
	static const char* GetInputBuffer()
		{ return _inputBuffer; }
	static void SetInputString(const char* str);

	// --- history buffer functions ---
	static void CmdHistoryBack();
	static void CmdHistoryForward();
	static void CmdHistoryClear();

	// --- color functions ---
	static const math3d::vec4f& GetColor(Color color)
		{ return _colors[color]; }
	static const math3d::vec4f& GetDefaultTextColor()
		{ return _colors[CLR_LIGHTGRAY]; }

private:
	class ConsoleObjectList;

	static bool RegisterObject(ConsoleObject& obj);
	static void Print(const char* prefix, const char* str, bool newline, va_list vl);
	static short* GetLine(int index);
	static void NewLine();
	static bool HasColorEscape(const char* str);
	static bool HasEscapeCharacter(const char* str);

	static const char COLOR_ESCAPE_CHAR = '^';
	static const int MAX_INPUT_BUFFER_SIZE = 512;
	static const int MAX_HISTORY_BUFFER_ENTRIES = 64;

	static bool _initialized;
	static ConsoleObjectList _consoleObjList;
	static ConsoleObject* _staticObjects;			// list of static objects to be registered when console initializes
	static short* _buffer;							// pointer to console buffer
	static int _bufferSize;							// size in bytes of the whole console buffer
	static int _bufferLineSize;						// maximum line size including null character
	static int _bufferLineCount;					// maximum number of lines
	static int _firstLine;							// index of line where new text will be printed
	static int _pos;								// position in line where new text will be printed
	static int _lastLine;							// index of last line of text
	static int _currentLine;						// index of first line of text to be drawn at bottom
	static char _inputBuffer[MAX_INPUT_BUFFER_SIZE];
	static int _inputPos;							// position where next character will be inserted
	static int _inputSize;							// size of input string
	static InputMode _inputMode;					// insert or overwrite mode
	static char _historyBuffer[MAX_HISTORY_BUFFER_ENTRIES][MAX_INPUT_BUFFER_SIZE];
	static int _historyFirstLine;					// position of first free history slot
	static int _historyLastLine;					// position of oldest command in history
	static int _historyCurrentLine;					// points to current command while moving through history
	static const math3d::vec4f _colors[CLR_COUNT];	// array of console colors
};

// -------- base class for console variables and commands --------

class Console::ConsoleObject
{
public:
	ConsoleObject(const char* name, const char* help);
	~ConsoleObject() {}

	virtual void Execute(char* params) = 0;

	const char* GetName() const
		{ return _name; }
	const char* GetHelp() const
		{ return _help; }

protected:
	const char* _name;
	const char* _help;

private:
	friend class Console;

	// used by console when shutting down
	virtual void ReleaseResources() {}

	ConsoleObject* next;
};

// --------- console variable base class ---------

class Console::Variable: public Console::ConsoleObject
{
public:
	Variable(const char* name, const char* help, bool read_only = false)
		: ConsoleObject(name, help)
		{ _readOnly = read_only; }

	void Execute(char* params);

	virtual void GetStringValue(char* str) const = 0;
	virtual void SetStringValue(const char* val) = 0;
	virtual void GetDefaultStringValue(char* str) const = 0;
	void SetReadOnly(bool read_only)
		{ _readOnly = read_only; }
	bool IsReadOnly() const
		{ return _readOnly; }

protected:
	bool _readOnly; // this determines if variable can be modified in console by user
};

// ------- int variable -------

class Console::IntVar: public Console::Variable
{
public:
	IntVar(
		const char* name, int default_value, const char* help = 0, bool read_only = false,
		int min_val = std::numeric_limits<int>::min(), int max_val = std::numeric_limits<int>::max());

	operator int() const
		{ return _value; }
	operator int&()
		{ return _value; }
	void GetStringValue(char* str) const;
	void SetStringValue(const char* val);
	void GetDefaultStringValue(char* str) const;

private:
	int _value;
	int _defaultValue;
	int _minValue;
	int _maxValue;
};

// ------- float variable -------

class Console::FloatVar: public Console::Variable
{
public:
	FloatVar(
		const char* name, float default_value, const char* help = 0, bool read_only = false,
		float min_val = std::numeric_limits<float>::min(), float max_val = std::numeric_limits<float>::max());

	operator float() const
		{ return _value; }
	operator float&()
		{ return _value; }
	void GetStringValue(char* str) const;
	void SetStringValue(const char* val);
	void GetDefaultStringValue(char* str) const;

private:
	float _value;
	float _defaultValue;
	float _minValue;
	float _maxValue;
};

// ------- float variable -------

class Console::BoolVar: public Console::Variable
{
public:
	BoolVar(const char* name, bool default_value, const char* help = 0, bool read_only = false);

	operator bool() const
		{ return _value; }
	operator bool&()
		{ return _value; }
	void GetStringValue(char* str) const;
	void SetStringValue(const char* val);
	void GetDefaultStringValue(char* str) const;

private:
	bool _value;
	bool _defaultValue;
};

// ------- string variable --------

class Console::StringVar: public Console::Variable
{
public:
	StringVar(const char* name, const char* default_value, const char** allowed_values = 0, const char* help = 0, bool read_only = false);

	operator const char*() const
		{ return _value; }
	void GetStringValue(char* str) const;
	void SetStringValue(const char* val);
	void GetDefaultStringValue(char* str) const;

private:
	void ReleaseResources();

	const char* _value;
	const char* _defaultValue;
	const char** _allowedValues;
	bool _staticString;
};

// ------------ console Command ------------

class Console::Command: public Console::ConsoleObject
{
public:
	typedef void (*FuncPtr)(char* args);

	Command(const char* name, const char* help, FuncPtr func_ptr)
		:ConsoleObject(name, help)
		{ _funcPtr = func_ptr; }

	void Execute(char* params);

private:
	FuncPtr _funcPtr;
};

// ------------ console object list --------------
// class for holding console variables and comands
// -----------------------------------------------

class Console::ConsoleObjectList
{
public:
	// object registration errors
	enum Error
	{
		ERR_NO_ERROR,
		ERR_ALREADY_EXISTS,
		ERR_INVALID_CHARACTER
	};

	ConsoleObjectList();

	Error Add(ConsoleObject& var);
	ConsoleObject* Remove(ConsoleObject& var);
	ConsoleObject* Remove(const char* name);
	void Clear();
	ConsoleObject* Find(const char* name);
	int GetBeginWith(const char* prefix, ConsoleObject** objects);

private:
	ConsoleObject*& GetListForChar(char c);
	bool CheckName(const char* name);

	// Objects are placed in a list based on their name's start letter.
	// Also, an underscore is a valid character for object names
	ConsoleObject* _objLists['z' - 'a' + 2]; // 26 letters + underscore
	int _count;
};


#endif // _CONSOLE_H_
