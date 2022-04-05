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
#include <functional>

std::fstream fp;
std::vector<std::string> leerFichero(std::string rutaFichero);
int leerLineas();
void buscarPalabra(std::vector<std::string> vector, Buscador buscador);
int main(int argc, char *argv[]){
    
    if(argc!=4){
        std::cout<<"Numero de argumentos incorrecto"<<std::endl;
        exit(1);
    }
    
    std::string palabraBuscada= argv[2];
    int numeroHilos= atoi(argv[3]);

    std::vector<std::thread> vhilos;
    std::string rutaFichero(argv[1]); 
    std::vector<std::string> lineas= leerFichero(rutaFichero);
    int numeroLineas= lineas.size();


    for (unsigned i=0; i<numeroHilos-1;i++){
        int lim1=((numeroHilos/numeroHilos)*i);
        int lim2=(lim1+(numeroLineas/numeroHilos)-1);
        std::vector<std::string> vectorParcial;

        Buscador buscador(palabraBuscada, i, lim1, lim2);

        for(int j=lim1; j<lim2;j++){
            vectorParcial.push_back(lineas[j]);
        }

        vhilos.push_back(std::thread(buscarPalabra, vectorParcial, buscador)); //Todo funcion que ejecute cada hilo
    }
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
    
}


std::vector<std::string> leerFichero(std::string rutaFichero){
    std::ifstream archivo_entrada(rutaFichero);
    std::string linea;
    std::vector<std::string> lineas;

    while(getline(archivo_entrada, linea)){
        lineas.push_back(linea);
    }
    return lineas;
}

/* DEPRECATED
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
*/

void buscarPalabra(std::vector<std::string> vector, Buscador buscador){
    //Meter en vector las lineas enteras de todo el fichero (global)
    //buscarPalabra solo busque entre lim1 y lim2 (posiciones del vector)
    //Busqueda de cada linea va a ser local para buscar las palabras (solo va a salir la palabra anterio y posterior y ya)
    //Comprobar si la palabra es la ultima de una linea, que busque en la linea siguiente
    std::vector<std::string> palabras;

    for(int i=0; i<vector.size(); i++){

    }

}

class Buscador
{
    public:
    Buscador(std::string, int, int, int);
    std::string palabraBuscada;
    int id;
    int lineaInicio;
    int lineaFinal;
    std::queue<ResultadoBusqueda> colaResultados;
};
Buscador::Buscador(std::string palabraBuscada, int id, int lineaInicio, int lineaFinal)
{
    this->palabraBuscada= palabraBuscada;
    this->id= id;
    this->lineaInicio=lineaInicio;
    this->lineaFinal=lineaFinal;
}

class ResultadoBusqueda{
    std::string palabraAnterior;
    std::string palabraPosterior;
    int lineaResultado;
};

