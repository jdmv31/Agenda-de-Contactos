#include "Interfaz.h"
#include <iostream>

using std::string;

// =========================================================
// IMPLEMENTACIÓN VISTA MODIFICAR
// =========================================================
VistaModificar::VistaModificar(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    // FILL para expandir horizontal y verticalmente
    m_CenterBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::FILL);
    m_CenterBox.set_halign(Gtk::Align::FILL);
    numeral = 0;
    numeroTelefonico = 0;

    m_PanelLista.set_orientation(Gtk::Orientation::VERTICAL);
    m_PanelLista.set_spacing(15);
    m_PanelLista.set_margin(20);
    m_PanelLista.set_valign(Gtk::Align::FILL);
    m_PanelLista.set_halign(Gtk::Align::FILL);

    m_LblTituloLista.set_markup("<span size='x-large' weight='bold' foreground='#023e8a'>Modificar Contactos</span>");
    
    // IMPORTANTE: Quitamos size_request y activamos expand
    m_ScrolledWindow.set_vexpand(true);
    m_ScrolledWindow.set_hexpand(true);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_has_frame(true);
    
    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10); 
    m_ListadoBox.set_margin(10);
    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolverMenu.set_label("Volver al Menú");
    m_BtnVolverMenu.add_css_class("favorito"); 
    m_BtnVolverMenu.set_hexpand(true);

    m_PanelLista.append(m_LblTituloLista);
    m_PanelLista.append(m_ScrolledWindow);
    m_PanelLista.append(m_BtnVolverMenu);

    m_PanelEdicion.set_orientation(Gtk::Orientation::VERTICAL);
    m_PanelEdicion.set_spacing(15);
    m_PanelEdicion.set_margin(40);
    m_PanelEdicion.set_valign(Gtk::Align::FILL);
    m_PanelEdicion.set_halign(Gtk::Align::FILL);

    m_LblTituloEdicion.set_markup("<span size='x-large' weight='bold'>Editando Contacto</span>");
    
    m_InfoBar.add_child(m_LblInfoBar);    
    m_InfoBar.set_show_close_button(true);
    m_InfoBar.hide();
    m_InfoBar.signal_response().connect([this](int){ m_InfoBar.hide(); });

    m_GridCampos.set_row_spacing(15);
    m_GridCampos.set_column_spacing(15);
    m_GridCampos.set_hexpand(true);

    auto* lblNom = Gtk::make_managed<Gtk::Label>("Nombre:");
    lblNom->set_halign(Gtk::Align::END);
    auto* lblApe = Gtk::make_managed<Gtk::Label>("Apellido:");
    lblApe->set_halign(Gtk::Align::END);
    auto* lblCor = Gtk::make_managed<Gtk::Label>("Correo:");
    lblCor->set_halign(Gtk::Align::END);

    m_EntryNombre.set_hexpand(true);
    m_EntryApellido.set_hexpand(true);
    m_EntryEmail.set_hexpand(true);

    m_GridCampos.attach(*lblNom, 0, 0);
    m_GridCampos.attach(m_EntryNombre, 1, 0);
    m_GridCampos.attach(*lblApe, 0, 1);
    m_GridCampos.attach(m_EntryApellido, 1, 1);
    m_GridCampos.attach(*lblCor, 0, 2);
    m_GridCampos.attach(m_EntryEmail, 1, 2);

    m_LblTlfFijo.set_halign(Gtk::Align::CENTER);
    m_LblTlfFijo.set_margin_top(10);
    m_LblTlfFijo.set_margin_bottom(10);

    m_BtnGuardar.set_label("Guardar Cambios");
    m_BtnGuardar.add_css_class("boton-guardar");
    m_BtnGuardar.set_hexpand(true);
    
    m_BtnCancelarEdicion.set_label("Cancelar");
    m_BtnCancelarEdicion.add_css_class("favorito"); 
    m_BtnCancelarEdicion.set_hexpand(true);

    m_PanelEdicion.append(m_LblTituloEdicion);
    m_PanelEdicion.append(m_InfoBar);
    m_PanelEdicion.append(m_LblTlfFijo);
    m_PanelEdicion.append(m_GridCampos);
    m_PanelEdicion.append(m_BtnGuardar);
    m_PanelEdicion.append(m_BtnCancelarEdicion);

    m_CenterBox.append(m_PanelLista);
    append(m_CenterBox);

    m_BtnVolverMenu.signal_clicked().connect(sigc::mem_fun(*this, &VistaModificar::on_volver_menu_clicked));
    m_BtnCancelarEdicion.signal_clicked().connect(sigc::mem_fun(*this, &VistaModificar::on_cancelar_edicion_clicked));
    m_BtnGuardar.signal_clicked().connect(sigc::mem_fun(*this, &VistaModificar::on_guardar_edicion_clicked));

    m_notebook.signal_switch_page().connect([this](Gtk::Widget* page, guint page_num) {
        if (page == this) cargar_contactos();
    });
}

