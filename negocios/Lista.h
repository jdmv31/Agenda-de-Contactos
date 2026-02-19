#pragma once
#include <string>
#include <functional>

using std::string;

struct Nodo2{
    string nombre;
    string apellido;
    int numeral;
    long telefono;
    string accion;
    Nodo2* sig;
};

class Lista{
    private:
        Nodo2* heap;
        Nodo2* actual;
        int cantOperaciones;
    public:
        Lista();
        ~Lista();
        void insertarOperacion(string nombre, string apellido, int numeral, long telefono, string accion);
        Nodo2* crearNodo(string nombre, string apellido, int numeral, long telefono, string accion);
        void recorrerLista(std::function<void(Nodo2*)> accion);
};