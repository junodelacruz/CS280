#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "lex.h"

using namespace std;

void pushUniqueString(std::vector<string> &vec, string value, int &num)
{
    if (find(vec.begin(), vec.end(), value) == vec.end())
    {
        vec.push_back(value);
        num++;
    }
}

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
    map<Token, string> kwSet;
    vector<LexItem> otherSet;
    vector<LexItem> allTokens;

    unordered_map<Token, string> keywords = {
        {AND, "and"},
        {IF, "if"},
        {ELSE, "else"},
        {ELSIF, "elsif"},
        {PUT, "put"},
        {PUTLN, "putln"},
        {GET, "get"},
        {INT, "int"},
        {FLOAT, "float"},
        {CHAR, "char"},
        {STRING, "string"},
        {BOOL, "bool"},
        {PROCEDURE, "procedure"},
        {TRUE, "true"},
        {FALSE, "false"},
        {END, "end"},
        {IS, "is"},
        {BEGIN, "begin"},
        {THEN, "then"},
        {CONST, "const"},
        {MOD, "mod"},
        {OR, "or"},
        {NOT, "not"},
    };

    while ((token = getNextToken(file, linenum)) != DONE)
    {
        totalTok++;
        if (token.GetToken() == ICONST || token.GetToken() == FCONST)
        {
            pushUniqueString(numSet, token.GetLexeme(), totalNum);
            allTokens.push_back(token);
        }
        else if (token.GetToken() == CCONST || token.GetToken() == SCONST)
        {
            pushUniqueString(strSet, token.GetLexeme(), totalStr);
            allTokens.push_back(token);
        }
        else if (token.GetToken() == IDENT)
        {
            pushUniqueString(idSet, token.GetLexeme(), totalIden);
            allTokens.push_back(token);
        }
        else if (keywords.find(token.GetToken()) != keywords.end())
        {
            kwSet.insert({token.GetToken(), token.GetLexeme()});
            allTokens.push_back(token);
        }
        else
        {
            otherSet.push_back(token);
            allTokens.push_back(token);
        }
    }
    bool summary = true;
    totalKw = kwSet.size();
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
        cout << "Lines: " << linenum - 1 << endl
             << "Total Tokens: " << totalTok << endl
             << "Numerals: " << totalNum << endl
             << "Characters and Strings : " << totalStr << endl
             << "Identifiers: " << totalIden << endl
             << "Keywords: " << totalKw << endl;
    }

    sort(idSet.begin(), idSet.end());
    sort(strSet.begin(), strSet.end());

    vector<double> sortedNums;
    for (auto elem : numSet)
    {
        sortedNums.push_back(stod(elem));
    }
    sort(sortedNums.begin(), sortedNums.end());

    if (numFlag && summary)
    {
        cout << "NUMERIC CONSTANTS:" << endl;
        for (int i = 0; i < sortedNums.size(); i++)
        {
            cout << sortedNums[i];
            if (i != sortedNums.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
    if (strFlag && summary)
    {
        cout << "CHARACTERS AND STRINGS:" << endl;
        for (int i = 0; i < strSet.size(); i++)
        {
            cout << '"' << strSet[i] << '"';
            if (i != strSet.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
    if (idFlag && summary)
    {
        cout << "IDENTIFIERS:" << endl;
        for (int i = 0; i < idSet.size(); i++)
        {
            cout << idSet[i];
            if (i != idSet.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
    if (kwFlag && summary)
    {
        cout << "KEYWORDS:" << endl;
        for (auto elem = kwSet.begin(); elem != kwSet.end(); ++elem)
        {
            cout << elem->second;
            if (std::next(elem) != kwSet.end())
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
    return 0;
}