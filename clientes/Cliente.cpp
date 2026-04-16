#include "Cliente.hpp"
#include <iostream>
#include <cstring>

using namespace std;

namespace {
    bool copiarCadenaSegura(char* destino, int tamDestino, const char* origen, bool permitirVacio) {
        if (destino == nullptr || origen == nullptr) {
            return false;
        }

        if (!permitirVacio && std::strlen(origen) == 0) {
            return false;
        }

        if (static_cast<int>(std::strlen(origen)) >= tamDestino) {
            return false;
        }

        std::strncpy(destino, origen, tamDestino - 1);
        destino[tamDestino - 1] = '\0';
        return true;
    }
}

Cliente::Cliente() {
    id = 0;
    nombre[0] = '\0';
    cedula[0] = '\0';
    telefono[0] = '\0';
    email[0] = '\0';
    direccion[0] = '\0';
    totalGastado = 0.0f;
    cantidadCompras = 0;
    eliminado = false;
    fechaCreacion = std::time(nullptr);
    fechaUltimaModificacion = fechaCreacion;
}

Cliente::Cliente(const char* nombre,
                 const char* cedula,
                 const char* telefono,
                 const char* email,
                 const char* direccion) : Cliente() {
    setNombre(nombre);
    setCedula(cedula);
    setTelefono(telefono);
    setEmail(email);
    setDireccion(direccion);
}

int Cliente::getId() const { return id; }
const char* Cliente::getNombre() const { return nombre; }
const char* Cliente::getCedula() const { return cedula; }
const char* Cliente::getTelefono() const { return telefono; }
const char* Cliente::getEmail() const { return email; }
const char* Cliente::getDireccion() const { return direccion; }
float Cliente::getTotalGastado() const { return totalGastado; }
int Cliente::getCantidadCompras() const { return cantidadCompras; }
bool Cliente::isEliminado() const { return eliminado; }
std::time_t Cliente::getFechaCreacion() const { return fechaCreacion; }
std::time_t Cliente::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

void Cliente::setId(int nuevoId) {
    if (nuevoId >= 0) {
        id = nuevoId;
    }
}

bool Cliente::setNombre(const char* nuevoNombre) {
    return copiarCadenaSegura(nombre, sizeof(nombre), nuevoNombre, false);
}

bool Cliente::setCedula(const char* nuevaCedula) {
    return copiarCadenaSegura(cedula, sizeof(cedula), nuevaCedula, false);
}

bool Cliente::setTelefono(const char* nuevoTelefono) {
    return copiarCadenaSegura(telefono, sizeof(telefono), nuevoTelefono, true);
}

bool Cliente::setEmail(const char* nuevoEmail) {
    return copiarCadenaSegura(email, sizeof(email), nuevoEmail, false);
}

bool Cliente::setDireccion(const char* nuevaDireccion) {
    return copiarCadenaSegura(direccion, sizeof(direccion), nuevaDireccion, true);
}

bool Cliente::setTotalGastado(float nuevoTotalGastado) {
    if (nuevoTotalGastado < 0) {
        return false;
    }

    totalGastado = nuevoTotalGastado;
    return true;
}

bool Cliente::setCantidadCompras(int nuevaCantidadCompras) {
    if (nuevaCantidadCompras < 0) {
        return false;
    }

    cantidadCompras = nuevaCantidadCompras;
    return true;
}

void Cliente::setEliminado(bool estado) {
    eliminado = estado;
}

void Cliente::setFechaCreacion(std::time_t fecha) {
    fechaCreacion = fecha;
}

void Cliente::setFechaUltimaModificacion(std::time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Cliente::tieneDatosValidosBasicos() const {
    return id >= 0 &&
           std::strlen(nombre) > 0 &&
           std::strlen(cedula) > 0 &&
           std::strlen(email) > 0 &&
           totalGastado >= 0 &&
           cantidadCompras >= 0;
}

void Cliente::mostrarInformacionBasica() const {
    if (eliminado) {
        return;
    }

    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | Cedula: " << cedula
         << " | Email: " << email << endl;
}

void Cliente::mostrarInformacionCompleta() const {
    cout << "ID                   : " << id << endl;
    cout << "Nombre               : " << nombre << endl;
    cout << "Cedula               : " << cedula << endl;
    cout << "Telefono             : " << telefono << endl;
    cout << "Email                : " << email << endl;
    cout << "Direccion            : " << direccion << endl;
    cout << "Total gastado        : " << totalGastado << endl;
    cout << "Cantidad compras     : " << cantidadCompras << endl;
    cout << "Eliminado            : " << (eliminado ? "Si" : "No") << endl;
    cout << "Fecha creacion       : " << fechaCreacion << endl;
    cout << "Ultima modificacion  : " << fechaUltimaModificacion << endl;
}

int Cliente::obtenerTamano() {
    return sizeof(Cliente);
}