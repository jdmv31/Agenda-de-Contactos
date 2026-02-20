#pragma once
#include <string>

using std::string;
// Declaraciones de clases externas
class TablaHash;
class Lista;

// Clase para gestionar el guardado y carga de archivos
class GestorArchivos{
    private:
        int operacionesRealizadas; // Contador de operaciones realizadas
        int contactosAlmacenados; // Contador de contactos almacenados
    public:
        GestorArchivos();
        // Guardado y carga de la estructura principal
        void guardarDatos(TablaHash& tabla);
        void cargarDatos(TablaHash& tabla);

        // Exportación de los archivos guardados en formato CSV
        bool exportarCSV(TablaHash& tabla);
        // Gestión de contadores individuales 
        void guardarContador();
        int leerContador();
        // Guardado y carga de listas enlazadas
        void guardarLista(Lista& lista);
        void leerLista(Lista& lista);
        // Registro de historial de operaciones
        void guardarOperaciones();
        int leerOperaciones();
};