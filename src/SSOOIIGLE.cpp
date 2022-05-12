/*CLASE DONDE SE REALIZARA LA BUSQUEDA*/

/*Includes*/
#include "definitions.h"
#include "client.h"
#include "colors.h"
#include "finder.h"
#include "request.h"
#include "search_result.h"

/*Variables globales*/

std::queue<Search_Result> queueResults;
std::vector<Request> clientRequestFree;
std::vector<Request> clientRequestPremium;
std::vector<Client> listClients;
std::vector<Request> requestsDone;


std::queue<Client> q_clients_pay;
std::queue<Client> q_client_find;
/*std::vector<std::thread> vThreadClient;
std::vector<std::string> bookPath;
std::vector<Request> requestsDone;

std::queue<Search_Result> queueResults;
std::vector<Request> clientRequestFree;
std::vector<Request> clientRequestPremium;
std::vector<Client> listClients;
std::mutex mutex;
std::mutex mutexQRequests;
std::mutex mutexFinderChildren;
std::mutex mutexReduceBalance;
std::mutex mutexFinishedThreads;
std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests);
std::unique_lock<std::mutex> uniLockFinderChildren(mutexFinderChildren);

std::queue<Client> q_clients_pay;
std::queue<Client> q_client_find;

std::condition_variable cvClient;
std::condition_variable cvPay;
std::condition_variable cvFinder;

std::atomic<int> g_id_request(0);
std::atomic<int> g_id_client(0);
std::vector<std::string> books = {"./17-LEYES-DEL-TRABJO-EN-EQUIPO.txt", "./21-LEYES-DEL-LIDERAZGO.txt", "./25-MANERAS-DE-GANARSE-A-LA-GENTE.txt",
"./ACTITUD-DE-VENDEDOR.txt", "./El-oro-y-la-ceniza.txt", "./La-última-sirena.txt", "./prueba.txt",
"./SEAMOS-PERSONAS-DE-INFLUENCIA.txt", "./VIVE-TU-SUEÑO.txt"};
*/



std::vector<std::string> readFile(std::string bookPath);
int readLines();
void createClient();
void findWord(int nbook, int iteration, std::vector<std::string> vector, int lowerLimit, int uppwerLimit, Request requestCurrent);
std::string eraseSymbols(std::string line);
void printResults(Request req);
void createPath();
void systemPay();
void finder();
int asignBalance(int typeClient);
void launchThreads();
int compareClient(Request request);


int main(int argc, char *argv[])
{

    std::cout << RESET << "\nBienvenido a " << BLUE << "SS" << RED << "O" << YELLOW << "O" << BLUE << "II" << GREEN << "GL" << RED << "E\n"
              << std::endl;
    launchThreads();
}

//! DEPRECATED
/* Creamos la ruta de los libros y llamamos al metodo readFile para dividir el fichero en lineas */
/* void createPath()
{
    std::string path = "Libros_P2/";
    for (int i = 0; i < (int)books.size(); i++) {
        bookPath.push_back(path + books[i]);
        std::vector<std::string> vLines = readFile(bookPath[i]);
    }
}*/

