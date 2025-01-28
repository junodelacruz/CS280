#include <iostream>
using namespace std;

int main(void){
    string input = "";
    cout << "Welcome to CS280 (Spring 2025) \nWhat is your first name, last name, and section number?" << endl;
    cin >> input;
    char section = input.at(input.length() - 1);
    cout << section;
    cout << "Hello " + input + ", Welcome to CS 280 Section ";
}

