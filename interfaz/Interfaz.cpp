#include "Interfaz.hpp"
#include "../utilidades/Formatos.hpp"
#include "../productos/operacionesProductos.hpp"
#include "../proveedores/operacionesProveedores.hpp"
#include "../clientes/operacionesClientes.hpp"
#include "../transacciones/operacionesTransacciones.hpp"

#include <iostream>
#include <cstdlib>

using namespace std;

int Interfaz::leerOpcion() {
    char buffer[20];
    cin.getline(buffer, 20);

    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }

    return atoi(buffer);
}

void Interfaz::menuProductos() {
    int opcion;

    do {
        cout << "\n==================== MODULO PRODUCTOS ====================\n";
        cout << "1. Registrar producto\n";
        cout << "2. Listar productos\n";
        cout << "3. Buscar producto por ID\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                registrarProducto();
                Formatos::pausar();
                break;
            case 2:
                listarProductos();
                Formatos::pausar();
                break;
            case 3:
                buscarProductoPorID();
                Formatos::pausar();
                break;
            case 0:
                break;
            default:
                cout << "ERROR: Opcion invalida.\n";
                Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuProveedores() {
    int opcion;

    do {
        cout << "\n==================== MODULO PROVEEDORES ====================\n";
        cout << "1. Registrar proveedor\n";
        cout << "2. Listar proveedores\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                registrarProveedor();
                Formatos::pausar();
                break;
            case 2:
                listarProveedores();
                Formatos::pausar();
                break;
            case 0:
                break;
            default:
                cout << "ERROR: Opcion invalida.\n";
                Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuClientes() {
    int opcion;

    do {
        cout << "\n==================== MODULO CLIENTES ====================\n";
        cout << "1. Registrar cliente\n";
        cout << "2. Listar clientes\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                registrarCliente();
                Formatos::pausar();
                break;
            case 2:
                listarClientes();
                Formatos::pausar();
                break;
            case 0:
                break;
            default:
                cout << "ERROR: Opcion invalida.\n";
                Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuTransacciones() {
    int opcion;

    do {
        cout << "\n==================== MODULO TRANSACCIONES ====================\n";
        cout << "1. Registrar venta simple\n";
        cout << "2. Listar transacciones\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                registrarVentaSimple();
                Formatos::pausar();
                break;
            case 2:
                listarTransacciones();
                Formatos::pausar();
                break;
            case 0:
                break;
            default:
                cout << "ERROR: Opcion invalida.\n";
                Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuPrincipal() {
    int opcion;

    do {
        cout << "\n==================== SISTEMA DE INVENTARIO V3 ====================\n";
        cout << "1. Modulo Productos\n";
        cout << "2. Modulo Proveedores\n";
        cout << "3. Modulo Clientes\n";
        cout << "4. Modulo Transacciones\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";

        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                menuProductos();
                break;
            case 2:
                menuProveedores();
                break;
            case 3:
                menuClientes();
                break;
            case 4:
                menuTransacciones();
                break;
            case 0:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "ERROR: Opcion invalida.\n";
                Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::ejecutar() {
    menuPrincipal();
}