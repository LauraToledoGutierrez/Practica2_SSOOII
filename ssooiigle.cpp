#include <definitions.h>
#include <ResultadoBusqueda.h>
#include <Buscador.h>

std::fstream fp;
std::vector<Buscador> buscadorHilos;
std::queue<Resultado_Busqueda> colaResultados;
std::mutex semaforo_;

std::vector<std::string> leerFichero(std::string rutaFichero);
int leerLineas();
void crearHilos(int numeroLineas, int numeroHilos, std::string palabraBuscada, std::vector<std::string> lineas);
void buscarPalabra(int iteracion, std::vector<std::string> vector);
std::string eliminarSimbolos(std::string linea);
void imprimir();

int main(int argc, char *argv[]){
    
    if(argc!=4){
        std::cout<<"Numero de argumentos incorrecto!, <nombre_fichero> <palabra> <numero_hilos>"<<std::endl;
        exit(1);
    }
    
    std::string palabraBuscada= argv[2];
    int numeroHilos= atoi(argv[3]);

    std::vector<std::thread> vhilos;
    std::string rutaFichero(argv[1]); 
    std::vector<std::string> lineas= leerFichero(rutaFichero);
    int numeroLineas= lineas.size();

    crearHilos(numeroLineas, numeroHilos, palabraBuscada, lineas);
    /*for (unsigned i=0; i<numeroHilos;i++){
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
    imprimir();*/
}
void crearHilos(int numeroLineas, int numeroHilos, std::string palabraBuscada, std::vector<std::string> lineas){
    std::vector<std::thread> vhilos;

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
    std::queue<Resultado_Busqueda> colaResultados;
    Resultado_Busqueda resultados;
    
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

void imprimir()
{
    Buscador buscador;
    
    for(int i=0; i<buscadorHilos.size(); i++)
    {
        if(!buscadorHilos[i].getColaResultados().empty()){

            for(int j=0;j<buscadorHilos[i].getColaResultados().size();j++){
                std::cout<<"Hilo: "<<buscadorHilos[i].getId()<< " Inicio:"<<buscadorHilos[i].getLineaInicio()
                    <<" - final: "<<buscadorHilos[i].getLineaFinal()<<" "<<buscadorHilos[i].getPalabraBuscada()<<" "
                    <<buscadorHilos[i].getColaResultados().front().getPalabraAnterior()<<" "<<std::endl;

                buscadorHilos[i].getColaResultados().pop();
            }
        }

    }
}


