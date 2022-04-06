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
#include <iterator>
#include <sstream>

class ResultadoBusqueda{
    std::string palabraAnterior;
    std::string palabraPosterior;
    int lineaResultado;
    public:
    void setPalabraAnterior(std::string palabra)
    {
        palabraAnterior=palabra;
    }

    void setPalabraPosterior(std::string palabra)
    {
        palabraPosterior=palabra;
    }
    void setLineaResultado(int linea)
    {
        lineaResultado=linea;
    }
};


class Buscador
{
    std::string palabraBuscada;
    int id;
    int lineaInicio;
    int lineaFinal;
    std::queue<ResultadoBusqueda> colaResultados;

    public:
    Buscador(){}
    Buscador(std::string palabra, int Id, int lineaAnt, int lineaPost): palabraBuscada(palabra), id(Id), lineaInicio(lineaAnt), lineaFinal(lineaPost){}
    std::string getPalabraBuscada()
    {
        return palabraBuscada;
    }

    void setColaResultados(std::queue<ResultadoBusqueda> resultados)
    {
        colaResultados= resultados;
    }

    int getId()
    {
        return id;
    }

    int getLineaInicio()
    {
        return lineaInicio;
    }

    int getLineaFinal()
    {
        return lineaFinal;
    }

    std::queue<ResultadoBusqueda> getColaResultados()
    {
        return colaResultados;
    }
};



std::fstream fp;
std::vector<std::string> leerFichero(std::string rutaFichero);
int leerLineas();
void buscarPalabra(std::vector<std::string> vector, Buscador buscador);
std::string eliminarSimbolos(std::string linea);
void imprimir(std::vector<Buscador> buscador);
int main(int argc, char *argv[]){
    
    if(argc!=4){
        std::cout<<"Numero de argumentos incorrecto"<<std::endl;
        exit(1);
    }
    
    std::string palabraBuscada= argv[2];
    int numeroHilos= atoi(argv[3]);

    std::vector<std::thread> vhilos;
    std::vector<Buscador> buscadorHilos;
    
    std::string rutaFichero(argv[1]); 
    std::vector<std::string> lineas= leerFichero(rutaFichero);
    int numeroLineas= lineas.size();

    for (unsigned i=0; i<numeroHilos-1;i++){
        int lim1=((numeroHilos/numeroHilos)*i);
        int lim2=(lim1+(numeroLineas/numeroHilos)-1);
        std::vector<std::string> vectorParcial;

        Buscador buscador(palabraBuscada, i, lim1, lim2);
        buscadorHilos.push_back(buscador);

        for(int j=lim1; j<lim2;j++){
            vectorParcial.push_back(lineas[j]);
        }

        vhilos.push_back(std::thread(buscarPalabra, vectorParcial, buscador)); //Todo funcion que ejecute cada hilo
    }
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
    imprimir(buscadorHilos);
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
    std::queue<ResultadoBusqueda> colaResultados;
    ResultadoBusqueda resultados;


    for(int i=0; i<vector.size(); i++){
        vector[i]=eliminarSimbolos(vector[i]);
        std::transform(vector[i].begin(), vector[i].end(), vector[i].begin(), ::tolower);
        std::string linea= vector[i];
        std::istringstream trocearLinea(linea);
        std::copy(std::istream_iterator<std::string>(trocearLinea), std::istream_iterator<std::string>(), back_inserter(palabras));
        for(int j=0; j<palabras.size(); j++){
            if(palabras[j].compare(buscador.getPalabraBuscada())==0)
            {
                std::cout<<"Palabra encontrada en linea "<<i+1<<std::endl;
                resultados.setLineaResultado(i+1);
                if(j==0)
                    resultados.setPalabraAnterior("---");
                else
                    resultados.setPalabraAnterior(palabras[j-1]);

                if(j==palabras.size()-1)
                    resultados.setPalabraPosterior("---");
                else
                    resultados.setPalabraPosterior(palabras[j+1]);
                colaResultados.push(resultados);
            }
        }
        palabras.clear();
    }


    buscador.setColaResultados(colaResultados);
}
std::string eliminarSimbolos(std::string linea)
{
    for(int i=0, tamanio= linea.size(); i<tamanio; i++)
    {
        if(ispunct(linea[i]))
        {
            linea.erase(i--, 1);
            tamanio= linea.size();
        }
    }
    return linea;
}

void imprimir(std::vector<Buscador> buscadorHilos)
{
    for(int i=0; i<buscadorHilos.size(); i++)
    {
        while(!buscadorHilos[i].getColaResultados().empty())
            std::cout<<"Hilo: "<<buscadorHilos[i].getId()<< " Inicio:"<<buscadorHilos[i].getLineaInicio()
                <<" - final: "<<buscadorHilos[i].getLineaFinal()<<" "<<std::endl;
    }
}


