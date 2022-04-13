#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <chrono>

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

void printMatriz(vector<vector<float>> &matriz)
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

float coeficienteA(float diffEntreRadios, int j, int radioInterno)
{ // crea el coeficiente para t_j-1,k

	float radius_j = j * diffEntreRadios + radioInterno;

	return 1 / (pow(diffEntreRadios, 2)) - 1 / (radius_j * diffEntreRadios);
}

float coeficienteB(float diffEntreAngulos, float diffEntreRadios, int j, int radioInterno)
{ // crea el coeficiente para t_j,k

	float radius_j = j * diffEntreRadios + radioInterno;

	return 1 / (radius_j * diffEntreRadios) - 2 / pow(diffEntreRadios, 2) - 2 / ((pow(radius_j, 2)) * pow(diffEntreAngulos, 2));
}

float coeficienteC(float diffEntreRadios)
{ // crea el coeficiente para t_j+1,k

	return 1 / pow(diffEntreRadios, 2);
}

float coeficienteD(float diffEntreAngulos, float diffEntreRadios, int j, int radioInterno)
{ // crea el coeficiente para t_j,k-1 y t_j,k+1

	float radius_j = j * diffEntreRadios + radioInterno;

	return 1 / (pow(diffEntreAngulos, 2) * pow(radius_j, 2));
}

void crearMatrizA(int cantAngulos, int cantRadios, int comienzoPared, int finalPared, vector<vector<float>> &matrizA, int instancia);

void eliminacionGaussiana(vector<vector<float>> &matrizA);

void resolverSistema(vector<vector<float>> &matrizA);

void resolucionLU(vector<vector<float>> &matrizA);
void resolverLYB(vector<vector<float>> &matrizA);
void resolverUXY(vector<vector<float>> &matrizA);