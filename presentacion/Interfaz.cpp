#include "Interfaz.h"
#include <iostream>

using std::string;


// =========================================================
// IMPLEMENTACIÓN VISTA MODIFICAR
// =========================================================
VistaModificar::VistaModificar(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    m_CenterBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::CENTER);
    m_CenterBox.set_halign(Gtk::Align::CENTER);
    numeral = 0;
    numeroTelefonico = 0;


    // ==========================================
    // CONFIGURACIÓN PANEL 1: LISTA
    // ==========================================
    m_PanelLista.set_orientation(Gtk::Orientation::VERTICAL);
    m_PanelLista.set_spacing(15);
    m_PanelLista.set_margin(20);
    m_PanelLista.add_css_class("card"); 

    m_LblTituloLista.set_markup("<span size='x-large' weight='bold' foreground='#023e8a'>Modificar Contactos</span>");
    
    m_ScrolledWindow.set_size_request(350, 450);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_has_frame(true);
    
    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10); 
    m_ListadoBox.set_margin(10);
    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolverMenu.set_label("Volver al Menú");

    m_PanelLista.append(m_LblTituloLista);
    m_PanelLista.append(m_ScrolledWindow);
    m_PanelLista.append(m_BtnVolverMenu);

    // ==========================================
    // CONFIGURACIÓN PANEL 2: EDICIÓN
    // ==========================================
    m_PanelEdicion.set_orientation(Gtk::Orientation::VERTICAL);
    m_PanelEdicion.set_spacing(15);
    m_PanelEdicion.set_margin(40);
    m_PanelEdicion.add_css_class("card");

    m_LblTituloEdicion.set_markup("<span size='x-large' weight='bold'>Editando Contacto</span>");
    
    // InfoBar para errores
    m_InfoBar.add_child(m_LblInfoBar);    
    m_InfoBar.set_show_close_button(true);
    m_InfoBar.hide();
    m_InfoBar.signal_response().connect([this](int){ m_InfoBar.hide(); });

    // Grid de edición
    m_GridCampos.set_row_spacing(15);
    m_GridCampos.set_column_spacing(15);

    auto* lblNom = Gtk::make_managed<Gtk::Label>("Nombre:");
    lblNom->set_halign(Gtk::Align::END);
    auto* lblApe = Gtk::make_managed<Gtk::Label>("Apellido:");
    lblApe->set_halign(Gtk::Align::END);
    auto* lblCor = Gtk::make_managed<Gtk::Label>("Correo:");
    lblCor->set_halign(Gtk::Align::END);

    m_GridCampos.attach(*lblNom, 0, 0);
    m_GridCampos.attach(m_EntryNombre, 1, 0);
    m_GridCampos.attach(*lblApe, 0, 1);
    m_GridCampos.attach(m_EntryApellido, 1, 1);
    m_GridCampos.attach(*lblCor, 0, 2);
    m_GridCampos.attach(m_EntryEmail, 1, 2);

    // El teléfono no se edita, se muestra como etiqueta fija
    m_LblTlfFijo.set_halign(Gtk::Align::CENTER);
    m_LblTlfFijo.set_margin_top(10);
    m_LblTlfFijo.set_margin_bottom(10);

    m_BtnGuardar.set_label("Guardar Cambios");
    m_BtnGuardar.add_css_class("boton-guardar");
    m_BtnCancelarEdicion.set_label("Cancelar");

    m_PanelEdicion.append(m_LblTituloEdicion);
    m_PanelEdicion.append(m_InfoBar);
    m_PanelEdicion.append(m_LblTlfFijo);
    m_PanelEdicion.append(m_GridCampos);
    m_PanelEdicion.append(m_BtnGuardar);
    m_PanelEdicion.append(m_BtnCancelarEdicion);

    // Por defecto mostramos la lista
    m_CenterBox.append(m_PanelLista);
    append(m_CenterBox);

    // --- SEÑALES ---
    m_BtnVolverMenu.signal_clicked().connect(sigc::mem_fun(*this, &VistaModificar::on_volver_menu_clicked));
    m_BtnCancelarEdicion.signal_clicked().connect(sigc::mem_fun(*this, &VistaModificar::on_cancelar_edicion_clicked));
    m_BtnGuardar.signal_clicked().connect(sigc::mem_fun(*this, &VistaModificar::on_guardar_edicion_clicked));

    // Recarga automática
    m_notebook.signal_switch_page().connect([this](Gtk::Widget* page, guint page_num) {
        if (page == this) cargar_contactos();
    });
}

