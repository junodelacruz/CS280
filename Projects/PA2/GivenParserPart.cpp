/* Implementation of Recursive-Descent Parser
	for the Simple Ada-Like (SADAL) Language
 * parser.cpp
 * Programming Assignment 2
 * Spring 2025
*/
#include <queue>
#include "parser.h"

map<string, bool> defVar;

namespace Parser
{
	bool pushed_back = false;
	LexItem pushed_token;

	static LexItem GetNextToken(istream &in, int &line)
	{
		if (pushed_back)
		{
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem &t)
	{
		if (pushed_back)
		{
			// cout << t;
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

int ErrCount()
{
	return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

// Prog ::= PROCEDURE ProcName IS ProcBody
bool Prog(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok != PROCEDURE)
	{
		ParseError(line, "Incorrect compilation file.");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (defVar.find(tok.GetLexeme()) == defVar.end())
	{
		defVar.insert({tok.GetLexeme(), true});
	}
	else
	{
		ParseError(line, "Variable Redefinition");
		ParseError(line, "Incorrect identifiers list in Declaration Statement.");
		defVar[tok.GetLexeme()] = false;
		return false;
	}

	Parser::PushBackToken(tok);
	if (!ProcName(in, line))
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != IS)
	{
		ParseError(line, "Missing 'IS'");
		return false;
	}

	if (!ProcBody(in, line))
	{
		ParseError(line, "Incorrect Procedure Definition.");
		return false;
	}

	int count = 0;
	cout << "Declared Variables:" << endl;
	for (auto elem : defVar)
	{
		count++;
		cout << elem.first;
		if (count != defVar.size())
		{
			cout << ", ";
		}
	}
	cout << endl
		 << endl;
	cout << "(DONE)" << endl;
	return true;
}
// End of Prog

// ProcBody ::= DeclPart BEGIN StmtList END ProcName ;
bool ProcBody(istream &in, int &line)
{
	LexItem tok;

	if (DeclPart(in, line))
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == BEGIN)
		{
			if (StmtList(in, line))
			{
				tok = Parser::GetNextToken(in, line);
				if (tok == END)
				{
					if (ProcName(in, line))
					{
						tok = Parser::GetNextToken(in, line);
						if (tok == SEMICOL)
						{
							return true;
						}
					}
				}
			}
		}
	}
	ParseError(line, "Incorrect procedure body.");
	return false;
}
// End of ProcBody

// ProcName ::= IDENT
bool ProcName(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);

	if (tok != IDENT)
	{
		ParseError(line, "Missing Procedure Name.");
		return false;
	}
	return true;
}
// End of ProcName

// DeclPart ::= DeclStmt { DeclStmt }
bool DeclPart(istream &in, int &line)
{
	bool status = false;
	LexItem tok;
	// cout << "in DeclPart" << endl;
	status = DeclStmt(in, line);
	if (status)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == BEGIN)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else
		{
			Parser::PushBackToken(tok);
			status = DeclPart(in, line);
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Part.");
		return false;
	}
	return true;
} // End of DeclPart function

// DeclStmt ::= IDENT {, IDENT } : [CONSTANT] Type [(Range)] [:= Expr] ;
bool DeclStmt(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok != IDENT)
	{
		ParseError(line, "No IDENT");
		return false;
	}

	if (defVar.find(tok.GetLexeme()) == defVar.end())
	{
		defVar.insert({tok.GetLexeme(), true});
	}
	else
	{
		ParseError(line, "Variable Redefinition");
		ParseError(line, "Incorrect identifiers list in Declaration Statement.");
		defVar[tok.GetLexeme()] = false;
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == COMMA)
		{
			tok = Parser::GetNextToken(in, line);
			if (tok != IDENT)
			{
				ParseError(line, "No IDENT");
				return false;
			}
			if (defVar.find(tok.GetLexeme()) == defVar.end())
			{
				defVar.insert({tok.GetLexeme(), true});
			}
			else
			{
				ParseError(line, "Variable Redefinition");
				ParseError(line, "Incorrect identifiers list in Declaration Statement.");
				defVar[tok.GetLexeme()] = false;
				return false;
			}
		}
		else
		{
			if (tok == COLON)
			{
				break;
			}
			ParseError(line, "Missing comma in declaration statement.");
			ParseError(line, "Incorrect identifiers list in Declaration Statement.");
			return false;
		}
	}

	if (tok != COLON)
	{
		ParseError(line, "Missing colon");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != CONST)
	{
		Parser::PushBackToken(tok);
	}

	if (!Type(in, line))
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == LPAREN)
	{
		if (!Range(in, line))
		{
			ParseError(line, "Invalid Range");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok != RPAREN)
		{
			ParseError(line, "Missing paren for range");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
	}

	if (tok == ASSOP)
	{
		if (!Expr(in, line))
		{
			ParseError(line, "Invalid EXPR in DeclStmt");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
	}

	if (tok != SEMICOL)
	{
		ParseError(line, "Missing semicolon at end of statement");
		return false;
	}

	return true;
}
// End of DeclStmt

// Type ::= INTEGER | FLOAT | BOOLEAN | STRING | CHARACTER
bool Type(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok == INT || tok == FLOAT || tok == BOOL || tok == STRING || tok == CHAR)
	{
		return true;
	}

	ParseError(line, "Incorrect Declaration Type.");
	return false;
}
// End of Type

