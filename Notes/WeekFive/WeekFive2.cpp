// Lecture 10 Building Lexical Analyzer [2025-2-24 Mon]
#include <fstream>
#include <iostream>

using namespace std;

struct LexItem
{
    string token;
    string lexeme;
    int linenumber;
};
LexItem getNextToken(istream &in, int &linenumber)
{
    // char next;
    // while(cin >> next){
    //     if(next){

    //     }
    // }
    return (LexItem){};
}

int main(int argc, char *argv[])
{
    ifstream fin("main.cc");
    int line = 1;

    getNextToken(fin, line);

    // ifstream fin(argc == 2 ? argv[1] : "default.txt");

    // //to check if it opens
    // if(!fin) {
    //     cout << "failed" << endl;
    // }
    // else {
    //     cout << "success" << endl;
    // }

    return 0;
}