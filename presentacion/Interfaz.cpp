#include "Interfaz.h"
#include <iostream>

using std::string;

GestionarContactos::GestionarContactos(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    m_CenterBox.set_expand(true);
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(10);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_markup("<span size='large' weight='bold'>Gestión de Contactos</span>");
    m_LblTitulo.set_margin_bottom(15);

    m_BtnAgregar.set_label("Agregar Contacto");
    m_BtnModificar.set_label("Modificar Contacto");
    m_BtnEliminar.set_label("Eliminar Contacto");
    m_BtnConsultar.set_label("Consultar Contacto");
    m_BtnVolver.set_label("Volver al Menú Principal");

    // Estilos (Usando tus clases CSS)
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::CENTER); // Centrado Vertical
    m_CenterBox.set_halign(Gtk::Align::CENTER); // Centrado Horizontal

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_BtnAgregar.add_css_class("boton-crud");
    m_BtnModificar.add_css_class("boton-filtro");
    m_BtnEliminar.add_css_class("boton-filtro");
    m_BtnConsultar.add_css_class("boton-filtro");

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_BtnAgregar);
    m_CardBox.append(m_BtnModificar);
    m_CardBox.append(m_BtnEliminar);
    m_CardBox.append(m_BtnConsultar);
    m_CardBox.append(m_BtnVolver);

    m_CenterBox.append(m_CardBox);
    append(m_CenterBox);

    // Eventos de Navegación
    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &GestionarContactos::on_volver_clicked));
    
    // Al darle a agregar, vamos a la página de VistaGestion (Página 2)
    m_BtnAgregar.signal_clicked().connect([this]{ m_notebook.set_current_page(2); });
}

void GestionarContactos::on_volver_clicked() { m_notebook.set_current_page(0); }


// --- IMPLEMENTACIÓN VISTA AGREGAR (Sin dirección) ---
VistaGestion::VistaGestion(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    // 1. CENTRADO TOTAL
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::CENTER);
    m_CenterBox.set_halign(Gtk::Align::CENTER);

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);

    m_LblTitulo.set_text("Crear Nuevo Contacto");

    // 2. CONFIGURACIÓN DEL DROPDOWN (PREFIJOS)
    // Usamos un modelo de strings explícito
    auto lista_prefijos = Gtk::StringList::create({"0412", "0416", "0414", "0422", "0424", "0426"});
    m_ComboPrefijo.set_model(lista_prefijos);
    m_ComboPrefijo.set_selected(0);
    
    // IMPORTANTE: Esto suele evitar el error de 'width' no inicializado en Windows
    m_ComboPrefijo.set_size_request(100, -1); 

    // 3. CONTENEDOR DEL TELÉFONO
    auto* boxTlfHorizontal = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    boxTlfHorizontal->set_spacing(8);
    boxTlfHorizontal->append(m_ComboPrefijo);
    boxTlfHorizontal->append(m_EntryTlf);
    m_EntryTlf.set_hexpand(true);
    m_EntryTlf.set_placeholder_text("Número (Ej: 1234567)");

    // 4. CONFIGURACIÓN DE LOS DEMÁS CAMPOS
    m_EntryNombre.set_placeholder_text("Nombre");
    m_EntryApellido.set_placeholder_text("Apellido");
    m_EntryEmail.set_placeholder_text("Correo Electrónico");

    // 5. GRID (Asegúrate de no repetir m_EntryTlf aquí abajo)
    m_GridCampos.set_row_spacing(12);
    m_GridCampos.set_column_spacing(12);
    
    m_GridCampos.attach(m_EntryNombre, 0, 0);
    m_GridCampos.attach(m_EntryApellido, 1, 0);
    m_GridCampos.attach(*boxTlfHorizontal, 0, 1, 2, 1); // Adjuntamos el Box, NO el entry solo
    m_GridCampos.attach(m_EntryEmail, 0, 2, 2, 1);

    // 6. BOTONES
    m_BtnGuardar.set_label("Guardar Contacto");
    m_BtnGuardar.add_css_class("boton-guardar");
    m_BtnVolver.set_label("Cancelar");

    // 7. ARMAR INTERFAZ
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_GridCampos);
    m_CardBox.append(m_LblError);
    m_CardBox.append(m_BtnGuardar);
    m_CardBox.append(m_BtnVolver);

    m_CenterBox.append(m_CardBox);
    append(m_CenterBox);

    // 8. SEÑALES
    m_BtnVolver.signal_clicked().connect([this]{ 
        m_LblError.set_markup("");
        m_notebook.set_current_page(1); 
    });
    
    m_BtnGuardar.signal_clicked().connect(sigc::mem_fun(*this, &VistaGestion::on_guardar_clicked));
}
// ... (Implementaciones de on_guardar_clicked y on_volver_clicked se mantienen)

