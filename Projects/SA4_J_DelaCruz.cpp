#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <cctype>
#include <regex>
#include <vector>
#include <map>

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

    for (int i = 1; i < argc; i++)
    {
        string argument = argv[i];
        if (argument == "-kw")
        {
            kwFlag = true;
        }
        else if (argument == "-sp")
        {
            spFlag = true;
        }
        else if (argument == "-id")
        {
            idFlag = true;
        }
        else if(i!=1)
        {
            cout << "UNRECOGNIZED FLAG " << argument << endl;
            exit(1);
        }
    }
    if (file.peek() == EOF)
    {
        cout << "File Is Empty." << endl;
        exit(1);
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

    map<string, int> specialWords;
    map<string, int> keywords;
    map<string, int> identifiers;

    string line;
    int lineNum = 0;
    while (getline(file, line))
    {
        lineNum++;

        stringstream ss(line);
        string word;

        regex spPattern("[$@%][a-zA-Z0-9_]*");
        regex idPattern("[a-zA-Z][a-zA-Z0-9]*");

        while (ss >> word)
        {
            total++;
            // cout << word << endl;
            string originalWord = word;
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            
            bool findKw = find(keywordsMatch.begin(), keywordsMatch.end(), word) != keywordsMatch.end();
            if (regex_match(originalWord, spPattern))
            {
                if (originalWord.substr(0, 1) == "%")
                {
                    spPercent++;
                    sp++;
                }
                else if (originalWord.substr(0, 1) == "$")
                {
                    spDolla++;
                    sp++;
                }
                else if (originalWord.substr(0, 1) == "@")
                {
                    spAt++;
                    sp++;
                }

                if (specialWords.find(originalWord) != specialWords.end())
                {
                    specialWords[originalWord]++;
                }
                else
                {
                    specialWords[originalWord] = 1;
                }
            }
            else if (findKw)
            {
                kw++;
                if (keywords.find(word) != keywords.end())
                {
                    keywords[word]++;
                }
                else
                {
                    keywords[word] = 1;
                }
            }
            else if (regex_match(originalWord, idPattern))
            {
                id++;
                if (identifiers.find(originalWord) != identifiers.end())
                {
                    identifiers[originalWord]++;
                }
                else
                {
                    identifiers[originalWord] = 1;
                }
            }
            else
            {
                if (spFlag == true && (word.substr(0, 1) == "@" || word.substr(0, 1) == "%" || word.substr(0, 1) == "$"))
                {
                    cout << "Invalid Special Word: " << word << endl;
                }
                else if (idFlag == true && isalpha(word.at(0)) != 0)
                {
                    cout << "Invalid Identifier Word: " << word << endl;
                }
            }
        }
    }
    cout << "Total Number of Lines: " << lineNum << endl;
    cout << "Number of Words: " << total << endl;
    cout << "Number of Special Words: " << sp << endl;
    cout << "Number of Identifiers: " << id << endl;
    cout << "Number of Keywords: " << kw << endl;

    if (spFlag)
    {
        cout << endl << "List of Special Words and their number of occurrences:" << endl;
        for (auto i = specialWords.begin(); i != specialWords.end(); i++)
        {
            cout << i->first << ": " << i->second << endl;
        }
    }
    if (kwFlag)
    {
        cout << endl << "List of Keywords and their number of occurrences:" << endl;
        for (auto i = keywords.begin(); i != keywords.end(); i++)
        {
            cout << i->first << ": " << i->second << endl;
        }
    }
    if (idFlag)
    {
        cout << endl << "List of Identiers and their number of occurrences:" << endl;
        for (auto i = identifiers.begin(); i != identifiers.end(); i++)
        {
            cout << i->first << ": " << i->second << endl;
        }
    }
    return 0;
}
