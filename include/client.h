/**************************************************
 * Class: Client
 * Description: Holds the definition of the Client
 *  object and the functionality of the Client thread
 *
 *************************************************/

#pragma once

#ifndef Client

#include "request.h"
#include "./definitions.h"
#include "./colors.h"

/*External methods and variables from the SSOOIIGLE.cpp defined*/
extern std::vector<Request> clientRequestFree;
extern std::vector<Request> requestsDone;
extern std::vector<Request> clientRequestPremium;
extern void printResults(Request req, double time_exe);
extern std::vector<std::string> readFile(std::string bookPath);
extern void findWord(int nbook, int iteration, std::vector<std::string> vector, int lowerLimit, int uppwerLimit, Request requestCurrent);
extern void systemPay();

class Client
{
    int idClient;   // Holds an unique ID that identifies the client
    int typeClient; // Contains the type of the client
                    // 0: Premium Plus       1: Premium with balance     2: Free
    int balance; // Contains the balance of each client. In case of Premium Plus, balance= INT_MAX

    Request request; // We create the request that each client will create

public:
    /***************************************
     * Description: When creating a client thread, this will be the execution of those theads
     *      (They perform the enqueueing of the requests, waiting for the results of the search and printing the results)
     * Parameters: None
     * Version: 1.0
     * Date: 10/04/2022
     * *************************************/
    void operator()()
    {
        double t0, t1;
        //Calculate total search time for the client 
        double totalTime;
        std::vector<std::string> dictionary = {"casa", "telefono", "final"}; // Dictionary of all the available words to be searched
        srand(time(NULL));
        int wordToSearch = rand() % dictionary.size(); // Pick a random word from the dictionary
        int idRequest = g_id_request++;                // Assign a unique ID for this Request
        std::cout << "Client: " <<RED<<idClient<<RESET<< " type: " <<GREEN<<typeClient<<RESET << " and searching word: " << BLUE<<dictionary[wordToSearch] <<RESET<< " has been launched." << std::endl;
        Request request(idRequest, idClient, dictionary[wordToSearch], typeClient); // Initialize the request

        /** Different implementations for different kind of clients **/
        if (typeClient == 2)
        {
            t0 = clock();
            mutexAlgo.lock();
            std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests); //* Mutual exclusion for enqueueing Requests
            clientRequestFree.push_back(std::move(request));
            cvClient.notify_one(); //*Notify Finder thread so that they search into the queue
            // uniLockQRequests.unlock(); //?FIXME

            /* Once the search is found, we look into the requestsDone vector */
            for (int i = 0; i < requestsDone.size(); i++)
            { 
                /*If the client ID from the finalized requests is the same than ours*/
                if (requestsDone[i].getIdClient() == idClient)
                { 
                    t1 = clock();
                    totalTime = (t1 - t0) / CLOCKS_PER_SEC;
                    printResults(requestsDone[i], totalTime);
                }
            }
            mutexAlgo.unlock();
        }
        /** If client is Premium or Premium Plus **/
        else
        {
            t0 = clock();
            std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests);
            clientRequestPremium.push_back(std::move(request)); // Enqueue requests on the premium requests queue
            cvClient.notify_one();
            //uniLockQRequests.unlock(); //?FIXME

            for (int i = 0; i < requestsDone.size(); i++)
            {
                if (requestsDone[i].getIdClient() == idClient)
                {
                    t1 = clock();
                    totalTime = (t1 - t0) / CLOCKS_PER_SEC;
                    printResults(requestsDone[i], totalTime);
                }
            }
        }
    }

    Client() {}
    Client(int idClient, int typeClient, int balance, Request request) : idClient(idClient), typeClient(typeClient), balance(balance), request(request) {}

    /*********
     *Method: getIdClient
     *Description: Returns the id of this Client
     *Return type: int
     */
    int getidClient()
    {
        return idClient;
    }

    /*********
     *Method: setIdClient
     *Description: Updates the id of this client
     *Parameters: int id of the client
     */
    void setidClient(int idC)
    {
        this->idClient = idC;
    }

    /*********
     *Method: getTypeClient
     *Description: Returns the type this client is
     *Return type: int
     */
    int getTypeClient()
    {
        return typeClient;
    }

    /*********
     *Method: getBalance
     *Description: Returns the remaining balance of this client
     *Return type: int
     */
    int getBalance()
    {
        return balance;
    }

    /*********
     *Method: setBalance
     *Description: Updates the balance of this client
     *Parameters: int newBalance
     */
    void setBalance(int newBalance)
    {
        balance = newBalance;
    }
};

#endif