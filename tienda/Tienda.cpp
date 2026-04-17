#include "Tienda.hpp"
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

Tienda::Tienda() {
    nombre[0] = '\0';
    rif[0] = '\0';
    totalProductos = 0;
    totalProveedores = 0;
    totalClientes = 0;
    totalTransacciones = 0;
    eliminado = false;
    fechaCreacion = std::time(nullptr);
    fechaUltimaModificacion = fechaCreacion;
}

Tienda::Tienda(const char* nombre, const char* rif) : Tienda() {
    setNombre(nombre);
    setRif(rif);
}

const char* Tienda::getNombre() const { return nombre; }
const char* Tienda::getRif() const { return rif; }
int Tienda::getTotalProductos() const { return totalProductos; }
int Tienda::getTotalProveedores() const { return totalProveedores; }
int Tienda::getTotalClientes() const { return totalClientes; }
int Tienda::getTotalTransacciones() const { return totalTransacciones; }
bool Tienda::isEliminado() const { return eliminado; }
std::time_t Tienda::getFechaCreacion() const { return fechaCreacion; }
std::time_t Tienda::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Tienda::setNombre(const char* nuevoNombre) {
    return copiarCadenaSegura(nombre, sizeof(nombre), nuevoNombre, false);
}

bool Tienda::setRif(const char* nuevoRif) {
    return copiarCadenaSegura(rif, sizeof(rif), nuevoRif, false);
}

bool Tienda::setTotalProductos(int nuevoTotal) {
    if (nuevoTotal < 0) return false;
    totalProductos = nuevoTotal;
    return true;
}

bool Tienda::setTotalProveedores(int nuevoTotal) {
    if (nuevoTotal < 0) return false;
    totalProveedores = nuevoTotal;
    return true;
}

bool Tienda::setTotalClientes(int nuevoTotal) {
    if (nuevoTotal < 0) return false;
    totalClientes = nuevoTotal;
    return true;
}

bool Tienda::setTotalTransacciones(int nuevoTotal) {
    if (nuevoTotal < 0) return false;
    totalTransacciones = nuevoTotal;
    return true;
}

void Tienda::setEliminado(bool estado) {
    eliminado = estado;
}

void Tienda::setFechaCreacion(std::time_t fecha) {
    fechaCreacion = fecha;
}

void Tienda::setFechaUltimaModificacion(std::time_t fecha) {
    fechaUltimaModificacion = fecha;
}

void Tienda::mostrarInformacionBasica() const {
    cout << "Tienda: " << nombre
         << " | RIF: " << rif << endl;
}

void Tienda::mostrarInformacionCompleta() const {
    cout << "Nombre               : " << nombre << endl;
    cout << "RIF                  : " << rif << endl;
    cout << "Total productos      : " << totalProductos << endl;
    cout << "Total proveedores    : " << totalProveedores << endl;
    cout << "Total clientes       : " << totalClientes << endl;
    cout << "Total transacciones  : " << totalTransacciones << endl;
    cout << "Eliminado            : " << (eliminado ? "Si" : "No") << endl;
    cout << "Fecha creacion       : " << fechaCreacion << endl;
    cout << "Ultima modificacion  : " << fechaUltimaModificacion << endl;
}

int Tienda::obtenerTamano() {
    return sizeof(Tienda);
}