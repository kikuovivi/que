#include "Transaccion.hpp"
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

Transaccion::Transaccion() {
    id = 0;
    tipo[0] = '\0';
    idRelacionado = 0;
    cantidadDetalles = 0;
    for (int i = 0; i < 20; i++) {
        detalles[i].idProducto = 0;
        detalles[i].cantidad = 0;
        detalles[i].precioUnitario = 0.0f;
        detalles[i].subtotal = 0.0f;
    }
    total = 0.0f;
    cancelada = false;
    eliminado = false;
    fechaCreacion = std::time(nullptr);
    fechaUltimaModificacion = fechaCreacion;
}

Transaccion::Transaccion(const char* tipo, int idRelacionado) : Transaccion() {
    setTipo(tipo);
    setIdRelacionado(idRelacionado);
}

int Transaccion::getId() const { return id; }
const char* Transaccion::getTipo() const { return tipo; }
int Transaccion::getIdRelacionado() const { return idRelacionado; }
int Transaccion::getCantidadDetalles() const { return cantidadDetalles; }
float Transaccion::getTotal() const { return total; }
bool Transaccion::isCancelada() const { return cancelada; }
bool Transaccion::isEliminado() const { return eliminado; }
std::time_t Transaccion::getFechaCreacion() const { return fechaCreacion; }
std::time_t Transaccion::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

DetalleTransaccion Transaccion::getDetalle(int indice) const {
    DetalleTransaccion detalleVacio{0, 0, 0.0f, 0.0f};

    if (indice < 0 || indice >= cantidadDetalles || indice >= 20) {
        return detalleVacio;
    }

    return detalles[indice];
}

void Transaccion::setId(int nuevoId) {
    if (nuevoId >= 0) {
        id = nuevoId;
    }
}

bool Transaccion::setTipo(const char* nuevoTipo) {
    return copiarCadenaSegura(tipo, sizeof(tipo), nuevoTipo, false);
}

bool Transaccion::setIdRelacionado(int nuevoIdRelacionado) {
    if (nuevoIdRelacionado <= 0) {
        return false;
    }

    idRelacionado = nuevoIdRelacionado;
    return true;
}

bool Transaccion::setCantidadDetalles(int nuevaCantidadDetalles) {
    if (nuevaCantidadDetalles < 0 || nuevaCantidadDetalles > 20) {
        return false;
    }

    cantidadDetalles = nuevaCantidadDetalles;
    return true;
}

bool Transaccion::configurarDetalle(int indice, int idProducto, int cantidad, float precioUnitario, float subtotal) {
    if (indice < 0 || indice >= 20) {
        return false;
    }

    if (idProducto <= 0 || cantidad <= 0 || precioUnitario < 0 || subtotal < 0) {
        return false;
    }

    detalles[indice].idProducto = idProducto;
    detalles[indice].cantidad = cantidad;
    detalles[indice].precioUnitario = precioUnitario;
    detalles[indice].subtotal = subtotal;

    return true;
}

bool Transaccion::setTotal(float nuevoTotal) {
    if (nuevoTotal < 0) {
        return false;
    }

    total = nuevoTotal;
    return true;
}

void Transaccion::setCancelada(bool estado) {
    cancelada = estado;
}

void Transaccion::setEliminado(bool estado) {
    eliminado = estado;
}

void Transaccion::setFechaCreacion(std::time_t fecha) {
    fechaCreacion = fecha;
}

void Transaccion::setFechaUltimaModificacion(std::time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Transaccion::tieneDatosValidosBasicos() const {
    return id >= 0 &&
           std::strlen(tipo) > 0 &&
           idRelacionado > 0 &&
           cantidadDetalles >= 0 &&
           cantidadDetalles <= 20 &&
           total >= 0;
}

void Transaccion::mostrarInformacionBasica() const {
    if (eliminado) {
        return;
    }

    cout << "ID: " << id
         << " | Tipo: " << tipo
         << " | Relacionado: " << idRelacionado
         << " | Total: " << total << endl;
}

void Transaccion::mostrarInformacionCompleta() const {
    cout << "ID                   : " << id << endl;
    cout << "Tipo                 : " << tipo << endl;
    cout << "ID relacionado       : " << idRelacionado << endl;
    cout << "Cantidad detalles    : " << cantidadDetalles << endl;
    cout << "Total                : " << total << endl;
    cout << "Cancelada            : " << (cancelada ? "Si" : "No") << endl;
    cout << "Eliminado            : " << (eliminado ? "Si" : "No") << endl;
    cout << "Fecha creacion       : " << fechaCreacion << endl;
    cout << "Ultima modificacion  : " << fechaUltimaModificacion << endl;

    for (int i = 0; i < cantidadDetalles; i++) {
        cout << "Detalle " << (i + 1) << ":" << endl;
        cout << "  Producto ID   : " << detalles[i].idProducto << endl;
        cout << "  Cantidad      : " << detalles[i].cantidad << endl;
        cout << "  Precio Unit.  : " << detalles[i].precioUnitario << endl;
        cout << "  Subtotal      : " << detalles[i].subtotal << endl;
    }
}

int Transaccion::obtenerTamano() {
    return sizeof(Transaccion);
}