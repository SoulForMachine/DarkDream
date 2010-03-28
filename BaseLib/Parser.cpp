
#include "Memory.h"
#include "Algorithm.h"
#include "FileUtil.h"
#include "Console.h"
#include "Parser.h"

using namespace Memory;
using namespace FileUtil;


const Parser::Operator Parser::_operators[OP_COUNT] =
{
	{ OP_LESS, "<" },
	{ OP_GREATER, ">" },
	{ OP_EQUAL, "==" },
	{ OP_NOT_EQUAL, "!=" },
	{ OP_LESS_OR_EQUAL, "<=" },
	{ OP_GREATER_OR_EQUAL, ">=" },

	{ OP_ASSIGN, "=" },
	{ OP_PLUS, "+" },
	{ OP_MINUS, "-" },
	{ OP_MULTIPLY, "*" },
	{ OP_DIVIDE, "/" },
	{ OP_MODULO, "%" },
};

const Parser::Punct Parser::_puncts[PUNCT_COUNT] =
{
	{ PUNCT_OPEN_BRACE, "{" },
	{ PUNCT_CLOSE_BRACE, "}" },
	{ PUNCT_OPEN_BRACKET, "[" },
	{ PUNCT_CLOSE_BRACKET, "]" },
	{ PUNCT_OPEN_PARENTHESES, "(" },
	{ PUNCT_CLOSE_PARENTHESES, ")" },
	{ PUNCT_SEMICOLON, ";" },
	{ PUNCT_COLON, ":" },
	{ PUNCT_COMMA, "," },
	{ PUNCT_DOT, "." },
};


static
bool IsOperatorChar(char c)
{
	const char* op = "<>=!%*/-+";
	while(*op)
		if(c == *op++)
			return true;
	return false;
}

static
bool IsPunctChar(char c)
{
	const char* op = "()[]{};:,.";
	while(*op)
		if(c == *op++)
			return true;
	return false;
}


Parser::Parser()
	: _tokens(tempPool),
	_keywords(tempPool)
{
	_buffer = 0;
	_ptr = 0;
	_line = 1;
}

Parser::~Parser()
{
	Unload();
}

bool Parser::LoadFile(const tchar* file_name)
{
	FsysFile file;
	if(!file.Open(file_name, _t("rb")))
		return false;

	return LoadFile(file);
}

bool Parser::LoadFile(FileUtil::File& file)
{
	Unload();

	long size = file.GetSize();
	_buffer = new(tempPool) char[size + 1];
	file.Read(_buffer, size);
	_buffer[size] = '\0';
	_ptr = _buffer;

	// replace carriage returns with spaces
	char* p = _buffer;
	while(*p)
	{
		if(*p == '\r')
			*p = ' ';
		++p;
	}

	return true;
}

void Parser::LoadMem(const char* buf, size_t size)
{
	Unload();

	_buffer = new(tempPool) char[size + 1];
	strncpy(_buffer, buf, size);
	_buffer[size] = '\0';
	_ptr = _buffer;

	// replace carriage returns with spaces
	char* p = _buffer;
	while(*p)
	{
		if(*p == '\r')
			*p = ' ';
		++p;
	}
}

void Parser::Unload()
{
	delete[] _buffer;
	_buffer = 0;
	_ptr = 0;
	_line = 1;

	for(size_t i = 0; i < _keywords.GetCount(); ++i)
		delete[] _keywords[i];
	_keywords.Clear();

	DeleteTokens();
}

void Parser::Parse()
{
	DeleteTokens();
	Token token;

	while(1)
	{
		try
		{
			GetToken(token);
		}
		catch(ParserEndOfInputException&)
		{
			break;
		}
		catch(...)
		{
			DeleteTokens();
			throw;
		}

		_tokens.PushBack(token);
	}
}

void Parser::SetKeywords(const char** keywords, size_t count)
{
	_keywords.SetCount(count);
	for(size_t i = 0; i < count; ++i)
	{
		_keywords[i] = new(tempPool) char[strlen(keywords[i]) + 1];
		strcpy(_keywords[i], keywords[i]);
	}

	QSortStrings((const char**)_keywords.GetData(), count);
}

void Parser::ExpectTokenString(const char* str)
{
	Token token;
	GetToken(token);

	bool result = !strcmp(str, token.str);
	if(!result)
		throw ParserUnexpectedException(_line, str, token.str);
}

void Parser::ExpectTokenType(TokenType type, int sub_type)
{
	Token token;
	GetToken(token);

	bool result = (token.type == type);

	if(result)
	{
		switch(token.type)
		{
		case TOK_LITERAL:
			result = (token.subTypeLiteral == sub_type);
			break;
		case TOK_OPERATOR:
			result = (token.subTypeOperator == sub_type);
			break;
		case TOK_PUNCTUATION:
			result = (token.subTypePunct == sub_type);
			break;
		}
	}

	if(!result)
	{
		const char* str = "<unknown>";
		switch(token.type)
		{
		case TOK_KEYWORD:
			str = "keyword";
			break;
		case TOK_IDENTIFIER:
			str = "identifier";
			break;
		case TOK_LITERAL:
			str = "literal";
			break;
		case TOK_OPERATOR:
			str = GetOperatorStr((OperatorType)sub_type);
			break;
		case TOK_PUNCTUATION:
			str = GetPunctStr((PunctType)sub_type);
			break;
		}

		throw ParserUnexpectedException(_line, str, token.str);
	}
}

