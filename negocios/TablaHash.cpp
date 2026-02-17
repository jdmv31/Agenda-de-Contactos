#include "negocios/TablaHash.h"

TablaHash::TablaHash(){
    for (int i = 0; i < MAXELEMENTOS;i++){
        tablaHash[i] = nullptr;
    }
}


TablaHash::~TablaHash() {
    for (int i = 0; i < MAXELEMENTOS; i++) {
        Nodo* actual = tablaHash[i];

        while (actual != nullptr) {
            Nodo* borrar = actual;
            actual = actual->sig;
            delete borrar; 
        }

        tablaHash[i] = nullptr; 
    }
}

int TablaHash::hashing (long telefono){
    return telefono % 10;
}

Nodo* TablaHash::crearNodo(string nombre, string apellido, string correo, int numeral, long numero){
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->nombre = nombre;
    nuevoNodo->apellido = apellido;
    nuevoNodo->correo = correo;
    nuevoNodo->numeral = numeral;
    nuevoNodo->numeroTelefonico = numero;
    nuevoNodo->sig = nullptr;

    return nuevoNodo;
}

void TablaHash::agregarContacto(string nombre, string apellido, string correo, int numeral, long numero){
    int indice = hashing(numero);
    Nodo* nuevoNodo = crearNodo(nombre,apellido,correo,numeral,numero);
    if (tablaHash[indice] == nullptr)
        tablaHash[indice] = nuevoNodo;
    else{
        nuevoNodo->sig = tablaHash[indice];
        tablaHash[indice] = nuevoNodo;
    }
}

void TablaHash::recorrerTabla(std::function<void(Nodo*)> accion) {
    for (int i = 0; i < MAXELEMENTOS; i++) {
        Nodo* actual = tablaHash[i];
        
        while (actual != nullptr) {
            accion(actual);
            actual = actual->sig;
        }
    }
}

bool TablaHash::eliminarContacto(long numero){
    int indice = hashing(numero);
    Nodo*anterior = nullptr;
    Nodo*actual = tablaHash[indice];

    while (actual != nullptr){
        if (actual->numeroTelefonico == numero){
            if (anterior == nullptr)
                tablaHash[indice] = actual->sig;
            else
                anterior->sig = actual->sig;
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->sig;
    }
    return false;
}


bool TablaHash::modificarContacto(long numero, string nombreN, string apellidoN, string correoN){
    int indice = hashing(numero);
    Nodo* actual = tablaHash[indice];

    while (actual != nullptr) {
        if (actual->numeroTelefonico == numero) {
            actual->nombre = nombreN;
            actual->apellido = apellidoN;
            actual->correo = correoN;
            return true;
        }
        actual = actual->sig;
    }
    return false; 
}