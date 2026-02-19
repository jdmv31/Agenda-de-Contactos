#pragma once
#include <string>
#include <functional>
#include "datos/Ficheros.h"
#define MAXELEMENTOS 10

using std::string;

struct Nodo{
    string nombre;
    string apellido;
    string correo;
    int numeral;
    long numeroTelefonico;
    Nodo* sig;
};

class TablaHash{
    private:
        Nodo* tablaHash [MAXELEMENTOS];
    public:
        TablaHash();
        ~TablaHash();
        int hashing (long telefono);
        void agregarContacto(string nombre, string apellido, string correo, int numeral, long numero);
        Nodo* crearNodo(string nombre, string apellido, string correo, int numeral, long numero);
        void recorrerTabla(std::function<void(Nodo*)> accion);
        bool eliminarContacto(long numero);
        bool modificarContacto(long numero, string nombreN, string apellidoN, string correoN);
};
