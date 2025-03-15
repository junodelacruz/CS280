#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "lex.h"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream file(argv[1]);

    bool numFlag = false;
    bool strFlag = false;
    bool idFlag = false;
    bool kwFlag = false;
    bool allFlag = false;

    if (argc == 1)
    {
        cout << "No specified input file." << endl;
        exit(1);
    }
    if (!file)
    {
        cout << "CANNOT OPEN THE FILE " << argv[1] << endl;
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        string argument = argv[i];

        if (argument.substr(0, 1) == "-")
        {
            if (argument == "-str")
            {
                strFlag = true;
            }
            else if (argument == "-id")
            {
                idFlag = true;
            }
            else if (argument == "-kw")
            {
                kwFlag = true;
            }
            else if (argument == "-all")
            {
                allFlag = true;
            }
            else if (argument == "-num")
            {
                numFlag = true;
            }
            else
            {
                cout << "Unrecognized flag {" << argument << "}" << endl;
                exit(1);
            }
        }
        else if (i != 1)
        {
            cout << "Only one file name is allowed." << endl;
            exit(1);
        }
    }
    if (file.peek() == EOF)
    {
        cout << "Empty file." << endl;
        exit(1);
    }

    int linenum = 1;
    int totalTok = 0;
    int totalNum = 0;
    int totalStr = 0;
    int totalIden = 0;
    int totalKw = 0;
    LexItem token;
    vector<string> numSet;
    vector<string> strSet;
    vector<string> idSet;
    vector<LexItem> kwSet;
    vector<LexItem> otherSet;
    vector<LexItem> allTokens;

    vector<Token> keywords = {IF, ELSE, ELSIF, PUT, PUTLN, GET, INT, FLOAT, CHAR, STRING, BOOL, PROCEDURE, TRUE, FALSE, END, IS, BEGIN, THEN, CONST};

    while ((token = getNextToken(file, linenum)) != DONE)
    {
        totalTok++;
        if (token.GetToken() == ICONST || token.GetToken() == FCONST)
        {
            numSet.push_back(token.GetLexeme());
            allTokens.push_back(token);
            totalNum++;
        }
        else if (token.GetToken() == CCONST || token.GetToken() == SCONST)
        {
            totalStr++;
            allTokens.push_back(token);
            strSet.push_back(token.GetLexeme());
        }
        else if (token.GetToken() == IDENT)
        {
            totalIden++;
            allTokens.push_back(token);
            idSet.push_back(token.GetLexeme());
        }
        else if (find(keywords.begin(), keywords.end(), token.GetToken()) != keywords.end())
        {
            totalKw++;
            allTokens.push_back(token);
            kwSet.push_back(token);
        }
        else
        {
            otherSet.push_back(token);
            allTokens.push_back(token);
        }
    }
    bool summary = true;
    if (allFlag)
    {
        for (auto lexitem : allTokens)
        {
            cout << lexitem << endl;
            if (lexitem.GetToken() == ERR)
            {
                summary = false;
                break;
            }
        }
    }
    if (summary)
    {
        cout << endl;
        cout << "Lines: " << linenum << endl
             << "Total Tokens: " << totalTok << endl
             << "Numerals: " << totalNum << endl
             << "Characters and Strings: " << totalStr << endl
             << "Identifiers: " << totalIden << endl
             << "Keywords: " << totalKw << endl;
    }

    sort(idSet.begin(), idSet.end());

    if (numFlag && !allFlag)
    {
        for (auto lexitem : numSet)
        {
            cout << lexitem << endl;
        }
    }
    if (strFlag && !allFlag)
    {
        for (auto lexitem : strSet)
        {
            cout << lexitem << endl;
        }
    }
    if (idFlag && !allFlag)
    {
        cout << "IDENTIFIERS: " << endl;
        for (int i = 0; i < idSet.size(); i++)
        {
            cout << idSet[i];
            if (i != idSet.size() - 1)
            {
                cout << ", ";
            }
        }
    }
    if (kwFlag && !allFlag)
    {
        cout << "KEYWORDS: " << endl;
        for (int i = 0; i < kwSet.size(); i++)
        {
            cout << kwSet[i];
            if (i != kwSet.size() - 1)
            {
                cout << ", ";
            }
        }
    }
    bool compareByToken(const LexItem &a, const LexItem &b)
    {
        return a.GetToken() < b.GetToken();
    }
    return 0;
}