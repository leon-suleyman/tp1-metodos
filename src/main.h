#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;


int parseIntFromString(string string){

    int result; 

    istringstream ss(string);
    
    if (!(ss >> result)) {
    cerr << "Invalid number: " << string << '\n';
    } else if (!ss.eof()) {
    cerr << "Trailing characters after number: " << string << '\n';
    }

    return result;
}

int getNextValueFromInputFile(ifstream & inputFile){

    int result;
    string nextValue;

    inputFile >> nextValue;
    result = parseIntFromString(nextValue);

    return result;

}


void handleError(int);

