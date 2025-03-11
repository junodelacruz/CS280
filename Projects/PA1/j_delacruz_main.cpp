#include <iostream>
#include <fstream>
#include <string>
#include "lex.h"

using namespace std;

int main(int argc, char *argv[])
{
    ifstream file(argv[1]);

    bool numFlag = false;
    bool strFlag = false;
    bool idFlag = false;
    bool kwFlag = false;

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
        if (argument == "-all")
        {
            numFlag = true;
            strFlag = true;
            idFlag = true;
            kwFlag = true;
        }
        else if (argument.substr(0, 1) == "-")
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
            else
            {
                cout << "Unrecognized flag {" << argument << "}" << endl;
                exit(1);
            }
        }
        else if (i != 1)
        {
            cout << "Only one file name is allowed." << endl;
        }
    }
    if (file.peek() == EOF)
    {
        cout << "Empty file." << endl;
        exit(1);
    }

    if (numFlag || strFlag || idFlag || kwFlag)
    {
        return 0;
    }
    return 0;
}