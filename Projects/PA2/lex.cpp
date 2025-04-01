#include "lex.h"
#include <unordered_map>
#include <iostream>
#include <algorithm>

LexItem getNextToken(istream &in, int &linenum)
{
	enum TokState
	{
		START,
		INID,
		ININT,
		INSTRING,
		INFLOAT,

	} lexstate = START;
	string lexeme;
	string line;
	char ch;
	unordered_map<char, Token> symbols = {
		{'+', PLUS},
		{'*', MULT},
		{'/', DIV},
		{'=', EQ},
		{'<', LTHAN},
		{'>', GTHAN},
		{'&', CONCAT},
		{',', COMMA},
		{';', SEMICOL},
		{'(', LPAREN},
		{')', RPAREN},
		{':', COLON},
		{'.', DOT},
	};

	while (in.get(ch))
	{
		switch (lexstate)
		{
		case START:
			if (isspace(ch))
			{
				if (ch == '\n')
				{
					linenum++;
				}
				continue;
			}
			else if (ch == '-')
			{
				if (in.peek() == '-')
				{
					getline(in, line);
					linenum++;
					break;
				}
				else
				{
					return LexItem(MINUS, lexeme, linenum);
				}
			}
			else if (isalpha(ch))
			{
				lexeme += ch;
				lexstate = INID;
			}
			else if (isdigit(ch))
			{
				lexeme += ch;
				lexstate = ININT;
			}
			else if (ch == '"' || ch == '\'')
			{
				lexeme += ch;
				lexstate = INSTRING;
			}
			else if (ch == ':')
			{
				lexeme += ch;
				if (in.peek() == '=')
				{
					lexeme += in.get();
					return LexItem(ASSOP, lexeme, linenum);
				}
				else
				{
					return LexItem(COLON, lexeme, linenum);
				}
			}
			else if (ch == '<')
			{
				lexeme += ch;
				if (in.peek() == '=')
				{
					lexeme += in.get();
					return LexItem(LTE, lexeme, linenum);
				}
				return LexItem(LTHAN, lexeme, linenum);
			}
			else if (ch == '>')
			{
				lexeme += ch;
				if (in.peek() == '=')
				{
					lexeme += in.get();
					return LexItem(GTE, lexeme, linenum);
				}
				return LexItem(GTHAN, lexeme, linenum);
			}
			else if (ch == '/')
			{
				lexeme += ch;
				if (in.peek() == '=')
				{
					lexeme += in.get();
					return LexItem(NEQ, lexeme, linenum);
				}
				return LexItem(DIV, lexeme, linenum);
			}
			else if (ch == '*')
			{
				lexeme += ch;
				if (in.peek() == '*')
				{
					lexeme += in.get();
					return LexItem(EXP, lexeme, linenum);
				}
				return LexItem(MULT, lexeme, linenum);
			}
			else if (symbols.count(ch))
			{
				lexeme += ch;
				return LexItem(symbols[ch], lexeme, linenum);
			}
			else
			{
				lexeme += ch;
				return LexItem(ERR, lexeme, linenum);
			}
			break;
		case INID:
			if (isalnum(ch) || (ch == '_' && lexeme.back() != '_'))
			{
				lexeme += ch;
			}
			else
			{
				in.unget();
				transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
				return id_or_kw(lexeme, linenum);
			}
			break;
		case ININT:
			// ICONST
			if (isdigit(ch)) // digit?
			{
				lexeme += ch;
			}
			else if (ch == '.')
			{
				if (isdigit(in.peek()))
				{
					lexeme += ch;
					lexstate = INFLOAT;
					break;
				}
				in.unget();
				return LexItem(ICONST, lexeme, linenum);
			}
			else if (ch == 'e' || ch == 'E') // E?
			{
				if (in.peek() == '+' || in.peek() == '-')
				{
					lexeme += ch;
					lexeme += in.get();
					if (isdigit(in.peek()))
					{
						while (isdigit(in.peek()))
						{
							lexeme += in.get();
						}
						return LexItem(ICONST, lexeme, linenum);
					}
					else
					{
						in.unget();
						return LexItem(ERR, lexeme, linenum);
					}
				}
				else if (isdigit(in.peek()))
				{
					lexeme += ch;
					while (isdigit(in.peek()))
					{
						lexeme += in.get();
					}
					return LexItem(ICONST, lexeme, linenum);
				}
				else if (isspace(in.peek()))
				{
					in.unget();
					lexstate = START;
					break;
				}
				else
				{
					in.unget();
					return LexItem(ICONST, lexeme, linenum);
				}
			}
			else
			{
				in.unget();
				return LexItem(ICONST, lexeme, linenum);
			}
			break;
		case INFLOAT:
			// FCONST
			if (isdigit(ch))
			{
				lexeme += ch;
			}
			else if (ch == 'e' || ch == 'E')
			{
				if (in.peek() == '+' || in.peek() == '-')
				{
					lexeme += ch;
					lexeme += in.get();
					if (isdigit(in.peek()))
					{
						while (isdigit(in.peek()))
						{
							lexeme += in.get();
						}
						return LexItem(FCONST, lexeme, linenum);
					}
					else
					{
						in.unget();
						return LexItem(ERR, lexeme, linenum);
					}
				}
				else if (isdigit(in.peek()))
				{
					lexeme += ch;
					while (isdigit(in.peek()))
					{
						lexeme += in.get();
					}
					return LexItem(FCONST, lexeme, linenum);
				}
				else if (isspace(in.peek()))
				{
					in.unget();
					return LexItem(FCONST, lexeme, linenum);
				}
				else
				{
					in.unget();
					return LexItem(FCONST, lexeme, linenum);
				}
			}
			else if (ch == '.')
			{
				lexeme += ch;
				return LexItem(ERR, lexeme, linenum);
			}
			else
			{
				in.unget();
				return LexItem(FCONST, lexeme, linenum);
			}
			break;
		case INSTRING:
			if (ch != '\n')
			{
				if (lexeme.at(0) == '\'' && lexeme.length() > 2)
				{
					return LexItem(ERR, " Invalid character constant " + lexeme + '\'', linenum);
				}
				if (ch == '"')
				{
					if (lexeme.at(0) == '"')
					{
						lexeme.erase(remove(lexeme.begin(), lexeme.end(), '"'), lexeme.end());
						return LexItem(SCONST, lexeme, linenum);
					}
					else if (lexeme.at(0) == '\'')
					{
						return LexItem(ERR, " Invalid string constant " + lexeme + '"', linenum);
					}
					else
					{
						return LexItem(ERR, "New line is an invalid character constant.", linenum);
					}
				}
				else if (ch == '\'')
				{
					if (lexeme.at(0) == '\'' && lexeme.length() == 2)
					{
						lexeme.erase(remove(lexeme.begin(), lexeme.end(), '\''), lexeme.end());
						return LexItem(CCONST, lexeme, linenum);
					}
					else if (lexeme.at(0) == '"')
					{
						return LexItem(ERR, " Invalid string constant " + lexeme + '\'', linenum);
					}
					else
					{
						return LexItem(ERR, "New line is an invalid character constant.", linenum);
					}
				}
				else if (in.peek() == '\n')
				{
					return LexItem(ERR, " Invalid string constant " + lexeme + ' ', linenum);
				}
				lexeme += ch;
			}
			break;
		}
	}
	return LexItem(DONE, "", linenum);
}

