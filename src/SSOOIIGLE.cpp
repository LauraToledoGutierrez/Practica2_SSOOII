/*INCLUDES*/
#include "request.h"
#include "definitions.h"
#include "client.h"
#include "colors.h"
#include "finder.h"
#include "search_result.h"

/*GLOBAL VARIABLES*/
std::queue<Search_Result> queueResults;
std::vector<Request> clientRequestFree;
std::vector<Request> clientRequestPremium;
std::vector<Client> listClients;
std::vector<Request> requestsDone;
std::queue<Client> q_clients_pay;
std::queue<Client> q_client_find;

/* METHOD DEFINITIONS */
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

/***********************************
 * Method: main()
 * Description: main method for execution
 * Parameters: argc, *argv[]
 ***********************************/
int main(int argc, char *argv[])
{
    std::cout << RESET << "\nBienvenido a " << BLUE << "SS" << RED << "O" << YELLOW << "O" << BLUE << "II" << GREEN << "GL" << RED << "E\n"
              << RESET<< std::endl;
    launchThreads();
}

/***********************************
 * Method: createClient()
 * Description: Creates the clients of our program with the corresponding attributes. 
 *  Creates every 15 seconds the defined number of clients.
 ***********************************/
void createClient()
{
    while (1)
    {
        for (int i = 0; i < NUMBERCLIENTS; i++)
        {
            //Mutex used to protect the variable "balance"
            mutexBalance.lock();
            Request req(0,0,"",0);
            srand(time(NULL));
            //Generates client type randomly
            int typeClient = rand() % (3);
            //Allocates the balance to the client depends client type
            int balance = asignBalance(typeClient);

            g_id_client++;
            //Creates the client with the corresponding attributes 
            Client client(g_id_client, typeClient, balance, req);
            listClients.push_back(client);
            vThreadClient.push_back(std::thread(std::move(client)));

            std::this_thread::sleep_for(std::chrono::seconds(1));

            mutexBalance.unlock();
        }
        std::this_thread::sleep_for(std::chrono::seconds(15));
    }
}

/***********************************
 * Method: asignBalance
 * Description: Calculates the balance of each client depending on what type it is
 * Parameters: typeClient
 ***********************************/
int asignBalance(int typeClient)
{
    srand(time(NULL));
    int balance_;
    switch (typeClient)
    {
    //Premium client with unlimited balance
    case 0:
        balance_ = INT_MAX;
        break;
    //Premium client with limited balance
    case 1:
        balance_ = 1 + rand() % (300 - 1);
        break;
    //Free client with limited balance
    case 2:
        balance_ = 1 + rand() % (50 - 1);
        break;
    }
    return balance_;
}

/***********************************
 * Method: launchThreads
 * Description: Launchs the different type of thread (threads of finder, thread of system pay and threads of clients)
 ***********************************/
void launchThreads()
{
    //Vector to save the differents threads generated
    std::vector<std::thread> vThread;

    std::cout << "Se ha lanzado el sistema de pago" << std::endl;
    //Launchs the thread of system pay
    vThread.push_back(std::thread(std::move(systemPay)));

    //Launchs the diferents threads of finder
    for (int i = 0; i < NUMBERFINDER; i++)
    {
        Finder finder("",0,0,0);
        vThread.push_back(std::thread(std::move(finder)));
        std::cout << "Se ha lanzado el buscador" << std::endl;
    }
    //Launchs the threads of client
    createClient();
    
    std::for_each(vThreadClient.begin(), vThreadClient.end(), std::mem_fn(&std::thread::join));
}

/***********************************
 * Method: systemPay
 * Description: System of pay is a sleeper thread, it wakes up when the finder wants recharge the balance of the client
 ***********************************/
void systemPay()
{
    // Semaforo para sincronizacion y pasarle el id del cliente

    int newBalance = 20;
    std::mutex mutex_pay;
    std::unique_lock<std::mutex> lk(mutex_pay);

    //Infinite loop to keep waiting until a finder needs its service
    while (1)
    {
        //Conditional variable that allows to take clients from the queue "q_clients_pay"
        cvPay.wait(lk, []
                   { return !q_clients_pay.empty(); });
        //Creates a client with the same attributes as the queue client and recharge its balance
        Client client = q_clients_pay.front();
        q_clients_pay.pop();
        std::cout << "El cliente " << client.getidClient() << " esta en el sistema de pago." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "El cliente " << client.getidClient() << " ya ha recargado su saldo." << std::endl;
        client.setBalance(newBalance);
        //Put the client back in the queue "q_client_find"
        q_client_find.push(client);

        //Notifies the finder that it has already done its job 
        cvFinder.notify_all();
    }
}

/***********************************
 * Method: readFIle()
 * Description: Divides the book into lines
 * Parameters: bookPath 
 ***********************************/
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

/***********************************
 * Method: findWord()
 * Description: Finds the word in the book.
 *  This method is implemented to use different threads for the same book
 * Parameters: nbook, iteration, vector, loweLimit, upperLimit, requestCurrent
 ***********************************/