void VistaModificar::mostrarExito(std::string mensaje) {
    m_LblInfoBar.set_markup("<span foreground='#2a9d8f' weight='bold'>Éxito: " + mensaje + "</span>");
    m_InfoBar.set_message_type(Gtk::MessageType::INFO);
    m_InfoBar.show();
}

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

    tabla.modificarContacto(m_numeroActual, nNombre, nApellido, nCorreo);
    contactosRecientes.insertarOperacion(nNombre,nApellido,numeral,numeroTelefonico,"Contacto Modificado");
    gestorFicheros.guardarLista(contactosRecientes);
    gestorFicheros.guardarOperaciones();
    gestorFicheros.guardarDatos(tabla);
    m_BtnGuardar.set_sensitive(false);
    m_BtnCancelarEdicion.set_sensitive(false);
    mostrarExito("Modificación exitosa.");

    Glib::signal_timeout().connect([this]() -> bool {
        m_InfoBar.hide(); 
        m_BtnGuardar.set_sensitive(true);
        m_BtnCancelarEdicion.set_sensitive(true);
        m_CenterBox.remove(m_PanelEdicion);
        m_CenterBox.append(m_PanelLista);
        cargar_contactos();
        return false; 
    }, 1500);
}

Gtk::Box* VistaModificar::crear_tarjeta_modificar(std::string nombre, std::string apellido, std::string tlf, std::string correo, long numero_real) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);
    tarjeta->add_css_class("tarjeta-contacto"); 
    tarjeta->set_hexpand(true);
    tarjeta->set_halign(Gtk::Align::FILL);

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
        m_numeroActual = numero_real; 
        m_EntryNombre.set_text(nombre);
        m_EntryApellido.set_text(apellido);
        m_EntryEmail.set_text(correo);
        m_LblTlfFijo.set_markup("<span foreground='#555'>Teléfono asociado: <b>" + tlf + "</b></span>");
        this->numeral = std::stoi(tlf);
        this->numeroTelefonico = numero_real;
        m_CenterBox.remove(m_PanelLista);
        m_CenterBox.append(m_PanelEdicion);
    });

    tarjeta->append(*icono);
    tarjeta->append(*cajaTextos);
    tarjeta->append(*btnEditar);

    return tarjeta;
}

void VistaModificar::cargar_contactos() {
    while (auto* child = m_ListadoBox.get_first_child()) {
        m_ListadoBox.remove(*child);
    }
    tabla.recorrerTabla([this](Nodo* contacto) {
        std::string tlf_completo = "0" + std::to_string(contacto->numeral) + "-" + std::to_string(contacto->numeroTelefonico);
        auto* nueva_tarjeta = crear_tarjeta_modificar(
            contacto->nombre, contacto->apellido, tlf_completo, contacto->correo, contacto->numeroTelefonico
        );
        m_ListadoBox.append(*nueva_tarjeta);
    });
}

// =========================================================
// IMPLEMENTACIÓN VISTA ELIMINAR
// =========================================================
VistaEliminar::VistaEliminar(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    set_valign(Gtk::Align::FILL);
    set_halign(Gtk::Align::FILL);
    set_expand(true);
    contactoEliminado = false;

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(20);
    m_CardBox.set_hexpand(true);
    m_CardBox.set_vexpand(true);

    m_LblTitulo.set_markup("<span size='x-large' weight='bold' foreground='#d62828'>Eliminar Contactos</span>");
    m_LblTitulo.set_margin_bottom(10);

    m_ScrolledWindow.set_vexpand(true);
    m_ScrolledWindow.set_hexpand(true);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_has_frame(true);

    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10); 
    m_ListadoBox.set_margin(10);

    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("favorito"); 
    m_BtnVolver.set_margin_top(10);
    m_BtnVolver.set_hexpand(true);

    m_InfoBar.add_child(m_LblInfoBar);    
    m_InfoBar.set_show_close_button(true);
    m_InfoBar.hide(); 
    m_InfoBar.signal_response().connect([this](int){ m_InfoBar.hide(); });

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_ScrolledWindow);
    m_CardBox.append(m_InfoBar);
    m_CardBox.append(m_BtnVolver);

    append(m_CardBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaEliminar::on_volver_clicked));

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
    tarjeta->set_hexpand(true);
    tarjeta->set_halign(Gtk::Align::FILL);

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

    auto* btnEliminar = Gtk::make_managed<Gtk::Button>();
    btnEliminar->set_icon_name("user-trash-symbolic"); 
    btnEliminar->set_valign(Gtk::Align::CENTER);
    
    btnEliminar->signal_clicked().connect([this, nombre, apellido, tlf, numero]() {
        auto* dialog = new Gtk::MessageDialog(
            *dynamic_cast<Gtk::Window*>(this->get_root()), 
            "Confirmar Eliminación", false, Gtk::MessageType::WARNING, Gtk::ButtonsType::YES_NO, true
        );
        dialog->add_css_class("mi-dialogo");
        dialog->set_secondary_text("¿Está seguro de que desea eliminar a " + nombre + " de forma permanente?");
  
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
                        return false;                 
                    }, 1500);
                }
            }
            delete dialog; 
        });
        dialog->show();
    });
    
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
            contacto->nombre, contacto->apellido, tlf_completo, contacto->correo, contacto->numeroTelefonico
        );
        m_ListadoBox.append(*nueva_tarjeta);
    });
}