// StmtList ::= Stmt { Stmt }
bool StmtList(istream &in, int &line)
{
	bool status;
	LexItem tok;
	// cout << "in StmtList" << endl;
	status = Stmt(in, line);
	tok = Parser::GetNextToken(in, line);
	while (status && (tok != END && tok != ELSIF && tok != ELSE))
	{
		Parser::PushBackToken(tok);
		status = Stmt(in, line);
		tok = Parser::GetNextToken(in, line);
	}
	if (!status)
	{
		ParseError(line, "Syntactic error in statement list.");
		return false;
	}
	Parser::PushBackToken(tok); // push back the END token
	return true;
}
// End of StmtList

// Stmt ::= AssignStmt | PrintStmts | GetStmt | IfStmt
bool Stmt(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok == GET)
	{
		Parser::PushBackToken(tok);
		if (!GetStmt(in, line))
		{
			ParseError(line, "Invalid get statement.");
			return false;
		}
	}
	else if (tok == PUT || tok == PUTLN)
	{
		Parser::PushBackToken(tok);
		if (!PrintStmts(in, line))
		{
			ParseError(line, "Invalid put statement.");
			return false;
		}
	}
	else if (tok == IF)
	{
		Parser::PushBackToken(tok);
		if (!IfStmt(in, line))
		{
			ParseError(line, "Invalid if statement.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(tok);
		if (!AssignStmt(in, line))
		{
			ParseError(line, "Invalid assignment statement.");
			return false;
		}
	}

	return true;
}
// End of Stmt

// PrintStmts ::= (PutLine | Put) ( Expr) ;
bool PrintStmts(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok == PUT || tok == PUTLN)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok != LPAREN)
		{
			ParseError(line, "Missing Left Parenthesis");
			return false;
		}

		if (!Expr(in, line))
		{
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok != RPAREN)
		{
			ParseError(line, "Missing Right Parenthesis");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL)
		{
			line--;
			ParseError(line, "Missing semicolon at end of statement");
			return false;
		}
		return true;
	}
	return false;
}
// End of PrintStmts

// GetStmt := Get (Var) ;
bool GetStmt(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok != GET)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != LPAREN)
	{
		return false;
	}

	if (!Var(in, line))
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RPAREN)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != SEMICOL)
	{
		ParseError(line, "Missing semicolon at end of statement");
		return false;
	}
	return true;
}
// End of GetStmt

// IfStmt ::= IF Expr THEN StmtList { ELSIF Expr THEN StmtList } [ ELSE StmtList ] END IF ;
bool IfStmt(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok != IF)
	{
		return false;
	}

	if (!Expr(in, line))
	{
		ParseError(line, "Invalid if statement condition");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != THEN)
	{
		ParseError(line, "Missing Statement for If-Stmt Then-clause");
		return false;
	}

	if (!StmtList(in, line))
	{
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == ELSIF)
		{
			if (!Expr(in, line))
			{
				return false;
			}

			tok = Parser::GetNextToken(in, line);
			if (tok != THEN)
			{
				ParseError(line, "Missing Statement for If-Stmt Then-clause");
				return false;
			}

			if (!StmtList(in, line))
			{
				return false;
			}
		}
		else
		{
			break;
		}
	}

	if (tok == ELSE)
	{
		if (!StmtList(in, line))
		{
			return false;
		}
		tok = Parser::GetNextToken(in, line);
	}

	if (tok != END)
	{
		ParseError(line, "Missing closing END IF for IF-statement.");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != IF)
	{
		ParseError(line, "Missing closing END IF for IF-statement.");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != SEMICOL)
	{
		ParseError(line, "Missing semicolon at end of statement");
		return false;
	}

	return true;
}
// End of IfStmt

// AssignStmt ::= Var := Expr ;
bool AssignStmt(istream &in, int &line)
{
	LexItem tok;

	if (!Var(in, line))
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != ASSOP)
	{
		ParseError(line, "Missing Assignment Operator");
		return false;
	}

	if (!Expr(in, line))
	{
		ParseError(line, "Missing Expression in Assignment Statement");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != SEMICOL)
	{
		ParseError(line, "Missing semicolon at end of statement");
		return false;
	}

	return true;
}
// End of AssignStmt

// Var ::= IDENT
bool Var(istream &in, int &line)
{

	// maybe add undefined reference thing here for variables
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok != IDENT)
	{
		Parser::PushBackToken(tok);
		return false;
	}

	return true;
}
// End of Var

// Expr ::= Relation {(AND | OR) Relation }
bool Expr(istream &in, int &line)
{
	LexItem tok;

	if (!Relation(in, line))
	{
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == AND || tok == OR)
		{
			if (!Relation(in, line))
			{
				return false;
			}
		}
		else
		{
			Parser::PushBackToken(tok);
			break;
		}
	}

	return true;
}
// End of Expr

