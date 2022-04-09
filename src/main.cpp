#include "./main.h"
#include <math.h>

// Variables
int internalRadius;
int externalRadius;
int m; // particiones del radio
int n; // particiones angulares
int isoterma;
int numeroDeInstancias;

vector<vector<double>> internalTemperatures;
vector<vector<double>> externalTemperatures;

int main(int argc, char *argv[])
{

	try
	{
		if (argc != 4)
			throw 500;

		string inputFileName = argv[1];
		string outputFileName = argv[2];

		int method = parseIntFromString(argv[3]);

		if (method > 1 || method < 0)
			throw 501;

		ifstream inputFile;
		inputFile.open(inputFileName);
		if (inputFile.is_open())
		{

			cout << "Cargando archivo " << inputFileName << endl;

			internalRadius = getNextIntFromInputFile(inputFile);

			externalRadius = getNextIntFromInputFile(inputFile);

			m = getNextIntFromInputFile(inputFile);

			n = getNextIntFromInputFile(inputFile);

			isoterma = getNextIntFromInputFile(inputFile);

			numeroDeInstancias = getNextIntFromInputFile(inputFile);

			vector<double> temps(n);
			for (int j = 0; j < numeroDeInstancias; j++)
			{
				for (int i = 0; i < n; i++)
				{
					temps[i] = getNextFloatFromInputFile(inputFile);
				}
				internalTemperatures.push_back(temps);

				for (int i = 0; i < n; i++)
				{
					temps[i] = getNextFloatFromInputFile(inputFile);
				}
				externalTemperatures.push_back(temps);
			}

			cout << "OK" << endl;
		}
		else
		{
			throw 502;
		}

		int tam_matriz = n * m;
		vector<vector<double>> matrizA(tam_matriz, vector<double>(tam_matriz + 1)); //Agrego unna columna extra para los B
		crearMatrizA(n, m, internalRadius, externalRadius, matrizA, 0);
		

		printMatriz(matrizA);

		if (method == 0)
		{ // EG
			eliminacionGaussiana(matrizA);
		}

		printMatriz(matrizA);

		// idea de resolver el sistema de ecuación:
		// tratar a la matriz como el sistema de ecuacion, donde cada valor es el coeficiente de la variable que da su columna
		// ir de abajo pa arriba en la matriz y cambiar el valor de la columna hacia arriba con el valor encontrado, ahí el valor de esa casilla pasa de ser el coeficiente a ser el valor mismo
	}
	catch (int e)
	{
		handleError(e);
	}

	return 0;
}

void handleError(int error)
{

	if (error == 500)
	{
		clog << "La entrada debe tener 3 argumentos: archivo de entrada, archivo de salida y método (0 EG, 1 LU)" << endl;
	}
	else if (error == 501)
	{
		clog << "El método debe ser 0 o 1 (0 EG, 1 LU)" << endl;
	}
	else if (error == 502)
	{
		clog << "Error al abrir el archivo de entrada" << endl;
	}
}

double coeficienteA(double diffEntreRadios, int j, int radioInterno)
{ // crea el coeficiente para t_j-1,k

	double radius_j = j * diffEntreRadios + radioInterno;

	return 1 / (pow(diffEntreRadios, 2)) - 1 / (radius_j * diffEntreRadios);
}

double coeficienteB(double diffEntreAngulos, double diffEntreRadios, int j, int radioInterno)
{ // crea el coeficiente para t_j,k

	double radius_j = j * diffEntreRadios + radioInterno;

	return 1 / (radius_j * diffEntreRadios) - 2 / pow(diffEntreRadios, 2) - 2 / ((pow(radius_j, 2)) * pow(diffEntreAngulos, 2));
}

double coeficienteC(double diffEntreRadios)
{ // crea el coeficiente para t_j+1,k

	return 1 / pow(diffEntreRadios, 2);
}

double coeficienteD(double diffEntreAngulos, double diffEntreRadios, int j, int radioInterno)
{ // crea el coeficiente para t_j,k-1 y t_j,k+1

	double radius_j = j * diffEntreRadios + radioInterno;

	return 1 / (pow(diffEntreAngulos, 2) * pow(radius_j, 2));
}