// =========================================================
// IMPLEMENTACIÓN VISTA CONSULTA
// =========================================================
VistaConsulta::VistaConsulta(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    set_valign(Gtk::Align::FILL);
    set_halign(Gtk::Align::FILL);
    set_expand(true);

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(20);
    m_CardBox.set_hexpand(true);
    m_CardBox.set_vexpand(true);

    m_LblTitulo.set_markup("<span size='x-large' weight='bold'>Directorio de Contactos</span>");
    m_LblTitulo.set_margin_bottom(10);

    m_ScrolledWindow.set_vexpand(true);
    m_ScrolledWindow.set_hexpand(true);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC); 
    m_ScrolledWindow.set_has_frame(true);

    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10);
    m_ListadoBox.set_margin(10);

    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("favorito"); 
    m_BtnVolver.set_margin_top(10);
    m_BtnVolver.set_hexpand(true);

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

Gtk::Box* VistaConsulta::crear_tarjeta_contacto(string nombre, string apellido, string tlf, string correo) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);
    tarjeta->add_css_class("tarjeta-contacto"); 
    tarjeta->set_hexpand(true);
    tarjeta->set_halign(Gtk::Align::FILL);

    auto* icono = Gtk::make_managed<Gtk::Image>();
    icono->set("presentacion/assets/contacto.png");
    icono->set_pixel_size(60);
    icono->set_valign(Gtk::Align::CENTER);

    auto* cajaTextos = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cajaTextos->set_spacing(3);
    cajaTextos->set_valign(Gtk::Align::CENTER);

    auto* lblNombre = Gtk::make_managed<Gtk::Label>();
    lblNombre->set_markup("<span size='large' weight='bold'>" + nombre + " " + apellido + "</span>");
    lblNombre->set_halign(Gtk::Align::START);

    auto* lblTlf = Gtk::make_managed<Gtk::Label>();
    lblTlf->set_markup("<span foreground='#555555'>📞 " + tlf + "</span>");
    lblTlf->set_halign(Gtk::Align::START);

    auto* lblCorreo = Gtk::make_managed<Gtk::Label>();
    lblCorreo->set_markup("<span foreground='#0066cc'>✉ " + correo + "</span>");
    lblCorreo->set_halign(Gtk::Align::START);

    cajaTextos->append(*lblNombre);
    cajaTextos->append(*lblTlf);
    cajaTextos->append(*lblCorreo);

    tarjeta->append(*icono);
    tarjeta->append(*cajaTextos);

    return tarjeta;
}

void VistaConsulta::cargar_contactos() {
    while (auto* child = m_ListadoBox.get_first_child()) {
        m_ListadoBox.remove(*child);
    }
    tabla.recorrerTabla([this](Nodo* contacto) {
        std::string tlf_completo = "0" + std::to_string(contacto->numeral) + "-" + std::to_string(contacto->numeroTelefonico);
        auto* nueva_tarjeta = crear_tarjeta_contacto(contacto->nombre, contacto->apellido, tlf_completo, contacto->correo);
        m_ListadoBox.append(*nueva_tarjeta);
    });
}

