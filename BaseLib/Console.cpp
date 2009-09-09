
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <cstring>
#include "Memory.h"
#include "Console.h"

using namespace Memory;
using namespace math3d;


bool Console::_initialized;
Console::ConsoleObjectList Console::_consoleObjList;
Console::ConsoleObject* Console::_staticObjects;
short* Console::_buffer;
int Console::_bufferSize;
int Console::_bufferLineSize;
int Console::_bufferLineCount;
int Console::_firstLine;
int Console::_pos;
int Console::_lastLine;
int Console::_currentLine;
char Console::_inputBuffer[MAX_INPUT_BUFFER_SIZE];
int Console::_inputPos;
int Console::_inputSize;
Console::InputMode Console::_inputMode;
char Console::_historyBuffer[MAX_HISTORY_BUFFER_ENTRIES][MAX_INPUT_BUFFER_SIZE];
int Console::_historyFirstLine;
int Console::_historyLastLine;
int Console::_historyCurrentLine;

const vec4f Console::_colors[CLR_COUNT] =
{
	vec4f(1.0f, 1.0f, 1.0f, 1.0f), // white
	vec4f(0.75f, 0.75f, 0.75f, 1.0f), // light gray
	vec4f(0.25f, 0.25f, 0.25f, 1.0f), // dark gray
	vec4f(1.0f, 0.0f, 0.0f, 1.0f), // red
	vec4f(0.0f, 1.0f, 0.0f, 1.0f), // green
	vec4f(0.0f, 0.0f, 1.0f, 1.0f), // blue
	vec4f(1.0f, 1.0f, 0.0f, 1.0f), // yellow
	vec4f(1.0f, 0.5f, 0.0f, 1.0f), // orange
	vec4f(0.0f, 1.0f, 1.0f, 1.0f), // cyan
	vec4f(1.0f, 0.0f, 1.0f, 1.0f), // magenta
};


// ------------ console object list ------------

Console::ConsoleObjectList::ConsoleObjectList()
{
	memset(&_objLists[0], 0, sizeof(_objLists));
	_count = 0;
}

Console::ConsoleObjectList::Error Console::ConsoleObjectList::Add(ConsoleObject& obj)
{
	if(!CheckName(obj.GetName()))
		return ERR_INVALID_CHARACTER;

	ConsoleObject*& list = GetListForChar(*obj.GetName());

	if(list)
	{
		ConsoleObject* ptr = list;
		ConsoleObject** prev = &list;
		while(ptr)
		{
			int result = stricmp(obj.GetName(), ptr->GetName());

			if(result == 0)
				return ERR_ALREADY_EXISTS;

			// if less than current object, insert before it
			if(result < 0)
			{
				obj.next = ptr;
				*prev = &obj;

				return ERR_NO_ERROR;
			}

			prev = &ptr->next;
			ptr = ptr->next;
		}

		// add to end of list
		*prev = &obj;
		obj.next = 0;
	}
	else
	{
		obj.next = 0;
		list = &obj;
	}

	return ERR_NO_ERROR;
}

Console::ConsoleObject* Console::ConsoleObjectList::Remove(ConsoleObject& obj)
{
	return Remove(obj.GetName());
}

Console::ConsoleObject* Console::ConsoleObjectList::Remove(const char* name)
{
	if(!CheckName(name))
		return 0;

	ConsoleObject*& list = GetListForChar(*name);

	if(list)
	{
		ConsoleObject* ptr = list;
		ConsoleObject* prev = 0;
		while(ptr)
		{
			if(!stricmp(ptr->GetName(), name))
			{
				if(prev)
					prev->next = ptr->next;
				else
					list = 0;

				return ptr;
			}

			prev = ptr;
			ptr = ptr->next;
		}
	}

	return 0;
}

