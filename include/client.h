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
    
    std::promise<std::queue<Search_Result>> prom;
    std::future<std::queue<Search_Result>> fut= prom.get_future();

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
            clientRequestFree.push(request);
            cvClient.notify_one(); //*Notify Finder thread so that they search into the queue
            std::queue<Search_Result> results= clientRequestFree.front().getIdClient().fut.get();
            
            uniLockQRequests.unlock();

        }
        else{
            std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests);
            clientRequestPremium.push(request);
            cvClient.notify_one();
            std::queue<Search_Result> results= clientRequestPremium.front().fut.get();
            uniLockQRequests.unlock();
        }


    }

    Client() {}
    Client(int idClient, int typeClient) : idClient(idClient), typeClient(typeClient), balance(balance) {}

    void asignBalance(int typeClient)
    {

        srand(time(NULL));

        switch (typeClient)
        {
        case 0:
            balance = INT_MAX;
            break;
        case 1:
            balance = 1 + rand() % (100 - 1);
            break;
        case 2:
            balance = 1 + rand() % (30 - 1);
            break;
        }
    }
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
