#include "datos/Ficheros.h"
#include "negocios/Lista.h"
#include "negocios/TablaHash.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include "PathConfig.h"

namespace fs = std::filesystem;


GestorArchivos::GestorArchivos(){
    operacionesRealizadas = 0;
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


void GestorArchivos::guardarDatos(TablaHash& tabla){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "tablaHash";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "tablaHash.txt";
    std::ofstream archivo(rutaArchivo,std::ios::out);
    archivo.imbue(std::locale("C"));

    tabla.recorrerTabla([&archivo](Nodo* contacto){
        archivo << contacto->nombre << "|"
                << contacto->apellido << "|"
                << contacto->correo << "|"
                << contacto->numeral << "|"
                << contacto->numeroTelefonico << "|\n";
    });

    archivo.close();
}
void GestorArchivos::cargarDatos(TablaHash& tabla){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "tablaHash";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "tablaHash.txt";
    std::ifstream archivo(rutaArchivo,std::ios::in);

    std::string linea;
    while (std::getline(archivo,linea)){
        if (linea.empty())
            continue;
        std::stringstream ss (linea);
        std::string nombre, apellido, correo, numeral, telefono;
        
        std::getline(ss,nombre,'|');
        std::getline(ss,apellido,'|');
        std::getline(ss,correo,'|');
        std::getline(ss,numeral,'|');
        std::getline(ss,telefono,'|');

        if (!nombre.empty() && !telefono.empty())
            tabla.agregarContacto(nombre,apellido,correo,std::stoi(numeral),std::stol(telefono));
    }


    archivo.close();
}


void GestorArchivos::guardarContador(){
    contactosAlmacenados++;
    int contadorGuardado = contactosAlmacenados;
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorContactos";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "contador.txt";
    std::ofstream archivo(rutaArchivo,std::ios::out);
    if (!archivo.is_open())
        return;
    
    archivo<<contadorGuardado;

    archivo.close();
}


int GestorArchivos::leerContador(){
    int num = 0;
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorContactos";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "contador.txt";
    std::ifstream archivo(rutaArchivo,std::ios::in);
    if (!archivo.is_open())
        return 0;

    archivo >> num;
    contactosAlmacenados = num;

    return num;
}


void GestorArchivos::guardarLista(Lista& lista){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contactosRecientes";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "operaciones.txt";
    std::ofstream archivo(rutaArchivo,std::ios::out);
    archivo.imbue(std::locale("C"));

    lista.recorrerLista([&archivo](Nodo2* operacion){
        archivo << operacion->nombre << "|"
                << operacion->apellido << "|"
                << operacion->numeral << "|"
                << operacion->telefono << "|"
                << operacion->accion << "|\n";
    });

    archivo.close();
}

void GestorArchivos::leerLista(Lista& lista){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contactosRecientes";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "operaciones.txt";
    std::ifstream archivo(rutaArchivo,std::ios::in);

    std::string linea;
    while (std::getline(archivo,linea)){
        if (linea.empty())
            continue;
        std::stringstream ss (linea);
        std::string nombre, apellido, numeral, telefono, accion;
        
        std::getline(ss,nombre,'|');
        std::getline(ss,apellido,'|');
        std::getline(ss,numeral,'|');
        std::getline(ss,telefono,'|');
        std::getline(ss,accion,'|');

        if (!nombre.empty() && !telefono.empty())
            lista.insertarOperacion(nombre,apellido,std::stoi(numeral),std::stol(telefono),accion);
    }


    archivo.close();
}

void GestorArchivos::guardarOperaciones(){
    operacionesRealizadas++;
    int contadorGuardado = operacionesRealizadas;
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorOperaciones";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "contador.txt";
    std::ofstream archivo(rutaArchivo,std::ios::out);
    if (!archivo.is_open())
        return;
    
    archivo<<contadorGuardado;

    archivo.close();
}

int GestorArchivos::leerOperaciones(){
    int num = 0;
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorOperaciones";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "contador.txt";
    std::ifstream archivo(rutaArchivo,std::ios::in);
    if (!archivo.is_open())
        return 0;

    archivo >> num;
    operacionesRealizadas = num;

    return num;
}