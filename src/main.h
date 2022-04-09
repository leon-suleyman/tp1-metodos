#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int parseIntFromString(string string)
{

	int result;

	istringstream ss(string);

	if (!(ss >> result))
	{
		cerr << "Invalid number: " << string << '\n';
		throw 0;
	}
	else if (!ss.eof())
	{
		cerr << "Trailing characters after number: " << string << '\n';
		throw 0;
	}

	return result;
}

double parseFloatFromString(string string)
{

	double result;

	istringstream ss(string);

	if (!(ss >> result))
	{
		cerr << "Invalid number: " << string << '\n';
		throw 0;
	}
	else if (!ss.eof())
	{
		cerr << "Trailing characters after number: " << string << '\n';
		throw 0;
	}

	return result;
}

int getNextIntFromInputFile(ifstream &inputFile)
{

	int result;
	string nextValue;

	inputFile >> nextValue;
	result = parseIntFromString(nextValue);

	return result;
}

double getNextFloatFromInputFile(ifstream &inputFile)
{

	double result;
	string nextValue;

	inputFile >> nextValue;
	result = parseFloatFromString(nextValue);

	return result;
}

void printMatriz(vector<vector<double>> matriz)
{
	int cantFilas = matriz.size();
	int cantColumnas = matriz[0].size();

	for (int i = 0; i < cantFilas; i++)
	{
		clog << "(";
		for (int j = 0; j < cantColumnas; j++)
		{
			clog << matriz[i][j];

			if (j != cantColumnas - 1)
			{
				clog << ", ";
			}
		}
		clog << ")" << endl;
	}

	clog << endl;
}

void handleError(int);

double coeficienteA(double diffEntreRadios, int j, int radioInterno);
double coeficienteB(double diffEntreAngulos, double diffEntreRadios, int j, int radioInterno);
double coeficienteC(double diffEntreRadios);
double coeficienteD(double diffEntreAngulos, double diffEntreRadios, int j, int radioInterno);

vector<vector<double>> crearMatrizA(int cantAngulos, int cantRadios, int comienzoPared, int finalPared);

void eliminacionGaussiana(vector<vector<double>> &matrizA);