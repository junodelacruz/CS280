#include "lex.h"

LexItem getNextToken(istream &in, int &linenum)
{
    enum TokState
    {
        START,
        INID,
        ININT,

    } lexstate = START;
    string lexeme;
    char ch;
    while (in.get(ch))
    {
        switch (lexstate)
        {
        case START:
            break;
        }
    }
}