void VistaModificar::mostrarExito(std::string mensaje) {
    m_LblInfoBar.set_markup("<span foreground='#2a9d8f' weight='bold'>Éxito: " + mensaje + "</span>");
    m_InfoBar.set_message_type(Gtk::MessageType::INFO);
    m_InfoBar.show();
}

// --- MÉTODOS DE NAVEGACIÓN ---
void VistaModificar::on_volver_menu_clicked() {
    m_notebook.set_current_page(1); 
}

void VistaModificar::on_cancelar_edicion_clicked() {
    m_InfoBar.hide();
    m_CenterBox.remove(m_PanelEdicion);
    m_CenterBox.append(m_PanelLista);
}

void VistaModificar::mostrarError(std::string mensaje) {
    m_LblInfoBar.set_markup("<span foreground='red' weight='bold'>Error: " + mensaje + "</span>");
    m_InfoBar.set_message_type(Gtk::MessageType::ERROR);
    m_InfoBar.show();
}

// --- LÓGICA DE GUARDADO ---
void VistaModificar::on_guardar_edicion_clicked() {
    std::string nNombre = m_EntryNombre.get_text();
    std::string nApellido = m_EntryApellido.get_text();
    std::string nCorreo = m_EntryEmail.get_text();

    if (nNombre.empty() || nApellido.empty() || nCorreo.empty()) {
        mostrarError("Todos los campos son obligatorios.");
        return;
    }
    if (nCorreo.find('@') == std::string::npos) {
        mostrarError("El correo es inválido.");
        return;
    }

    // 1. Guardamos los cambios en la Tabla Hash
    tabla.modificarContacto(m_numeroActual, nNombre, nApellido, nCorreo);
    contactosRecientes.insertarOperacion(nNombre,nApellido,numeral,numeroTelefonico,"Contacto Modificado");
    gestorFicheros.guardarLista(contactosRecientes);
    gestorFicheros.guardarOperaciones();
    gestorFicheros.guardarDatos(tabla);
    // 2. Bloqueamos los botones para que el usuario no toque nada mientras espera
    m_BtnGuardar.set_sensitive(false);
    m_BtnCancelarEdicion.set_sensitive(false);

    // 3. Mostramos el mensaje verde
    mostrarExito("Modificación exitosa.");

    // 4. Creamos el temporizador de 1.5 segundos (1500 ms)
    Glib::signal_timeout().connect([this]() -> bool {
        
        // --- LO QUE PASA DESPUÉS DEL SEGUNDO Y MEDIO ---
        m_InfoBar.hide(); // Ocultamos el mensaje
        
        // Reactivamos los botones para la próxima vez que se edite a alguien
        m_BtnGuardar.set_sensitive(true);
        m_BtnCancelarEdicion.set_sensitive(true);
        
        // Cambiamos los paneles visuales
        m_CenterBox.remove(m_PanelEdicion);
        m_CenterBox.append(m_PanelLista);
        
        // Recargamos la lista para que la tarjeta muestre el nuevo nombre/correo
        cargar_contactos();
        
        return false; // Destruimos el temporizador
    }, 1500);
}