void VistaGestion::on_volver_clicked() { 
    m_LblError.set_markup(""); 
    m_notebook.set_current_page(0); 
}

void VistaGestion::on_guardar_clicked() {
    std::string nombre = m_EntryNombre.get_text();
    std::string tlf = m_EntryTlf.get_text();

    if (nombre.empty() || tlf.empty()) {
        m_LblError.set_markup("<span color='#BC6C25' weight='bold'>ERROR: Nombre y Teléfono son obligatorios.</span>");
        return;
    }

    std::cout << "Guardando: " << nombre << " | Tlf: " << tlf << std::endl;
    m_LblError.set_markup("<span color='#283618' weight='bold'>¡Contacto Guardado Exitosamente!</span>");
    
    m_EntryNombre.set_text("");
    m_EntryApellido.set_text("");
    m_EntryTlf.set_text("");
    m_EntryEmail.set_text("");

    Glib::signal_timeout().connect_once([this]() {        
        m_LblError.set_markup("");
        m_notebook.set_current_page(0);
    }, 1500);
}

// =========================================================
// 2. IMPLEMENTACIÓN VISTA BÚSQUEDA
// =========================================================
VistaBusqueda::VistaBusqueda(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // --- Configuración de m_CenterBox ---
    m_CenterBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::FILL);

    // --- Panel Input ---
    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(40);
    m_CardBox.set_valign(Gtk::Align::CENTER);
    m_CardBox.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Buscar en la Agenda");
    m_LblTitulo.set_margin_bottom(10);

    m_LblInstruccion.set_markup("Ingrese Nombre o Teléfono:");
    
    m_EntryKeyword.set_placeholder_text("Ej: Maria");
    m_EntryKeyword.set_hexpand(true);

    m_LblError.set_markup(""); 
    
    m_BtnBuscar.set_label("Buscar");
    m_BtnBuscar.add_css_class("boton-crud"); 

    m_BtnVolver.set_label("Volver");

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_LblInstruccion);
    m_CardBox.append(m_EntryKeyword); 
    m_CardBox.append(m_BtnBuscar);
    m_CardBox.append(m_BtnVolver);
    m_CardBox.append(m_LblError);    

    // --- Panel Resultados ---
    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin(40);
    m_CardBoxResultados.set_valign(Gtk::Align::CENTER);
    m_CardBoxResultados.set_halign(Gtk::Align::CENTER);

    m_LblResTitulo.set_text("Resultados de la Búsqueda");
    
    m_TxtResContenido.set_editable(false);
    m_TxtResContenido.set_wrap_mode(Gtk::WrapMode::WORD);

    m_ScrollRes.set_child(m_TxtResContenido);
    m_ScrollRes.set_size_request(400, 250);
    m_ScrollRes.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrollRes.set_has_frame(true);

    m_BtnNuevaBusqueda.set_label("Nueva Búsqueda");
    m_BtnNuevaBusqueda.add_css_class("boton-filtro");

    m_BtnIrMenu.set_label("Volver al Menú");

    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_ScrollRes);
    m_CardBoxResultados.append(m_BtnNuevaBusqueda);
    m_CardBoxResultados.append(m_BtnIrMenu);

    // CORRECCIÓN: Usar append. Inicialmente mostramos el input
    m_CenterBox.append(m_CardBox);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_volver_clicked));
    m_BtnBuscar.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_buscar_clicked));
    m_BtnNuevaBusqueda.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_nueva_busqueda_clicked));
    m_BtnIrMenu.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_volver_clicked));
}

void VistaBusqueda::on_volver_clicked() { 
    m_LblError.set_markup("");
    m_notebook.set_current_page(0); 
}

void VistaBusqueda::on_nueva_busqueda_clicked() {
    m_LblError.set_markup("");
    m_EntryKeyword.set_text(""); 
    
    // Limpiamos m_CenterBox y ponemos el panel de input
    m_CenterBox.remove(m_CardBoxResultados); 
    m_CenterBox.append(m_CardBox); 
}

void VistaBusqueda::on_buscar_clicked() {
    std::string termino = m_EntryKeyword.get_text();
    
    if (termino.empty()) {
        m_LblError.set_markup("<span color='#BC6C25'>Ingrese un dato para buscar.</span>");
        return;
    }

    auto buffer = m_TxtResContenido.get_buffer();
    buffer->set_text("Resultados simulados para: " + termino + "\n\n1. Maria Perez [0414-123456]");
    
    // Cambiamos a la vista de resultados
    m_CenterBox.remove(m_CardBox);
    m_CenterBox.append(m_CardBoxResultados);
}

