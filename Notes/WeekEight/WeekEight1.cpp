// Lecture 14 Syntax Analysis Parsing [2025-3-13 Thu]
#include <iostream>

using namespace std;

// A -> (N U T)*    N - nonterminal  T - terminal  U - union  * - 0 or more repetitions  A - single non terminal symbol
// A -> a (context free grammer style)
// <A> -> a (BNF)

// <E> ::= <T | <E> + <T>
// <T> ::= 0 | ... | 9
// example of code for the defined above for <E> expressions and <T> terms
// bool E(void)
// {
//     if (T())
//     {
//         return true;
//     }
//     else if (E())
//     {
//         if (s[0] == '+')
//         {
//             s.subtr(1);
//             if (T())
//             {
//                 return true;
//             }
//             return false;
//         }
//         return false;
//     }
//     return false;
// }

// A ::= a A | B  BNF
// A ::= {a} B  EBNF
// a BNF is already an EBNF grammar, but conversion is still possible as seen above. sort of like how a square is a rectangle, but not vise versa.

// <E> ::= <T> | <T> <TS>
// <T> ::= 0 | ... | 9
// <TS> ::= <OP> <T> | <OP> <T> <TS>
// <OP> ::= + | -
//
// "1-2+3"
// <E> -> <T> <TS>
//     -> 1 <TS>
//     -> 1 <OP> <T> <TS>
//     -> 1 - <T> <TS>
//     -> 1 - 2 <TS>
//     -> 1 - 2 <OP> <T>
//     -> 1 - 2 + <T>
//     -> 1 - 2 + 3

// recursive descent parsing
// this should be how pa2 is gonna be (lecture 14 slides page#19)
// once a token is noticed, call the lex function
// int main(void)
// {
//     if (nextToken == ID_CODE)
//     {
//         lex();
//     }
//     else if (nextToken == INT_CODE)
//     {
//         lex();
//     }
//     else if (nextToken == LP_CODE)
//     {
//         lex();
//         expr();
//         if (nextToken == RP_CODE)
//         {
//             lex();
//         }
//         else
//         {
//             error();
//         }
//     }
//     else
//     {
//         error();
//     }
//     return 0;
// }

// A ::= a B | B A b
// B ::= a A | b
// "ab"
// A -> a B
//   -> a b
//
// A -> B A b
//   -> a A A b

int main(void)
{

    return 0;
}