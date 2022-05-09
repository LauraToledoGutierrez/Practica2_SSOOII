#include <iostream>
#include <bits/stdc++.h>
#include <queue>
#include "request.h"

class Client{
    int idClient;
    int typeClient;     //0: Premium
                        //1: Premium with balance
                        //2: Free
    int balance;
    Request request;

    Client(){}
    Client(int idClient, int typeClient): idClient(idClient), typeClient(typeClient), balance(balance){}

    void asignBalance(int typeClient) {

        srand(time(NULL));

        switch(typeClient){
            case 0:
                balance=INT_MAX;
                break;
            case 1:
                balance= 1+ rand()% (100 - 1);       
                break;
            case 2:
                balance = 1 + rand() % (30 - 1); 
                break;
        }
    }
    int getidCliente() {
        return idClient;
    }

    void setidCliente(int idC) {
        this->idClient = idC;
    }

    int getTipo() {
        return typeClient;
    }

    int getSaldo() {
        return balance;
    }

    void setSaldo(int newSaldo) {
        balance = newSaldo;
    }

    Request getRequest(){
        return request;
    }
    void setRequest(Request request){
        this->request = request;
    }

};
