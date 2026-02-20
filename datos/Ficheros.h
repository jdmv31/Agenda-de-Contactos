#pragma once
#include <string>

using std::string;

class TablaHash;
class Lista;

class GestorArchivos{
    private:
        int operacionesRealizadas;
        int contactosAlmacenados;
    public:
        GestorArchivos();
        void guardarDatos(TablaHash& tabla);
        void cargarDatos(TablaHash& tabla);
        bool exportarCSV(TablaHash& tabla);
        void guardarContador();
        int leerContador();
        void guardarLista(Lista& lista);
        void leerLista(Lista& lista);
        void guardarOperaciones();
        int leerOperaciones();
};