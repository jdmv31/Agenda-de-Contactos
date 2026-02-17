#pragma once
#include "negocios/TablaHash.h"

class GestorArchivos{
    private:
        int contactosAlmacenados;
    public:
        GestorArchivos();
        bool exportarCSV(TablaHash& tabla);
};