// Relation ::= SimpleExpr [ ( = | /= | < | <= | > | >= ) SimpleExpr ]
bool Relation(istream &in, int &line)
{
	LexItem tok;
	if (!SimpleExpr(in, line))
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == EQ || tok == NEQ || tok == LTHAN || tok == LTE || tok == GTHAN || tok == GTE)
	{
		if (!SimpleExpr(in, line))
		{
			return false;
		}
		return true;
	}

	Parser::PushBackToken(tok);
	return true;
}
// End of Relation

// SimpleExpr ::= STerm { ( + | - | & ) STerm }
bool SimpleExpr(istream &in, int &line)
{
	LexItem tok;

	if (!STerm(in, line))
	{
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == PLUS || tok == MINUS || tok == AND)
		{
			if (!STerm(in, line))
			{
				ParseError(line, "Missing operand after operator");
				return false;
			}
		}
		else
		{
			Parser::PushBackToken(tok);
			break;
		}
	}
	return true;
}
// End of SimpleExpr

// STerm ::= [ ( + | - ) ] Term
bool STerm(istream &in, int &line)
{
	LexItem tok;
	int sign;

	tok = Parser::GetNextToken(in, line);
	if (tok == PLUS)
	{
		sign = 1;
	}
	else if (tok == MINUS)
	{
		sign = 0;
	}
	if (tok == PLUS || tok == MINUS)
	{
		if (!Term(in, line, sign))
		{
			return false;
		}
		return true;
	}

	Parser::PushBackToken(tok);
	if (!Term(in, line, sign))
	{
		return false;
	}
	return true;
}
// End of STerm

// Term ::= Factor { ( * | / | MOD ) Factor }
bool Term(istream &in, int &line, int sign)
{
	LexItem tok;

	if (!Factor(in, line, sign))
	{
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == MULT || tok == DIV || tok == MOD)
		{
			if (!Factor(in, line, sign))
			{
				ParseError(line, "Missing operand after operator");
				return false;
			}
		}
		else
		{
			Parser::PushBackToken(tok);
			break;
		}
	}
	return true;
}
// End of Term

// Factor ::= Primary [** [(+ | -)] Primary ] | NOT Primary
bool Factor(istream &in, int &line, int sign)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok == NOT)
	{
		Parser::PushBackToken(tok);
		if (!Primary(in, line, sign))
		{
			ParseError(line, "Incorrect operand");
			return false;
		}
		return true;
	}
	else
	{
		Parser::PushBackToken(tok);
		if (!Primary(in, line, sign))
		{
			ParseError(line, "Incorrect operand");
			return false;
		}
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == EXP)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == PLUS || tok == MINUS)
		{
			if (!Primary(in, line, sign))
			{
				ParseError(line, "Incorrect operand");
				return false;
			}
			return true;
		}

		Parser::PushBackToken(tok);
		if (!Primary(in, line, sign))
		{
			ParseError(line, "Incorrect operand");
			return false;
		}
	}

	Parser::PushBackToken(tok);
	return true;
}
// End of Factor

// Primary ::= Name | ICONST | FCONST | SCONST | BCONST | CCONST | (Expr)
bool Primary(istream &in, int &line, int sign)
{
	LexItem tok;
	tok = Parser::GetNextToken(in, line);
	if (tok == ICONST || tok == FCONST || tok == SCONST || tok == BCONST || tok == CCONST || tok == LPAREN)
	{
		if (tok == LPAREN)
		{
			if (!Expr(in, line))
			{
				ParseError(line, "Invalid Expression");
				return false;
			}

			tok = Parser::GetNextToken(in, line);
			if (tok != RPAREN)
			{
				ParseError(line, "Missing right parenthesis after expression");
				return false;
			}
		}
		return true;
	}
	else
	{
		Parser::PushBackToken(tok);
		if (Name(in, line))
		{
			return true;
		}
	}
	ParseError(line, "Invalid reference to a variable.");
	return false;
}
// End of Primary

// Name ::= IDENT [ ( Range ) ]
bool Name(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (defVar.find(tok.GetLexeme()) == defVar.end())
	{
		ParseError(line, "Using Undefined Variable");
		return false;
	}

	if (tok != IDENT)
	{
		ParseError(line, "Missing IDENT in Name");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == LPAREN)
	{
		if (!Range(in, line))
		{
			ParseError(line, "Missing range after lparen in Name");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok != RPAREN)
		{
			ParseError(line, "Missing RPAREN after range in Name");
			return false;
		}

		return true;
	}

	Parser::PushBackToken(tok);
	return true;
}
// End of Name

// Range ::= SimpleExpr [. . SimpleExpr ]
bool Range(istream &in, int &line)
{
	LexItem tok;

	if (!SimpleExpr(in, line))
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == DOT)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok != DOT)
		{
			return false;
		}

		if (!SimpleExpr(in, line))
		{
			return false;
		}
		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}
// End of Range