// --- FÁBRICA DE TARJETAS ---
Gtk::Box* VistaModificar::crear_tarjeta_modificar(std::string nombre, std::string apellido, std::string tlf, std::string correo, long numero_real) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);
    tarjeta->add_css_class("tarjeta-contacto"); 

    auto* icono = Gtk::make_managed<Gtk::Image>();
    icono->set("presentacion/assets/contacto.png");
    icono->set_pixel_size(50);
    icono->set_valign(Gtk::Align::CENTER);

    auto* cajaTextos = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cajaTextos->set_spacing(3);
    cajaTextos->set_valign(Gtk::Align::CENTER);
    cajaTextos->set_hexpand(true);

    auto* lblNombre = Gtk::make_managed<Gtk::Label>();
    lblNombre->set_markup("<span size='large' weight='bold'>" + nombre + " " + apellido + "</span>");
    lblNombre->set_halign(Gtk::Align::START);

    auto* lblTlf = Gtk::make_managed<Gtk::Label>();
    lblTlf->set_markup("<span foreground='#555555'>📞 " + tlf + "</span>");
    lblTlf->set_halign(Gtk::Align::START);

    cajaTextos->append(*lblNombre);
    cajaTextos->append(*lblTlf);

    auto* btnEditar = Gtk::make_managed<Gtk::Button>();
    btnEditar->set_valign(Gtk::Align::CENTER);

    auto* iconoEditar = Gtk::make_managed<Gtk::Image>();
    iconoEditar->set("presentacion/assets/editar.png");
    iconoEditar->set_pixel_size(24);
    btnEditar->set_child(*iconoEditar);
    btnEditar->signal_clicked().connect([this, nombre, apellido, tlf, correo, numero_real]() {
        m_numeroActual = numero_real; // Guardamos el ID
        
        // Llenamos el formulario con los datos viejos
        m_EntryNombre.set_text(nombre);
        m_EntryApellido.set_text(apellido);
        m_EntryEmail.set_text(correo);
        m_LblTlfFijo.set_markup("<span foreground='#555'>Teléfono asociado: <b>" + tlf + "</b></span>");
        this->numeral = std::stoi(tlf);
        this->numeroTelefonico = numero_real;
        // Cambiamos la vista
        m_CenterBox.remove(m_PanelLista);
        m_CenterBox.append(m_PanelEdicion);
    });

    tarjeta->append(*icono);
    tarjeta->append(*cajaTextos);
    tarjeta->append(*btnEditar);

    return tarjeta;
}

// --- CARGA DE DATOS ---
void VistaModificar::cargar_contactos() {
    while (auto* child = m_ListadoBox.get_first_child()) {
        m_ListadoBox.remove(*child);
    }

    tabla.recorrerTabla([this](Nodo* contacto) {
        std::string tlf_completo = "0" + std::to_string(contacto->numeral) + "-" + std::to_string(contacto->numeroTelefonico);

        auto* nueva_tarjeta = crear_tarjeta_modificar(
            contacto->nombre, 
            contacto->apellido, 
            tlf_completo, 
            contacto->correo,
            contacto->numeroTelefonico
        );

        m_ListadoBox.append(*nueva_tarjeta);
    });
}


// =========================================================
// IMPLEMENTACIÓN VISTA ELIMINAR
// =========================================================
VistaEliminar::VistaEliminar(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    set_valign(Gtk::Align::CENTER);
    set_halign(Gtk::Align::CENTER);
    set_expand(true);
    contactoEliminado = false;

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(20);
    m_CardBox.add_css_class("card"); 

    // Título con un toque de advertencia (color rojizo)
    m_LblTitulo.set_markup("<span size='x-large' weight='bold' foreground='#d62828'>Eliminar Contactos</span>");
    m_LblTitulo.set_margin_bottom(10);

    m_ScrolledWindow.set_size_request(350, 450);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_has_frame(true);

    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10); 
    m_ListadoBox.set_margin(10);

    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.set_margin_top(10);
    m_LblTitulo.set_margin_bottom(10);

    // ---> CONFIGURAR EL INFOBAR AQUÍ <---
    m_InfoBar.add_child(m_LblInfoBar);    
    m_InfoBar.set_show_close_button(true);
    m_InfoBar.hide(); // Oculto por defecto
    m_InfoBar.signal_response().connect([this](int){ m_InfoBar.hide(); });

    m_ScrolledWindow.set_size_request(350, 450);

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_ScrolledWindow);
    m_CardBox.append(m_InfoBar);
    m_CardBox.append(m_BtnVolver);

    append(m_CardBox);

    // Señal de botón volver
    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaEliminar::on_volver_clicked));

    // RECARGA AUTOMÁTICA: Detecta cuando entramos a esta pestaña
    m_notebook.signal_switch_page().connect([this](Gtk::Widget* page, guint page_num) {
        if (page == this) {
            cargar_contactos();
        }
    });
}

void VistaEliminar::mostrarExito(std::string mensaje) {
    m_LblInfoBar.set_markup("<span foreground='#2a9d8f' weight='bold'>Éxito: " + mensaje + "</span>");
    m_InfoBar.set_message_type(Gtk::MessageType::INFO);
    m_InfoBar.show();
}

