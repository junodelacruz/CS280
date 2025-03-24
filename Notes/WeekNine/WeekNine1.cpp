// Lecture 15 PA2 Description for Building SADAL Language Parser [2025-3-24 Mon]

// (slide 12) getnexttoken first thing it does is checks if it previously pushed back a token. use a flag maybe? if we did push something back,
// take that toke. if not, get the next token from the lexer - kapleau

// as we come across variables in our parser, add them to a map for future reference (slide 13)
// ex. x = 1; read x and store it as a potential variable, if it has been declared the value in the map is true, if not declared it is false.

// think about, specifically in regard to variables, the type of the variable. might be important when we go to our interpreter for next pa.

#include <iostream>

using namespace std;

int main(void)
{
    return 0;
}