void findWord(int nbook, int iteration, std::vector<std::string> vector, int lowerLimit, int upperLimit, Request requestCurrent) 
{
    std::vector<std::string> words;
    std::vector<std::vector<Search_Result>> vResults;
    Search_Result results;
    int resultLine;
    
    //Gets the client from the request
    int indexClient = compareClient(requestCurrent);

    // IMPLEMENTATION OF FINDING WORD
    for (int i = lowerLimit; i < upperLimit; i++)
    {
        std::string line = eraseSymbols(vector[i]);
        
        //Transforms all the words of the line in lowercase
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        //Divides the line into words
        std::istringstream trocearLinea(line);
        std::copy(std::istream_iterator<std::string>(trocearLinea), std::istream_iterator<std::string>(), back_inserter(words));

        //Loop the words of the line
        for (int j = 0; j < words.size(); j++)
        {            
            //If the word it is looking at is the same as the one it is looking for
            if (words[j].compare(requestCurrent.getwordToSearch()) == 0 && listClients[indexClient].getBalance() > 0)
            {
                //Decreases the balance every time there is an occurrence
                mutexReduceBalance.lock();
                listClients[indexClient].setBalance(listClients[indexClient].getBalance()-1);
                mutexReduceBalance.unlock();
                results.setresultLine(i);

                //If it is the first word of the line
                if (j == 0)
                    results.setPreviousWord("---");
                else
                    results.setPreviousWord(words[j - 1]);
                //If it is the last word of the line
                if (j == words.size() - 1)
                    results.setNextWord("---");
                else
                    results.setNextWord(words[j + 1]);

                //Adds the search result in the queue
                requestCurrent.searchResults.push_back(results);
                //Notifies the finder that it has already done its job 
                cvFinder.notify_one();
            
            //If it still havent found the word but the client is type free and it hasnt balance, its request ends
            } else if(listClients[indexClient].getTypeClient()==2 && listClients[indexClient].getBalance()<1){
                //Finalices its request
                requestCurrent.end_Request();
                requestsDone.push_back(requestCurrent);
                //Notifies the finder that it has already done its job 
                cvFinder.notify_one();
            }
        }
        words.clear();

        // FIXME
        mutexFinishedThreads.lock();
        requestCurrent.setFinishedThreads(requestCurrent.getFinishedThreads()+1);
        mutexFinishedThreads.unlock();

        //FIXME
        //If all the finder threads have finished their search
        if(listClients[indexClient].getTypeClient()!=2 && requestCurrent.getFinishedThreads()==(NUMBERTHREADS*books.size())){ 
            requestCurrent.end_Request();
            requestsDone.push_back(requestCurrent);
            cvFinder.notify_one();
        }
    }
    //If its is a client of type premium with limited balance and its balance is 0, it goes to system of pay
    if (listClients[indexClient].getBalance() == 0 && !requestCurrent.getEndRequest() && listClients[indexClient].getTypeClient() == 1) //Pay System
    {
        q_clients_pay.push(listClients[indexClient]);
        //Notifies to system pay that a thread needs it
        cvPay.notify_one();
    }
}

/***********************************
 * Method: compareClient()
 * Description: Compares the client ids of the system with the client id of the request. 
 *  Its help to know who is the client ask for the request
 * Parameters: request
 ***********************************/
int compareClient(Request request)
{
    int i;
    //Loop the list with all clients
    for (i = 0; i < listClients.size(); i++)
    {
        if (listClients[i].getidClient() == request.getIdClient())
        {
            return i;
        }
    }
    return 0;
}

/***********************************
 * Method: eraseSymbols()
 * Description: Removes any character that may appear in the book 
 * Parameters: line
 ***********************************/
std::string eraseSymbols(std::string line)
{
    for (int i = 0, size = line.size(); i < size; i++)
    {
        //If it is a character, it removes it from the line
        if (ispunct(line[i]))
        {
            line.erase(i--, 1);
            size = line.size();
        }
    }
    return line;
}

/***********************************
 * Method: printResults()
 * Description: Prints differents results from differents searches and total search time for each client
 * Parameters: req, time_exe
 ***********************************/
void printResults(Request req, double time_exe)
{
    int counter = 0;
    std::vector<Search_Result> results;
    std::ofstream file;

    //Gives the client id of the request
    std::string id = std::to_string(req.getIdClient());
    //Creates the path of the new folder where it save the results 
    std::string path = "./results/client_" + id + ".txt";
    file.open(path);

    //Gives the results of the request
    results = req.getSearchResults();

    //Prints the differents results
    file << BLUE << "[Client: " << req.getIdClient() << " ";
    for (int i = 0; i < results.size(); i++)
    {
        file << BLUE << " :: Line " << results[i].getresultLine() << " ";
        file << GREEN << "... " << results[i].getPreviousWord() << " ";
        file << RED << req.getwordToSearch() << " ";
        file << BLUE << results[i].getNextWord() << " ..." << std::endl;
        file << " El tiempo de ejecucion del cliente " << req.getIdClient() << " es de " << time_exe << " segundos.\n ";

        counter++;
    }

    std::cout << RESET << "Fin de " << BLUE << "SS" << RED << "O" << YELLOW << "O" << BLUE << "II" << GREEN << "GL" << RED << "E" << std::endl;
}