void VistaEliminar::on_volver_clicked() {
    m_notebook.set_current_page(1);
}

Gtk::Box* VistaEliminar::crear_tarjeta_eliminar(string nombre, string apellido, string tlf, string correo, long numero) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);
    tarjeta->add_css_class("tarjeta-contacto"); 

    auto* icono = Gtk::make_managed<Gtk::Image>();
    icono->set("presentacion/assets/contacto.png");
    icono->set_pixel_size(50);
    icono->set_valign(Gtk::Align::CENTER);

    auto* cajaTextos = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cajaTextos->set_spacing(3);
    cajaTextos->set_valign(Gtk::Align::CENTER);
    cajaTextos->set_hexpand(true); // Empuja el botón a la derecha

    auto* lblNombre = Gtk::make_managed<Gtk::Label>();
    lblNombre->set_markup("<span size='large' weight='bold'>" + nombre + " " + apellido + "</span>");
    lblNombre->set_halign(Gtk::Align::START);

    auto* lblTlf = Gtk::make_managed<Gtk::Label>();
    lblTlf->set_markup("<span foreground='#555555'>📞 " + tlf + "</span>");
    lblTlf->set_halign(Gtk::Align::START);

    cajaTextos->append(*lblNombre);
    cajaTextos->append(*lblTlf);

    auto* btnEliminar = Gtk::make_managed<Gtk::Button>();
    btnEliminar->set_icon_name("user-trash-symbolic"); 
    btnEliminar->set_valign(Gtk::Align::CENTER);
    
    btnEliminar->signal_clicked().connect([this, nombre, apellido, tlf, numero]() {
        auto* dialog = new Gtk::MessageDialog(
            *dynamic_cast<Gtk::Window*>(this->get_root()), 
            "Confirmar Eliminación", 
            false, 
            Gtk::MessageType::WARNING, 
            Gtk::ButtonsType::YES_NO, 
            true
        );
        
        dialog->set_secondary_text("¿Está seguro de que desea eliminar a " + nombre + " de forma permanente?");
  
        // Asegúrate de capturar nombre, apellido y tlf en esta segunda lambda también
        dialog->signal_response().connect([this, dialog, numero, nombre, apellido, tlf](int response_id) {
            if (response_id == Gtk::ResponseType::YES) {
                if (tabla.eliminarContacto(numero)){
                    contactosRecientes.insertarOperacion(nombre, apellido, std::stoi(tlf), numero, "Contacto Eliminado");
                    gestorFicheros.guardarLista(contactosRecientes);
                    gestorFicheros.guardarOperaciones();
                    gestorFicheros.guardarDatos(tabla);
                    mostrarExito("Contacto eliminado exitosamente.");
                    Glib::signal_timeout().connect([this]() -> bool {
                        m_InfoBar.hide();
                        cargar_contactos();           
                        m_notebook.set_current_page(1);
                        return false;                 
                    }, 1500);
                    cargar_contactos();
                }
            }
            delete dialog; 
        });

        dialog->show();
    });

    // ¡ELIMINA las líneas de "if (contactoEliminado)" que estaban aquí abajo!
    
    tarjeta->append(*icono);
    tarjeta->append(*cajaTextos);
    tarjeta->append(*btnEliminar);

    return tarjeta;
}

void VistaEliminar::cargar_contactos() {
    while (auto* child = m_ListadoBox.get_first_child()) {
        m_ListadoBox.remove(*child);
    }

    tabla.recorrerTabla([this](Nodo* contacto) {
        std::string tlf_completo = "0" + std::to_string(contacto->numeral) + "-" + std::to_string(contacto->numeroTelefonico);
        auto* nueva_tarjeta = crear_tarjeta_eliminar(
            contacto->nombre, 
            contacto->apellido, 
            tlf_completo, 
            contacto->correo,
            contacto->numeroTelefonico
        );

        m_ListadoBox.append(*nueva_tarjeta);
    });
}

