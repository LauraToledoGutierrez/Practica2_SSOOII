#include <iostream>
#include <bits/stdc++.h>
#include <queue>
#include "request.h"
#include "definitions.h"

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
    void operator()() const
    {
        std::vector<std::string> dictionary = {"casa", "telefono", "final"};
        srand(time(NULL));
        int wordToSearch= rand()%dictionary.size();
        int idRequest= g_id_request++;
        Request request(idRequest, idClient, dictionary[wordToSearch]);
        if(typeClient==2){
            std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests); //* Mutual exclusion for enqueueing Requests
            clientRequestFree.push_back(request);
            cvClient.notify_one(); //*Notify Finder thread so that they search into the queue

            //std::queue<Search_Result> results= clientRequestFree.front().getIdClient().fut.get();
            for(int i=0; i<clientRequestFree.size(); i++){
                if(requestsDone[i].getIdClient()==idClient){
                    printResults(requestsDone[i].getSearchResults());
                }
            }
            uniLockQRequests.unlock();

        }
        else{
            std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests);
            clientRequestPremium.push_back(request);
            cvClient.notify_one();
            std::queue<Search_Result> results= clientRequestPremium.front().fut.get();
            uniLockQRequests.unlock();
        }


    }
    

    Client() {}
    Client(int idClient, int typeClient, int balance) : idClient(idClient), typeClient(typeClient), balance(balance) {}

    int getidCliente()
    {
        return idClient;
    }

    void setidCliente(int idC)
    {
        this->idClient = idC;
    }

    int getTipo()
    {
        return typeClient;
    }

    int getSaldo()
    {
        return balance;
    }

    void setSaldo(int newSaldo)
    {
        balance = newSaldo;
    }
};