void crearMatrizA(int cantAngulos, int cantRadios, int comienzoPared, int finalPared, vector<vector<double>> &matrizA, int instancia)
{
	double diffEntreAngulos = 2 * M_PI / cantAngulos; //deltaTheta
	double diffEntreRadios = (finalPared - comienzoPared) / cantRadios; //deltaR

	int ultima_columna = cantAngulos * cantRadios; //La columna de los B
	//se usa directo sin restarle 1 porque es una columna extra

	int fila, columna;
	double coefA, coefB, coefC, coefD; //coeficiente de la ecuación de temperatura
	coefC = coeficienteC(diffEntreRadios); // pre calculamos "c" ya que no necesita j
	for (int k = 0; k < cantAngulos; k++)
	{
		for (int j = 0; j < cantRadios; j++)
		{

			if(j == 0 || j == cantRadios - 1)
			{
				//es dato que ya tenemos y por tanto va un 1 en t_j,k y el resto ceros
				fila = k * cantRadios + j;
				columna = k * cantRadios + j;
				matrizA[fila][columna] = 1;
				matrizA[fila][ultima_columna] = (j == 0) ? internalTemperatures[instancia][k] : externalTemperatures[instancia][k] ;
			}
			else
			{

				coefA = coeficienteA(diffEntreRadios, j, comienzoPared);
				coefB = coeficienteB(diffEntreAngulos, diffEntreRadios, j, comienzoPared);
				coefD = coeficienteD(diffEntreAngulos, diffEntreRadios, j, comienzoPared);

				// asignamos "b" a t_j,k
				fila = k * cantRadios + j;
				columna = k * cantRadios + j;
				matrizA[fila][columna] = coefB;

				// asignamos "a" a t_j-1,k
				fila = k * cantRadios + j;
				columna = k * cantRadios + j - 1;
				matrizA[fila][columna] = coefA;
				

				// asignamos "c" a t_j+1,k
				fila = k * cantRadios + j;
				columna = k * cantRadios + j + 1; 
				matrizA[fila][columna] = coefC;

				// asignamos "d" a t_j,k-1
				if (k != 0)
				{ // si no es el angulo 0, k-1 es directo k-1
					fila = k * cantRadios + j;
					columna = (k - 1) * cantRadios + j;
					matrizA[fila][columna] = coefD;
				}
				else
				{ // si es 0, hay que usar el angulo 2*PI para k-1
					fila = k * cantRadios + j; 
					columna = (cantAngulos - 1) * cantRadios + j;
					matrizA[fila][columna] = coefD;
				}

				// asignamos "d" a t_j,k+1
				if (k != cantAngulos - 1)
				{ // si no es n-1, usamos k+1
					fila = k * cantRadios + j; 
					columna = (k + 1) * cantRadios + j;
					matrizA[fila][columna] = coefD;
				}
				else
				{ // si es n-1, usamos el angulo 0
					fila = k * cantRadios + j; 
					columna = 0 + j;
					matrizA[fila][columna] = coefD;
				}
			}
		}
	}
}

void eliminacionGaussiana(vector<vector<double>> &matrizA)
{
	for (int i = 0; i < n * m - 1; i++) // para cada columna menos la ultima y excluyendo la columna de Bs
	{

		for (int j = i + 1; j < n * m; j++) // para cada valor por debajo de la diagonal
		{
			double m_ji = matrizA[j][i] / matrizA[i][i]; // obtengo el coeficiente que lo iguala (no me lo guardo por ahora)

			for (int k = i; k < n * m + 1; k++) // y efectuo la resta de la fila j por la fila i multiplicada por el coeficiente
			{
				double resultado_de_la_resta = matrizA[j][k] - m_ji * matrizA[i][k];
				if(abs(resultado_de_la_resta) - 0.000000000001f < 0){
					resultado_de_la_resta = 0;
				}
				matrizA[j][k] = resultado_de_la_resta;
			}
		}
	}
}