VistaConsulta::VistaConsulta(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    set_valign(Gtk::Align::CENTER);
    set_halign(Gtk::Align::CENTER);
    set_expand(true);

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(20);
    m_CardBox.add_css_class("card"); 

    m_LblTitulo.set_markup("<span size='x-large' weight='bold'>Directorio de Contactos</span>");
    m_LblTitulo.set_margin_bottom(10);

    m_ScrolledWindow.set_size_request(350, 450);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC); // Scroll vertical automático
    m_ScrolledWindow.set_has_frame(true);

    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10);
    m_ListadoBox.set_margin(10);

    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.set_margin_top(10);

    // Ensamblamos todo
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_ScrolledWindow);
    m_CardBox.append(m_BtnVolver);

    append(m_CardBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaConsulta::on_volver_clicked));

    m_notebook.signal_switch_page().connect([this](Gtk::Widget* page, guint page_num) {
        if (page == this) {
            cargar_contactos();
        }
    });
}

void VistaConsulta::on_volver_clicked() {
    m_notebook.set_current_page(0);
}

// ======================================================================
// FÁBRICA DE TARJETAS DE CONTACTO (El diseño que pediste)
// ======================================================================
Gtk::Box* VistaConsulta::crear_tarjeta_contacto(string nombre, string apellido, string tlf, string correo) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);

    tarjeta->add_css_class("tarjeta-contacto"); 

    auto* icono = Gtk::make_managed<Gtk::Image>();
    icono->set("presentacion/assets/contacto.png");
    icono->set_pixel_size(60);
    icono->set_valign(Gtk::Align::CENTER);

    // 3. La caja vertical para los textos (Derecha)
    auto* cajaTextos = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cajaTextos->set_spacing(3);
    cajaTextos->set_valign(Gtk::Align::CENTER);

    // Nombre y Apellido (Arriba, en negrita)
    auto* lblNombre = Gtk::make_managed<Gtk::Label>();
    lblNombre->set_markup("<span size='large' weight='bold'>" + nombre + " " + apellido + "</span>");
    lblNombre->set_halign(Gtk::Align::START);

    // Teléfono (En medio)
    auto* lblTlf = Gtk::make_managed<Gtk::Label>();
    lblTlf->set_markup("<span foreground='#555555'>📞 " + tlf + "</span>");
    lblTlf->set_halign(Gtk::Align::START);

    // Correo (Debajo)
    auto* lblCorreo = Gtk::make_managed<Gtk::Label>();
    lblCorreo->set_markup("<span foreground='#0066cc'>✉ " + correo + "</span>");
    lblCorreo->set_halign(Gtk::Align::START);

    // Ensamblamos los textos
    cajaTextos->append(*lblNombre);
    cajaTextos->append(*lblTlf);
    cajaTextos->append(*lblCorreo);

    // Ensamblamos la tarjeta completa
    tarjeta->append(*icono);
    tarjeta->append(*cajaTextos);

    return tarjeta;
}

void VistaConsulta::cargar_contactos() {
    // 1. LIMPIEZA: Eliminamos las tarjetas visuales anteriores para no duplicar
    while (auto* child = m_ListadoBox.get_first_child()) {
        m_ListadoBox.remove(*child);
    }

    // 2. RECORRIDO: Pedimos los datos frescos a la tabla
    tabla.recorrerTabla([this](Nodo* contacto) {
        std::string tlf_completo = "0" + std::to_string(contacto->numeral) + "-" + std::to_string(contacto->numeroTelefonico);

        auto* nueva_tarjeta = crear_tarjeta_contacto(
            contacto->nombre, 
            contacto->apellido, 
            tlf_completo, 
            contacto->correo
        );

        m_ListadoBox.append(*nueva_tarjeta);
    });
}

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
    m_BtnConsultar.set_label("Consultar Contactos");
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
    m_BtnConsultar.signal_clicked().connect([this]{ m_notebook.set_current_page(6); });
    m_BtnEliminar.signal_clicked().connect([this]{ m_notebook.set_current_page(7); });
    m_BtnModificar.signal_clicked().connect([this]{ m_notebook.set_current_page(8); });
}

void GestionarContactos::on_volver_clicked() {
    m_notebook.set_current_page(0); 
}

