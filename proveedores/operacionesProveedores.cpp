#include "operacionesProveedores.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>

using namespace std;

namespace {
    void leerLinea(char* buffer, int tam) {
        cin.getline(buffer, tam);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

bool rifProveedorDuplicado(const char* rifBuscado) {
    ifstream archivo(Constantes::ARCHIVO_PROVEEDORES, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        archivo.close();
        return false;
    }

    Proveedor proveedor;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(GestorArchivos::obtenerOffset<Proveedor>(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));

        if (!archivo.good()) {
            archivo.close();
            return false;
        }

        if (!proveedor.isEliminado() && strcmp(proveedor.getRif(), rifBuscado) == 0) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

int buscarIndiceProveedorPorID(int idBuscado) {
    return GestorArchivos::buscarIndicePorID<Proveedor>(Constantes::ARCHIVO_PROVEEDORES, idBuscado);
}

bool leerProveedorPorIndice(int indice, Proveedor& proveedor) {
    return GestorArchivos::leerRegistroPorIndice<Proveedor>(Constantes::ARCHIVO_PROVEEDORES, indice, proveedor);
}

bool leerProveedorPorID(int idBuscado, Proveedor& proveedor) {
    return GestorArchivos::leerRegistroPorID<Proveedor>(Constantes::ARCHIVO_PROVEEDORES, idBuscado, proveedor);
}

void registrarProveedor() {
    Proveedor proveedor;
    char buffer[300];

    cout << "\n========== REGISTRAR PROVEEDOR ==========\n";

    cout << "Nombre: ";
    leerLinea(buffer, 100);
    if (!proveedor.setNombre(buffer)) {
        cout << "ERROR: El nombre no puede estar vacio o excede el tamano permitido.\n";
        return;
    }

    cout << "RIF: ";
    leerLinea(buffer, 20);
    if (!proveedor.setRif(buffer)) {
        cout << "ERROR: El RIF no puede estar vacio o excede el tamano permitido.\n";
        return;
    }

    if (!Validaciones::validarRIF(proveedor.getRif())) {
        cout << "ERROR: El RIF no tiene un formato valido.\n";
        return;
    }

    if (rifProveedorDuplicado(proveedor.getRif())) {
        cout << "ERROR: Ya existe un proveedor con ese RIF.\n";
        return;
    }

    cout << "Telefono: ";
    leerLinea(buffer, 20);
    if (!proveedor.setTelefono(buffer)) {
        cout << "ERROR: El telefono excede el tamano permitido.\n";
        return;
    }

    if (strlen(proveedor.getTelefono()) > 0 && !Validaciones::validarTelefono(proveedor.getTelefono())) {
        cout << "ERROR: El telefono no tiene un formato valido.\n";
        return;
    }

    cout << "Email: ";
    leerLinea(buffer, 100);
    if (!proveedor.setEmail(buffer)) {
        cout << "ERROR: El email no puede estar vacio o excede el tamano permitido.\n";
        return;
    }

    if (!Validaciones::validarEmail(proveedor.getEmail())) {
        cout << "ERROR: El email no tiene un formato valido.\n";
        return;
    }

    cout << "Direccion: ";
    leerLinea(buffer, 200);
    if (!proveedor.setDireccion(buffer)) {
        cout << "ERROR: La direccion excede el tamano permitido.\n";
        return;
    }

    if (!proveedor.tieneDatosValidosBasicos()) {
        cout << "ERROR: El proveedor no cumple las validaciones basicas.\n";
        return;
    }

    cout << "\nConfirmar guardado? (S/N): ";
    leerLinea(buffer, 10);
    if (toupper(static_cast<unsigned char>(buffer[0])) != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (GestorArchivos::guardarRegistro<Proveedor>(Constantes::ARCHIVO_PROVEEDORES, proveedor)) {
        cout << "Proveedor guardado correctamente.\n";
    } else {
        cout << "ERROR: No se pudo guardar el proveedor.\n";
    }
}

void listarProveedores() {
    ifstream archivo(Constantes::ARCHIVO_PROVEEDORES, ios::binary);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo de proveedores.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        cout << "ERROR: No se pudo leer el header de proveedores.\n";
        archivo.close();
        return;
    }

    if (header.registrosActivos == 0) {
        cout << "\nNo hay proveedores registrados.\n";
        archivo.close();
        return;
    }

    cout << "\n==================== LISTADO DE PROVEEDORES ====================\n";

    Proveedor proveedor;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(GestorArchivos::obtenerOffset<Proveedor>(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));

        if (!archivo.good()) {
            cout << "ERROR: Fallo al leer un registro de proveedor.\n";
            archivo.close();
            return;
        }

        if (proveedor.isEliminado()) {
            continue;
        }

        cout << "\nID        : " << proveedor.getId() << "\n";
        cout << "Nombre    : " << proveedor.getNombre() << "\n";
        cout << "RIF       : " << proveedor.getRif() << "\n";
        cout << "Telefono  : " << proveedor.getTelefono() << "\n";
        cout << "Email     : " << proveedor.getEmail() << "\n";
        cout << "Direccion : " << proveedor.getDireccion() << "\n";
    }

    archivo.close();
}