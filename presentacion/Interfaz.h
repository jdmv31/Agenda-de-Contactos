#pragma once
#include <gtkmm.h>
#include <string>

#pragma once
#include <gtkmm.h>
#include <string>

class GestionarContactos : public Gtk::Box {
public: 
    GestionarContactos(Gtk::Notebook& notebook);
protected:
    Gtk::Notebook& m_notebook;
    Gtk::Box m_CenterBox;
    Gtk::Box m_CardBox;
    Gtk::Label m_LblTitulo;
    
    Gtk::Button m_BtnAgregar;
    Gtk::Button m_BtnModificar;
    Gtk::Button m_BtnEliminar;
    Gtk::Button m_BtnConsultar;
    Gtk::Button m_BtnVolver;

    void on_volver_clicked();
};

// 2. EL FORMULARIO (Agregar Contacto)
class VistaGestion : public Gtk::Box {
public:
    VistaGestion(Gtk::Notebook& notebook);
protected:
    Gtk::Notebook& m_notebook;
    Gtk::Box m_CenterBox;
    Gtk::Box m_CardBox;
    Gtk::Label m_LblTitulo;
    Gtk::Label m_LblError;

    Gtk::Grid m_GridCampos;
    Gtk::Entry m_EntryNombre;
    Gtk::Entry m_EntryApellido;
    Gtk::Entry m_EntryTlf;
    Gtk::Entry m_EntryEmail;

    Gtk::Button m_BtnGuardar;
    Gtk::Button m_BtnVolver;
    Gtk::DropDown m_ComboPrefijo;
    Gtk::Box m_BoxTelefono;
    void on_guardar_clicked();
    void on_volver_clicked();
};

class CrearContacto : public Gtk::Box {
    public:
        CrearContacto(Gtk::Notebook& notebook);

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CenterBox;
        Gtk::Box m_CardBox;

        Gtk::Label m_LblTitulo;
        Gtk::Label m_LblError;

        // Campos del formulario
        Gtk::Grid m_GridCampos;
        Gtk::Entry m_EntryNombre;
        Gtk::Entry m_EntryApellido;
        Gtk::Entry m_EntryTlf;
        Gtk::Entry m_EntryEmail;

        Gtk::Button m_BtnGuardar;
        Gtk::Button m_BtnVolver;

        void on_guardar_clicked();
        void on_volver_clicked();
};

// =========================================================
// CLASE 2: VISTA BÚSQUEDA
// =========================================================
class VistaBusqueda : public Gtk::Box {
    public:
        VistaBusqueda(Gtk::Notebook& notebook);

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CenterBox;
        
        // Panel de Input
        Gtk::Box m_CardBox;
        Gtk::Label m_LblTitulo;
        Gtk::Label m_LblInstruccion;
        Gtk::Entry m_EntryKeyword;
        Gtk::Button m_BtnBuscar;
        Gtk::Button m_BtnVolver;
        Gtk::Label m_LblError;

        // Panel de Resultados
        Gtk::Box m_CardBoxResultados;
        Gtk::Label m_LblResTitulo;
        Gtk::ScrolledWindow m_ScrollRes;
        Gtk::TextView m_TxtResContenido;
        Gtk::Button m_BtnNuevaBusqueda;
        Gtk::Button m_BtnIrMenu;

        void on_buscar_clicked();
        void on_nueva_busqueda_clicked();
        void on_volver_clicked();
};

// =========================================================
// CLASE 3: VISTA RECIENTES (Dashboard rápido)
// =========================================================
class VistaRecientes : public Gtk::Box {
    public:
        VistaRecientes(Gtk::Notebook& notebook);
        void actualizar_datos(); // Llamar al entrar a la pestaña

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CenterBox;
        Gtk::Box m_CardBoxResultados;

        Gtk::Label m_LblResTitulo;
        Gtk::Label m_LblTotal;
        
        // Slots para los 3 últimos
        Gtk::Label m_LblReciente1;
        Gtk::Label m_LblReciente2;
        Gtk::Label m_LblReciente3;

        Gtk::Button m_BtnVolver;

        void on_volver_clicked();
};

// =========================================================
// CLASE 4: VISTA EXPORTAR / LISTADO COMPLETO
// =========================================================
class VistaExportar : public Gtk::Box {
    public:
        VistaExportar(Gtk::Notebook& notebook);
        void cargar_datos();

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CenterBox;
        Gtk::Box m_CardBoxResultados;

        Gtk::Label m_LblTitulo;
        Gtk::ScrolledWindow m_ScrollRes;
        Gtk::TextView m_TxtResContenido; // Aquí mostraremos la "tabla" CSV
        
        Gtk::Button m_BtnExportar;
        Gtk::Button m_BtnVolver;
        Gtk::Label m_LblMensaje;

        void on_exportar_clicked();
        void on_volver_clicked();
};

class Interfaz : public Gtk::Window {
    public:
        Interfaz();
        virtual ~Interfaz();

    protected:
        // Contenedor principal de pestañas
        Gtk::Notebook m_Notebook;

        // --- PESTAÑA INICIO (MENÚ) ---
        Gtk::Box m_CenterBoxInicio;
        Gtk::Box m_MenuContainer;
        Gtk::Label m_titulo_principal;

        Gtk::Button m_BtnGestion;   // Gestionar Contactos
        Gtk::Button m_BtnBuscar;    // Buscar Contacto
        Gtk::Button m_BtnRecientes; // Visualizar Recientes
        Gtk::Button m_BtnExportar;  // Exportar CSV
        Gtk::Button m_BtnSalir;     // Salir

        void on_salir_clicked();
};