VistaGestion::VistaGestion(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    set_valign(Gtk::Align::CENTER);
    set_halign(Gtk::Align::CENTER);
    set_expand(true);

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(20);
    m_CardBox.set_margin(40);
    m_CardBox.add_css_class("card"); // Por si usas estilos CSS

    m_LblTitulo.set_markup("<span size='x-large' weight='bold'>Crear Nuevo Contacto</span>");
    m_LblTitulo.set_margin_bottom(10);

    // --- CONFIGURACIÓN DE ETIQUETAS ---
    m_LblNombre.set_text("Nombre:");
    m_LblNombre.set_halign(Gtk::Align::END);
    
    m_LblApellido.set_text("Apellido:");
    m_LblApellido.set_halign(Gtk::Align::END);
    
    m_LblTlf.set_text("Teléfono:");
    m_LblTlf.set_halign(Gtk::Align::END);
    
    m_LblEmail.set_text("Correo:");
    m_LblEmail.set_halign(Gtk::Align::END);

    // --- CONFIGURACIÓN DE ENTRADAS ---
    m_EntryPrefijo.set_placeholder_text("0412");
    m_EntryPrefijo.set_max_length(4);
    m_EntryPrefijo.set_size_request(60, -1);

    m_EntryTlf.set_placeholder_text("1234567");
    m_EntryTlf.set_hexpand(true);

    // Contenedor horizontal para Prefijo + Número
    auto* boxTlf = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    boxTlf->set_spacing(5);
    boxTlf->append(m_EntryPrefijo);
    boxTlf->append(m_EntryTlf);

    // --- CONFIGURACIÓN DEL INFOBAR (GTK4 Corrección) ---
    // Usamos get_content_area() en lugar de set_child()
    m_InfoBar.add_child(m_LblInfoBar);    
    m_InfoBar.set_show_close_button(true);
    m_InfoBar.hide(); // Oculta por defecto

    // Conectar señal de cierre
    m_InfoBar.signal_response().connect([this](int){ m_InfoBar.hide(); });

    // --- ORGANIZACIÓN EN EL GRID ---
    m_GridCampos.set_row_spacing(15);
    m_GridCampos.set_column_spacing(15);

    // Fila 0: Nombre
    m_GridCampos.attach(m_LblNombre, 0, 0);
    m_GridCampos.attach(m_EntryNombre, 1, 0);

    // Fila 1: Apellido
    m_GridCampos.attach(m_LblApellido, 0, 1);
    m_GridCampos.attach(m_EntryApellido, 1, 1);

    // Fila 2: Teléfono (Label + Box con prefijo y número)
    m_GridCampos.attach(m_LblTlf, 0, 2);
    m_GridCampos.attach(*boxTlf, 1, 2);

    // Fila 3: Email
    m_GridCampos.attach(m_LblEmail, 0, 3);
    m_GridCampos.attach(m_EntryEmail, 1, 3);

    // --- BOTONES ---
    m_BtnGuardar.set_label("Guardar Contacto");
    m_BtnGuardar.add_css_class("boton-guardar");
    m_BtnGuardar.set_margin_top(10);

    m_BtnVolver.set_label("Cancelar");

    // --- ARMADO FINAL (Orden secuencial correcto) ---
    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_InfoBar);     // <-- InfoBar se coloca justo debajo del título
    m_CardBox.append(m_GridCampos);
    m_CardBox.append(m_BtnGuardar);
    m_CardBox.append(m_BtnVolver);

    append(m_CardBox);

    // --- SEÑALES ---
    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaGestion::on_volver_clicked));
    m_BtnGuardar.signal_clicked().connect(sigc::mem_fun(*this, &VistaGestion::on_guardar_clicked));
    // Reactivar el botón y ocultar el mensaje al empezar a escribir de nuevo
    m_EntryNombre.signal_changed().connect([this]() {
        if (!m_BtnGuardar.get_sensitive()) {
            m_BtnGuardar.set_sensitive(true);
            m_InfoBar.hide();
        }
    });
}

void VistaGestion::on_volver_clicked() { 
    m_LblError.set_text(""); 
    m_notebook.set_current_page(1); 
}

void VistaGestion::mostrarExito(string mensaje) {
    m_LblInfoBar.set_markup("<span foreground='#2a9d8f' weight='bold'>Éxito: " + mensaje + "</span>");
    m_InfoBar.set_message_type(Gtk::MessageType::INFO);
    m_InfoBar.show();
}

