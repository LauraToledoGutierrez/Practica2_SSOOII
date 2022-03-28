#include <iostream>
#include <mutex>
#include <thread>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

std::fstream fp;

void leerFichero(std::string rutaFichero);
int leerLineas();

int main(int argc, char *argv[]){
    std::string rutaFichero(argv[1]); 
    leerFichero(rutaFichero);
    leerLineas();
}

void leerFichero(std::string rutaFichero){
    int g_lineasFichero=0;
    fp.open(rutaFichero);
    if (!fp.is_open()) {
        std::cout <<"Error al abrir " << rutaFichero << "\n";
        exit(EXIT_FAILURE);
    }
}

int leerLineas(){

    int lineasFichero;
    char cadena[1024];

    while (!fp.eof()){
        fp.getline(cadena, 1024);
        lineasFichero++;
    
    }
    std::cout << "Total de lineas: " <<lineasFichero<< "\n";

    fp.close();
    return lineasFichero;
}
