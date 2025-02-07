#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <algorithm>

using namespace std;

int main(void) {
    string filename;
    cout << "Enter the file name to read from:" << endl;
    cin >> filename;
    
    ifstream file(filename+".txt"); //added .txt for testing purposes, vocareum only takes filename no extension

    if(!file) {
        cout << endl << "CANNOT OPEN THE FILE " << filename << endl;
        file.close();
        exit(1);
    }

    if(file.peek() == EOF) {
        cout << "The File Is Empty." << endl;
        exit(1);
    }


    int totalLines = 0;
    int commentedLines = 0;
    int commandLines = 0;
    int bold = 0; 
    int italic = 0;
    int underline = 0;
    int regular = 0;

    string line;
    while(getline(file, line)) {
        totalLines++;

        transform(line.begin(), line.end(), line.begin(), ::tolower);

        if(line.substr(0,2) == "//") {
            commentedLines++;          
        }
        if(line.substr(0,2) == ">>") { 

            string command = line.substr(2);
            command.erase(remove_if(command.begin(), command.end(), ::isspace), command.end());

            if(command == "bo" || command.substr(0,4) == "bold") {
                bold++;
                commandLines++;
            }
            else if(command == "it" || command.substr(0,6) == "italic") {
                italic++;
                commandLines++;
            }
            else if(command == "un" || command.substr(0,9) == "underline") {
                underline++;
                commandLines++;
            }
            else if(command == "re" || command.substr(0,7) == "regular") {
                regular++;
                commandLines++;
            }
            else if(command.empty() == true || command == ">re"){
                cout << "Error: No command name following a command prompt in line " << totalLines << endl;
            }
            else {
                cout << "Error: Unrecognizable command name \"" << command << "\" in line " << totalLines << endl;
            }
        }
    }

    cout << "\nTotal lines: " << totalLines << endl << "Commented lines: " << commentedLines << endl 
    << "Command lines: " << commandLines << endl << "Bold commands: " << bold << endl << "Italic commands: " << italic 
    << endl << "Regular commands: " << regular << endl << "Underline commands: " << underline << endl;

    return 0;
}