void VistaGestion::limpiarCampos() {
    m_EntryNombre.set_text("");
    m_EntryApellido.set_text("");
    m_EntryPrefijo.set_text("");
    m_EntryTlf.set_text("");
    m_EntryEmail.set_text("");
}

void VistaGestion::mostrarError(string mensaje) {
    m_LblInfoBar.set_markup("<span foreground='red' weight='bold'>Error: " + mensaje + "</span>");
    m_InfoBar.set_message_type(Gtk::MessageType::ERROR);
    m_InfoBar.show();
}

void VistaGestion::on_guardar_clicked() {
    std::string nombre = m_EntryNombre.get_text();
    std::string apellido = m_EntryApellido.get_text();
    std::string numeral = m_EntryPrefijo.get_text();
    std::string telefono = m_EntryTlf.get_text();
    std::string correo = m_EntryEmail.get_text();
    if (nombre.empty()){
        mostrarError("El nombre es obligatorio.");
        return;
    }
    if (apellido.empty()){
        mostrarError("El apellido es obligatorio.");
        return;
    }
    if(numeral.empty()){
        mostrarError("El numeral es obligatorio.");
        return;
    }
    if (telefono.empty()){
        mostrarError("El numero telefonico es obligatorio.");
        return;
    }
    if (telefono.length() != 7){
        mostrarError("El numero telefonico debe contener 7 numeros.");
        return;
    }
    if (correo.find('@') == std::string::npos){
        mostrarError("El correo es invalido, le falta el [@]");
        return;
    }
    if (correo.empty()){
        mostrarError("El correo es obligatorio.");
        return;
    }
    m_InfoBar.hide();
    tabla.agregarContacto(nombre, apellido, correo, std::stoi(numeral), std::stol(telefono));
    gestorFicheros.guardarDatos(tabla);
    gestorFicheros.guardarContador();
    m_BtnGuardar.set_sensitive(false);
    limpiarCampos();
    mostrarExito("Contacto guardado exitosamente.");
    contactosRecientes.insertarOperacion(nombre,apellido,std::stoi(numeral),std::stol(telefono),"Contacto Agregado");
    gestorFicheros.guardarLista(contactosRecientes);
    gestorFicheros.guardarOperaciones();
    gestorFicheros.guardarDatos(tabla);

    Glib::signal_timeout().connect([this]() -> bool {
        m_InfoBar.hide();                  
        m_BtnGuardar.set_sensitive(true);   
        m_notebook.set_current_page(1);
        return false; 
    }, 2500);
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

VistaRecientes::VistaRecientes(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    set_valign(Gtk::Align::CENTER);
    set_halign(Gtk::Align::CENTER);
    set_expand(true);

    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin(20);
    m_CardBoxResultados.add_css_class("card"); 

    m_LblResTitulo.set_markup("<span size='x-large' weight='bold'>Actividad Reciente</span>");
    m_LblResTitulo.set_margin_bottom(10);

    // Configuración del contenedor de la lista con Scroll
    m_ScrolledWindow.set_size_request(350, 450);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_has_frame(true);

    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10);
    m_ListadoBox.set_margin(10);

    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.set_margin_top(10);

    // Ensamblaje
    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_ScrolledWindow);
    m_CardBoxResultados.append(m_BtnVolver);

    append(m_CardBoxResultados);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecientes::on_volver_clicked));

    // RECARGA AUTOMÁTICA: Recargar tarjetas al entrar en la pestaña
    m_notebook.signal_switch_page().connect([this](Gtk::Widget* page, guint page_num) {
        if (page == this) {
            actualizar_datos();
        }
    });
}

