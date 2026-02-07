#include "Interfaz.h"
#include <iostream>
#include <glibmm/fileutils.h>

using std::string;

// Constructor de la Ventana Principal
VistaPrincipal::VistaPrincipal()
    : m_MainBox(Gtk::Orientation::VERTICAL),
      m_TopBar(Gtk::Orientation::HORIZONTAL),
      m_FilterBar(Gtk::Orientation::HORIZONTAL),
      // Inicializamos botones con texto claro
      m_BtnGestion("➕ Crear Contacto"), 
      m_BtnRecientes("🕒 Recientes"),
      m_BtnFavoritos("★ Favoritos"),
      m_BtnGuardar("💾 Guardar Todo")
{
    set_title("Agenda de Contactos");
    set_default_size(900, 900); // Tamaño ajustado

    // 1. Cargar el CSS corregido
    cargar_estilos_css();

    // --- BARRA SUPERIOR ---
    m_TopBar.set_margin(15);
    m_TopBar.set_spacing(10);

    m_SearchBar.set_placeholder_text("Buscar contacto...");
    m_SearchBar.set_hexpand(true);

    // CONFIGURACIÓN BOTÓN GESTIÓN (Color Terracota)
    m_BtnGestion.add_css_class("boton-crud"); 
    
    // Conectamos la señal para abrir formulario
    m_BtnGestion.signal_clicked().connect(
        sigc::mem_fun(*this, &VistaPrincipal::abrir_formulario_crear)
    );

    m_TopBar.append(m_SearchBar);
    m_TopBar.append(m_BtnGestion);

    // --- BARRA DE FILTROS ---
    m_FilterBar.set_margin_start(15);
    m_FilterBar.set_margin_end(15);
    m_FilterBar.set_margin_bottom(10);
    m_FilterBar.set_spacing(10);

    // APLICAR CLASES CSS (Colores)
    // Recientes -> Verde Oliva
    m_BtnRecientes.add_css_class("boton-filtro");
    
    // Favoritos -> Mostaza (Usa doble clase: base + especifica)
    m_BtnFavoritos.add_css_class("boton-filtro");
    m_BtnFavoritos.add_css_class("favorito");

    // Guardar -> Verde Oscuro (Kombu)
    m_BtnGuardar.add_css_class("boton-guardar");

    m_FilterBar.append(m_BtnRecientes);
    m_FilterBar.append(m_BtnFavoritos);
    m_FilterBar.append(m_BtnGuardar);

    // --- LISTA DE CONTACTOS ---
    m_ScrollPanel.set_child(m_ListaContactos);
    m_ScrollPanel.set_expand(true);
    m_ScrollPanel.set_margin(15);
    m_ScrollPanel.set_has_frame(true); // Marco visible

    // Contacto de prueba para verificar visualización
    auto fila_prueba = Gtk::make_managed<Gtk::Label>("Maria - 0414-123456");
    fila_prueba->set_xalign(0.0); // Alinear a la izquierda
    fila_prueba->set_margin(10);
    m_ListaContactos.append(*fila_prueba);

    // --- ENSAMBLAJE FINAL ---
    m_MainBox.append(m_TopBar);
    m_MainBox.append(m_FilterBar);
    m_MainBox.append(m_ScrollPanel);

    set_child(m_MainBox);
}

VistaPrincipal::~VistaPrincipal() {}

// --- VENTANA EMERGENTE (FORMULARIO) ---
void VistaPrincipal::abrir_formulario_crear() {
    auto ventana = new Gtk::Window();
    ventana->set_title("Nuevo Contacto");
    ventana->set_transient_for(*this);
    ventana->set_modal(true);
    ventana->set_default_size(400, 500);
    ventana->set_resizable(false);

    auto grid = Gtk::make_managed<Gtk::Grid>();
    grid->set_row_spacing(15);
    grid->set_column_spacing(15);
    grid->set_margin(25);

    // Helper para crear campos rápido
    int fila = 0;
    auto agregar = [&](const string& txt) {
        auto lbl = Gtk::make_managed<Gtk::Label>(txt);
        lbl->set_xalign(0); // Alinear labels a la izquierda
        grid->attach(*lbl, 0, fila);
        
        auto entry = Gtk::make_managed<Gtk::Entry>();
        entry->set_hexpand(true);
        grid->attach(*entry, 1, fila);
        fila++;
        return entry;
    };

    // Campos
    auto eNombre = agregar("Nombre:");
    auto eApellido = agregar("Apellido:");
    auto eTlf = agregar("Teléfono:");
    auto eDir = agregar("Dirección:");
    auto eCumple = agregar("Cumpleaños:");
    auto eGmail = agregar("Gmail:");

    // Botón Guardar del Formulario
    auto btnGuardar = Gtk::make_managed<Gtk::Button>("Guardar Contacto");
    btnGuardar->add_css_class("boton-guardar"); // Verde oscuro
    
    btnGuardar->signal_clicked().connect([=]() {
        std::cout << "Guardando: " << eNombre->get_text() << std::endl;
        ventana->close();
    });

    // Limpieza de memoria al cerrar
    ventana->signal_hide().connect([ventana]() { delete ventana; });

    grid->attach(*btnGuardar, 0, fila, 2, 1);
    
    ventana->set_child(*grid);
    ventana->present();
}

void VistaPrincipal::cargar_estilos_css() {
    auto css = Gtk::CssProvider::create();
    try {
        css->load_from_path("presentacion/style.css");
        Gtk::StyleContext::add_provider_for_display(
            Gdk::Display::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
    } catch(...) {
        std::cerr << "Error cargando CSS. Revisa la ruta." << std::endl;
    }
}