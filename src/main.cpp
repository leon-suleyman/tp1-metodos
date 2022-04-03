#include "./main.h"


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

            for (int i = 0; i < n; i++)
            {
                internalTemperatures.push_back(getNextFloatFromInputFile(inputFile));
            }
            
            for (int i = 0; i < n; i++)
            {
                externalTemperatures.push_back(getNextFloatFromInputFile(inputFile));
            }

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