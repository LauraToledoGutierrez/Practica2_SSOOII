#pragma once

#ifndef Client

#include "request.h"
#include "./definitions.h"

extern std::vector<Request> clientRequestFree;
extern std::vector<Request> requestsDone;
extern std::vector<Request> clientRequestPremium;
extern void printResults(Request req);
extern std::vector<std::string> readFile(std::string bookPath);
extern void findWord(int nbook, int iteration, std::vector<std::string> vector, int lowerLimit, int uppwerLimit, Request requestCurrent);
extern void systemPay();

class Client
{
    int idClient;
    int typeClient; // 0: Premium
                    // 1: Premium with balance
                    // 2: Free
    int balance;
    Request request;
    
public:

    /***************************************
     * Description: When calling a client thread, this will be the execution of that theads 
     *      (Enqueueing requests, waiting fo the results of the search and printing the results)
     * Parameters: None
     * Version: 1.0
     * Date: 10/04/2022
     * *************************************/
    void operator()() 
    {
        std::vector<std::string> dictionary = {"casa", "telefono", "final"};
        srand(time(NULL));
        int wordToSearch= rand()%dictionary.size();
        int idRequest= g_id_request++;
        Request request(idRequest, idClient, dictionary[wordToSearch], typeClient);
        std::cout<<"Se ha lanzado el cliente "<<idClient<<" de tipo "<<request.getTypeClient()<< " y busca la palabra "<<dictionary[wordToSearch]<<std::endl;

        if(typeClient==2){
            mutexAlgo.lock();
            std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests); //* Mutual exclusion for enqueueing Requests
            clientRequestFree.push_back(request);
            cvFinder.notify_one(); //*Notify Finder thread so that they search into the queue

            for(int i=0; i<requestsDone.size(); i++){
                if(requestsDone[i].getIdClient()==idClient){
                    printResults(requestsDone[i]);
                }
            }
            mutexAlgo.unlock();

        }
        else if(typeClient == 1 || typeClient == 0){
            mutexAlgo.lock();
            std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests);
            clientRequestPremium.push_back(request);
            cvClient.notify_one();

            for(int i=0; i<requestsDone.size(); i++){
                if(requestsDone[i].getIdClient()==idClient){
                    printResults(requestsDone[i]);
                }
            }
            mutexAlgo.unlock();
        }
    }
    
    Client() {}
    Client(int idClient, int typeClient, int balance, Request request) : idClient(idClient), typeClient(typeClient), balance(balance), request(request){}

    int getidClient()
    {
        return idClient;
    }

    void setidClient(int idC)
    {
        this->idClient = idC;
    }

    int getTypeClient()
    {
        return typeClient;
    }

    int getBalance()
    {
        return balance;
    }

    void setBalance(int newSaldo)
    {
        balance = newSaldo;
    }
};

#endif