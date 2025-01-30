#include <iostream>
using namespace std;

int main(void){
    string input = "";
    cout << "Welcome to CS 280 (Spring 2025)\nWhat are your first name, last name, and section number?" << endl;
    getline(cin, input);
    string section;
    for(int i = 0; i<input.length(); i++){
        if(isdigit(input[i])){
            section += input[i];
        }
    }
    string firstname = input.substr(0, input.find(" "));
    cout << "Hello " + firstname + ", Welcome to CS 280 Section " + section + ".\n";
}

