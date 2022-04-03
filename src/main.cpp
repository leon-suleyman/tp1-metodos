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

    try {
        if (argc != 4)
            throw 500;

        string inputFileName = argv[1];
        string outputFileName = argv[2];
        
        int method = parseIntFromString(argv[3]);

        if (method > 1 || method < 0)
            throw 501;


        ifstream inputFile;
        inputFile.open(inputFileName);
        if(inputFile.is_open()){

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
            
            vector< vector< float >> matrizA = crearMatrizA((int)n, (int)m , (float)internalRadius, (float)externalRadius) ;
            vector< vector< float >> stripB(1, vector<float>(n*(m+1))) ;


            cout << "OK" << endl;

        }else { 
            throw 502;
        }

    }
    catch (int e){
        handleError(e);
    }

    return 0;
}


void handleError(int error){

    if (error == 500){
        clog << "La entrada debe tener 3 argumentos: archivo de entrada, archivo de salida y método (0 EG, 1 LU)" << endl;
    }
    else if (error == 501) { 
        clog << "El método debe ser 0 o 1 (0 EG, 1 LU)" << endl;
    }
    else if (error == 502) { 
        clog << "Error al abrir el archivo de entrada" << endl;
    }
}

float multiplicanteA(float diffEntreRadios, int j){ //crea el multiplicante para t_j-1,k
    return 1/(pow(diffEntreRadios, 2)) - 1/(pow(diffEntreRadios, 2))/j ;
}

float multiplicanteB(float diffEntreAngulos, float diffEntreRadios, int j){ //crea el multiplicante para t_j,k
    return 1/j/pow(diffEntreRadios,2) - 2/j/(pow(diffEntreRadios,2))/pow(diffEntreAngulos,2) ;
}

float multiplicanteC(float diffEntreRadios){ //crea el multiplicante para t_j+1,k
    return 1/pow(diffEntreRadios,2) ;
}

float multiplicanteD(float diffEntreAngulos, float diffEntreRadios, int j){ //crea el multiplicante para t_j,k-1 y t_j,k+1
    return 1/j/(pow(diffEntreRadios,2))/pow(diffEntreAngulos,2);
}

vector< vector< float > > crearMatrizA (int cantAngulos, int cantRadios, float comienzoPared, float finalPared){
    float diffEntreAngulos = 2*M_PI/cantAngulos;
    float diffEntreRadios = (comienzoPared - finalPared)/cantRadios;

    int tam_matriz = cantAngulos*(cantRadios+1) ;
    vector< vector< float >> matrizA( tam_matriz, vector<float>(tam_matriz)) ;

    float mult_c = multiplicanteC(diffEntreRadios); //pre calculamos "c" ya que no necesita j
    for(int k = 0; k<cantAngulos; k++){
        for(int j = 0; j<=cantRadios; j++ ){
            //asignamos "b" a t_j,k
            matrizA[k*cantRadios + j][k*cantRadios + j] = multiplicanteB(diffEntreAngulos, diffEntreRadios, j) ;
            
            //asignamos "a" a t_j-1,k
            if(j != 0){
                matrizA[k*cantRadios + j][k*cantRadios + j-1] = multiplicanteA(diffEntreRadios, j) ;
            }
            
            //asignamos "c" a t_j+1,k
            if(j != cantRadios){
                matrizA[k*cantRadios + j][k*cantRadios + j+1] = mult_c ;
            }

            //asignamos "d" a t_j,k-1
            if(k != 0){ //si no es el angulo 0, k-1 es directo k-1
                matrizA[k*cantRadios + j][(k-1)*cantRadios + j] = multiplicanteD(diffEntreAngulos, diffEntreRadios, j) ;
            }else{ //si es 0, hay que usar el angulo 2*PI para k-1
                matrizA[k*cantRadios + j][(cantAngulos-1)*cantRadios + j] = multiplicanteD(diffEntreAngulos, diffEntreRadios, j) ;
            }

            //asignamos "d" a t_j,k+1
            if(k != cantAngulos-1){ //si no es n-1, usamos k+1
                matrizA[k*cantRadios + j][(k+1)*cantRadios + j] = multiplicanteD(diffEntreAngulos, diffEntreRadios, j) ;
            }else{ //si es n-1, usamos el angulo 0
                matrizA[k*cantRadios + j][0 + j] = multiplicanteD(diffEntreAngulos, diffEntreRadios, j) ;
            }

        }
    }

    return matrizA;

}