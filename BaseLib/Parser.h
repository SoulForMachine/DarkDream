
#ifndef _BASELIB_PARSER_H_
#define _BASELIB_PARSER_H_

#include "List.h"
#include "StaticArray.h"
#include "SmartPtr.h"
#include "Exception.h"


namespace FileUtil
{
	class File;
}


class BASELIB_API ParserException: public Exception
{
public:
	ParserException(const char* desc)
		: Exception(desc) {}

protected:
	ParserException() {}
};

class BASELIB_API ParserNoInputException: public ParserException
{
public:
	ParserNoInputException()
		: ParserException("No input is loaded for parsing.") {}
};

class BASELIB_API ParserEndOfInputException: public ParserException
{
public:
	ParserEndOfInputException()
		: ParserException("Parsing reached the end of input buffer.") {}
};

class BASELIB_API ParserInvalidCharException: public ParserException
{
public:
	ParserInvalidCharException(char c, int line)
	{
		_c = c;
		_line = line;
		SetDesc("Invalid character \'%c\' on line %d.", c, line);
	}

	char GetChar() const
		{ return _c; }
	int GetLine() const
		{ return _line; }

private:
	char _c;
	int _line;
};

class BASELIB_API ParserStrMissQuoteException: public ParserException
{
public:
	ParserStrMissQuoteException(int line)
	{
		_line = line;
		SetDesc("String missing closing quote on line %d.", line);
	}

	int GetLine() const
		{ return _line; }

private:
	int _line;
};

class BASELIB_API ParserUnexpectedException: public ParserException
{
public:
	ParserUnexpectedException(int line, const char* expected, const char* found)
	{
		if(found)
			SetDesc("Parse error on line %d: expected \'%s\', found \'%s\'.", line, expected, found);
		else
			SetDesc("Parse error on line %d: expected \'%s\'.", line, expected);
	}
};

class BASELIB_API ParserTooLongException: public ParserException
{
public:
	ParserTooLongException(int line, const char* what, size_t max_size)
	{
		SetDesc("Parse error on line %d: %s too long, max size is %u.", line, what, max_size);
	}
};



class BASELIB_API Parser
{
public:
	enum TokenType
	{
		TOK_KEYWORD,
		TOK_IDENTIFIER,
		TOK_LITERAL,
		TOK_OPERATOR,
		TOK_PUNCTUATION,
	};

	enum LiteralType
	{
		LIT_STRING,
		LIT_INTEGER,
		LIT_FLOAT,
		LIT_BOOL
	};

	enum OperatorType
	{
		// logic
		OP_LESS,
		OP_GREATER,
		OP_EQUAL,
		OP_NOT_EQUAL,
		OP_LESS_OR_EQUAL,
		OP_GREATER_OR_EQUAL,

		// arithmetic
		OP_ASSIGN,
		OP_PLUS,
		OP_MINUS,
		OP_MULTIPLY,
		OP_DIVIDE,
		OP_MODULO,

		OP_COUNT
	};

	enum PunctType
	{
		PUNCT_OPEN_BRACE,
		PUNCT_CLOSE_BRACE,
		PUNCT_OPEN_BRACKET,
		PUNCT_CLOSE_BRACKET,
		PUNCT_OPEN_PARENTHESES,
		PUNCT_CLOSE_PARENTHESES,
		PUNCT_SEMICOLON,
		PUNCT_COLON,
		PUNCT_COMMA,
		PUNCT_DOT,

		PUNCT_COUNT
	};

	struct Token
	{
		SmartPtr<char> str;
		int length;
		TokenType type;
		union
		{
			LiteralType subTypeLiteral;
			OperatorType subTypeOperator;
			PunctType subTypePunct;
		};
	};

	Parser();
	~Parser();

	bool LoadFile(const tchar* file_name);
	bool LoadFile(FileUtil::File& file);
	void LoadMem(const char* buf, size_t size);
	void Unload();

	void Parse();
	void SetKeywords(const char** keywords, size_t count);
	const List<Token>& GetTokenList() const
		{ return _tokens; }
	bool IsEndOfInput() const
		{ return !*_ptr; }
	int GetCurrentLine() const
		{ return _line; }
	char GetInvalidChar() const
		{ return _invalidChar; }

	void ExpectTokenString(const char* str);
	void ExpectTokenType(TokenType type, int sub_type);
	void ReadInt(int& i);
	void ReadFloat(float& f);
	void ReadVec2f(float* v);
	void ReadVec3f(float* v);
	void ReadVec4f(float* v);
	void ReadVec2i(int* v);
	void ReadVec3i(int* v);
	void ReadVec4i(int* v);
	void ReadString(char* str, size_t max_char);
	void ReadBool(bool& b);
	void ReadIdentifier(char* str, size_t max_char);
	void ReadOperator(OperatorType& op_type);
	void ReadToken(Token& token);

private:
	struct Operator
	{
		OperatorType type;
		const char* str;
	};

	struct Punct
	{
		PunctType type;
		const char* str;
	};

	void GetToken(Token& token);
	OperatorType GetOperatorType(const char* op_str);
	const char* GetOperatorStr(OperatorType type);
	PunctType GetPunctType(const char* p_str);
	const char* GetPunctStr(PunctType type);
	void DeleteTokens();

	char* _buffer;
	char* _ptr;
	StaticArray<char*> _keywords;
	List<Token> _tokens;
	int _line; // current line
	char _invalidChar; // an invalid character that parser has encountered
	static const Operator _operators[OP_COUNT];
	static const Punct _puncts[PUNCT_COUNT];
};


#endif // _BASELIB_PARSER_H_
