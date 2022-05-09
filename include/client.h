#include <iostream>
#include <bits/stdc++.h>
#include <queue>

class Client{
    int idClient;
    int typeClient;     //0: Premium
                        //1: Premium with balance
                        //2: Free
    int balance;

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


};
