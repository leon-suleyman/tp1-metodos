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

void printMatriz(vector<vector<float>> matriz)
{
    int cantFilas = matriz.size();
    int cantColumnas = matriz[0].size();
    

    for (int i = 0; i < cantFilas; i++)
    {
        clog << "(";
        for (int j = 0; j < cantColumnas; j++)
        {
            clog << matriz[i][j];

            if(j != cantColumnas -1){
                clog << ", ";
            } 
        }
        clog << ")" << endl;
    }
    
    clog << endl;
}

void handleError(int);

float coeficienteA(float diffEntreRadios, int j, int radioInterno);
float coeficienteB(float diffEntreAngulos, float diffEntreRadios, int j, int radioInterno);
float coeficienteC(float diffEntreRadios);
float coeficienteD(float diffEntreAngulos, float diffEntreRadios, int j, int radioInterno);


vector< vector< float > > crearMatrizA (int cantAngulos, int cantRadios, int comienzoPared, int finalPared);

void eliminacionGaussiana (vector<vector<float>>& matrizA);