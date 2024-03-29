/*CLASE DONDE SE REALIZARA LA BUSQUEDA*/

/*Includes*/
#include "definitions.h"
#include "thread"
#include "finder.h"
#include "colors.h"
#include "request.h"
#include "client.h"
#define NUMEROCLIENTES 10

/*Variables globales*/
std::vector<Finder> threadFinder;
std::queue<Search_Result> queueResults;
std::queue<Request> clientRequestFree;
std::queue<Request> clientRequestPremium;
std::mutex mutex;
std::vector<std::string> bookPath;

std::vector<std::string> readFile(std::string bookPath);
int readLines();
void createClient(int nLines, int nThread, std::string searchWord, std::vector<std::string> vLines);
void findWord(int iteration, std::vector<std::string> vector);
std::string eraseSymbols(std::string line);
void printResults();
void createPath();

std::vector<std::string> books = {"17-LEYES-DEL-TRABJO-EN-EQUIPO.txt", "21-LEYES-DEL-LIDERAZGO.txt", "25-MANERAS-DE-GANARSE-A-LA-GENTE.txt",
                        "ACTITUD-DE-VENDEDOR.txt", "El-oro-y-la-ceniza.txt", "La-última-sirena.txt", "prueba.txt", 
                        "SEAMOS-PERSONAS-DE-INFLUENCIA.txt", "VIVE-TU-SUEÑO.txt"};

std::vector<std::string> dictionary = {"casa", "telefono", "final"};


int main(int argc, char *argv[])
{

    /*Controla si se introducen los argumentos correctos*/
    if (argc != 4)
    {
        std::cout << RED << "Numero de argumentos incorrecto! <nombre_fichero> <palabra> <numero_hilos>" << std::endl;
        exit(1);
    }
    std::cout << RESET << "\nBienvenido a " << BLUE << "SS" <<RED<< "O"<< YELLOW <<"O"<<BLUE<<"II"<<GREEN<< "GL"<<RED<< "E\n"<<std::endl;

    /*Guardamos los argumentos en variables*/
    std::string bookPath(argv[1]);
    std::string searchWord = argv[2];
    int nThread = atoi(argv[3]);

    //NOTA: RECORRER EL VECTOR DE LIBROS Y LLAMAR X VECES AL METODO -> CREAR METODO APARTE

    std::vector<std::string> vLines = readFile(bookPath);
    int nLines = vLines.size();

    createClient(nThread, nLines, searchWord, vLines);
    printResults();
}

void createPath()
{
    std::string path = "Libros_P2/";
    for (int i = 0; i < (int)books.size(); i++) {
        bookPath.push_back(path + books[i]);
    }
}

// DEPRECATED 
/*void crearHilos(int numeroHilos,int numeroLineas,std::string palabraBuscada,std::vector<std::string> lineas)
{
    std::vector<std::thread> vhilos;
    for (int i = 0; i < numeroHilos; i++)
    {
        int limiteInferior, limiteSuperior;

        //Si es el primer hilo
        if (i == 0)
            limiteInferior = 1;
        else
            limiteInferior = ((numeroLineas / numeroHilos) * i + 1);

        //Si es el ultimo hilo
        if (i == numeroHilos - 1)
            limiteSuperior = numeroLineas;
        else
            limiteSuperior = (limiteInferior + (numeroLineas / numeroHilos) - 1);

        std::vector<std::string> vectorParcial;
        //Creamos el objeto buscador con sus correspondientes valores
        Buscador buscador(palabraBuscada, i, limiteInferior, limiteSuperior);
        threadFinder.push_back(buscador);

        for (int j = limiteInferior - 1; j < limiteSuperior; j++)
        {
            vectorParcial.push_back(lineas[j]);
        }
        //Cada hilo realizara el metodo de buscarPalabra
        vhilos.push_back(std::thread(findWord, i, vectorParcial));
    }

    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
}
*/

void createClient (int nThread,int nLines,std::string searchWord,std::vector<std::string> vLines){
    std::vector<std::thread> vClients;

    for(int i=0; i<NUMEROCLIENTES; i++){

        srand(time(NULL));
        int typeClient= rand()%(3-1);
        int book = rand() % dictionary.size();
        //El libro de cada cliente no se si lo tenemos que meter en la clase cliente, que supongo, es lo que mas sentido tiene
        Client client(i, typeClient);

        if(typeClient==2){ //FREE 
            clientRequestFree.push(client);
        }
        else if(typeClient==0){ //PREMIUM
            
        }
        else{ //PREMIUM WITH BALANCE

        }

    }
}