void Parser::ReadInt(int& i)
{
	Token token;
	GetToken(token);

	if(token.type != TOK_LITERAL || token.subTypeLiteral != LIT_INTEGER)
	{
		throw ParserUnexpectedException(_line, "integer", 0);
	}

	i = atoi(token.str);
}

void Parser::ReadFloat(float& f)
{
	Token token;
	GetToken(token);

	if(token.type != TOK_LITERAL || token.subTypeLiteral != LIT_FLOAT)
	{
		throw ParserUnexpectedException(_line, "float", 0);
	}

	f = (float)atof(token.str);
}

void Parser::ReadVec2f(float* v)
{
	ExpectTokenType(TOK_PUNCTUATION, PUNCT_OPEN_BRACKET);

	ReadFloat(v[0]);
	ReadFloat(v[1]);

	ExpectTokenType(TOK_PUNCTUATION, PUNCT_CLOSE_BRACKET);
}

void Parser::ReadVec3f(float* v)
{
	ExpectTokenType(TOK_PUNCTUATION, PUNCT_OPEN_BRACKET);

	ReadFloat(v[0]);
	ReadFloat(v[1]);
	ReadFloat(v[2]);

	ExpectTokenType(TOK_PUNCTUATION, PUNCT_CLOSE_BRACKET);
}

void Parser::ReadVec4f(float* v)
{
	ExpectTokenType(TOK_PUNCTUATION, PUNCT_OPEN_BRACKET);

	ReadFloat(v[0]);
	ReadFloat(v[1]);
	ReadFloat(v[2]);
	ReadFloat(v[3]);

	ExpectTokenType(TOK_PUNCTUATION, PUNCT_CLOSE_BRACKET);
}

void Parser::ReadVec2i(int* v)
{
	ExpectTokenType(TOK_PUNCTUATION, PUNCT_OPEN_BRACKET);

	ReadInt(v[0]);
	ReadInt(v[1]);

	ExpectTokenType(TOK_PUNCTUATION, PUNCT_CLOSE_BRACKET);
}

void Parser::ReadVec3i(int* v)
{
	ExpectTokenType(TOK_PUNCTUATION, PUNCT_OPEN_BRACKET);

	ReadInt(v[0]);
	ReadInt(v[1]);
	ReadInt(v[2]);

	ExpectTokenType(TOK_PUNCTUATION, PUNCT_CLOSE_BRACKET);
}

void Parser::ReadVec4i(int* v)
{
	ExpectTokenType(TOK_PUNCTUATION, PUNCT_OPEN_BRACKET);

	ReadInt(v[0]);
	ReadInt(v[1]);
	ReadInt(v[2]);
	ReadInt(v[3]);

	ExpectTokenType(TOK_PUNCTUATION, PUNCT_CLOSE_BRACKET);
}

void Parser::ReadString(char* str, size_t max_char)
{
	Token token;
	GetToken(token);

	if(token.type != TOK_LITERAL || token.subTypeLiteral != LIT_STRING)
	{
		throw ParserUnexpectedException(_line, "string", 0);
	}

	// copy the string, exclude quotes
	size_t len = strlen(token.str);
	if(len - 2 >= max_char)
	{
		throw ParserTooLongException(_line, "string", max_char);
	}

	token.str[len - 1] = '\0';
	strcpy(str, token.str + 1);
}

void Parser::ReadBool(bool& b)
{
	Token token;
	GetToken(token);

	if(token.type != TOK_LITERAL || token.subTypeLiteral != LIT_BOOL)
	{
		throw ParserUnexpectedException(_line, "bool", 0);
	}

	b = (strcmp(token.str, "True") == 0);
}

void Parser::ReadIdentifier(char* str, size_t max_char)
{
	Token token;
	GetToken(token);

	if(token.type != TOK_IDENTIFIER)
	{
		throw ParserUnexpectedException(_line, "identifier", 0);
	}

	// copy the string
	size_t len = strlen(token.str);
	if(len >= max_char)
	{
		throw ParserTooLongException(_line, "identifier", max_char);
	}

	strcpy(str, token.str);
}

void Parser::ReadOperator(OperatorType& op_type)
{
	Token token;
	GetToken(token);

	if(token.type != TOK_OPERATOR)
	{
		throw ParserUnexpectedException(_line, "operator", 0);
	}

	op_type = GetOperatorType(token.str);
}

void Parser::ReadToken(Token& token)
{
	GetToken(token);
}


