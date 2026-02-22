#pragma once
#include <gtkmm.h>
#include <string>
#include "negocios/TablaHash.h"
#include "datos/Ficheros.h"
#include "negocios/Lista.h"

using std::string;

extern TablaHash tabla;
extern GestorArchivos gestorFicheros;
extern Lista contactosRecientes;

class VistaModificar : public Gtk::Box {
    public:
        VistaModificar(Gtk::Notebook& notebook);
        void cargar_contactos();

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CenterBox;

        Gtk::Box m_PanelLista;
        Gtk::Label m_LblTituloLista;
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::Box m_ListadoBox; 
        Gtk::Button m_BtnVolverMenu;

        Gtk::Box m_PanelEdicion;
        Gtk::Label m_LblTituloEdicion;
        Gtk::Label m_LblTlfFijo; 
        
        Gtk::Grid m_GridCampos;
        Gtk::Entry m_EntryNombre;
        Gtk::Entry m_EntryApellido;
        Gtk::Entry m_EntryEmail;

        Gtk::InfoBar m_InfoBar;
        Gtk::Label m_LblInfoBar;

        Gtk::Button m_BtnGuardar;
        Gtk::Button m_BtnCancelarEdicion;

        long m_numeroActual; 
        int numeral;
        long numeroTelefonico;

        void on_volver_menu_clicked();
        void on_cancelar_edicion_clicked();
        void on_guardar_edicion_clicked();
        void mostrarError(std::string mensaje);
        void mostrarExito(std::string mensaje);
        Gtk::Box* crear_tarjeta_modificar(std::string nombre, std::string apellido, std::string tlf, std::string correo, long numero_real);
};

class VistaEliminar : public Gtk::Box {
    public:
        VistaEliminar(Gtk::Notebook& notebook);
        void cargar_contactos();

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CardBox;
        Gtk::Label m_LblTitulo;
        
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::Box m_ListadoBox; 
        
        Gtk::Button m_BtnVolver;

        bool contactoEliminado;
        Gtk::InfoBar m_InfoBar;
        Gtk::Label m_LblInfoBar;
        void mostrarExito(std::string mensaje);

        void on_volver_clicked();
        Gtk::Box* crear_tarjeta_eliminar(string nombre, string apellido, string tlf, string correo, long numero);
};

class VistaConsulta : public Gtk::Box {
    public:
        VistaConsulta(Gtk::Notebook& notebook);
        void cargar_contactos(); 

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CardBox; 
        Gtk::Label m_LblTitulo;
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::Box m_ListadoBox; 
        Gtk::Button m_BtnVolver;

        void on_volver_clicked();
        Gtk::Box* crear_tarjeta_contacto(string nombre, string apellido, string tlf, string correo);
};

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

class VistaGestion : public Gtk::Box {
    public:
        VistaGestion(Gtk::Notebook& notebook);
    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CardBox; 
        Gtk::Label m_LblTitulo, m_LblError;

        Gtk::Label m_LblNombre, m_LblApellido, m_LblTlf, m_LblEmail, m_LblInfoBar;
        Gtk::InfoBar m_InfoBar;

        Gtk::Grid m_GridCampos;
        Gtk::Entry m_EntryNombre, m_EntryApellido, m_EntryTlf, m_EntryEmail, m_EntryPrefijo;
        Gtk::Button m_BtnGuardar, m_BtnVolver;

        void on_guardar_clicked();
        void on_volver_clicked();
        void mostrarError(string mensaje);
        void mostrarExito(string mensaje);
        void limpiarCampos();
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

class VistaBusqueda : public Gtk::Box {
    public:
        VistaBusqueda(Gtk::Notebook& notebook);
        void cargar_contactos_filtro(std::string termino);

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CardBox;
        Gtk::Label m_LblTitulo;
        Gtk::Box m_BoxBusqueda;
        Gtk::Entry m_EntryKeyword;
        Gtk::Label m_LblError;
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::Box m_ListadoBox; 
        Gtk::Button m_BtnVolver;

        void on_buscar_clicked();
        void on_volver_clicked();
        Gtk::Box* crear_tarjeta_contacto(string nombre, string apellido, string tlf, string correo);
};
class VistaRecientes : public Gtk::Box {
    public:
        VistaRecientes(Gtk::Notebook& notebook);
        void actualizar_datos(); 

    protected:
        Gtk::Notebook& m_notebook;
        Gtk::Box m_CardBoxResultados;
        Gtk::Label m_LblResTitulo;
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::Box m_ListadoBox; 
        Gtk::Button m_BtnVolver;

        void on_volver_clicked();
        Gtk::Box* crear_tarjeta_reciente(std::string nombre, std::string apellido, std::string tlf, std::string accion);
};

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
        Gtk::TextView m_TxtResContenido; 
        
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
        Gtk::Box m_PhoneFrame;
        Gtk::Box m_PhoneScreen;
        Gtk::Notebook m_Notebook;

        Gtk::Box m_CenterBoxInicio;
        Gtk::Box m_MenuContainer;
        Gtk::Label m_titulo_principal;

        Gtk::Button m_BtnGestion;   
        Gtk::Button m_BtnBuscar;    
        Gtk::Button m_BtnRecientes; 
        Gtk::Button m_BtnExportar;  
        Gtk::Button m_BtnSalir;     
        void on_salir_clicked();
};

