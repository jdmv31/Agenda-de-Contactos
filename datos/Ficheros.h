#pragma once

class TablaHash;

class GestorArchivos{
    private:
        int contactosAlmacenados;
    public:
        GestorArchivos();
        bool exportarCSV(TablaHash& tabla);
};