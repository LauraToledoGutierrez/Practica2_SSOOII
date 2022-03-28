#include <iostream>
#include <string>
#include <ctype.h>
#include <locale>
#include <algorithm>
#include <thread>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <queue>

std::fstream fp;
void leerFichero(std::string rutaFichero);
int leerLineas();

int main(int argc, char *argv[]){
    
    if(argc!=4){
        std::cout<<"Numero de argumentos incorrecto"<<std::endl;
        exit(1);
    }
    
    std::string palabraBuscada= argv[2];
    int numeroHilos= atoi(argv[3]);

    std::vector<std::thread> vhilos;
    std::string rutaFichero(argv[1]); 
    leerFichero(rutaFichero);
    int numeroLineas= leerLineas();


    for (unsigned i=0; i<numeroHilos-1;i++){
        int lim1=((numeroHilos/numeroHilos)*i);
        int lim2=((numeroLineas/numeroHilos)*(i+1))+1;
        vhilos.push_back(std::thread(buscarPalabra,lim1,lim2)); //Todo funcion que ejecute cada hilo
    }
    
}


void leerFichero(std::string rutaFichero){
    fp.open(rutaFichero);
    if (!fp.is_open()) {
        std::cout <<"Error al abrir " << rutaFichero << "\n";
        exit(EXIT_FAILURE);
    }
}

int leerLineas(){
    int lineasFichero=0;
    char cadena[1024];

    while (!fp.eof()){
        fp.getline(cadena, 1024);
        lineasFichero++;
    
    }
    std::cout << "Total de lineas: " <<lineasFichero<< "\n";

    fp.close();
    return lineasFichero;
}

void buscarPalabra(int lim1, int lim2){

}

class Buscador
{
    std::string palabraBuscada;
    int id;
    int lineaInicio;
    int lineaFinal;
    std::queue<ResultadoBusqueda> colaResultados;
};

class ResultadoBusqueda{
    std::string palabraAnterior;
    std::string palabraPosterior;
    int lineaResultado;
};
