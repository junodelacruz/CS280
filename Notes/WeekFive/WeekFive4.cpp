// Lecture 10 Building Lexical Analyzer [2025-2-27 Thu]
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    char ch;
    string s;

    cout << (char)cin.peek() << endl;
    cin.get(ch);
    cout << ch << endl;

    cin >> s;
    cout << s << endl;

    cin.get(ch);
    cout << ch << endl;
    cin.putback(ch);
    cin >> s;
    cout << s << endl;

    return 0;
}