#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>

using namespace std;

int main(void)
{
    string filename;
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

    int kw = 0;
    int sp = 0;
    int id = 0;
    list<string> keywords = {"begin", "end", "if", "else", "while", "for", "break",
                             "continue", "case", "switch", "class", "public", "private", "abstract", "final"};

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string word;
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        while (ss >> word)
        {
            // cout << word << endl;
            bool found = find(keywords.begin(), keywords.end(), word) != keywords.end();
            if (word.substr(0, 1) == "$" || word.substr(0, 1) == "@" || word.substr(0, 1) == "%")
            {
                sp++;
            }
            else if (found)
            {
                kw++;
            }
            else
            {
                id++;
            }
        }
    }

    cout << sp << " " << kw << " " << id << endl;

    return 0;
}