// =========================================================
// 3. IMPLEMENTACIÓN VISTA RECIENTES
// =========================================================
VistaRecientes::VistaRecientes(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // Configuración m_CenterBox
    m_CenterBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::FILL);

    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(10);
    m_CardBoxResultados.set_margin(40);
    m_CardBoxResultados.set_valign(Gtk::Align::CENTER);
    m_CardBoxResultados.set_halign(Gtk::Align::CENTER);

    m_LblResTitulo.set_text("Contactos Recientes");
    m_LblResTitulo.set_margin_bottom(15);

    m_LblTotal.set_markup("<b>Total Registrados:</b> 4 Contactos");
    m_LblReciente1.set_text("1. Juan Perez (Hoy 10:00 AM)");
    m_LblReciente2.set_text("2. Ana Gomez (Ayer)");
    m_LblReciente3.set_text("3. Carlos Ruiz (Hace 2 días)");

    m_BtnVolver.set_label("Volver al Menú");

    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_LblTotal);
    
    auto separador = Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::HORIZONTAL);
    m_CardBoxResultados.append(*separador);
    
    m_CardBoxResultados.append(m_LblReciente1);
    m_CardBoxResultados.append(m_LblReciente2);
    m_CardBoxResultados.append(m_LblReciente3);
    m_CardBoxResultados.append(m_BtnVolver);

    // CORRECCIÓN: append
    m_CenterBox.append(m_CardBoxResultados);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecientes::on_volver_clicked));
}

void VistaRecientes::actualizar_datos() {
    // Backend
}

void VistaRecientes::on_volver_clicked() { 
    m_notebook.set_current_page(0); 
}

// =========================================================
// 4. IMPLEMENTACIÓN VISTA EXPORTAR
// =========================================================
VistaExportar::VistaExportar(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    // Configuración m_CenterBox
    m_CenterBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::FILL);

    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin(40);
    m_CardBoxResultados.set_valign(Gtk::Align::CENTER);
    m_CardBoxResultados.set_halign(Gtk::Align::CENTER);

    m_LblTitulo.set_text("Exportar Datos");

    m_TxtResContenido.set_editable(false);
    m_TxtResContenido.set_monospace(true);
    
    m_ScrollRes.set_child(m_TxtResContenido);
    m_ScrollRes.set_size_request(500, 300);
    m_ScrollRes.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrollRes.set_has_frame(true);

    m_BtnExportar.set_label("Exportar a .CSV");
    m_BtnExportar.add_css_class("boton-guardar"); 

    m_BtnVolver.set_label("Volver");

    m_LblMensaje.set_markup("");

    m_CardBoxResultados.append(m_LblTitulo);
    m_CardBoxResultados.append(m_ScrollRes);
    m_CardBoxResultados.append(m_BtnExportar);
    m_CardBoxResultados.append(m_LblMensaje);
    m_CardBoxResultados.append(m_BtnVolver);

    // CORRECCIÓN: append
    m_CenterBox.append(m_CardBoxResultados);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaExportar::on_volver_clicked));
    m_BtnExportar.signal_clicked().connect(sigc::mem_fun(*this, &VistaExportar::on_exportar_clicked));
}

void VistaExportar::cargar_datos() {
    auto buffer = m_TxtResContenido.get_buffer();
    buffer->set_text("ID, Nombre, Telefono, Email\n1, Juan, 0414111, j@j.com\n2, Pedro, 0424222, p@p.com\n");
}

void VistaExportar::on_volver_clicked() { 
    m_LblMensaje.set_markup("");
    m_notebook.set_current_page(0); 
}

void VistaExportar::on_exportar_clicked() {
    m_LblMensaje.set_markup("<span color='#283618' weight='bold'>Archivo generado correctamente.</span>");
}

