

// pa1 instead of returning an error switch the lexstate back to start

#include "lex.h"

LexItem getNextToken(istream &in, int &linenum)
{
    enum TokState
    {
        START,
        INID,
        ININT,
        INSTRING,

    } lexstate = START;
    string lexeme;
    char ch;
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
            else if (isalpha(ch) || ch == '_')
            {
                lexeme += ch;
                lexstate = INID;
            }
            else if (isdigit(ch))
            {
                lexeme += ch;
                lexstate = ININT;
            }
            else if (ch == '\"' || ch == '\'')
            {
                lexeme += ch;
                lexstate = INSTRING;
            }
            break;
        case INID:
            if (isalpha(ch) || ch == '_' || isdigit(ch))
            {
                lexeme += ch;
            }
            else
            {
                return LexItem(IDENT, lexeme, linenum);
            }
            break;
        case ININT:
            // ICONST
            if (isspace(ch))
            {
                return LexItem(ICONST, lexeme, linenum);
            }
            else if (isdigit(ch)) // digit?
            {
                lexeme += ch;
            }
            else if (ch == 'E') // E?
            {
                lexeme += ch;
                if (in.peek() == '+' || in.peek() == '-') // check if char after E is + or -
                {
                    lexeme += in.get();
                    if (isdigit(in.peek())) // check if char after + or - is a digit
                    {
                        while (isdigit(in.peek())) // append all additional digits after + -
                        {
                            lexeme += in.get();
                        }
                        return LexItem(ICONST, lexeme, linenum); // after appending, return the lexitem
                    }
                    else // no digit after + or -
                    {
                        return LexItem(ERR, lexeme, linenum); // error
                    }
                }
                else if (isdigit(in.peek())) // check if char after E is just a number
                {
                    while (isdigit(in.peek())) // append all numbers after E
                    {
                        lexeme += in.get();
                    }
                    return LexItem(ICONST, lexeme, linenum); // return lexitem
                }
                else // after E what if it is neither a +/- or a digit? return error
                {
                    return LexItem(ERR, lexeme, linenum);
                }
            }
            // FCONST
            else if (ch == '.')
            {
                if (isdigit(in.peek()))
                {
                    lexeme += ch;
                }
                else
                {
                    return LexItem(ERR, lexeme, linenum);
                }
            }
            else
            {
            }
            break;
        case INSTRING:
            if (ch != '\'' || ch != '\"')
            {
                lexeme += ch;
            }
            else
            {
                lexeme += ch;
                if (lexeme[0] == ch)
                {
                    if (lexeme.length() == 3)
                    {
                        return LexItem(CCONST, lexeme, linenum);
                    }
                    else
                    {
                        return LexItem(SCONST, lexeme, linenum);
                    }
                }
                else
                {
                    return LexItem(ERR, lexeme, linenum);
                }
            }
            break;
        }
    }
}