void Console::ConsoleObjectList::Clear()
{
	for(int i = 0; i < COUNTOF(_objLists); ++i)
		for(ConsoleObject* obj = _objLists[i]; obj; obj = obj->next)
			obj->ReleaseResources();

	memset(&_objLists[0], 0, sizeof(_objLists));
	_count = 0;
}

Console::ConsoleObject* Console::ConsoleObjectList::Find(const char* name)
{
	if(!name || !*name || !CheckName(name))
		return 0;

	ConsoleObject*& list = GetListForChar(*name);
	ConsoleObject* ptr = list;
	while(ptr)
	{
		int result = stricmp(name, ptr->GetName());
		if(!result)
			return ptr;
		else if(result < 0)
			return 0; // search name is lexicographically less than current objects name - end of search

		ptr = ptr->next;
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------
// Function returns objects whose names begin with given prefix. Call this with objects parameter
// set to 0 to retrieve only number of objects, then call it second time with a pointer to buffer
// large enough to retrieve objects themselves
//-----------------------------------------------------------------------------------------------
int Console::ConsoleObjectList::GetBeginWith(const char* prefix, ConsoleObject** objects)
{
	int count = 0;
	if(CheckName(prefix))
	{
		int len = strlen(prefix);
		ConsoleObject* ptr = GetListForChar(*prefix);
		while(ptr)
		{
			if(!strnicmp(prefix, ptr->GetName(), len))
			{
				if(objects)
					objects[count] = ptr;
				++count;
			}
			ptr = ptr->next;
		}
	}

	return count;
}

Console::ConsoleObject*& Console::ConsoleObjectList::GetListForChar(char c)
{
	assert((tolower(c) >= 'a' && tolower(c) <= 'z') || c == '_');
	return (c == '_')? _objLists[0]: _objLists[tolower(c) - 'a' + 1];
}

//-----------------------------------------------
// check if the variable or command name contains
// valid characters
//-----------------------------------------------
bool Console::ConsoleObjectList::CheckName(const char* name)
{
	char c = *name++;
	if(!isalpha(c) && c != '_')
		return false;

	while(c = *name++)
		if(!isalnum(c) && c != '_')
			return false;

	return true;
}

// --------- ConsoleObject -----------

Console::ConsoleObject::ConsoleObject(const char* name, const char* help)
{
	_name = name;
	_help = help;

	if(_staticObjects != (ConsoleObject*)-1)
	{
		this->next = _staticObjects;
		_staticObjects = this;
	}
	else
	{
		RegisterObject(*this);
	}
}

// --------- ConsoleVariable -----------

void Console::Variable::Execute(char* params)
{
	if(!params || !*params)
	{
		char val[128];
		char def[128];
		GetStringValue(val);
		GetDefaultStringValue(def);
		PrintLn("   %s = ^8%s^1, default is ^8%s", _name, val, def);
	}
	else
		SetStringValue(params);
}

// -------- int variable ---------

Console::IntVar::IntVar(const char* name, int default_value, const char* help, bool read_only, int min_val, int max_val)
	: Variable(name, help, read_only)
{
	_value = default_value;
	_defaultValue = default_value;
	_minValue = min_val;
	_maxValue = max_val;
}

void Console::IntVar::GetStringValue(char* str) const
{
	_itoa(_value, str, 10);
}

void Console::IntVar::SetStringValue(const char* val)
{
	_value = atoi(val);

	if(_value > _maxValue)
		_value = _maxValue;
	else if(_value < _minValue)
		_value = _minValue;
}

void Console::IntVar::GetDefaultStringValue(char* str) const
{
	_itoa(_defaultValue, str, 10);
}

// ------- float variable -------

Console::FloatVar::FloatVar(const char* name, float default_value, const char* help, bool read_only, float min_val, float max_val)
	: Variable(name, help, read_only)
{
	_value = default_value;
	_defaultValue = default_value;
	_minValue = min_val;
	_maxValue = max_val;
}

void Console::FloatVar::GetStringValue(char* str) const
{
	sprintf(str, "%f", _value);
}

void Console::FloatVar::SetStringValue(const char* val)
{
	_value = (float)atof(val);

	if(_value > _maxValue)
		_value = _maxValue;
	else if(_value < _minValue)
		_value = _minValue;
}

void Console::FloatVar::GetDefaultStringValue(char* str) const
{
	sprintf(str, "%f", _defaultValue);
}

// ------- boolean variable --------

Console::BoolVar::BoolVar(const char* name, bool default_value, const char* help, bool read_only)
	: Variable(name, help, read_only)
{
	_value = default_value;
	_defaultValue = default_value;
}

void Console::BoolVar::GetStringValue(char* str) const
{
	str[0] = _value? '1': '0';
	str[1] = '\0';
}

void Console::BoolVar::SetStringValue(const char* val)
{
	_value = (atoi(val) != 0);
}

void Console::BoolVar::GetDefaultStringValue(char* str) const
{
	str[0] = _defaultValue? '1': '0';
	str[1] = '\0';
}

// ------- string variable --------

Console::StringVar::StringVar(const char* name, const char* default_value, const char** allowed_values, const char* help, bool read_only)
	: Variable(name, help, read_only)
{
	_value = default_value;
	_defaultValue = default_value;
	_allowedValues = allowed_values;
	_staticString = true;
}

void Console::StringVar::GetStringValue(char* str) const
{
	strcpy(str, _value);
}

void Console::StringVar::SetStringValue(const char* value)
{
	// if we have a set of allowed values, new value must match one of them;
	// otherwise, any string is good

	if(_allowedValues)
	{
		const char** str = _allowedValues;
		while(*str)
		{
			if(!strcmp(value, *str))
			{
				_value = *str;
				break;
			}
			++str;
		}
	}
	else
	{
		if(!strcmp(value, _defaultValue))
		{
			if(!_staticString)
				delete[] _value;
			_value = _defaultValue;
			_staticString = true;
		}
		else
		{
			if(_staticString)
			{
				_value = StringDup(value);
				_staticString = false;
			}
			else
			{
				int len;
				if((len = strlen(value)) != strlen(_value))
				{
					delete[] _value;
					_value = new(stringPool) char[len + 1];
				}
				char* ptr = const_cast<char*>(_value); // dynamic buffer, safe to cast
				strcpy(ptr, value);
			}
		}
	}
}

void Console::StringVar::GetDefaultStringValue(char* str) const
{
	strcpy(str, _defaultValue);
}

void Console::StringVar::ReleaseResources()
{
	if(!_staticString)
		delete[] _value;
}

// ------------ console Command ------------

void Console::Command::Execute(char* params)
{
	if(_funcPtr)
		_funcPtr(params);
	else
		PrintWarning("Function pointer is 0 for command %s.", _name);
}

// ------------ Console functions -------------

bool Console::Init(int line_size, int line_count)
{
	_bufferLineSize = line_size;
	_bufferLineCount = line_count;
	_bufferSize = line_size * line_count;
	_buffer = new(mainPool) short[_bufferLineSize * _bufferLineCount];
	_firstLine = 0;
	_pos = 0;
	_lastLine = 0;
	_currentLine = 0;
	_inputBuffer[0] = '\0';
	_inputPos = 0;
	_inputSize = 0;
	_inputMode = INM_INSERT;
	_historyFirstLine = 0;
	_historyLastLine = 0;
	_historyCurrentLine = 0;

	_initialized = true;

	ConsoleObject* next_obj;
	for(ConsoleObject* obj = _staticObjects; obj; obj = next_obj)
	{
		next_obj = obj->next;
		RegisterObject(*obj);
	}
	_staticObjects = (ConsoleObject*)-1;

	return true;
}

void Console::Deinit()
{
	if(_initialized)
	{
		delete[] _buffer;
		_buffer = 0;
		_consoleObjList.Clear();
		_initialized = false;
	}
}

void Console::Print(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	Print("", str, false, args);

	va_end(args);
}

void Console::PrintLn(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	Print("", str, true, args);

	va_end(args);
}

void Console::PrintWarning(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	Print("^7WARNING: ^1", str, true, args);

	va_end(args);
}

void Console::PrintError(const char* str, ...)
{
	va_list args;
	va_start(args, str);

	Print("^3ERROR: ^1", str, true, args);

	va_end(args);
}

void Console::ExecuteStatement()
{
	if(_inputSize > 0)
	{
		// remove white space from begining and end of input
		char* input = _inputBuffer;
		while(iswspace(*input)) ++input;
		char* end = _inputBuffer + _inputSize - 1;
		while(end > input && iswspace(*end))
			*end-- = '\0';

		if(*input)
		{
			// save this command in history buffer if it's not the same as previous
			bool prev_same;
			if(_historyFirstLine == _historyLastLine)
				prev_same = false;
			else
				prev_same = !strcmp(_historyBuffer[(_historyFirstLine == 0)? MAX_HISTORY_BUFFER_ENTRIES - 1: _historyFirstLine - 1], input);

			if(!prev_same)
			{
				strcpy(_historyBuffer[_historyFirstLine], input);
				_historyFirstLine = (_historyFirstLine + 1) % MAX_HISTORY_BUFFER_ENTRIES;
				if(_historyLastLine == _historyFirstLine)
					_historyLastLine = (_historyLastLine + 1) % MAX_HISTORY_BUFFER_ENTRIES;
			}
			_historyCurrentLine = _historyFirstLine;

			// execute statement in input buffer
			ExecuteStatement(input);
		}

		_inputBuffer[0] = '\0';
		_inputPos = 0;
		_inputSize = 0;
	}
}

void Console::ExecuteStatement(const char* statement)
{
	if(!statement || !*statement)
		return;

	PrintLn("#%s", statement);
	char* buf = new(tempPool) char[strlen(statement) + 1];
	strcpy(buf, statement);

	// first word is the name of variable/command, the rest is parameters
	char* ptr = buf;
	while(iswspace(*ptr)) ++ptr; // skip whitespace if any
	char* name = ptr; // mark name
	while(*ptr && !iswspace(*ptr)) ++ptr; // go through the non-space characters
	char* params;
	if(*ptr)
	{
		*ptr++ = '\0'; // put null character at end of name
		while(iswspace(*ptr)) ++ptr; // skip whitespace if any
	}
	params = ptr; // mark parameters

	ConsoleObject* obj = _consoleObjList.Find(name);
	if(obj)
	{
		obj->Execute(params);
	}
	else
	{
		PrintLn("   %s^1: No such command or variable", name);
	}

	delete[] buf;
}

void Console::ClearBuffer()
{
	_firstLine = 0;
	_pos = 0;
	_lastLine = 0;
	_currentLine = 0;
}

// Moves current scroll position by an offset. Positive offset moves
// forward through buffer, negative offset moves backward
void Console::Scroll(int offset)
{
	if(IsEmpty() || offset == 0 || abs(offset) > (int)_bufferLineCount - 1 )
		return;

	bool forward = (offset > 0);
	offset = abs(offset);

	while(offset)
	{
		bool b = true;
		while(1)
		{
			if(*GetLine(_currentLine) != 0)
			{
				if(!b)
					break;
				b = false;
			}

			if(forward)
			{
				if(_currentLine == _firstLine)
					break;
				else if(_currentLine == _bufferLineCount - 1)
					_currentLine = 0;
				else
					++_currentLine;
			}
			else
			{
				if(_currentLine == _lastLine)
					break;
				if(_currentLine == 0)
					_currentLine = _bufferLineCount - 1;
				else
					--_currentLine;
			}
		}

		--offset;
	}
}

void Console::ScrollToBeginning()
{
	_currentLine = _lastLine;
}

void Console::ScrollToEnd()
{
	_currentLine = _firstLine;
}

void Console::SetBufferSize(int line_size, int line_count)
{
	if(line_size == _bufferLineSize && line_count == _bufferLineCount)
		return;

	short* new_buffer = new(mainPool) short[line_size * line_count];
	int first_line = 0;
	int last_line = 0;
	int pos = 0;

	if(!IsEmpty())
	{
		short* src_line = GetLine(_lastLine);
		short* src_end_line = GetLine((_firstLine + 1) % _bufferLineCount);
		short* src_buf_end = _buffer + _bufferSize;
		short* dest_line = new_buffer;
		short* dest_buf_end = new_buffer + line_size * line_count;

		short* src = src_line;
		short* dest = dest_line;
		int sc = _bufferLineSize;
		int dc = line_count;

		while(src_line != src_end_line)
		{
			bool line_end = !*src;
			*dest++ = *src++;
			++pos;

			if(!sc-- || line_end)
			{
				src_line += _bufferLineSize;
				if(src_line == src_buf_end)
					src_line = _buffer;
				src = src_line;
				sc = _bufferLineSize;
			}

			if(!dc-- || line_end)
			{
				dest_line += line_count;
				if(dest_line == dest_buf_end)
					dest_line = new_buffer;
				dest = dest_line;
				dc = line_count;

				pos = 0;
				first_line = (first_line + 1) % line_count;
				if(first_line == last_line)
					last_line = (last_line + 1) % line_count;
			}
		}
	}

	delete[] _buffer;
	_buffer = new_buffer;
	_bufferLineCount = line_count;
	_bufferLineSize = line_size;
	_bufferSize = line_count * line_size;
	_firstLine = first_line;
	_pos = pos;
	_lastLine = last_line;
	_currentLine = _firstLine;
}

int Console::GetBufferLineSize()
{
	return _bufferLineSize;
}

int Console::GetBufferLineCount()
{
	return _bufferLineCount;
}

int Console::GetBufferSize()
{
	return _bufferSize;
}

const short* Console::GetBufferFirstLine()
{
	return IsEmpty()? 0: GetLine(_firstLine);
}

const short* Console::GetBufferCurrentLine()
{
	return IsEmpty()? 0: GetLine(_currentLine);
}

const short* Console::GetBufferNextLine(const short* line)
{
	// return 0 if we are at end of buffer
	if(line == GetLine(_lastLine))
		return 0;

	const short* next_line = line - _bufferLineSize;
	return (next_line < _buffer)? GetLine(_bufferLineCount - 1): next_line;
}

bool Console::IsEmpty()
{
	return (_firstLine == _lastLine && _pos == 0);
}

bool Console::IsFull()
{
	return ((_firstLine + 1) % _bufferLineCount == _lastLine && _pos > 0);
}

bool Console::RegisterObject(ConsoleObject& obj)
{
	ConsoleObjectList::Error result = _consoleObjList.Add(obj);
	if(result == ConsoleObjectList::ERR_ALREADY_EXISTS)
	{
		PrintWarning("Unable to register ^8%s^1: variable or command with this name already exists.", obj.GetName());
		return false;
	}
	else if(result == ConsoleObjectList::ERR_INVALID_CHARACTER)
	{
		PrintWarning(
			"Unable to register ^8%s^1: the name is invalid; "
			"Name can contain any alphabet letter, number or an \'_\', "
			"but cannot start with a number.", obj.GetName());
		return false;
	}
	return true;
}

void Console::Print(const char* prefix, const char* str, bool newline, va_list vl)
{
	if(!_initialized || !str)
		return;

	int prefix_size = strlen(prefix);
	char* buf = new(tempPool) char[_vscprintf(str, vl) + 1 + prefix_size];
	if(prefix_size)
		strcpy(buf, prefix);
	vsprintf(buf + prefix_size, str, vl);

	short* line = GetLine(_firstLine);
	Color color = CLR_LIGHTGRAY; // default color
	char* text = buf;

	while(*text)
	{
		while(HasColorEscape(text))
		{
			color = Color(text[1] - '0');
			text += 2;
		}
		if(HasEscapeCharacter(text))
			++text;
		
		if(*text == '\n')
		{
			line[_pos] = 0;
			++text;
			NewLine();
			line = GetLine(_firstLine);
		}
		else if(*text == '\r')
		{
			++text;
			_pos = 0;
		}
		else
		{
			line[_pos++] = *text++ | color << 8;
		}

		if(_pos == _bufferLineSize)
		{
			NewLine();
			line = GetLine(_firstLine);
		}
	}

	line[_pos] = 0;
	delete[] buf;
	if(newline)
		NewLine();
}

inline
short* Console::GetLine(int index)
{
	return _buffer + index * _bufferLineSize;
}

void Console::NewLine()
{
	bool modify_cur_line = (_currentLine == _firstLine);

	_firstLine = (_firstLine + 1) % _bufferLineCount;
	_pos = 0;
	short* line = GetLine(_firstLine);
	*line = 0;

	// if we are at end of buffer, move the last line one line forward
	if(_firstLine == _lastLine)
		_lastLine = (_lastLine + 1) % _bufferLineCount;

	// move bottom display line if necesary
	if(modify_cur_line)
		_currentLine = _firstLine;
}

inline
bool Console::HasColorEscape(const char* str)
{
	return (str[0] == COLOR_ESCAPE_CHAR && str[1] != COLOR_ESCAPE_CHAR);
}

inline
bool Console::HasEscapeCharacter(const char* str)
{
	return (str[0] == COLOR_ESCAPE_CHAR && str[1] == COLOR_ESCAPE_CHAR);
}

void Console::InputChar(char c)
{
	if(_inputSize < MAX_INPUT_BUFFER_SIZE - 1)
	{
		if(_inputMode == INM_INSERT)
		{
			if(_inputPos == _inputSize)
			{
				_inputBuffer[_inputPos++] = c;
				_inputBuffer[_inputPos] = '\0';
			}
			else
			{
				int pos = _inputSize;
				while(pos >= _inputPos)
				{
					_inputBuffer[pos + 1] = _inputBuffer[pos];
					--pos;
				}
				_inputBuffer[_inputPos++] = c;
			}
			++_inputSize;
		}
		else // overwrite
		{
			if(_inputPos == _inputSize)
			{
				_inputBuffer[_inputPos++] = c;
				_inputBuffer[_inputPos] = '\0';
				++_inputSize;
			}
			else
			{
				_inputBuffer[_inputPos++] = c;
			}
		}
	}
}

//--------------------------------------------
// delete character at current cursor position
//--------------------------------------------
void Console::DeleteChar()
{
	if(_inputBuffer[_inputPos])
	{
		int pos = _inputPos;
		while(_inputBuffer[pos] = _inputBuffer[pos + 1])
			++pos;
		--_inputSize;
	}
}

//------------------------------------------------
// delete character before current cursor position
//------------------------------------------------
void Console::DeletePrevChar()
{
	if(_inputPos > 0)
	{
		int pos = --_inputPos;
		while(_inputBuffer[pos] = _inputBuffer[pos + 1])
			++pos;
		--_inputSize;
	}
}

void Console::ClearInput()
{
	_inputBuffer[0] = '\0';
	_inputPos = 0;
	_inputSize = 0;
}

//---------------------------------------------------------------------------------
// Completes a part of variable's or command's name in input buffer;
// If more then one variable or command start with that string, they are printed
// to console and input buffer is completed upto common prefix part of their names.
// If input buffer contains whole command name and a space after it,
// help for that variable or command is printed to console.
//---------------------------------------------------------------------------------
void Console::AutoCompleteInput()
{
	if(_inputSize > 0)
	{
		// parse the name
		char name[MAX_INPUT_BUFFER_SIZE];
		char* ptr = _inputBuffer;
		while(iswspace(*ptr)) ++ptr;
		int i = 0;
		while(*ptr && !iswspace(*ptr))
			name[i++] = *ptr++;
		name[i] = '\0';

		// if space is input after the name, find that command and print it's help
		if(*ptr == ' ')
		{
			ConsoleObject* obj = _consoleObjList.Find(name);
			if(obj)
			{
				strcpy(name, obj->GetName());
				while(iswspace(*ptr)) ++ptr;
				strcat(name, " ");
				strcat(name, ptr);
				SetInputString(name);
				const char* help = obj->GetHelp();
				if(help && *help)
					PrintLn("   " ESC_CYAN "%s", help);
			}
		}
		else
		{
			// try to complete the input
			int count = _consoleObjList.GetBeginWith(name, 0);
			if(count)
			{
				ConsoleObject** objs = new(tempPool) ConsoleObject*[count];
				_consoleObjList.GetBeginWith(name, objs);
				if(count == 1)
				{
					SetInputString(objs[0]->GetName());
					InputChar(' ');
				}
				else
				{
					ClearInput();
					int i = 0;
					bool unmatch = false;
					while(char c = objs[0]->GetName()[i])
					{
						for(int j = 1; j < count; ++j)
						{
							if(tolower(objs[j]->GetName()[i]) != tolower(c))
							{
								unmatch = true;
								break;
							}
						}

						if(unmatch)
						{
							break;
						}
						else
						{
							InputChar(c);
							++i;
						}
					}

					PrintLn("Matches:");
					for(i = 0; i < count; ++i)
						PrintLn("   %s", objs[i]->GetName());
				}
				delete[] objs;
			}
		}
	}
}

void Console::MoveInputPosNextWord()
{
	while(_inputBuffer[_inputPos] != ' ')
	{
		if(_inputPos == _inputSize)
			return;
		++_inputPos;
	}

	while(_inputBuffer[_inputPos] == ' ' && _inputPos < _inputSize)
		++_inputPos;
}

void Console::MoveInputPosPrevWord()
{
	if(_inputPos > 0)
		--_inputPos;

	while(_inputBuffer[_inputPos] == ' ' && _inputPos > 0)
		--_inputPos;

	while(_inputBuffer[_inputPos] != ' ')
	{
		if(_inputPos == 0)
			return;
		--_inputPos;
	}
	++_inputPos;
}

void Console::SetInputString(const char* str)
{
	strncpy(_inputBuffer, str, MAX_INPUT_BUFFER_SIZE);
	_inputBuffer[MAX_INPUT_BUFFER_SIZE - 1] = '\0';
	_inputSize = _inputPos = strlen(_inputBuffer);
}

void Console::CmdHistoryBack()
{
	if(_historyCurrentLine == _historyLastLine)
		return;
	else if(_historyCurrentLine == 0)
		_historyCurrentLine = MAX_HISTORY_BUFFER_ENTRIES - 1;
	else
		--_historyCurrentLine;

	SetInputString(_historyBuffer[_historyCurrentLine]);
}

void Console::CmdHistoryForward()
{
	if(_historyCurrentLine == _historyFirstLine)
	{
		ClearInput();
		return;
	}

	if(_historyCurrentLine == MAX_HISTORY_BUFFER_ENTRIES - 1)
		_historyCurrentLine = 0;
	else
		++_historyCurrentLine;

	if(_historyCurrentLine == _historyFirstLine)
	{
		ClearInput();
		return;
	}

	SetInputString(_historyBuffer[_historyCurrentLine]);
}

void Console::CmdHistoryClear()
{
	_historyFirstLine = 0;
	_historyLastLine = 0;
	_historyCurrentLine = 0;
}