void createClient()
{
    while (1)
    {
        for (int i = 0; i < NUMBERCLIENTS; i++)
        {
            srand(time(NULL));
            int typeClient = rand() % (3 - 1);
            int balance = asignBalance(typeClient);

            // El libro de cada cliente no se si lo tenemos que meter en la clase cliente, que supongo, es lo que mas sentido tiene
            g_id_client++;
            Client client(g_id_client, typeClient, balance);
            listClients.push_back(client);
            vThreadClient.push_back(std::thread(client));

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}
int asignBalance(int typeClient)
{
    srand(time(NULL));
    int balance_;
    switch (typeClient)
    {
    case 0:
        balance_ = INT_MAX;
        break;
    case 1:
        balance_ = 1 + rand() % (100 - 1);
        break;
    case 2:
        balance_ = 1 + rand() % (30 - 1);
        break;
    }
    return balance_;
}

void launchThreads()
{
    std::vector<std::thread> vThread;

    std::cout << "Se ha lanzado el sistema de pago" << std::endl;
    vThread.push_back(std::thread(systemPay));

    for (int i = 0; i < NUMBERFINDER; i++)
    {
        Finder finder;
        vThread.push_back(std::thread(finder));
        std::cout << "Se ha lanzado el buscador" << std::endl;
    }
    createClient();
}

/* La idea del metodo es crear un bucle infinito que solo se ejecute cuando la cola de clientes pendientes de pago tenga algun elemento, esto
lo controlamos con la cv. Cuando se ejecuta creamos un cliente con los datos del cliente de la cola y le añadimos el nuevo saldo, a continuacion
metemos el cliente de nuevo en una nueva cola que pasaremos al metodo busqueda. Notificaremos de que ya se ha recargado al metodo busqueda haciendo
uso de notify_all()*/
void systemPay()
{
    // Variable condicion cv.wait{return !queue.empty}
    // Semaforo para sincronizacion y pasarle el id del cliente

    int newBalance = 20;
    std::mutex mutex_pay;
    std::unique_lock<std::mutex> lk(mutex_pay);

    while (1)
    {
        cvPay.wait(lk, []
                   { return !q_clients_pay.empty(); });

        Client client = q_clients_pay.front();
        q_clients_pay.pop();
        std::cout << "El cliente " << client.getidClient() << " esta en el sistema de pago." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "El cliente " << client.getidClient() << " ya ha recargado su saldo." << std::endl;
        client.setBalance(newBalance);
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
void findWord(int nbook, int iteration, std::vector<std::string> vector, int lowerLimit, int upperLimit, Request requestCurrent) // TODO Cuando se queda sin saldo, encola en q_clients_pay
{
    std::vector<std::string> words;
    std::vector<std::vector<Search_Result>> vResults;
    Search_Result results;
    int resultLine;

    int indexClient = compareClient(requestCurrent);

    // IMPLEMENTATION OF FINDING WORD
    for (int i = lowerLimit; i < upperLimit; i++)
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
            if (words[j].compare(requestCurrent.getwordToSearch()) == 0 && listClients[indexClient].getBalance() > 0)
            {
                mutexReduceBalance.lock();
                listClients[indexClient].setBalance(listClients[indexClient].getBalance()-1);
                mutexReduceBalance.unlock();

                // std::cout<<"Palabra encontrada en linea "<<i+1+lineaR<<std::endl;
                results.setresultLine(i);
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
                // queueResults.push(results);

                requestCurrent.searchResults[nbook].push_back(results);

            } else if(listClients[indexClient].getTypeClient()==2 && listClients[indexClient].getBalance()<1){
                //TODO printf
                requestCurrent.end_Request();
                requestsDone.push_back(requestCurrent);
                cvFinder.notify_one();
            }
        }
        words.clear();

        mutexFinishedThreads.lock();
        requestCurrent.setFinishedThreads(requestCurrent.getFinishedThreads()+1);
        mutexFinishedThreads.unlock();

        if(listClients[indexClient].getTypeClient()!=2 && requestCurrent.getFinishedThreads()==(NUMBERTHREADS*books.size())){ //If all the finder threads have finished their search
            requestCurrent.end_Request();
            requestsDone.push_back(requestCurrent);
            cvFinder.notify_one();
        }
    }
    if (listClients[indexClient].getBalance() == 0 && !requestCurrent.getEndRequest() && listClients[indexClient].getTypeClient() == 2) //Pay System
    {
        q_clients_pay.push(listClients[indexClient]);
        cvFinder.notify_one();
    }
}

int compareClient(Request request)
{
    int i;
    for (i = 0; i < listClients.size(); i++)
    {
        if (listClients[i].getidClient() == request.getIdClient())
        {
            return i;
        }
    }
}

/* La idea de este metodo es que va a ser ejecutado por los buscadores. En este metodo controlamos las peticiones de busqueda y vemos el saldo que
le queda a cada cliente y si no tiene saldo llamamos al sistema de pago */
void finder()
{
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
// FIXME
void printResults(Request req)
{
    int counter = 0;
    std::vector<std::vector<Search_Result>> results;
    std::ofstream file;

    std::string id = std::to_string(req.getIdClient());
    std::string path = "./results/client_" + id + ".txt";
    file.open(path);
    results = req.getSearchResults();
    file << BLUE << "[Client: " << req.getIdClient() << " ";
    for (int i = 0; i < results.size(); i++)
    {

        for (int j = 0; j < results[i].size(); j++)
        {

            file << BLUE << " :: Line " << results[i][j].getresultLine() << " ";
            file << GREEN << "... " << results[i][j].getPreviousWord() << " ";
            file << RED << req.getwordToSearch() << " ";
            file << BLUE << results[i][j].getNextWord() << " ..." << std::endl;

            counter++;
        }
    }

    std::cout << RESET << "Fin de " << BLUE << "SS" << RED << "O" << YELLOW << "O" << BLUE << "II" << GREEN << "GL" << RED << "E" << std::endl;
}