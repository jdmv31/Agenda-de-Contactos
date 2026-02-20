#include "datos/Ficheros.h"
#include "negocios/Lista.h"
#include "negocios/TablaHash.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include "PathConfig.h"

namespace fs = std::filesystem;

// Constructor: Inicializa los contadores internos a cero al instanciar la clase
GestorArchivos::GestorArchivos(){
    operacionesRealizadas = 0;
    contactosAlmacenados = 0;
}
// Exporta los datos de la tabla hash a un archivo con formato .csv (separado por comas)
bool GestorArchivos::exportarCSV(TablaHash& tabla){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "csv";
    // Si la carpeta contenedora no existe, la crea
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
        
    fs::path rutaArchivo = carpeta / "contactos.csv";
    std::ofstream archivo(rutaArchivo, std::ios::out);
// Verifica si el archivo se abrió correctamente para evitar errores de escritura
    if (!archivo.is_open())
        return false;

    // Escribe el encabezado de las columnas
    archivo << "Nombre,Apellido,Correo,Numeral,Numero Telefonico\n";
    // Recorre la tabla hash con una función lambda y escribe cada atributo separado por coma
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

// Guarda todos los contactos de la tabla hash en un archivo de texto (separador '|')
void GestorArchivos::guardarDatos(TablaHash& tabla){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "tablaHash";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "tablaHash.txt";
    std::ofstream archivo(rutaArchivo,std::ios::out);
     // Fuerza la configuración regional estándar ("C") para evitar problemas con símbolos o números
    archivo.imbue(std::locale("C"));
// Recorre la tabla y guarda cada campo delimitado por el símbolo '|'
    tabla.recorrerTabla([&archivo](Nodo* contacto){
        archivo << contacto->nombre << "|"
                << contacto->apellido << "|"
                << contacto->correo << "|"
                << contacto->numeral << "|"
                << contacto->numeroTelefonico << "|\n";
    });

    archivo.close();
}

// Lee el archivo de texto y reconstruye los contactos en la tabla hash en memoria
void GestorArchivos::cargarDatos(TablaHash& tabla){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "tablaHash";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "tablaHash.txt";
    std::ifstream archivo(rutaArchivo,std::ios::in);

    std::string linea;
    // Lee el archivo línea por línea hasta llegar al final
    while (std::getline(archivo,linea)){
         // Ignora líneas en blanco para evitar errores
        if (linea.empty())
            continue;
        std::stringstream ss (linea);
        std::string nombre, apellido, correo, numeral, telefono;

         // Extrae los datos de la línea actual separándolos por '|'
        std::getline(ss,nombre,'|');
        std::getline(ss,apellido,'|');
        std::getline(ss,correo,'|');
        std::getline(ss,numeral,'|');
        std::getline(ss,telefono,'|');
// Si los campos principales no están vacíos, inserta el registro en la tabla hash

        if (!nombre.empty() && !telefono.empty())
            tabla.agregarContacto(nombre,apellido,correo,std::stoi(numeral),std::stol(telefono));
    }


    archivo.close();
}

// Incrementa y guarda en disco la cantidad total de contactos almacenados

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
    
        // Escribe el número entero en el archivo
    archivo<<contadorGuardado;

    archivo.close();
}

// Carga en memoria el contador guardado de contactos
int GestorArchivos::leerContador(){
    int num = 0;
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contadorContactos";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "contador.txt";
    std::ifstream archivo(rutaArchivo,std::ios::in);
    if (!archivo.is_open())
        return 0;// Retorna 0 si el archivo aún no existe

    archivo >> num;
    contactosAlmacenados = num; // Actualiza la variable de la clase

    return num;
}

// Guarda la lista de contactos u operaciones recientes en un archivo de texto
void GestorArchivos::guardarLista(Lista& lista){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contactosRecientes";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "operaciones.txt";
    std::ofstream archivo(rutaArchivo,std::ios::out);
    archivo.imbue(std::locale("C"));

    // Recorre la lista y guarda los nodos con el delimitador '|'
    lista.recorrerLista([&archivo](Nodo2* operacion){
        archivo << operacion->nombre << "|"
                << operacion->apellido << "|"
                << operacion->numeral << "|"
                << operacion->telefono << "|"
                << operacion->accion << "|\n";
    });

    archivo.close();
}

// Lee el archivo de operaciones recientes y reconstruye la lista enlazada

void GestorArchivos::leerLista(Lista& lista){
    fs::path rutaBase(RUTA_DATOS);
    fs::path carpeta = rutaBase / "contactosRecientes";
    if (!fs::exists(carpeta))
        fs::create_directories(carpeta);
    fs::path rutaArchivo = carpeta / "operaciones.txt";
    std::ifstream archivo(rutaArchivo,std::ios::in);

    std::string linea;
        // Procesamiento línea a línea del archivo para extraer los datos de cada operación
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
// Incrementa y registra la cantidad de operaciones del usuario en disco
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
// Lee el contador de operaciones registradas de sesiones anteriores
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