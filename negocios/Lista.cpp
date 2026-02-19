#include "negocios/Lista.h"

Lista::Lista(){
    heap = actual = nullptr;
    cantOperaciones = 0;
}

Lista::~Lista(){
    actual = heap;
    while (actual != nullptr){
        Nodo2* aux = actual;
        delete aux;
        actual = actual->sig;
    }
    actual = heap = nullptr;
}

Nodo2* Lista::crearNodo(string nombre, string apellido, int numeral, long telefono, string accion){
    Nodo2* nuevonodo = new Nodo2;
    nuevonodo->nombre = nombre;
    nuevonodo->apellido = apellido;
    nuevonodo->numeral = numeral;
    nuevonodo->telefono = telefono;
    nuevonodo->accion = accion;
    nuevonodo->sig = nullptr;
    return nuevonodo;
}

void Lista::insertarOperacion(string nombre, string apellido, int numeral, long telefono, string accion){
    Nodo2* nuevo = crearNodo(nombre,apellido,numeral,telefono,accion);
    if (heap == nullptr)
        heap = nuevo;
    else{
        nuevo->sig = heap;
        heap = nuevo;
    }
}

void Lista::recorrerLista(std::function<void(Nodo2*)> accion){
    actual = heap;
    while (actual != nullptr){
        accion(actual);
        actual = actual->sig;
    }

}