// =========================================================
// IMPLEMENTACIÓN INTERFAZ PRINCIPAL
// =========================================================
Interfaz::Interfaz() {
    set_title("Agenda de Contactos");
    set_default_size(800, 600);

    // 1. CARGA DEL CSS EXTERNO
    auto css_provider = Gtk::CssProvider::create();
    try {
        css_provider->load_from_path("presentacion/style.css"); // Asegúrate de que la ruta sea correcta
        
        Gtk::StyleContext::add_provider_for_display(
            Gdk::Display::get_default(), 
            css_provider, 
            GTK_STYLE_PROVIDER_PRIORITY_USER
        );
    } catch(const Glib::Error& ex) {
        // CORRECCIÓN: Capturar Glib::Error en lugar de CssProviderError
        std::cerr << "Error CSS: " << ex.what() << std::endl;
    }

    m_Notebook.set_scrollable(true);
    m_Notebook.set_show_tabs(false); 
    m_Notebook.set_show_border(false);

    // --- MENÚ PRINCIPAL ---
    // Configuración de m_CenterBoxInicio (Contenedor para centrar el menú)
    m_CenterBoxInicio.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBoxInicio.set_expand(true);
    m_CenterBoxInicio.set_valign(Gtk::Align::FILL);

    m_MenuContainer.set_orientation(Gtk::Orientation::VERTICAL);
    m_MenuContainer.set_spacing(12);
    m_MenuContainer.set_margin(50);
    m_MenuContainer.set_valign(Gtk::Align::CENTER);
    m_MenuContainer.set_halign(Gtk::Align::CENTER);

    m_titulo_principal.set_text("MENÚ PRINCIPAL");
    m_titulo_principal.set_margin_bottom(20);
    m_titulo_principal.set_markup("<span size='xx-large'>MENÚ PRINCIPAL</span>");

    m_BtnGestion.set_label("Gestionar Contactos");
    m_BtnGestion.add_css_class("boton-crud"); 

    m_BtnBuscar.set_label("Buscar Contacto");
    m_BtnBuscar.add_css_class("boton-filtro"); 

    m_BtnRecientes.set_label("Visualizar Recientes");
    m_BtnRecientes.add_css_class("boton-filtro"); 
    m_BtnRecientes.add_css_class("favorito"); 

    m_BtnExportar.set_label("Exportar Datos a CSV");
    m_BtnExportar.add_css_class("boton-filtro");

    m_BtnSalir.set_label("Salir del Sistema");

    m_MenuContainer.append(m_titulo_principal);
    m_MenuContainer.append(m_BtnGestion);
    m_MenuContainer.append(m_BtnBuscar);
    m_MenuContainer.append(m_BtnRecientes);
    m_MenuContainer.append(m_BtnExportar);
    m_MenuContainer.append(m_BtnSalir);

    // CORRECCIÓN: append
    m_CenterBoxInicio.append(m_MenuContainer);
    m_Notebook.append_page(m_CenterBoxInicio, "Inicio");

    // PÁGINA 1: Submenú de Gestión (Las 4 opciones: Agregar, Modificar, etc.)
    auto* subMenuGestion = Gtk::make_managed<GestionarContactos>(m_Notebook);
    m_Notebook.append_page(*subMenuGestion, "Menu-Gestion");

    // PÁGINA 2: Formulario de Agregar Contacto
    auto* vistaAgregar = Gtk::make_managed<VistaGestion>(m_Notebook);
    m_Notebook.append_page(*vistaAgregar, "Agregar-Contacto");

    // PÁGINA 3: Vista de Búsqueda
    auto* vistaBuscar = Gtk::make_managed<VistaBusqueda>(m_Notebook);
    m_Notebook.append_page(*vistaBuscar, "Buscar");

    // PÁGINA 4: Vista de Recientes
    auto* vistaRecientes = Gtk::make_managed<VistaRecientes>(m_Notebook);
    m_Notebook.append_page(*vistaRecientes, "Recientes");

    // PÁGINA 5: Vista de Exportar
    auto* vistaExportar = Gtk::make_managed<VistaExportar>(m_Notebook);
    m_Notebook.append_page(*vistaExportar, "Exportar");

    // 3. CONEXIONES DE BOTONES DEL MENÚ PRINCIPAL (Página 0)
    m_BtnGestion.signal_clicked().connect([this]{ 
        m_Notebook.set_current_page(1); // Va al submenú de gestión
    });

    m_BtnBuscar.signal_clicked().connect([this]{ 
        m_Notebook.set_current_page(3); 
    });

    m_BtnRecientes.signal_clicked().connect([this, vistaRecientes]{ 
        vistaRecientes->actualizar_datos(); 
        m_Notebook.set_current_page(4); 
    });

    m_BtnExportar.signal_clicked().connect([this, vistaExportar]{ 
        vistaExportar->cargar_datos(); 
        m_Notebook.set_current_page(5); 
    });
    m_BtnSalir.signal_clicked().connect(sigc::mem_fun(*this, &Interfaz::on_salir_clicked));

    set_child(m_Notebook);
}

Interfaz::~Interfaz() {}

void Interfaz::on_salir_clicked() { 
    set_visible(false); 
}