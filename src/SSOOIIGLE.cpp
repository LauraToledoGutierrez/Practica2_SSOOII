/*CLASE DONDE SE REALIZARA LA BUSQUEDA*/

/*Includes*/
#include "definitions.h"
#include "clases.h"
#include "colores.h"

/*Variables globales*/
std::vector<Buscador> buscadorHilos;
std::queue<Resultado_Busqueda> colaResultados;
std::mutex semaforo_;

std::vector<std::string> leerFichero(std::string rutaFichero);
int leerLineas();
void crearHilos(int numHilos, int numeroLineas, std::string palabraBuscar, std::vector<std::string>lineas);
void crearHilos(int numeroLineas, int numeroHilos, std::string palabraBuscada, std::vector<std::string> lineas);
void buscarPalabra(int iteracion, std::vector<std::string> vector);
std::string eliminarSimbolos(std::string linea);
void imprimir();

int main(int argc, char *argv[]){
    
    /*Controla si se introducen los argumentos correctos*/
    if(argc!=4){
        std::cout<<ROJO<<"Numero de argumentos incorrecto! <nombre_fichero> <palabra> <numero_hilos>"<<std::endl;
        exit(1);
    }
    std::vector<std::thread> vhilos;

    /*Guardamos los argumentos en variables*/
    std::string rutaFichero(argv[1]); 
    std::string palabraBuscada= argv[2];
    int numeroHilos= atoi(argv[3]);

    std::vector<std::string> lineas= leerFichero(rutaFichero);
    int numeroLineas= lineas.size();

    //crearHilos(numeroHilos, numeroLineas, palabraBuscada, lineas);
    for (int i=0; i<numeroHilos;i++){
        int limiteInferior, limiteSuperior;
        /*Si es el primer hilo*/
        if(i==0)
            limiteInferior=1;
        else
           limiteInferior=((numeroLineas/numeroHilos)*i+1);
        
        /*Si es el ultimo hilo*/
        if(i==numeroHilos-1)
            limiteSuperior= numeroLineas;
        else
            limiteSuperior=(limiteInferior+(numeroLineas/numeroHilos)-1);

        std::vector<std::string> vectorParcial;
        /*Creamos el objeto buscador con sus correspondientes valores*/
        Buscador buscador(palabraBuscada, i, limiteInferior, limiteSuperior);
        buscadorHilos.push_back(buscador);
        
        for(int j=limiteInferior-1; j<limiteSuperior;j++){
            vectorParcial.push_back(lineas[j]);
        }
        /*Cada hilo realizara el metodo de buscarPalabra*/
        vhilos.push_back(std::thread(buscarPalabra, i, vectorParcial)); 
    }
    
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
    imprimir();
    
}

/*void crearHilos(int numHilos, int numeroLineas, std::string palabraBuscar, std::vector<std::string>lineas){
    std::vector<std::thread> vhilos;
    for (unsigned i=0; i<numHilos;i++){
        int limiteInferior;
        if(i==0)
            limiteInferior=0;
        else
            limiteInferior=((numeroLineas/numHilos)*i+1);
        
        int limiteSuperior;
        if(i==numHilos-1)//if si i= numeroHilos-1 -> Lim2 = numeroLineas
            limiteSuperior= numeroLineas;
        else
            limiteSuperior=(limiteInferior+(numeroLineas/numHilos)-1);

        std::vector<std::string> vectorParcial;

        Buscador buscador(palabraBuscar, i, limiteInferior, limiteSuperior);
        buscadorHilos.push_back(buscador);
        
        for(int j=limiteInferior-1; j<=limiteSuperior;j++){
            vectorParcial.push_back(lineas[j]);
        }
        vhilos.push_back(std::thread(buscarPalabra, i, vectorParcial)); //Todo funcion que ejecute cada hilo
    }
    std::lock_guard<std::mutex> lockGuard_(semaforo_);
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
}*/

