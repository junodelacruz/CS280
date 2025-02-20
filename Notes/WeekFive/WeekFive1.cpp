// Lecture 9 Lexical Analysis [2025-2-20 Thu]
#include <iostream>
#include <string>

using namespace std;

int main(void) {
    // char ch;

    // cin >> ch;
    // cin.putback(ch);
    
    string s;
    cout << (char)cin.peek() << endl;
    cin >> s;
    cout << s << endl;
    cin.putback('h');
    // cin.putback('i');
    cout << s << endl;

    return 0;
}
