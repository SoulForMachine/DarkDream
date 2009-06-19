
#ifndef _BASELIB_PARSER_H_
#define _BASELIB_PARSER_H_

#include "List.h"
#include "StaticArray.h"
#include "SmartPtr.h"


namespace FileSys
{
	class File;
}

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

	enum Error
	{
		ERR_NO_ERROR,
		ERR_NO_INPUT_LOADED,
		ERR_END_OF_INPUT,
		ERR_INVALID_CHARACTER,
		ERR_STRING_MISSING_CLOSING_QUOTE,
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
	bool LoadFile(FileSys::File& file);
	void LoadMem(const char* buf, size_t size);
	void Unload();

	Error Parse();
	void SetKeywords(const char** keywords, size_t count);
	const List<Token>& GetTokenList() const
		{ return _tokens; }
	bool IsEndOfInput() const
		{ return !*_ptr; }
	int GetCurrentLine() const
		{ return _line; }
	char GetInvalidChar() const
		{ return _invalidChar; }

	bool ExpectTokenString(const char* str);
	bool ExpectTokenType(TokenType type, int sub_type);
	bool ReadInt(int& i);
	bool ReadFloat(float& f);
	bool ReadVec2f(float* v);
	bool ReadVec3f(float* v);
	bool ReadVec4f(float* v);
	bool ReadVec2i(int* v);
	bool ReadVec3i(int* v);
	bool ReadVec4i(int* v);
	bool ReadString(char* str, size_t max_char);
	bool ReadIdentifier(char* str, size_t max_char);
	bool ReadOperator(OperatorType& op_type);
	bool ReadToken(Token& token);

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

	Error GetToken(Token& token);
	OperatorType GetOperatorType(const char* op_str);
	const char* GetOperatorStr(OperatorType type);
	PunctType GetPunctType(const char* p_str);
	const char* GetPunctStr(PunctType type);
	void DeleteTokens();
	void PrintError(Error err);

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
