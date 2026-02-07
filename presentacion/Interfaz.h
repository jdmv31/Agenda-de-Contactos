#pragma once
#include <gtkmm.h>
#include <string>

class VistaPrincipal : public Gtk::Window {
public:
    VistaPrincipal();
    virtual ~VistaPrincipal();

protected:
    // --- CONTENEDORES ---
    Gtk::Box m_MainBox;
    Gtk::Box m_TopBar;
    Gtk::Box m_FilterBar;
    Gtk::ScrolledWindow m_ScrollPanel;
    
    // --- ELEMENTOS ---
    Gtk::SearchEntry m_SearchBar;
    
    // CAMBIO: Ahora es un botón normal, sin menú
    Gtk::Button m_BtnGestion; 

    // Botones de filtro
    Gtk::Button m_BtnRecientes;
    Gtk::Button m_BtnFavoritos;
    Gtk::Button m_BtnGuardar;

    // Lista
    Gtk::ListBox m_ListaContactos;

    // --- MÉTODOS ---
    void cargar_estilos_css();
    void abrir_formulario_crear(); // Abre la ventana directamente
};

