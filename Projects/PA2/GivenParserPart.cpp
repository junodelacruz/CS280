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

	if (!DeclPart(in, line))
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

	if (!StmtList(in, line))
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

	if (!ProcName(in, line))
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
	if (tok != IDENT)
	{
		ParseError(line, "No IDENT");
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
		}
		else
		{
			break;
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
		ParseError(line, "Missing Type in DeclStmt");
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
	}

	if (tok == ASSOP)
	{
		if (!Expr(in, line))
		{
			ParseError(line, "Invalid EXPR in DeclStmt");
			return false;
		}
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
	if (AssignStmt(in, line) || PrintStmts(in, line) || GetStmt(in, line) || IfStmt(in, line))
	{
		return true;
	}

	ParseError(line, "invalid stmts in stmt");
	return false;
}
// End of Stmt

// PrintStmts ::= (PutLine | Put) ( Expr) ;
bool PrintStmts(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok.GetLexeme() != "PutLine" || tok.GetLexeme() != "Put")
	{
		ParseError(line, "No putline in printstmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != LPAREN)
	{
		ParseError(line, "Missing Lparen in printstmt");
		return false;
	}

	if (!Expr(in, line))
	{
		ParseError(line, "Missing expr in printstmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RPAREN)
	{
		ParseError(line, "Missing Rparen in printstmt");
		return false;
	}

	return true;
}
// End of PrintStmts

// GetStmt := Get (Var) ;
bool GetStmt(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok.GetLexeme() != "Get")
	{
		ParseError(line, "Missing Get in getstmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != LPAREN)
	{
		ParseError(line, "Missing LPAREN in getstmt");
		return false;
	}

	if (!Var(in, line))
	{
		ParseError(line, "Missing Var in getstmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RPAREN)
	{
		ParseError(line, "Missing RPAREN in getstmt");
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
		ParseError(line, "Missing IF in Ifstmt");
		return false;
	}

	if (!Expr(in, line))
	{
		ParseError(line, "Missing EXPR in Ifstmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != THEN)
	{
		ParseError(line, "Missing THEN in Ifstmt");
		return false;
	}

	if (!StmtList(in, line))
	{
		ParseError(line, "Missing StmtList in Ifstmt");
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == ELSIF)
		{
			if (!Expr(in, line))
			{
				ParseError(line, "Missing EXPR in Ifstmt");
				return false;
			}

			tok = Parser::GetNextToken(in, line);
			if (tok != THEN)
			{
				ParseError(line, "Missing THEN in Ifstmt");
				return false;
			}

			if (!StmtList(in, line))
			{
				ParseError(line, "Missing StmtList in Ifstmt");
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
			ParseError(line, "missing stmtlist after ELSE in ifstmt");
			return false;
		}
	}

	if (tok != END)
	{
		ParseError(line, "Missing END in ifstmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != IF)
	{
		ParseError(line, "Missing IF in ifstmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != SEMICOL)
	{
		ParseError(line, "Missing semicol in ifstmt");
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
		ParseError(line, "Missing Var in AssignStmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != ASSOP)
	{
		ParseError(line, "Missing Assop in AssignStmt");
		return false;
	}

	if (!Expr(in, line))
	{
		ParseError(line, "Missing Expr in AssignStmt");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != SEMICOL)
	{
		ParseError(line, "Missing semicol in AssignStmt");
		return false;
	}

	return true;
}
// End of AssignStmt

// Var ::= IDENT
bool Var(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);
	if (tok != IDENT)
	{
		ParseError(line, "Invalid IDENT in Var");
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
		ParseError(line, "Missing relation in Expr");
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == AND || tok == OR)
		{
			if (!Relation(in, line))
			{
				ParseError(line, "Missing relation in Expr");
				return false;
			}
		}
		else
		{
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
		ParseError(line, "Missing simpleexpr in relation");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == EQ || tok == NEQ || tok == LTHAN || tok == LTE || tok == GTHAN || tok == GTE)
	{
		if (!SimpleExpr(in, line))
		{
			ParseError(line, "Missing SimpleExpr after operator in Relation");
			return false;
		}
	}
	return true;
}
// End of Relation

// SimpleExpr ::= STerm { ( + | - | & ) STerm }
bool SimpleExpr(istream &in, int &line)
{
	LexItem tok;

	if (!STerm(in, line))
	{
		ParseError(line, "Missing STerm in SimpleExpr");
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == PLUS || tok == MINUS || tok == AND)
		{
			if (!STerm(in, line))
			{
				ParseError(line, "Missing STerm after operator in SimpleExpr");
				return false;
			}
		}
		else
		{
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
			ParseError(line, "Missing Term after operator in STerm");
			return false;
		}
	}

	Parser::PushBackToken(tok);
	if (!Term(in, line, sign))
	{
		ParseError(line, "Missing Term after operator in STerm");
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
		ParseError(line, "Missing Factor in Term");
		return false;
	}

	while (true)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == MULT || tok == DIV || tok == MOD)
		{
			if (!Factor(in, line, sign))
			{
				ParseError(line, "Missing Factor in Term");
				return false;
			}
		}
		else
		{
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
		if (!Primary(in, line, sign))
		{
			ParseError(line, "Missing Primary in Factor");
			return false;
		}
		return true;
	}
	else if (!Primary(in, line, sign))
	{
		ParseError(line, "Missing Primary in Factor");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == EXP)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == PLUS || tok == MINUS)
		{
			if (!Primary(in, line, sign))
			{
				ParseError(line, "Missing primary after operator in Factor");
				return false;
			}
		}

		Parser::PushBackToken(tok);
		if (!Primary(in, line, sign))
		{
			ParseError(line, "Missing primary after operator in Factor");
			return false;
		}
	}
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
			Parser::PushBackToken(tok);
			if (!Expr(in, line))
			{
				ParseError(line, "Invalid Expr in Primary");
				return false;
			}

			tok = Parser::GetNextToken(in, line);
			if (tok != RPAREN)
			{
				ParseError(line, "Missing RPAREN after expr in primary");
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

	return false;
}
// End of Primary

// Name ::= IDENT [ ( Range ) ]
bool Name(istream &in, int &line)
{
	LexItem tok;

	tok = Parser::GetNextToken(in, line);

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
		ParseError(line, "Missing SimpleExpr in range");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == DOT)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok != DOT)
		{
			ParseError(line, "Missing dot after dot in range");
			return false;
		}

		if (!SimpleExpr(in, line))
		{
			ParseError(line, "Missing SimpleExpr in range");
			return false;
		}
	}

	return true;
}
// End of Range
