#include <iostream>
#include <bits/stdc++.h>
#include <queue>

class Cliente{
    int idCliente;
    int tipo;   //0: Premium
                //1: Premium con saldo
                //2: Gratuita
    int saldo;

    Cliente(){}
    Cliente(int idCliente, int tipo): idCliente(idCliente), tipo(tipo), saldo(saldo){}

    void asignarSaldo(int tipo) {

        srand(time(NULL));

        switch(tipo){
            case 0:
                saldo=INT_MAX;
                break;
            case 1:
                saldo= 1+ rand()% (100 - 1);       
                break;
            case 2:
                saldo = 1 + rand() % (30 - 1); 
                break;
        }
    }
    int getidCliente() {
        return idCliente;
    }

    void setidCliente(int idC) {
        this->idCliente = idC;
    }

    int getTipo() {
        return tipo;
    }

    int getSaldo() {
        return saldo;
    }

    void setSaldo(int nuevoSaldo) {
        saldo = nuevoSaldo;
    }


};