// =========================================================
// IMPLEMENTACIÓN GESTIONAR CONTACTOS
// =========================================================
GestionarContactos::GestionarContactos(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::FILL); 
    m_CenterBox.set_halign(Gtk::Align::FILL);

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_valign(Gtk::Align::CENTER); // Lo centramos verticalmente
    m_CardBox.set_halign(Gtk::Align::FILL);
    m_CardBox.set_margin_start(40);
    m_CardBox.set_margin_end(40);

    m_LblTitulo.set_markup("<span size='large' weight='bold'>Gestión de Contactos</span>");
    m_LblTitulo.add_css_class("titulo-secundario");
    m_LblTitulo.set_margin_bottom(15);

    m_BtnAgregar.set_label("Agregar Contacto");
    m_BtnModificar.set_label("Modificar Contacto");
    m_BtnEliminar.set_label("Eliminar Contacto");
    m_BtnConsultar.set_label("Consultar Contactos");
    m_BtnVolver.set_label("Volver al Menú Principal");
    m_BtnVolver.add_css_class("favorito"); 

    m_BtnAgregar.add_css_class("boton-crud");
    m_BtnModificar.add_css_class("boton-filtro");
    m_BtnEliminar.add_css_class("boton-filtro");
    m_BtnConsultar.add_css_class("boton-filtro");

    m_BtnAgregar.set_hexpand(true);
    m_BtnModificar.set_hexpand(true);
    m_BtnEliminar.set_hexpand(true);
    m_BtnConsultar.set_hexpand(true);
    m_BtnVolver.set_hexpand(true);

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_BtnAgregar);
    m_CardBox.append(m_BtnModificar);
    m_CardBox.append(m_BtnEliminar);
    m_CardBox.append(m_BtnConsultar);
    m_CardBox.append(m_BtnVolver);

    m_CenterBox.append(m_CardBox);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &GestionarContactos::on_volver_clicked));
    m_BtnAgregar.signal_clicked().connect([this]{ m_notebook.set_current_page(2); });
    m_BtnConsultar.signal_clicked().connect([this]{ m_notebook.set_current_page(6); });
    m_BtnEliminar.signal_clicked().connect([this]{ m_notebook.set_current_page(7); });
    m_BtnModificar.signal_clicked().connect([this]{ m_notebook.set_current_page(8); });
}

void GestionarContactos::on_volver_clicked() {
    m_notebook.set_current_page(0); 
}

// =========================================================
// IMPLEMENTACIÓN VISTA GESTIÓN (Agregar)
// =========================================================
VistaGestion::VistaGestion(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) {
    
    set_valign(Gtk::Align::FILL);
    set_halign(Gtk::Align::FILL);
    set_expand(true);

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(20);
    m_CardBox.set_margin_start(40);
    m_CardBox.set_margin_end(40);
    m_CardBox.set_valign(Gtk::Align::CENTER); // Centrado verticalmente el form
    m_CardBox.set_halign(Gtk::Align::FILL);

    m_LblTitulo.set_markup("<span size='x-large' foreground='#023e8a' weight='bold'>Crear Nuevo Contacto</span>");
    m_LblTitulo.set_margin_bottom(10);

    m_LblNombre.set_text("Nombre:");
    m_LblNombre.set_halign(Gtk::Align::END);
    
    m_LblApellido.set_text("Apellido:");
    m_LblApellido.set_halign(Gtk::Align::END);
    
    m_LblTlf.set_text("Teléfono:");
    m_LblTlf.set_halign(Gtk::Align::END);
    
    m_LblEmail.set_text("Correo:");
    m_LblEmail.set_halign(Gtk::Align::END);

    m_EntryPrefijo.set_placeholder_text("0412");
    m_EntryPrefijo.set_max_length(4);
    
    m_EntryTlf.set_placeholder_text("1234567");
    m_EntryTlf.set_hexpand(true);

    m_EntryNombre.set_placeholder_text("Vladimir");
    m_EntryNombre.set_hexpand(true);

    m_EntryApellido.set_placeholder_text("Salvatore");
    m_EntryApellido.set_hexpand(true);

    m_EntryEmail.set_placeholder_text("Vladimir22lol@gmail.com");
    m_EntryEmail.set_hexpand(true);

    auto* boxTlf = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    boxTlf->set_spacing(5);
    boxTlf->append(m_EntryPrefijo);
    boxTlf->append(m_EntryTlf);

    m_InfoBar.add_child(m_LblInfoBar);    
    m_InfoBar.set_show_close_button(true);
    m_InfoBar.hide(); 
    m_InfoBar.signal_response().connect([this](int){ m_InfoBar.hide(); });

    m_GridCampos.set_row_spacing(15);
    m_GridCampos.set_column_spacing(15);
    m_GridCampos.set_hexpand(true);

    m_EntryNombre.set_hexpand(true);
    m_EntryApellido.set_hexpand(true);
    m_EntryEmail.set_hexpand(true);

    m_GridCampos.attach(m_LblNombre, 0, 0);
    m_GridCampos.attach(m_EntryNombre, 1, 0);
    m_GridCampos.attach(m_LblApellido, 0, 1);
    m_GridCampos.attach(m_EntryApellido, 1, 1);
    m_GridCampos.attach(m_LblTlf, 0, 2);
    m_GridCampos.attach(*boxTlf, 1, 2);
    m_GridCampos.attach(m_LblEmail, 0, 3);
    m_GridCampos.attach(m_EntryEmail, 1, 3);

    m_BtnGuardar.set_label("Guardar Contacto");
    m_BtnGuardar.add_css_class("boton-guardar");
    m_BtnGuardar.set_margin_top(10);
    m_BtnGuardar.set_hexpand(true);

    m_BtnVolver.set_label("Cancelar");
    m_BtnVolver.add_css_class ("favorito");
    m_BtnVolver.set_hexpand(true);

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_InfoBar);     
    m_CardBox.append(m_GridCampos);
    m_CardBox.append(m_BtnGuardar);
    m_CardBox.append(m_BtnVolver);

    append(m_CardBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaGestion::on_volver_clicked));
    m_BtnGuardar.signal_clicked().connect(sigc::mem_fun(*this, &VistaGestion::on_guardar_clicked));
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
    if (nombre.empty() || apellido.empty() || numeral.empty() || telefono.empty() || correo.empty()){
        mostrarError("Todos los campos son obligatorios.");
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

    Glib::signal_timeout().connect([this]() -> bool {
        m_InfoBar.hide();                  
        m_BtnGuardar.set_sensitive(true);   
        m_notebook.set_current_page(1);
        return false; 
    }, 2500);
}

