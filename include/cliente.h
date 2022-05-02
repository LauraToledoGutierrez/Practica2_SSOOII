#include <iostream>
#include <bits/stdc++.h>
#include <queue>

class Cliente{
    int idCliente;
    std::string tipo;
    int saldo;

    Cliente(){}
    Cliente(int idCliente, std::string tipo, int saldo): idCliente(idCliente), tipo(tipo), saldo(saldo){}

    void asignarSaldo(std::string tipo) {

        srand(time(NULL));

        if(tipo.compare("premiumL") == 0){
            saldo = 1 + rand() % (100 - 1);        
        }
        else if (tipo.compare("gratis")==0) {
            saldo = 1 + rand() % (30 - 1); 
        }
        else {
            saldo = INT_MAX; 
        }
    }
    int getidCliente() {
        return idCliente;
    }

    void setidCliente(int idC) {
        this->idCliente = idC;
    }

    std::string getTipo() {
        return tipo;
    }

    int getSaldo() {
        return saldo;
    }

    void setSaldo(int nuevoSaldo) {
        saldo = nuevoSaldo;
    }


};