void launchThreads(){
    int nThreads = 0;
    std::vector<std::thread> vThread;
    for(int i=0; i<nThreads;i+++){
        if(i==0){
            vThread.push_back(std::thread(systemPay));
        }
        else if(i==1){
            v_hilos.push_back(std::thread(findWord));
        }
        else{
            //v_hilos.push_back(std::thread(nose));
            //¿?Sleep para hacer hilos cada x tiempo????
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

}

void systemPay(){
    //Variable condicion cv.wait{return !queue.empty}
    //Semaforo para sincronizacion y pasarle el id del cliente

    

}

/*Leemos el fichero para sacar las lineas*/
std::vector<std::string> readFile(std::string bookPath)
{
    std::ifstream inputFile(bookPath);
    std::string line;
    std::vector<std::string> vLines;

    while (getline(inputFile, line))
    {
        vLines.push_back(line);
    }
    return vLines;
}
/*Buscamos la palabra deseada en el libro*/
void findWord(int iteration, std::vector<std::string> vector)
{
    std::vector<std::string> words;
    std::queue<Search_Result> queueResults;
    Search_Result results;
    int resultLine;

    for (int i = 0; i < vector.size(); i++)
    {

        std::string line = eraseSymbols(vector[i]);
        /*Transformamos todas las palabras de la linea en minusculas*/
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        /*Troceamos la linea en las palabras*/
        std::istringstream trocearLinea(line);
        std::copy(std::istream_iterator<std::string>(trocearLinea), std::istream_iterator<std::string>(), back_inserter(words));

        /*Recorremos las palabras de la linea*/
        for (int j = 0; j < words.size(); j++)
        {
            /*Si la palabra que estamos mirando es igual a la que buscamos*/
            if (words[j].compare(threadFinder[iteration].getSearchedWord()) == 0)
            {
                resultLine = threadFinder[iteration].getInitialLine();
                // std::cout<<"Palabra encontrada en linea "<<i+1+lineaR<<std::endl;
                results.setresultLine(i + resultLine);
                /*Si es la primera palabra de la linea*/
                if (j == 0)
                    results.setPreviousWord("---");
                else
                    results.setPreviousWord(words[j - 1]);
                /*Si es la ultima palabra de la linea*/
                if (j == words.size() - 1)
                    results.setNextWord("---");
                else
                    results.setNextWord(words[j + 1]);

                /*Metemos los resultados en la cola resultados*/
                queueResults.push(results);
            }
        }
        words.clear();
    }

    std::lock_guard<std::mutex> lockGuard_(mutex);
    threadFinder[iteration].setQueueResults(queueResults);
}

/*Eliminamos cualquier simbolo que pueda aparecer en el libro que no sea letra*/
std::string eraseSymbols(std::string line)
{
    for (int i = 0, size = line.size(); i < size; i++)
    {
        /*Si es un caracter, lo eliminamos de la linea*/
        if (ispunct(line[i]))
        {
            line.erase(i--, 1);
            size = line.size();
        }
    }
    return line;
}
/*Imprimimos los resultados*/
void printResults()
{
    int counter=0;

    for (int i = 0; i < threadFinder.size(); i++)
    {
        std::queue<Search_Result> queue = threadFinder[i].getQueueResults();

        while (!queue.empty())
        {
            std::cout << BLUE << "[Hilo: " << threadFinder[i].getId();
            std::cout << RED << " Inicio: " << threadFinder[i].getInitialLine();
            std::cout << YELLOW << " - final: " << threadFinder[i].getFinalLine() << "]";
            std::cout << BLUE << " :: line " << queue.front().getresultLine() << " ";
            std::cout << GREEN << "... " << queue.front().getPreviousWord() << " ";
            std::cout << RED << threadFinder[i].getSearchedWord() << " ";
            std::cout << BLUE << queue.front().getNextWord() << " ..." << std::endl;

            counter++;
            queue.pop();
        }
    }
    std::cout<<RESET<<"\nLa palabra "<<RED<<threadFinder[0].getSearchedWord()<<RESET<<" aparece " <<PINK<< counter <<RESET<< " veces\n" <<" "<<std::endl;
    std::cout << RESET << "Fin de " << BLUE << "SS" <<RED<< "O"<< YELLOW <<"O"<<BLUE<<"II"<<GREEN<< "GL"<<RED<< "E"<<std::endl;

}