VistaBusqueda::VistaBusqueda(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    set_valign(Gtk::Align::FILL);
    set_halign(Gtk::Align::FILL);
    set_expand(true);

    m_CardBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBox.set_spacing(15);
    m_CardBox.set_margin(20);
    m_CardBox.set_hexpand(true);
    m_CardBox.set_vexpand(true);

    m_LblTitulo.set_markup("<span size='x-large' weight='bold' color='#383893'>Buscar en la Agenda</span>");
    m_LblTitulo.set_margin_bottom(10);

    m_BoxBusqueda.set_orientation(Gtk::Orientation::HORIZONTAL);
    m_BoxBusqueda.set_spacing(10);
    
    m_EntryKeyword.set_placeholder_text("Buscar por nombre, teléfono o correo...");
    m_EntryKeyword.set_hexpand(true);

    m_BoxBusqueda.append(m_EntryKeyword);

    m_LblError.set_markup(""); 
    m_ScrolledWindow.set_vexpand(true);
    m_ScrolledWindow.set_hexpand(true);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC); 
    m_ScrolledWindow.set_has_frame(true);

    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10);
    m_ListadoBox.set_margin(10);

    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("favorito"); 
    m_BtnVolver.set_margin_top(10);
    m_BtnVolver.set_hexpand(true);

    m_CardBox.append(m_LblTitulo);
    m_CardBox.append(m_BoxBusqueda);
    m_CardBox.append(m_LblError);
    m_CardBox.append(m_ScrolledWindow);
    m_CardBox.append(m_BtnVolver);

    append(m_CardBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaBusqueda::on_volver_clicked));
    m_EntryKeyword.signal_changed().connect(sigc::mem_fun(*this, &VistaBusqueda::on_buscar_clicked));

    m_notebook.signal_switch_page().connect([this](Gtk::Widget* page, guint page_num) {
        if (page == this) {
            m_EntryKeyword.set_text("");
            cargar_contactos_filtro(""); 
        }
    });
}

Gtk::Box* VistaBusqueda::crear_tarjeta_contacto(string nombre, string apellido, string tlf, string correo) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);
    tarjeta->add_css_class("tarjeta-contacto"); 
    tarjeta->set_hexpand(true);
    tarjeta->set_halign(Gtk::Align::FILL);

    auto* icono = Gtk::make_managed<Gtk::Image>();
    icono->set("presentacion/assets/contacto.png");
    icono->set_pixel_size(60);
    icono->set_valign(Gtk::Align::CENTER);

    auto* cajaTextos = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cajaTextos->set_spacing(3);
    cajaTextos->set_valign(Gtk::Align::CENTER);

    auto* lblNombre = Gtk::make_managed<Gtk::Label>();
    lblNombre->set_markup("<span size='large' weight='bold'>" + nombre + " " + apellido + "</span>");
    lblNombre->set_halign(Gtk::Align::START);

    auto* lblTlf = Gtk::make_managed<Gtk::Label>();
    lblTlf->set_markup("<span foreground='#555555'>📞 " + tlf + "</span>");
    lblTlf->set_halign(Gtk::Align::START);

    auto* lblCorreo = Gtk::make_managed<Gtk::Label>();
    lblCorreo->set_markup("<span foreground='#0066cc'>✉ " + correo + "</span>");
    lblCorreo->set_halign(Gtk::Align::START);

    cajaTextos->append(*lblNombre);
    cajaTextos->append(*lblTlf);
    cajaTextos->append(*lblCorreo);

    tarjeta->append(*icono);
    tarjeta->append(*cajaTextos);

    return tarjeta;
}

