#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


int parseIntFromString(string string){

    int result; 

    istringstream ss(string);
    
    if (!(ss >> result)) {
    cerr << "Invalid number: " << string << '\n';
    throw 0;
    } else if (!ss.eof()) {
    cerr << "Trailing characters after number: " << string << '\n';
    throw 0;
    }

    return result;
}

float parseFloatFromString(string string){

    float result; 

    istringstream ss(string);
    
    if (!(ss >> result)) {
    cerr << "Invalid number: " << string << '\n';
    throw 0;
    } else if (!ss.eof()) {
    cerr << "Trailing characters after number: " << string << '\n';
    throw 0;
    }

    return result;
}

int getNextIntFromInputFile(ifstream & inputFile){

    int result;
    string nextValue;

    inputFile >> nextValue;
    result = parseIntFromString(nextValue);

    return result;

}


float getNextFloatFromInputFile(ifstream & inputFile){

    float result;
    string nextValue;

    inputFile >> nextValue;
    result = parseFloatFromString(nextValue);

    return result;

}


void handleError(int);
vector< vector< int > > crearMatrizA (int cantAngulos, int cantRadios, int comienzoPared, int finalPared);
