#include "Proveedor.hpp"
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

Proveedor::Proveedor() {
    id = 0;
    nombre[0] = '\0';
    rif[0] = '\0';
    telefono[0] = '\0';
    email[0] = '\0';
    direccion[0] = '\0';
    eliminado = false;
    fechaCreacion = std::time(nullptr);
    fechaUltimaModificacion = fechaCreacion;
}

Proveedor::Proveedor(const char* nombre,
                     const char* rif,
                     const char* telefono,
                     const char* email,
                     const char* direccion) : Proveedor() {
    setNombre(nombre);
    setRif(rif);
    setTelefono(telefono);
    setEmail(email);
    setDireccion(direccion);
}

int Proveedor::getId() const { return id; }
const char* Proveedor::getNombre() const { return nombre; }
const char* Proveedor::getRif() const { return rif; }
const char* Proveedor::getTelefono() const { return telefono; }
const char* Proveedor::getEmail() const { return email; }
const char* Proveedor::getDireccion() const { return direccion; }
bool Proveedor::isEliminado() const { return eliminado; }
std::time_t Proveedor::getFechaCreacion() const { return fechaCreacion; }
std::time_t Proveedor::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

void Proveedor::setId(int nuevoId) {
    if (nuevoId >= 0) {
        id = nuevoId;
    }
}

bool Proveedor::setNombre(const char* nuevoNombre) {
    return copiarCadenaSegura(nombre, sizeof(nombre), nuevoNombre, false);
}

bool Proveedor::setRif(const char* nuevoRif) {
    return copiarCadenaSegura(rif, sizeof(rif), nuevoRif, false);
}

bool Proveedor::setTelefono(const char* nuevoTelefono) {
    return copiarCadenaSegura(telefono, sizeof(telefono), nuevoTelefono, true);
}

bool Proveedor::setEmail(const char* nuevoEmail) {
    return copiarCadenaSegura(email, sizeof(email), nuevoEmail, false);
}

bool Proveedor::setDireccion(const char* nuevaDireccion) {
    return copiarCadenaSegura(direccion, sizeof(direccion), nuevaDireccion, true);
}

void Proveedor::setEliminado(bool estado) {
    eliminado = estado;
}

void Proveedor::setFechaCreacion(std::time_t fecha) {
    fechaCreacion = fecha;
}

void Proveedor::setFechaUltimaModificacion(std::time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Proveedor::tieneDatosValidosBasicos() const {
    return id >= 0 &&
           std::strlen(nombre) > 0 &&
           std::strlen(rif) > 0 &&
           std::strlen(email) > 0;
}

void Proveedor::mostrarInformacionBasica() const {
    if (eliminado) {
        return;
    }

    cout << "ID: " << id
         << " | Nombre: " << nombre
         << " | RIF: " << rif
         << " | Email: " << email << endl;
}

void Proveedor::mostrarInformacionCompleta() const {
    cout << "ID                   : " << id << endl;
    cout << "Nombre               : " << nombre << endl;
    cout << "RIF                  : " << rif << endl;
    cout << "Telefono             : " << telefono << endl;
    cout << "Email                : " << email << endl;
    cout << "Direccion            : " << direccion << endl;
    cout << "Eliminado            : " << (eliminado ? "Si" : "No") << endl;
    cout << "Fecha creacion       : " << fechaCreacion << endl;
    cout << "Ultima modificacion  : " << fechaUltimaModificacion << endl;
}

int Proveedor::obtenerTamano() {
    return sizeof(Proveedor);
}