Gtk::Box* VistaRecientes::crear_tarjeta_reciente(string nombre, string apellido, string tlf, string accion) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);
    tarjeta->add_css_class("tarjeta-contacto"); 

    auto* icono = Gtk::make_managed<Gtk::Image>();
    icono->set("presentacion/assets/contacto.png");
    icono->set_pixel_size(50);
    icono->set_valign(Gtk::Align::CENTER);

    auto* cajaTextos = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cajaTextos->set_spacing(3);
    cajaTextos->set_valign(Gtk::Align::CENTER);
    cajaTextos->set_hexpand(true);

    // Color condicional basado en la acción
    std::string colorAccion = "#023e8a"; // Azul oscuro genérico
    if (accion == "Contacto Agregado" || accion == "Insertado") colorAccion = "#2a9d8f"; // Verde
    else if (accion == "Contacto Eliminado") colorAccion = "#d62828"; // Rojo
    else if (accion == "Contacto Modificado") colorAccion = "#e9c46a"; // Amarillo oscuro

    auto* lblAccion = Gtk::make_managed<Gtk::Label>();
    lblAccion->set_markup("<span foreground='" + colorAccion + "' weight='bold'>" + accion + "</span>");
    lblAccion->set_halign(Gtk::Align::START);

    auto* lblNombre = Gtk::make_managed<Gtk::Label>();
    lblNombre->set_markup("<span size='large' weight='bold'>" + nombre + " " + apellido + "</span>");
    lblNombre->set_halign(Gtk::Align::START);

    auto* lblTlf = Gtk::make_managed<Gtk::Label>();
    lblTlf->set_markup("<span foreground='#555555'>📞 " + tlf + "</span>");
    lblTlf->set_halign(Gtk::Align::START);

    cajaTextos->append(*lblAccion);
    cajaTextos->append(*lblNombre);
    cajaTextos->append(*lblTlf);

    tarjeta->append(*icono);
    tarjeta->append(*cajaTextos);

    return tarjeta;
}

void VistaRecientes::actualizar_datos() {
    while (auto* child = m_ListadoBox.get_first_child()) {
        m_ListadoBox.remove(*child);
    }

    contactosRecientes.recorrerLista([this](Nodo2* operacion) {
        std::string tlf_completo = "0" + std::to_string(operacion->numeral) + "-" + std::to_string(operacion->telefono);
        
        auto* tarjeta = crear_tarjeta_reciente(
            operacion->nombre, 
            operacion->apellido, 
            tlf_completo, 
            operacion->accion
        );
        m_ListadoBox.append(*tarjeta);
    });
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
    
    std::string contenido = "Nombre, Apellido, Correo, Numeral, Telefono\n";
    int contador = 0;

    // Recorremos la tabla y por cada nodo agregamos sus datos al string 'contenido'
    tabla.recorrerTabla([&contenido, &contador](Nodo* contacto) {
        contenido += contacto->nombre + ", " + 
                     contacto->apellido + ", " + 
                     contacto->correo + ", " + 
                     std::to_string(contacto->numeral) + ", " + 
                     std::to_string(contacto->numeroTelefonico) + "\n";
        contador++;
    });

    if (contador == 0) {
        buffer->set_text("");
        m_LblMensaje.set_markup("<span foreground='red'>No hay contactos en la tabla hash para exportar.</span>");
        m_BtnExportar.set_sensitive(false);
    } else {
        buffer->set_text(contenido);
        m_LblMensaje.set_markup("");
        m_BtnExportar.set_sensitive(true);
    }
}

void VistaExportar::on_exportar_clicked() { 
    bool exito = gestorFicheros.exportarCSV(tabla);
    
    if (exito) {
        m_LblMensaje.set_markup("<span foreground='green'>¡Datos exportados exitosamente a contactos.csv!</span>");
    } else {
        m_LblMensaje.set_markup("<span foreground='red'>Error al intentar exportar el archivo.</span>");
    }
}

void VistaExportar::on_volver_clicked() { 
    m_LblMensaje.set_markup("");
    m_TxtResContenido.get_buffer()->set_text(""); // Limpia la vista al salir
    m_notebook.set_current_page(0); 
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

    auto* vistaExportar = Gtk::make_managed<VistaExportar>(m_Notebook);
    m_Notebook.append_page(*vistaExportar, "Exportar");

    auto* vistaConsulta = Gtk::make_managed<VistaConsulta>(m_Notebook);
    m_Notebook.append_page(*vistaConsulta, "Consultar");

    auto* vistaEliminar = Gtk::make_managed<VistaEliminar>(m_Notebook);
    m_Notebook.append_page(*vistaEliminar, "Eliminar");

    auto* vistaModificar = Gtk::make_managed<VistaModificar>(m_Notebook);
    m_Notebook.append_page(*vistaModificar, "Modificar");

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