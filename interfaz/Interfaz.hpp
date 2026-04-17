#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

class Interfaz {
public:
    static void ejecutar();

private:
    static void menuPrincipal();
    static void menuProductos();
    static void menuProveedores();
    static void menuClientes();
    static void menuTransacciones();
    static int leerOpcion();
};

#endif