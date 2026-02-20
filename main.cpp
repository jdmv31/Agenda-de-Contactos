#include <gtkmm.h>
#include <presentacion/Interfaz.h>
#include <iostream>
#include <string>


TablaHash tabla;
GestorArchivos gestorFicheros;
Lista contactosRecientes;

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("Agenda de Contactos");
    if (gestorFicheros.leerOperaciones() != 0)
        gestorFicheros.leerLista(contactosRecientes);
    if (gestorFicheros.leerContador() != 0)
        gestorFicheros.cargarDatos(tabla);

    return app->make_window_and_run<Interfaz>(argc, argv);
}