void VistaBusqueda::cargar_contactos_filtro(std::string termino) {
    while (auto* child = m_ListadoBox.get_first_child()) {
        m_ListadoBox.remove(*child);
    }
    
    int contador = 0;
    tabla.recorrerTabla([this, termino, &contador](Nodo* contacto) {
        std::string tlf_completo = "0" + std::to_string(contacto->numeral) + "-" + std::to_string(contacto->numeroTelefonico);
        
        bool coincide = false;
        
        if (termino.empty()) {
            coincide = true;
        } else {
            if (contacto->nombre.find(termino) != std::string::npos || 
                contacto->apellido.find(termino) != std::string::npos ||
                tlf_completo.find(termino) != std::string::npos ||
                contacto->correo.find(termino) != std::string::npos) {
                coincide = true;
            }
        }

        if (coincide) {
            auto* nueva_tarjeta = crear_tarjeta_contacto(contacto->nombre, contacto->apellido, tlf_completo, contacto->correo);
            m_ListadoBox.append(*nueva_tarjeta);
            contador++;
        }
    });

    if (contador == 0 && !termino.empty()) {
        m_LblError.set_markup("<span color='#BC6C25'>No se encontraron resultados para la búsqueda.</span>");
    } else {
        m_LblError.set_markup("");
    }
}

void VistaBusqueda::on_buscar_clicked() {
    std::string termino = m_EntryKeyword.get_text();
    cargar_contactos_filtro(termino);
}

void VistaBusqueda::on_volver_clicked() {
    m_LblError.set_markup("");
    m_EntryKeyword.set_text("");
    m_notebook.set_current_page(0); 
}

// =========================================================
// IMPLEMENTACIÓN VISTA RECIENTES
// =========================================================
VistaRecientes::VistaRecientes(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    set_valign(Gtk::Align::FILL);
    set_halign(Gtk::Align::FILL);
    set_expand(true);

    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin(20);
    m_CardBoxResultados.set_vexpand(true);
    m_CardBoxResultados.set_hexpand(true);

    m_LblResTitulo.set_markup("<span size='x-large' weight='bold'>Actividad Reciente</span>");
    m_LblResTitulo.set_margin_bottom(10);

    m_ScrolledWindow.set_vexpand(true);
    m_ScrolledWindow.set_hexpand(true);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_has_frame(true);

    m_ListadoBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_ListadoBox.set_spacing(10);
    m_ListadoBox.set_margin(10);

    m_ScrolledWindow.set_child(m_ListadoBox);

    m_BtnVolver.set_label("Volver al Menú");
    m_BtnVolver.add_css_class("favorito"); 
    m_BtnVolver.set_margin_top(10);
    m_BtnVolver.set_hexpand(true);

    m_CardBoxResultados.append(m_LblResTitulo);
    m_CardBoxResultados.append(m_ScrolledWindow);
    m_CardBoxResultados.append(m_BtnVolver);

    append(m_CardBoxResultados);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaRecientes::on_volver_clicked));

    m_notebook.signal_switch_page().connect([this](Gtk::Widget* page, guint page_num) {
        if (page == this) actualizar_datos();
    });
}

Gtk::Box* VistaRecientes::crear_tarjeta_reciente(string nombre, string apellido, string tlf, string accion) {
    auto* tarjeta = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    tarjeta->set_spacing(15);
    tarjeta->set_margin(10);
    tarjeta->add_css_class("tarjeta-contacto"); 
    tarjeta->set_hexpand(true);
    tarjeta->set_halign(Gtk::Align::FILL);

    auto* icono = Gtk::make_managed<Gtk::Image>();
    icono->set("presentacion/assets/contacto.png");
    icono->set_pixel_size(50);
    icono->set_valign(Gtk::Align::CENTER);

    auto* cajaTextos = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cajaTextos->set_spacing(3);
    cajaTextos->set_valign(Gtk::Align::CENTER);
    cajaTextos->set_hexpand(true);

    std::string colorAccion = "#023e8a"; 
    if (accion == "Contacto Agregado" || accion == "Insertado") colorAccion = "#2a9d8f"; 
    else if (accion == "Contacto Eliminado") colorAccion = "#d62828"; 
    else if (accion == "Contacto Modificado") colorAccion = "#e9c46a"; 

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
        auto* tarjeta = crear_tarjeta_reciente(operacion->nombre, operacion->apellido, tlf_completo, operacion->accion);
        m_ListadoBox.append(*tarjeta);
    });
}

