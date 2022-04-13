#include "./main.h"

// Variables
int internalRadius;
int externalRadius;
int m; // particiones del radio
int n; // particiones angulares
int isoterma;
int numeroDeInstancias;
int method;

vector<vector<float>> internalTemperatures;
vector<vector<float>> externalTemperatures;

int main(int argc, char *argv[])
{

	try
	{
		if (argc != 4)
			throw 500;

		string inputFileName = argv[1];
		string outputFileName = argv[2];

		method = parseIntFromString(argv[3]);

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

			vector<float> temps(n);
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
		vector<vector<float>> matrizA(tam_matriz, vector<float>(tam_matriz + 1)); // Agrego una columna extra para los B
		crearMatrizA(n, m, internalRadius, externalRadius, matrizA, 0);
		// printMatriz(matrizA);

		// inicio el reloj para medir la duración del algoritmo
		auto start = chrono::steady_clock::now();

		eliminacionGaussiana(matrizA);

		if (method == 0)
		{
			resolverSistema(matrizA);
		}
		else
		{
			resolucionLU(matrizA);
		}

		// calculo cuanto tardó la ejecución
		auto end = chrono::steady_clock::now();
		double tiempoDeEjecucion = chrono::duration<double, milli>(end - start).count();

		clog << tiempoDeEjecucion << endl;

		printMatriz(matrizA);

		ofstream outputFile;
		outputFile.open(outputFileName);
		if (outputFile.is_open())
		{
			cout << "Escribiendo respuesta" << endl;

			string resultado;
			for (int i = 0; i < tam_matriz; i++)
			{
				resultado = to_string(matrizA[i][tam_matriz]) + "\n";

				outputFile.write(resultado.c_str(), resultado.size());
			}

			cout << "OK" << endl;
		}
		else
		{
			throw 503;
		}
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
	else if (error == 503)
	{
		clog << "Error al abrir el archivo de salida" << endl;
	}
}

void crearMatrizA(int cantAngulos, int cantRadios, int comienzoPared, int finalPared, vector<vector<float>> &matrizA, int instancia)
{
	float diffEntreAngulos = 2 * M_PI / cantAngulos;					// deltaTheta
	float diffEntreRadios = (finalPared - comienzoPared) / cantRadios; // deltaR

	int ultima_columna = cantAngulos * cantRadios; // La columna de los B
	// se usa directo sin restarle 1 porque es una columna extra

	int fila, columna;
	float coefA, coefB, coefC, coefD;	   // coeficiente de la ecuación de temperatura
	coefC = coeficienteC(diffEntreRadios); // pre calculamos "c" ya que no necesita j
	for (int k = 0; k < cantAngulos; k++)
	{
		for (int j = 0; j < cantRadios; j++)
		{

			if (j == 0 || j == cantRadios - 1)
			{
				// es dato que ya tenemos y por tanto va un 1 en t_j,k y el resto ceros
				fila = k * cantRadios + j;
				columna = k * cantRadios + j;
				matrizA[fila][columna] = 1;
				matrizA[fila][ultima_columna] = (j == 0) ? internalTemperatures[instancia][k] : externalTemperatures[instancia][k];
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

void eliminacionGaussiana(vector<vector<float>> &matrizA)
{
	for (int i = 0; i < n * m - 1; i++) // para cada columna menos la ultima y excluyendo la columna de Bs
	{

		for (int j = i + 1; j < n * m; j++) // para cada valor por debajo de la diagonal
		{
			float m_ji = matrizA[j][i] / matrizA[i][i]; // obtengo el coeficiente que lo iguala (no me lo guardo por ahora)

			for (int k = i; k < n * m + 1 - method; k++) // y efectuo la resta de la fila j por la fila i multiplicada por el coeficiente
			{
				float resultado_de_la_resta = matrizA[j][k] - m_ji * matrizA[i][k];
				if (abs(resultado_de_la_resta) - 0.000000000001f < 0)
				{
					resultado_de_la_resta = 0;
				}
				matrizA[j][k] = resultado_de_la_resta;
			}

			if (method)
			{
				matrizA[j][i] = m_ji;
			}
		}
	}
	printMatriz(matrizA);
}

void resolverSistema(vector<vector<float>> &matrizA)
{
	int tamano_matriz = n * m;

	// Se termina de diagonalizar la matriz
	for (int i = tamano_matriz - 1; i > 0; i--) // para cada columna menos la primera y exluyendo la columna de Bs
	{
		for (int j = i - 1; j >= 0; j--) // para cada valor por arriba de la diagonal
		{
			float m_ji = matrizA[j][i] / matrizA[i][i]; // obtengo el coeficiente que lo iguala (no me lo guardo por ahora)

			matrizA[j][i] = 0;
			matrizA[j][tamano_matriz] = matrizA[j][tamano_matriz] - m_ji * matrizA[i][tamano_matriz]; // aplicando movimiento sobre la columna de Bs
		}
	}

	// Normalizo la diagonal y obtengo las soluciones
	for (int i = 0; i < tamano_matriz; i++)
	{
		matrizA[i][tamano_matriz] = matrizA[i][tamano_matriz] / matrizA[i][i];
		matrizA[i][i] = 1;
	}
}

void resolucionLU(vector<vector<float>> &matrizA)
{
	// asumo que para este punto ya se llamó a EG
	resolverLYB(matrizA);
	resolverUXY(matrizA);
}

void resolverLYB(vector<vector<float>> &matrizA)
{
	// L es una matriz que tiene 1s en la diagonal y 0s por encima de la diagonal
	int tamano_matriz = n * m;
	for (int fila = 0; fila < tamano_matriz; fila++) // para cada fila
	{
		float sumando = 0;
		for (int columna = 0; columna < fila; columna++)
		{
			sumando += matrizA[fila][columna] * matrizA[columna][tamano_matriz];
		}
		float resta = matrizA[fila][tamano_matriz] - sumando;
		if (abs(resta) - 0.000000000001f < 0)
		{
			resta = 0;
		}
		matrizA[fila][tamano_matriz] = resta;
	}
}

void resolverUXY(vector<vector<float>> &matrizA)
{
	int tamano_matriz = n * m;
	for (int fila = tamano_matriz - 1; fila >= 0; fila--) // para cada fila desde el final
	{
		float sumando = 0;

		for (int columna = tamano_matriz - 1; columna > fila; columna--) //// para cada columna
		{
			sumando += matrizA[fila][columna] * matrizA[columna][tamano_matriz];
		}
		float resta = matrizA[fila][tamano_matriz] - sumando;
		if (abs(resta) - 0.000000000001f < 0)
		{
			resta = 0;
		}
		matrizA[fila][tamano_matriz] = resta / matrizA[fila][fila];
	}
}