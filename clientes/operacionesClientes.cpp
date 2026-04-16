#include "operacionesClientes.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"

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

bool cedulaClienteDuplicada(const char* cedulaBuscada) {
    ifstream archivo(Constantes::ARCHIVO_CLIENTES, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        archivo.close();
        return false;
    }

    Cliente cliente;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(GestorArchivos::obtenerOffset<Cliente>(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&cliente), sizeof(Cliente));

        if (!archivo.good()) {
            archivo.close();
            return false;
        }

        if (!cliente.isEliminado() && strcmp(cliente.getCedula(), cedulaBuscada) == 0) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

int buscarIndiceClientePorID(int idBuscado) {
    return GestorArchivos::buscarIndicePorID<Cliente>(Constantes::ARCHIVO_CLIENTES, idBuscado);
}

bool leerClientePorIndice(int indice, Cliente& cliente) {
    return GestorArchivos::leerRegistroPorIndice<Cliente>(Constantes::ARCHIVO_CLIENTES, indice, cliente);
}

bool leerClientePorID(int idBuscado, Cliente& cliente) {
    return GestorArchivos::leerRegistroPorID<Cliente>(Constantes::ARCHIVO_CLIENTES, idBuscado, cliente);
}

bool actualizarClienteEnArchivo(int indice, const Cliente& cliente) {
    return GestorArchivos::actualizarRegistro<Cliente>(Constantes::ARCHIVO_CLIENTES, indice, cliente);
}

void registrarCliente() {
    Cliente cliente;
    char buffer[300];

    cout << "\n========== REGISTRAR CLIENTE ==========\n";

    cout << "Nombre: ";
    leerLinea(buffer, 100);
    if (!cliente.setNombre(buffer)) {
        cout << "ERROR: El nombre no puede estar vacio o excede el tamano permitido.\n";
        return;
    }

    cout << "Cedula: ";
    leerLinea(buffer, 20);
    if (!cliente.setCedula(buffer)) {
        cout << "ERROR: La cedula no puede estar vacia o excede el tamano permitido.\n";
        return;
    }

    if (cedulaClienteDuplicada(cliente.getCedula())) {
        cout << "ERROR: Ya existe un cliente con esa cedula.\n";
        return;
    }

    cout << "Telefono: ";
    leerLinea(buffer, 20);
    if (!cliente.setTelefono(buffer)) {
        cout << "ERROR: El telefono excede el tamano permitido.\n";
        return;
    }

    if (strlen(cliente.getTelefono()) > 0 && !Validaciones::validarTelefono(cliente.getTelefono())) {
        cout << "ERROR: El telefono no tiene un formato valido.\n";
        return;
    }

    cout << "Email: ";
    leerLinea(buffer, 100);
    if (!cliente.setEmail(buffer)) {
        cout << "ERROR: El email no puede estar vacio o excede el tamano permitido.\n";
        return;
    }

    if (!Validaciones::validarEmail(cliente.getEmail())) {
        cout << "ERROR: El email no tiene un formato valido.\n";
        return;
    }

    cout << "Direccion: ";
    leerLinea(buffer, 200);
    if (!cliente.setDireccion(buffer)) {
        cout << "ERROR: La direccion excede el tamano permitido.\n";
        return;
    }

    if (!cliente.setTotalGastado(0.0f)) {
        cout << "ERROR: No se pudo inicializar el total gastado.\n";
        return;
    }

    if (!cliente.setCantidadCompras(0)) {
        cout << "ERROR: No se pudo inicializar la cantidad de compras.\n";
        return;
    }

    if (!cliente.tieneDatosValidosBasicos()) {
        cout << "ERROR: El cliente no cumple las validaciones basicas.\n";
        return;
    }

    cout << "\nConfirmar guardado? (S/N): ";
    leerLinea(buffer, 10);
    if (toupper(static_cast<unsigned char>(buffer[0])) != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (GestorArchivos::guardarRegistro<Cliente>(Constantes::ARCHIVO_CLIENTES, cliente)) {
        cout << "Cliente guardado correctamente.\n";
    } else {
        cout << "ERROR: No se pudo guardar el cliente.\n";
    }
}

void listarClientes() {
    ifstream archivo(Constantes::ARCHIVO_CLIENTES, ios::binary);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo de clientes.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        cout << "ERROR: No se pudo leer el header de clientes.\n";
        archivo.close();
        return;
    }

    if (header.registrosActivos == 0) {
        cout << "\nNo hay clientes registrados.\n";
        archivo.close();
        return;
    }

    cout << "\n==================== LISTADO DE CLIENTES ====================\n";

    Cliente cliente;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(GestorArchivos::obtenerOffset<Cliente>(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&cliente), sizeof(Cliente));

        if (!archivo.good()) {
            cout << "ERROR: Fallo al leer un registro de cliente.\n";
            archivo.close();
            return;
        }

        if (cliente.isEliminado()) {
            continue;
        }

        cout << "\nID              : " << cliente.getId() << "\n";
        cout << "Nombre          : " << cliente.getNombre() << "\n";
        cout << "Cedula          : " << cliente.getCedula() << "\n";
        cout << "Telefono        : " << cliente.getTelefono() << "\n";
        cout << "Email           : " << cliente.getEmail() << "\n";
        cout << "Direccion       : " << cliente.getDireccion() << "\n";
        cout << "Total gastado   : " << Formatos::formatearMoneda(cliente.getTotalGastado()) << "\n";
        cout << "Cantidad compras: " << cliente.getCantidadCompras() << "\n";
    }

    archivo.close();
}