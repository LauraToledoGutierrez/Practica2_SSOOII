/*CLASE DONDE SE REALIZARA LA BUSQUEDA*/

/*Includes*/
#include "definitions.h"
#include "finder.h"
#include "colors.h"
#include "request.h"
#include "client.h"
#define NUMEROCLIENTES 10
#define NUMBERTHREADS 2

/*Variables globales*/
std::vector<Finder> threadFinder;
std::queue<Search_Result> queueResults;
std::queue<Request> clientRequestFree;
std::queue<Request> clientRequestPremium;
std::mutex mutexQRequests;
std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests);
std::mutex mutex;
std::vector<std::string> bookPath;

std::queue<Client> q_clients_pay;
std::queue<Client> q_client_find;
std::vector<std::thread> vThreadClient;

std::condition_variable cvClient;
std::condition_variable cvPay;
std::condition_variable cvFinder;

std::atomic<int> g_id_request(0);

std::vector<std::string> readFile(std::string bookPath);
int readLines();
void createClient(int nLines, int nThread, std::string searchWord, std::vector<std::string> vLines);
void findWord(int iteration, std::vector<std::string> vector);
std::string eraseSymbols(std::string line);
void printResults();
void createPath();
void systemPay();
void finder();

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

    //std::vector<std::string> vLines = readFile(bookPath);
    //int nLines = vLines.size();

    //createClient(nThread, nLines, searchWord, vLines);
    printResults();
}

/* Creamos la ruta de los libros y llamamos al metodo readFile para dividir el fichero en lineas */
void createPath()
{
    std::string path = "Libros_P2/";
    for (int i = 0; i < (int)books.size(); i++) {
        bookPath.push_back(path + books[i]);
        std::vector<std::string> vLines = readFile(bookPath[i]);
    }
}

/***********************************
 * Method: createFinderThreads
 * Description: Divides a source into several parts so that each thread can look into a part of the source file
 * Parameters: numberLines(number of total lines of the source file), wordToSearch(word that we are searching in the files), partialLines(all the lines a thread must look into)
 * Version: 1.0
 * Date: 10/04/2022
 ***********************************/

void createFinderThread(int numberLines, std::string wordToSearch, std::vector<std::string>partialLines){
    std::vector<std::thread> vhilos;
    for (int i = 0; i < NUMBERTHREADS; i++)
    {
        int lowerLimit, upperLimit;

        //Si es el primer hilo
        if (i == 0)
            lowerLimit = 1;
        else
            lowerLimit = ((numberLines / NUMBERTHREADS) * i + 1);

        //Si es el ultimo hilo
        if (i == NUMBERTHREADS - 1)
            upperLimit = numberLines;
        else
            upperLimit = (lowerLimit + (numberLines / NUMBERTHREADS) - 1);

        std::vector<std::string> vectorParcial;
        //Creamos el objeto finder con sus correspondientes valores
        Finder finder(wordToSearch, i, lowerLimit, upperLimit);
        threadFinder.push_back(finder);

        for (int j = lowerLimit - 1; j < upperLimit; j++)
        {
            vectorParcial.push_back(partialLines[j]);
        }
        //Cada hilo realizara el metodo de buscarPalabra
        vhilos.push_back(std::thread(findWord, i, vectorParcial));
    }

    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
}


void createClient (int nThread,int nLines,std::string searchWord,std::vector<std::string> vLines){
    std::vector<std::thread> vClients;

    for(int i=0; i<NUMEROCLIENTES; i++){

        srand(time(NULL));
        int typeClient= rand()%(3-1);
        int wordToSearch = rand() % dictionary.size(); //! Tiene que implementarlo el cliente
        //El libro de cada cliente no se si lo tenemos que meter en la clase cliente, que supongo, es lo que mas sentido tiene
        Client client(i, typeClient);
        if(typeClient==2){ //FREE 
            clientRequestFree.push(client);
        }
        else if(typeClient==0){ //PREMIUM
            //clientRequestPremium.push()
        }
        else{ //PREMIUM WITH BALANCE

        }

    }
}

void launchThreads(){
    int nThreads = 5;
    std::vector<std::thread> vThread;

    std::cout <<"Se ha lanzado el sistema de pago" <<std::endl;
    vThread.push_back(std::thread(systemPay));

    for(int i=0; i<4;i++){
        std::cout <<"Se ha lanzado el buscador" <<std::endl;
        vThread.push_back(std::thread(findWord));
    }
    while(1){
        Client c;
        for(int j=0; j<20; j++){
            std::cout <<"Se ha lanzado el cliente" <<std::endl;
            vThreadClient.push_back(std::thread(c));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }     
}
    //std::for_each(vThread.begin(), vThread.end(), std::mem_fn(&std::thread::join));


/* La idea del metodo es crear un bucle infinito que solo se ejecute cuando la cola de clientes pendientes de pago tenga algun elemento, esto
lo controlamos con la cv. Cuando se ejecuta creamos un cliente con los datos del cliente de la cola y le añadimos el nuevo saldo, a continuacion
metemos el cliente de nuevo en una nueva cola que pasaremos al metodo busqueda. Notificaremos de que ya se ha recargado al metodo busqueda haciendo
uso de notify_all()*/
void systemPay(){ 
    //Variable condicion cv.wait{return !queue.empty}
    //Semaforo para sincronizacion y pasarle el id del cliente

    int newBalance = 20;
    std::mutex mutex_pay;
    std::unique_lock<std::mutex> lk(mutex_pay);

    while(1){
        cvPay.wait(lk, []{ return !q_clients_pay.empty();});

        Client client = q_clients_pay.front();
        q_clients_pay.pop();
        std::cout <<"El cliente "<< client.getidCliente() << " esta en el sistema de pago." <<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout <<"El cliente "<< client.getidCliente() << " ya ha recargado su saldo." <<std::endl;
        client.setSaldo(newBalance);
        q_client_find.push(client);

        cvFinder.notify_all();
    }
}

/*Leemos el fichero para sacar las partialLines*/
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
        /*Transformamos todas v_hilosas palabras de la linea en minusculas*/
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

/* La idea de este metodo es que va a ser ejecutado por los buscadores. En este metodo controlamos las peticiones de busqueda y vemos el saldo que 
le queda a cada cliente y si no tiene saldo llamamos al sistema de pago */
void finder(){
    //¿Cola de peticiones? -> si no esta vacia -> buscamos la palabra -> si no tiene saldo -> llamar al systemPay
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
