#include "datos/Ficheros.h"
#include "negocios/TablaHash.h"
#include <fstream>
#include <filesystem>
#include "PathConfig.h"

namespace fs = std::filesystem;


GestorArchivos::GestorArchivos(){
    contactosAlmacenados = 0;
}

bool GestorArchivos::exportarCSV(TablaHash& tabla){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "csv";
    
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
        
    fs::path rutaArchivo = carpeta / "contactos.csv";
    std::ofstream archivo(rutaArchivo, std::ios::out);

    if (!archivo.is_open())
        return false;
    
    archivo << "Nombre,Apellido,Correo,Numeral,Numero Telefonico\n";
    tabla.recorrerTabla([&archivo](Nodo* contacto) {
        archivo << contacto->nombre << ","
                << contacto->apellido << ","
                << contacto->correo << ","
                << contacto->numeral << ","
                << contacto->numeroTelefonico << "\n";
    });

    archivo.close();
    return true;
}