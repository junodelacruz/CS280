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
		ParseError(line, "Missing Procedure Name.");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (!ProcName(in, line))
	{
		ParseError(line, "Error no procname");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != IS)
	{
		ParseError(line, "Missing 'IS'");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (!ProcBody(in, line))
	{
		ParseError(line, "Invalid procedure body");
		return false;
	}

	return true;
}
// End of Prog

// ProcBody ::= DeclPart BEGIN StmtList END ProcName ;
bool ProcBody(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (!DeclPart)
	{
		ParseError(line, "No DeclPart");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != BEGIN)
	{
		ParseError(line, "No BEGIN");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (!StmtList)
	{
		ParseError(line, "No StmtList");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != END)
	{
		ParseError(line, "No END");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (!ProcName)
	{
		ParseError(line, "No ProcName");
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
// End of ProcBody

// ProcName ::= IDENT
bool ProcName(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok != IDENT)
	{
		ParseError(line, "Missing IDENT");
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
	if (tok == IDENT)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == COMMA)
		{
			DeclStmt(in, line);
		}
		else if (tok == COLON)
		{
			tok = Parser::GetNextToken(in, line);
			if (tok != CCONST || !Type(in, line))
			{
				ParseError(line, "Not CCONST or Type");
				return false;
			}

			tok = Parser::GetNextToken(in, line);
		}
		else
		{
			ParseError(line, "Missing colon");
			return false;
		}
	}
	else
	{
		ParseError(line, "No IDENT");
		return false;
	}

	return true;
}
// End of DeclStmt

// Type ::= INTEGER | FLOAT | BOOLEAN | STRING | CHARACTER
bool Type(istream &in, int &line)
{
	return true;
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
	return true;
}
// End of Stmt

// PrintStmts ::= (PutLine | Put) ( Expr) ;
bool PrintStmts(istream &in, int &line)
{
	return true;
}
// End of PrintStmts

// GetStmt := Get (Var) ;
bool GetStmt(istream &in, int &line)
{
	return true;
}
// End of GetStmt

// IfStmt ::= IF Expr THEN StmtList { ELSIF Expr THEN StmtList } [ ELSE StmtList ] END IF ;
bool IfStmt(istream &in, int &line)
{
	return true;
}
// End of IfStmt

// AssignStmt ::= Var := Expr ;
bool AssignStmt(istream &in, int &line)
{
	return true;
}
// End of AssignStmt

// Var ::= IDENT
bool Var(istream &in, int &line)
{
	return true;
}
// End of Var

// Expr ::= Relation {(AND | OR) Relation }
bool Expr(istream &in, int &line)
{
	return true;
}
// End of Expr

// Relation ::= SimpleExpr [ ( = | /= | < | <= | > | >= ) SimpleExpr ]
bool Relation(istream &in, int &line)
{
	return true;
}
// End of Relation

// SimpleExpr ::= STerm { ( + | - | & ) STerm }
bool SimpleExpr(istream &in, int &line)
{
	return true;
}
// End of SimpleExpr

// STerm ::= [ ( + | - ) ] Term
bool STerm(istream &in, int &line)
{
	return true;
}
// End of STerm

// Term ::= Factor { ( * | / | MOD ) Factor }
bool Term(istream &in, int &line, int &sign)
{
	return true;
}
// End of Term

// Factor ::= Primary [** [(+ | -)] Primary ] | NOT Primary
bool Factor(istream &in, int &line, int &sign)
{
	return true;
}
// End of Factor

// Primary ::= Name | ICONST | FCONST | SCONST | BCONST | CCONST | (Expr)
bool Primary(istream &in, int &line, int &sign)
{
	return true;
}
// End of Primary

// Name ::= IDENT [ ( Range ) ]
bool Name(istream &in, int &line)
{
	return true;
}
// End of Name

// Range ::= SimpleExpr [. . SimpleExpr ]
bool Range(istream &in, int &line)
{
	return true;
}
// End of Range
