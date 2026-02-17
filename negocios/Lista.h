#pragma once
#include <string>
#include <functional>

using std::string;

struct Nodo{
    string nombre;
    string apellido;
    int numeral;
    long telefono;
    string accion;
    Nodo* sig;
};

class Lista{
    private:
        Nodo* heap;
        Nodo* actual;
        int cantOperaciones;
    public:
        Lista();
        ~Lista();
        void insertarOperacion(string nombre, string apellido, int numeral, long telefono, string accion);
        Nodo* crearNodo(string nombre, string apellido, int numeral, long telefono, string accion);
        void recorrerLista(std::function<void(Nodo*)> accion);
};