void Parser::GetToken(Token& token)
{
	if(!_buffer)
		throw ParserNoInputException();

	if(IsEndOfInput())
		throw ParserEndOfInputException();

	char* p = _ptr;

	// skip whitespace
	while(iswspace(*p))
	{
		if(*p == '\n')
			++_line;
		++p;
		if(*p == 0)
			throw ParserEndOfInputException();
	}

	// skip comments
	while(*p == '/')
	{
		if(*(p+1) == '/')
		{
			p += 2;
			while(1)
			{
				if(*p == '\n')
				{
					++p;
					++_line;
					break;
				}
				else if(*p == 0)
				{
					throw ParserEndOfInputException();
				}
				++p;
			}
		}
		else if(*(p+1) == '*')
		{
			p += 2;
			while(1)
			{
				if(*p == '*' && *(p+1) == '/')
				{
					p += 2;
					break;
				}
				else if(*p == 0)
				{
					throw ParserEndOfInputException();
				}
				else if(*p == '\n')
				{
					++_line;
				}
				++p;
			}
		}
		else
		{
			++p;
		}

		// skip whitespace
		while(iswspace(*p))
		{
			if(*p == '\n')
				++_line;
			++p;
			if(*p == 0)
				throw ParserEndOfInputException();
		}
	}

	// get token
	char* t = p;
	if(isalpha(*p) || *p == '_')
	{
		token.type = TOK_IDENTIFIER;

		while(1)
		{
			++p;

			if(*p == 0 || iswspace(*p) || !(isalnum(*p) || *p == '_'))
				break;
		}
	}
	else if(isdigit(*p) || (*p == '.' && isdigit(*(p+1))) || (*p == '-' && isdigit(*(p+1))) || (*p == '-' && *(p+1) == '.'))
	{
		token.type = TOK_LITERAL;
		token.subTypeLiteral = LIT_INTEGER;

		bool dot = (*p == '.');

		while(1)
		{
			++p;

			if(*p == '.')
			{
				if(dot)
					break;
				else
				{
					token.subTypeLiteral = LIT_FLOAT;
					dot = true;
				}
			}
			else if(*p == 0 || iswspace(*p))
				break;
			else if(isalpha(*p))
			{
				_invalidChar = *p;
				throw ParserInvalidCharException(_invalidChar, _line);
			}
			else if(!isdigit(*p))
			{
				break;
			}
		}
	}
	else if(*p == '\"' || *p == '\'')
	{
		token.type = TOK_LITERAL;
		token.subTypeLiteral = LIT_STRING;

		char quote = *p;
		while(1)
		{
			++p;
			if(*p == '\n' || *p == '\0')
			{
				throw ParserStrMissQuoteException(_line);
			}
			else if(*p == '\\')
			{
				p++;
			}
			else if(*p == quote)
			{
				++p;
				break;
			}
		}
	}
	else if(IsOperatorChar(*p))
	{
		token.type = TOK_OPERATOR;

		switch(*p)
		{
		case '<':
		case '>':
		case '!':
		case '=':
		/*case '%':
		case '*':
		case '/':
		case '-':
		case '+':*/
			if(*(p+1) == '=')
				p += 2;
			else
				++p;

			break;
		default:
			++p;
			break;
		}
	}
	else if(IsPunctChar(*p))
	{
		token.type = TOK_PUNCTUATION;

		++p;
	}
	else
	{
		_invalidChar = *p;
		throw ParserInvalidCharException(_invalidChar, _line);
	}

	int len = p - t;
	token.str = new(tempPool) char[len + 1];
	strncpy(token.str, t, len);
	token.str[len] = '\0';
	token.length = len;

	if(token.type == TOK_IDENTIFIER)
	{
		// check if it's a keyword
		int index = BinSearch(token.str, (const char**)_keywords.GetData(), _keywords.GetCount());
		if(index != -1)
			token.type = TOK_KEYWORD;

		// check if it's a bool literal
		if(!strcmp(token.str, "True") || !strcmp(token.str, "False"))
		{
			token.type = TOK_LITERAL;
			token.subTypeLiteral = LIT_BOOL;
		}
	}
	else if(token.type == TOK_OPERATOR)
	{
		token.subTypeOperator = GetOperatorType(token.str);
	}
	else if(token.type == TOK_PUNCTUATION)
	{
		token.subTypePunct = GetPunctType(token.str);
	}

	_ptr = p;
}

Parser::OperatorType Parser::GetOperatorType(const char* op_str)
{
	for(int i = 0; i < OP_COUNT; ++i)
	{
		if(!strcmp(op_str, _operators[i].str))
			return _operators[i].type;
	}

	return OP_COUNT;
}

const char* Parser::GetOperatorStr(OperatorType type)
{
	for(int i = 0; i < OP_COUNT; ++i)
	{
		if(type == _operators[i].type)
			return _operators[i].str;
	}

	return "";
}

Parser::PunctType Parser::GetPunctType(const char* p_str)
{
	for(int i = 0; i < PUNCT_COUNT; ++i)
	{
		if(!strcmp(p_str, _puncts[i].str))
			return _puncts[i].type;
	}

	return PUNCT_COUNT;
}

const char* Parser::GetPunctStr(PunctType type)
{
	for(int i = 0; i < PUNCT_COUNT; ++i)
	{
		if(type == _puncts[i].type)
			return _puncts[i].str;
	}

	return "";
}

void Parser::DeleteTokens()
{
	_tokens.Clear();
}
