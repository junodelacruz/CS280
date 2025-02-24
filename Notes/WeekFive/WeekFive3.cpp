// Lecture 10 Building Lexical Analyzer [2025-2-24 Mon]
#include <iostream>
#include <string>

using namespace std;

// definition of data types: code for defining what these data types are
// <ident> ::= <letter> | <letter> <symbol>
// <symbol> ::= <letter> | <digit> | <letter> <symbol> | <digit> <symbol>
// <letter> ::= a | ... | z | _
// <digit> :: = 0 | ... | 9

string s;
bool ident(void);
bool symbol(void);
bool letter(void);
bool digit(void);

bool ident(void)
{
    if ('a' <= s[0] && s[0] <= 'z')
    {
        s = s.substr(1);
        if (symbol())
        {
            return true;
        }
    }
    return false;
}
bool symbol(void)
{
    if (letter() || digit())
    {
        if (symbol())
        {
            return true;
        }
        return true;
    }
    return false;
}

bool digit(void)
{
    if ('0' <= s[0] && s[0] <= '9')
    {
        s = s.substr(1);
        return true;
    }
    return false;
}

bool letter(void)
{
    if ('a' <= s[0] && s[0] <= 'z' || s[0] == '_')
    {
        s = s.substr(1);
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    s = argv[1];
    cout << "The string \"" << argv[1] << "\" is " << (ident() ? "" : "not") << " in the language." << endl;

    return 0;
}