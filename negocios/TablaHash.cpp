#include "negocios/TablaHash.h"

TablaHash::TablaHash(){

}


TablaHash::~TablaHash(){


}

int TablaHash::hashing (long telefono){
    return telefono % 10;
}

Nodo* TablaHash::crearNodo(string nombre, string correo, short numeral, long numero){
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->nombre = nombre;
    nuevoNodo->correo = correo;
    nuevoNodo->numeral = numeral;
    nuevoNodo->numeroTelefonico = numero;
    nuevoNodo->sig = nullptr;

    return nuevoNodo;
}

void TablaHash::agregarContacto(string nombre, string correo, short numeral, long numero){
    int indice = hashing(numero);
    Nodo* nuevoNodo = crearNodo(nombre,correo,numeral,numero);
    if (tablaHash[indice] == nullptr)
        tablaHash[indice] = nuevoNodo;
    else{
        nuevoNodo->sig = tablaHash[indice];
        tablaHash[indice] = nuevoNodo;
    }
}