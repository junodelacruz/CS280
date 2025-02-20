#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <cctype>
#include <regex>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    bool kwFlag = false;
    bool spFlag = false;
    bool idFlag = false;

    ifstream file(argv[1]);

    if (argc == 1)
    {
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
        exit(1);
    }
    if (!file)
    {
        cout << "CANNOT OPEN THE FILE " << argv[1] << endl;
        exit(1);
    }
    if (file.peek() == EOF)
    {
        cout << "The File Is Empty." << endl;
        exit(1);
    }

    for (int i = 1; i < argc; i++)
    {
        string argument = argv[i];
        if (argument == "-kw")
        {
            kwFlag = true;
        }
        if (argument == "-sp")
        {
            spFlag = true;
        }
        if (argument == "-id")
        {
            idFlag = true;
        }
    }

    int kw = 0;
    int spPercent = 0;
    int spDolla = 0;
    int spAt = 0;
    int id = 0;
    int total = 0;
    int sp = 0;

    list<string> keywordsMatch = {"begin", "end", "if", "else", "while", "for", "break",
                             "continue", "case", "switch", "class", "public", "private", "abstract", "final"};
    
    
    vector<string> keyword;

    string line;
    int lineNum = 0;
    while (getline(file, line))
    {
        lineNum++;

        stringstream ss(line);
        string word;
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        regex spPattern("[$@%][a-zA-Z0-9_]*");
        regex idPattern("[a-zA-Z][a-zA-Z0-9]*");

        while (ss >> word)
        {
            total++;
            // cout << word << endl;
            bool found = find(keywordsMatch.begin(), keywordsMatch.end(), word) != keywordsMatch.end();
            if (regex_match(word, spPattern))
            {
                if (word.substr(0, 1) == "%")
                {
                    spPercent++;
                    sp++;
                }
                else if (word.substr(0, 1) == "$")
                {
                    spDolla++;
                    sp++;
                }
                else if (word.substr(0, 1) == "@")
                {
                    spAt++;
                    sp++;
                }
            }
            else if (found)
            {
                kw++;
                keyword.push_back(word);
            }
            else if (regex_match(word, idPattern))
            {
                id++;
            }
            else
            {
                if (spFlag == true && (word.substr(0, 1) == "@" || word.substr(0, 1) == "%" || word.substr(0, 1) == "$"))
                {
                    cout << "Invalid Special Word at line " << lineNum << ": " << word << endl;
                }
                else if (idFlag == true && isalpha(word.at(0)) != 0)
                {
                    cout << "Invalid Identifier Word at line " << lineNum << ": " << word << endl;
                }
            }
        }
    }
    cout << "Total Number of Lines: " << lineNum << endl;
    cout << "Number of Words: " << total << endl;
    cout << "Number of Special Words: " << sp << endl;
    cout << "Number of Keywords: " << kw << endl;
    cout << "Number of Identifiers: " << id << endl;

    if (spFlag)
    {
        cout << "List of Special Words and their number of occurences: " << endl;
    }

    return 0;
}