LexItem id_or_kw(const string &lexeme, int linenum)
{
	unordered_map<string, Token> reservedWords = {
		{"procedure", PROCEDURE},
		{"string", STRING},
		{"else", ELSE},
		{"if", IF},
		{"integer", INT},
		{"float", FLOAT},
		{"character", CHAR},
		{"put", PUT},
		{"putline", PUTLN},
		{"get", GET},
		{"boolean", BOOL},
		{"true", TRUE},
		{"false", FALSE},
		{"elsif", ELSIF},
		{"is", IS},
		{"end", END},
		{"begin", BEGIN},
		{"then", THEN},
		{"constant", CONST},
		{"mod", MOD},
		{"and", AND},
		{"or", OR},
		{"not", NOT}};

	auto it = reservedWords.find(lexeme);
	if (it != reservedWords.end())
	{
		if (it->second == TRUE || it->second == FALSE)
		{
			return LexItem(BCONST, lexeme, linenum);
		}
		return LexItem(it->second, lexeme, linenum);
	}
	return LexItem(IDENT, lexeme, linenum);
}

ostream &operator<<(ostream &out, const LexItem &tok)
{
	map<Token, string> tokenToString = {
		{IF, "IF"}, {ELSE, "ELSE"}, {ELSIF, "ELSIF"}, {PUT, "PUT"}, {PUTLN, "PUTLN"}, {GET, "GET"}, {INT, "INT"}, {FLOAT, "FLOAT"}, {CHAR, "CHAR"}, {STRING, "STRING"}, {BOOL, "BOOL"}, {PROCEDURE, "PROCEDURE"}, {TRUE, "TRUE"}, {FALSE, "FALSE"}, {END, "END"}, {IS, "IS"}, {BEGIN, "BEGIN"}, {THEN, "THEN"}, {CONST, "CONST"}, {IDENT, "IDENT"}, {ICONST, "ICONST"}, {FCONST, "FCONST"}, {SCONST, "SCONST"}, {BCONST, "BCONST"}, {CCONST, "CCONST"}, {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULT, "MULT"}, {DIV, "DIV"}, {ASSOP, "ASSOP"}, {EQ, "EQ"}, {NEQ, "NEQ"}, {EXP, "EXP"}, {CONCAT, "CONCAT"}, {GTHAN, "GTHAN"}, {LTHAN, "LTHAN"}, {LTE, "LTE"}, {GTE, "GTE"}, {AND, "AND"}, {OR, "OR"}, {NOT, "NOT"}, {MOD, "MOD"}, {COMMA, "COMMA"}, {SEMICOL, "SEMICOL"}, {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {DOT, "DOT"}, {COLON, "COLON"}, {ERR, "ERR"}, {DONE, "DONE"}};

	if (tok == ICONST)
	{
		out << "ICONST: (" << tok.GetLexeme() << ')';
	}
	else if (tok == BCONST)
	{
		out << "BCONST: (" << tok.GetLexeme() << ')';
	}
	else if (tok == FCONST)
	{
		out << "FCONST: (" << tok.GetLexeme() << ')';
	}
	else if (tok == IDENT)
	{
		out << "IDENT: <" << tok.GetLexeme() << '>';
	}
	else if (tok == SCONST)
	{
		out << "SCONST: \"" << tok.GetLexeme() << '"';
	}
	else if (tok == CCONST)
	{
		out << "CCONST: \'" << tok.GetLexeme() << '\'';
	}
	else if (tok == ERR)
	{
		out << "ERR: In line " << tok.GetLinenum() << ", Error Message {" << tok.GetLexeme() << "}";
	}
	else
	{
		out << tokenToString[tok.GetToken()];
	}
	return out;
}