void VistaRecientes::on_volver_clicked() { 
    m_notebook.set_current_page(0); 
}

// =========================================================
// IMPLEMENTACIÓN VISTA EXPORTAR
// =========================================================
VistaExportar::VistaExportar(Gtk::Notebook& notebook) 
    : Gtk::Box(Gtk::Orientation::VERTICAL), m_notebook(notebook) 
{
    m_CenterBox.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBox.set_expand(true);
    m_CenterBox.set_valign(Gtk::Align::FILL);
    m_CenterBox.set_halign(Gtk::Align::FILL);

    m_CardBoxResultados.set_orientation(Gtk::Orientation::VERTICAL);
    m_CardBoxResultados.set_spacing(15);
    m_CardBoxResultados.set_margin_start(40);
    m_CardBoxResultados.set_margin_end(40);
    m_CardBoxResultados.set_valign(Gtk::Align::FILL);
    m_CardBoxResultados.set_halign(Gtk::Align::FILL);

    m_LblTitulo.set_text("Exportar Datos");
    m_LblTitulo.add_css_class("titulo-secundario");

    m_TxtResContenido.set_editable(false);
    m_TxtResContenido.set_monospace(true);
    
    m_ScrollRes.set_child(m_TxtResContenido);
    m_ScrollRes.set_vexpand(true);
    m_ScrollRes.set_hexpand(true);
    m_ScrollRes.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrollRes.set_has_frame(true);

    m_BtnExportar.set_label("Exportar a .CSV");
    m_BtnExportar.add_css_class("boton-guardar"); 
    m_BtnExportar.set_hexpand(true);

    m_BtnVolver.set_label("Volver");
    m_BtnVolver.add_css_class("favorito"); 
    m_BtnVolver.set_hexpand(true);

    m_LblMensaje.set_markup("");

    m_CardBoxResultados.append(m_LblTitulo);
    m_CardBoxResultados.append(m_ScrollRes);
    m_CardBoxResultados.append(m_BtnExportar);
    m_CardBoxResultados.append(m_BtnVolver);
    m_CardBoxResultados.append(m_LblMensaje);
    
    m_CenterBox.append(m_CardBoxResultados);
    append(m_CenterBox);

    m_BtnVolver.signal_clicked().connect(sigc::mem_fun(*this, &VistaExportar::on_volver_clicked));
    m_BtnExportar.signal_clicked().connect(sigc::mem_fun(*this, &VistaExportar::on_exportar_clicked));
}

void VistaExportar::cargar_datos() {
    auto buffer = m_TxtResContenido.get_buffer();
    std::string contenido = "Nombre, Apellido, Correo, Numeral, Telefono\n";
    int contador = 0;

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
        m_LblMensaje.set_markup("<span foreground='red'>No hay contactos para exportar.</span>");
        m_BtnExportar.set_sensitive(false);
    } else {
        buffer->set_text(contenido);
        m_LblMensaje.set_markup("");
        m_BtnExportar.set_sensitive(true);
    }
}

void VistaExportar::on_exportar_clicked() { 
    bool exito = gestorFicheros.exportarCSV(tabla);
    if (exito) m_LblMensaje.set_markup("<span foreground='green'>¡Datos exportados exitosamente a contactos.csv!</span>");
    else m_LblMensaje.set_markup("<span foreground='red'>Error al intentar exportar el archivo.</span>");
}

void VistaExportar::on_volver_clicked() { 
    m_LblMensaje.set_markup("");
    m_TxtResContenido.get_buffer()->set_text(""); 
    m_notebook.set_current_page(0); 
}