/*Leemos el fichero para sacar las lineas*/
std::vector<std::string> leerFichero(std::string rutaFichero)
{
    std::ifstream archivo_entrada(rutaFichero);
    std::string linea;
    std::vector<std::string> lineas;

    while(getline(archivo_entrada, linea)){
        lineas.push_back(linea);
    }
    return lineas;
}
/*Buscamos la palabra deseada en el libro*/
void buscarPalabra(int iteracion, std::vector<std::string> vector)
{
    std::vector<std::string> palabras;
    std::queue<Resultado_Busqueda> colaResultados;
    Resultado_Busqueda resultados;
    int lineaR;
    
    for(int i=0; i<vector.size(); i++){
        
        std::string linea=eliminarSimbolos(vector[i]);
        /*Transformamos todas las palabras de la linea en minusculas*/
        std::transform(linea.begin(), linea.end(), linea.begin(), ::tolower);
        /*Troceamos la linea en las palabras*/
        std::istringstream trocearLinea(linea);
        std::copy(std::istream_iterator<std::string>(trocearLinea), std::istream_iterator<std::string>(), back_inserter(palabras));
        
        /*Recorremos las palabras de la linea*/
        for(int j=0; j<palabras.size(); j++){
            /*Si la palabra que estamos mirando es igual a la que buscamos*/
            if(palabras[j].compare(buscadorHilos[iteracion].getPalabraBuscada())==0)
            {
                lineaR = buscadorHilos[iteracion].getLineaInicio();
                //std::cout<<"Palabra encontrada en linea "<<i+1+lineaR<<std::endl;
                resultados.setLineaResultado(i+lineaR);
                /*Si es la primera palabra de la linea*/
                if(j==0)
                    resultados.setPalabraAnterior("---");
                else
                    resultados.setPalabraAnterior(palabras[j-1]);
                /*Si es la ultima palabra de la linea*/
                if(j==palabras.size()-1)
                    resultados.setPalabraPosterior("---");
                else
                    resultados.setPalabraPosterior(palabras[j+1]);
                
                //resultados.toString(resultados);
                /*Metemos los resultados en la cola resultados*/    
                colaResultados.push(resultados);  
                buscadorHilos[iteracion].toString(buscadorHilos[iteracion]);
            }
        }
        palabras.clear();
    }
    
    std::lock_guard<std::mutex> lockGuard_(semaforo_);
    buscadorHilos[iteracion].setColaResultados(colaResultados);    
}
/*Eliminamos cualquier simbolo que pueda aparecer en el libro que no sea letra*/
std::string eliminarSimbolos(std::string linea)
{
    for (int i = 0, size = linea.size(); i < size; i++)
    {
        /*Si es un caracter, lo eliminamos de la linea*/
        if (ispunct(linea[i])) 
        {
            linea.erase(i--, 1);
            size = linea.size();
        }
    }
    return linea;
}
/*Imprimimos los resultados*/
void imprimir()
{
    for(int i=0; i<buscadorHilos.size(); i++)
    {
    int contador=buscadorHilos[i].getColaResultados().size();
      while(contador!=0){
            std::cout<<AZUL<<"[Hilo: "<<buscadorHilos[i].getId();
            std::cout<<ROJO<< " Inicio: "<<buscadorHilos[i].getLineaInicio();
            std::cout<<AMARILLO<<" - final: "<<buscadorHilos[i].getLineaFinal()<<"]";
            std::cout<<AZUL<<" :: linea "<<buscadorHilos[i].getColaResultados().front().getLineaResultado()<<" ";
            std::cout<<VERDE<<"... "<<buscadorHilos[i].getColaResultados().front().getPalabraAnterior()<< " ";
            std::cout<<ROJO<<buscadorHilos[i].getPalabraBuscada()<< " ";
            std::cout<<AZUL<<buscadorHilos[i].getColaResultados().front().getPalabraPosterior()<<" ..."<<std::endl;
            //buscadorHilos[i].getColaResultados().front().toString(buscadorHilos[i].getColaResultados().front());    
            
        contador--;
      }
    buscadorHilos[i].getColaResultados().pop();
    }
}


