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
#include <mutex>

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
    std::string getPalabraAnterior()
    {
        return palabraAnterior;
    }
    std::string getPalabraPosterior()
    {
        return palabraPosterior;
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
    void setId(int id)
    {
        this->id = id;
    }
    int getId()
    {
        return id;
    }

    int getLineaInicio()
    {
        return lineaInicio;
    }
    void setLineaInicio(int lineaFinal)
    {
        this->lineaInicio = lineaInicio;
    }

    int getLineaFinal()
    {
        return lineaFinal;
    }
    void setLineaFinal(int lineaFinal)
    {
        this->lineaFinal = lineaFinal;
    }

    std::queue<ResultadoBusqueda> getColaResultados()
    {
        return colaResultados;
    }
};

std::fstream fp;
std::vector<std::string> leerFichero(std::string rutaFichero);
int leerLineas();
void buscarPalabra(int iteracion, std::vector<std::string> vector);
std::string eliminarSimbolos(std::string linea);
void imprimir();

std::vector<Buscador> buscadorHilos;
std::mutex semaforo_;



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

    for (unsigned i=0; i<numeroHilos;i++){
        int limiteInferior;
        if(i==0)
            limiteInferior=0;
        else
            limiteInferior=((numeroLineas/numeroHilos)*i+1);
        
        int limiteSuperior;
        if(i==numeroHilos-1)//if si i= numeroHilos-1 -> Lim2 = numeroLineas
            limiteSuperior= numeroLineas;
        else
            limiteSuperior=(limiteInferior+(numeroLineas/numeroHilos)-1);

        std::vector<std::string> vectorParcial;

        Buscador buscador(palabraBuscada, i, limiteInferior, limiteSuperior);
        buscadorHilos.push_back(buscador);
        
        for(int j=limiteInferior; j<=limiteSuperior;j++){
            vectorParcial.push_back(lineas[j]);
        }
        vhilos.push_back(std::thread(buscarPalabra, i, vectorParcial)); //Todo funcion que ejecute cada hilo
    }
    
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));

    imprimir();
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

void buscarPalabra(int iteracion, std::vector<std::string> vector){
    std::vector<std::string> palabras;
    std::queue<ResultadoBusqueda> colaResultados;
    ResultadoBusqueda resultados;
    


    for(int i=0; i<vector.size(); i++){
        std::string linea=eliminarSimbolos(vector[i]);
        std::transform(linea.begin(), linea.end(), linea.begin(), ::tolower);
        std::istringstream trocearLinea(linea);
        std::copy(std::istream_iterator<std::string>(trocearLinea), std::istream_iterator<std::string>(), back_inserter(palabras));
        
        for(int j=0; j<palabras.size(); j++){
            if(palabras[j].compare(buscadorHilos[iteracion].getPalabraBuscada())==0)
            {
                //std::cout<<"Palabra encontrada en linea "<<i+1+buscadorHilos[iteracion].getLineaInicio()<<std::endl;

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
    
    std::lock_guard<std::mutex> lockGuard_(semaforo_);
    buscadorHilos[iteracion].setColaResultados(colaResultados);    
}

std::string eliminarSimbolos(std::string linea)
{
    for (int i = 0, size = linea.size(); i < size; i++)
    {
        if (ispunct(linea[i]))
        {
            linea.erase(i--, 1);
            size = linea.size();
        }
    }
    return linea;
}

/*void creacionHilos(int nHilos, std::vector<std::string> vectorFrag, std::vector<std::string> vectorLineas, std::string palabraBuscar){
    std::vector<std::thread> Vhilos;
    for(int i=0; i<nHilos;i++){
        Vhilos.push_back(std::thread(asignarLineas, i, std::ref(vectorFrag), Vhilos, palabraBuscar));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::for_each(Vhilos.begin(), Vhilos.end(), std::mem_fn(&std::thread::join));
}*/
/*void asignarLineas(int i, std::vector<std::string> vectorFrag, std::vector<int> vectorLineas, std::string palabraBuscar){

    int limiteInferior = i*vectorLineas[0]+1;
    int limiteSuperior = limiteInferior+vectorLineas[i]-1;

    Buscador buscador;
    buscador.setId(i);
    buscador.setLineaInicio(limiteInferior);
    buscador.setLineaFinal(limiteSuperior);

    vectorFrag.push_back(buscador);
    buscarPalabra(palabraBuscar, buscador);
}*/

void imprimir()
{
    for(int i=0; i<buscadorHilos.size(); i++)
    {
        while(!buscadorHilos[i].getColaResultados().empty()){
            std::cout<<"Hilo: "<<buscadorHilos[i].getId()<< " Inicio:"<<buscadorHilos[i].getLineaInicio()
                <<" - final: "<<buscadorHilos[i].getLineaFinal()<<" "<<std::endl;
            buscadorHilos[i].getColaResultados().pop();
        }
    }
}


