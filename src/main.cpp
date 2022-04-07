#include "./main.h"
#include <math.h>


//Variables
int internalRadius;
int externalRadius;
int m; //particiones del radio
int n; //particiones angulares
int isoterma;
int numeroDeInstancias;

vector<float> internalTemperatures;
vector<float> externalTemperatures;

int main(int argc, char* argv[]){

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
        if(inputFile.is_open())
        {

            cout << "Cargando archivo " << inputFileName << endl;
            
            internalRadius = getNextIntFromInputFile(inputFile);

            externalRadius = getNextIntFromInputFile(inputFile);

            m = getNextIntFromInputFile(inputFile);

            n = getNextIntFromInputFile(inputFile);

            isoterma = getNextIntFromInputFile(inputFile);

            numeroDeInstancias = getNextIntFromInputFile(inputFile);

            for (int j = 0; j < numeroDeInstancias; j++)
            {

                for (int i = 0; i < n; i++)
                {
                    internalTemperatures.push_back(getNextFloatFromInputFile(inputFile));
                }
                
                for (int i = 0; i < n; i++)
                {
                    externalTemperatures.push_back(getNextFloatFromInputFile(inputFile));
                }
                
            }
            
            cout << "OK" << endl;

        }
        else 
        { 
            throw 502;
        }

        vector< vector< float >> matrizA = crearMatrizA(n, m , internalRadius, externalRadius) ;
        vector< vector< float >> stripB(1, vector<float>(n*m)) ;


        if(method == 0)
        { //EG

            for (int i = 0; i < n*m - 1; i++) //para cada columna
            {
                
                for (int j = i+1; j < n*m; j++) //para cada valor por debajo de la diagonal
                {
                    float m_ji = matrizA[j][i]/matrizA[i][i]; //obtengo el coeficiente que lo iguala (no me lo guardo por ahora)

                    for (int k = i; k < n*m + 1; k++) //y efectuo la resta de la fila j por la fila i multiplicada por el coeficiente
                    {
                        matrizA[j][k] = matrizA[j][k] - m_ji*matrizA[i][k];
                    }
                    
                }                

            }
            
        } 

        printMatriz(matrizA);

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

float coeficienteA(float diffEntreRadios, int j){ //crea el coeficiente para t_j-1,k

    float radius_j = j*diffEntreRadios + internalRadius;

    return 1/(pow(diffEntreRadios, 2)) - 1/(radius_j*diffEntreRadios) ;
}

float coeficienteB(float diffEntreAngulos, float diffEntreRadios, int j){ //crea el coeficiente para t_j,k

    float radius_j = j*diffEntreRadios + internalRadius;

    return 1/(radius_j*diffEntreRadios) - 1/pow(diffEntreRadios,2) - 2/((pow(radius_j,2))*pow(diffEntreAngulos,2));

}

float coeficienteC(float diffEntreRadios){ //crea el coeficiente para t_j+1,k

    return 1/pow(diffEntreRadios,2);
}

float coeficienteD(float diffEntreAngulos, float diffEntreRadios, int j){ //crea el coeficiente para t_j,k-1 y t_j,k+1

    float radius_j = j*diffEntreRadios + internalRadius;

    return 1/(pow(diffEntreAngulos,2)*pow(radius_j,2));
}

vector< vector< float > > crearMatrizA (int cantAngulos, int cantRadios, int comienzoPared, int finalPared){
    float diffEntreAngulos = 2*M_PI/cantAngulos;
    float diffEntreRadios = (comienzoPared - finalPared)/cantRadios;

    int tam_matriz = cantAngulos*cantRadios ;
    vector< vector< float >> matrizA( tam_matriz, vector<float>(tam_matriz)) ;

    float mult_c = coeficienteC(diffEntreRadios); //pre calculamos "c" ya que no necesita j
    for(int k = 0; k<cantAngulos; k++){
        for(int j = 0; j<cantRadios; j++ ){

            //asignamos "b" a t_j,k
            matrizA[k*cantRadios + j][k*cantRadios + j] = coeficienteB(diffEntreAngulos, diffEntreRadios, j) ;
            
            //asignamos "a" a t_j-1,k
            if(j != 0){
                matrizA[k*cantRadios + j][k*cantRadios + j-1] = coeficienteA(diffEntreRadios, j) ;
            }
            
            //asignamos "c" a t_j+1,k
            if(j != cantRadios){
                matrizA[k*cantRadios + j][k*cantRadios + j+1] = mult_c ;
            }

            //asignamos "d" a t_j,k-1
            if(k != 0){ //si no es el angulo 0, k-1 es directo k-1
                matrizA[k*cantRadios + j][(k-1)*cantRadios + j] = coeficienteD(diffEntreAngulos, diffEntreRadios, j );
            }else{ //si es 0, hay que usar el angulo 2*PI para k-1
                matrizA[k*cantRadios + j][(cantAngulos-1)*cantRadios + j] = coeficienteD(diffEntreAngulos, diffEntreRadios, j) ;
            }

            //asignamos "d" a t_j,k+1
            if(k != cantAngulos-1){ //si no es n-1, usamos k+1
                matrizA[k*cantRadios + j][(k+1)*cantRadios + j] = coeficienteD(diffEntreAngulos, diffEntreRadios, j) ;
            }else{ //si es n-1, usamos el angulo 0
                matrizA[k*cantRadios + j][0 + j] = coeficienteD(diffEntreAngulos, diffEntreRadios, j) ;
            }

        }
    }

    return matrizA;

}