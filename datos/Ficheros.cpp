#include "datos/Ficheros.h"
#include <fstream>

GestorArchivos::GestorArchivos(){
    contactosAlmacenados = 0;
}

bool GestorArchivos::exportarCSV(TablaHash& tabla){
    std::ofstream archivo ("contactos.csv",std::ios::out);

    if (!archivo.is_open())
        return false;

}