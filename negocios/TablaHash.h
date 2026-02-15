#pragma once
#include <string>
#define MAXELEMENTOS 10

using std::string;

struct Nodo{
    string nombre;
    string correo;
    short numeral;
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
        void agregarContacto(string nombre, string correo, short numeral, long numero);
        Nodo* crearNodo(string nombre, string correo, short numeral, long numero);
};