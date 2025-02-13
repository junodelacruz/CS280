#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <cctype>
#include <regex>

using namespace std;

int main(void)
{
    string filename;
    string command;
    bool kwFlag = false;
    bool spFlag = false;
    bool idFlag = false;

    cin >> filename;

    ifstream file(filename + ".txt");

    if (filename.empty())
    {
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
        exit(1);
    }
    if (!file)
    {
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        exit(1);
    }
    if (file.peek() == EOF)
    {
        cout << "The File Is Empty." << endl;
        exit(1);
    }

    cin >> command;
    if (command == "-kw")
    {
        kwFlag = true;
    }
    else if (command == "-sp")
    {
        spFlag = true;
    }

    else if (command == "-id")
    {
        idFlag = true;
    }
    else
    {
        kwFlag = true;
        spFlag = true;
        idFlag = true;
    }

    int kw = 0;
    int spPercent = 0;
    int spDolla = 0;
    int spAt = 0;
    int id = 0;
    list<string> keywords = {"begin", "end", "if", "else", "while", "for", "break",
                             "continue", "case", "switch", "class", "public", "private", "abstract", "final"};

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string word;
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        regex spPattern("[$@%][a-zA-Z0-9_]*");
        regex idPattern("[a-zA-Z][a-zA-Z0-9]*");

        while (ss >> word)
        {
            // cout << word << endl;
            bool found = find(keywords.begin(), keywords.end(), word) != keywords.end();
            if (regex_match(word, spPattern))
            {
                if (word.substr(0, 1) == "%")
                {
                    spPercent++;
                }
                else if (word.substr(0, 1) == "$")
                {
                    spDolla++;
                }
                else if (word.substr(0, 1) == "@")
                {
                    spAt++;
                }
            }
            else if (found)
            {
                kw++;
            }
            else if (regex_match(word, idPattern))
            {
                id++;
            }
            else
            {
                //check if invalid special or ident
            }
        }
    }

    if (kwFlag)
    {
        cout << kw << endl;
    }
    if (spFlag)
    {
        cout << "Number of Special Words Starting With $: " << spDolla << endl;
        cout << "Number of Special Words Starting With @: " << spAt << endl;
        cout << "Number of Special Words Starting With %: " << spPercent << endl;
    }
    if (idFlag)
    {
        cout << id << endl;
    }

    return 0;
}