// =========================================================
// IMPLEMENTACIÓN INTERFAZ PRINCIPAL (MAIN MENU)
// =========================================================
Interfaz::Interfaz() {

    set_title("Agenda de Contactos");
    set_default_size(690, 1000); 

    auto css_provider = Gtk::CssProvider::create();
    try {
        css_provider->load_from_path("presentacion/style.css"); 
        Gtk::StyleContext::add_provider_for_display(
            Gdk::Display::get_default(), 
            css_provider, 
            GTK_STYLE_PROVIDER_PRIORITY_USER
        );
    } catch(const Glib::Error& ex) {
        std::cerr << "Error CSS: " << ex.what() << std::endl;
    }

    m_PhoneFrame.set_orientation(Gtk::Orientation::VERTICAL);
    m_PhoneFrame.add_css_class("phone-frame");

    m_PhoneScreen.set_orientation(Gtk::Orientation::VERTICAL);
    m_PhoneScreen.add_css_class("phone-screen");
    m_PhoneScreen.set_vexpand(true);
    m_PhoneScreen.set_hexpand(true);

    m_Notebook.set_scrollable(true);
    m_Notebook.set_show_tabs(false); 
    m_Notebook.set_show_border(false);
    m_Notebook.set_vexpand(true);
    m_Notebook.set_hexpand(true);

    // --- MENÚ PRINCIPAL ---
    m_CenterBoxInicio.set_orientation(Gtk::Orientation::VERTICAL);
    m_CenterBoxInicio.set_expand(true);
    
    // AQUÍ ES DONDE SUCEDE LA MAGIA DEL CENTRADO EN LA PANTALLA DE INICIO
    m_CenterBoxInicio.set_valign(Gtk::Align::CENTER); 
    m_CenterBoxInicio.set_halign(Gtk::Align::FILL);

    m_MenuContainer.set_orientation(Gtk::Orientation::VERTICAL);
    m_MenuContainer.set_spacing(12);
    m_MenuContainer.set_valign(Gtk::Align::CENTER);
    m_MenuContainer.set_halign(Gtk::Align::FILL); 
    m_MenuContainer.set_margin_start(40); 
    m_MenuContainer.set_margin_end(40);

    m_titulo_principal.set_text("MENÚ PRINCIPAL");
    m_titulo_principal.set_margin_bottom(20);
    m_titulo_principal.add_css_class("titulo-principal"); // Le asignamos una clase CSS

    m_BtnGestion.set_label("Gestionar Contactos ");
    m_BtnGestion.add_css_class("boton-crud"); 
    m_BtnGestion.set_hexpand(true);

    m_BtnBuscar.set_label("Buscar Contacto");
    m_BtnBuscar.add_css_class("boton-filtro"); 
    m_BtnBuscar.set_hexpand(true);

    m_BtnRecientes.set_label("Visualizar Recientes");
    m_BtnRecientes.add_css_class("boton-filtro"); 
    m_BtnRecientes.set_hexpand(true);

    m_BtnExportar.set_label("Exportar Datos a CSV");
    m_BtnExportar.add_css_class("boton-filtro");
    m_BtnExportar.set_hexpand(true);

    m_BtnSalir.set_label("Salir del Sistema");
    m_BtnSalir.add_css_class("favorito"); 
    m_BtnSalir.set_hexpand(true);

    m_MenuContainer.append(m_titulo_principal);
    m_MenuContainer.append(m_BtnGestion);
    m_MenuContainer.append(m_BtnBuscar);
    m_MenuContainer.append(m_BtnRecientes);
    m_MenuContainer.append(m_BtnExportar);
    m_MenuContainer.append(m_BtnSalir);

    m_CenterBoxInicio.append(m_MenuContainer);
    m_Notebook.append_page(m_CenterBoxInicio, "Inicio");

    auto* subMenuGestion = Gtk::make_managed<GestionarContactos>(m_Notebook);
    m_Notebook.append_page(*subMenuGestion, "Menu-Gestion");

    auto* vistaAgregar = Gtk::make_managed<VistaGestion>(m_Notebook);
    m_Notebook.append_page(*vistaAgregar, "Agregar-Contacto");

    auto* vistaBuscar = Gtk::make_managed<VistaBusqueda>(m_Notebook);
    m_Notebook.append_page(*vistaBuscar, "Buscar");

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

    m_BtnGestion.signal_clicked().connect([this]{ m_Notebook.set_current_page(1); });
    m_BtnBuscar.signal_clicked().connect([this]{ m_Notebook.set_current_page(3); });
    m_BtnRecientes.signal_clicked().connect([this, vistaRecientes]{ 
        vistaRecientes->actualizar_datos(); 
        m_Notebook.set_current_page(4); 
    });
    m_BtnExportar.signal_clicked().connect([this, vistaExportar]{ 
        vistaExportar->cargar_datos(); 
        m_Notebook.set_current_page(5); 
    });
    m_BtnSalir.signal_clicked().connect(sigc::mem_fun(*this, &Interfaz::on_salir_clicked));
    
    m_PhoneScreen.append(m_Notebook); 
    m_PhoneFrame.append(m_PhoneScreen);
    set_child(m_PhoneFrame);
}

Interfaz::~Interfaz() {}

void Interfaz::on_salir_clicked() { 
    set_visible(false); 
}