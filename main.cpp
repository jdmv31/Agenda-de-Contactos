#include <gtkmm.h>
#include <presentacion/Interfaz.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("Agenda de Contactos");

    return app->make_window_and_run<Interfaz>(argc, argv);
}