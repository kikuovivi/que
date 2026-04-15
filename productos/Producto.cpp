#include "Producto.hpp"
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

Producto::Producto() {
    id = 0;
    codigo[0] = '\0';
    nombre[0] = '\0';
    descripcion[0] = '\0';
    precio = 0.0f;
    stock = 0;
    idProveedor = 0;
    stockMinimo = 0;
    totalVendido = 0;
    eliminado = false;
    fechaCreacion = std::time(nullptr);
    fechaUltimaModificacion = fechaCreacion;
}

Producto::Producto(const char* codigo,
                const char* nombre,
                const char* descripcion,
                float precio,
                int stock,
                int idProveedor,
                int stockMinimo) : Producto() {
    setCodigo(codigo);
    setNombre(nombre);
    setDescripcion(descripcion);
    setPrecio(precio);
    setStock(stock);
    setIdProveedor(idProveedor);
    setStockMinimo(stockMinimo);
}

int Producto::getId() const { return id; }
const char* Producto::getCodigo() const { return codigo; }
const char* Producto::getNombre() const { return nombre; }
const char* Producto::getDescripcion() const { return descripcion; }
float Producto::getPrecio() const { return precio; }
int Producto::getStock() const { return stock; }
int Producto::getIdProveedor() const { return idProveedor; }
int Producto::getStockMinimo() const { return stockMinimo; }
int Producto::getTotalVendido() const { return totalVendido; }
bool Producto::isEliminado() const { return eliminado; }
std::time_t Producto::getFechaCreacion() const { return fechaCreacion; }
std::time_t Producto::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

void Producto::setId(int nuevoId) {
    if (nuevoId >= 0) {
        id = nuevoId;
    }
}

bool Producto::setCodigo(const char* nuevoCodigo) {
    return copiarCadenaSegura(codigo, sizeof(codigo), nuevoCodigo, false);
}

bool Producto::setNombre(const char* nuevoNombre) {
    return copiarCadenaSegura(nombre, sizeof(nombre), nuevoNombre, false);
}

bool Producto::setDescripcion(const char* nuevaDescripcion) {
    return copiarCadenaSegura(descripcion, sizeof(descripcion), nuevaDescripcion, true);
}

bool Producto::setPrecio(float nuevoPrecio) {
    if (nuevoPrecio <= 0) {
        return false;
    }

    precio = nuevoPrecio;
    return true;
}

bool Producto::setStock(int nuevoStock) {
    if (nuevoStock < 0) {
        return false;
    }

    stock = nuevoStock;
    return true;
}

bool Producto::setIdProveedor(int nuevoIdProveedor) {
    if (nuevoIdProveedor <= 0) {
        return false;
    }

    idProveedor = nuevoIdProveedor;
    return true;
}

bool Producto::setStockMinimo(int nuevoStockMinimo) {
    if (nuevoStockMinimo < 0) {
        return false;
    }

    stockMinimo = nuevoStockMinimo;
    return true;
}

bool Producto::setTotalVendido(int nuevoTotal) {
    if (nuevoTotal < 0) {
        return false;
    }

    totalVendido = nuevoTotal;
    return true;
}

void Producto::setEliminado(bool estado) {
    eliminado = estado;
}

void Producto::setFechaCreacion(std::time_t fecha) {
    fechaCreacion = fecha;
}

void Producto::setFechaUltimaModificacion(std::time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Producto::tieneDatosValidosBasicos() const {
    return id >= 0 &&
        std::strlen(codigo) > 0 &&
        std::strlen(nombre) > 0 &&
        precio > 0 &&
        stock >= 0 &&
        idProveedor > 0 &&
        stockMinimo >= 0 &&
        totalVendido >= 0;
}

bool Producto::tieneStockCritico() const {
    return !eliminado && stock <= stockMinimo;
}

void Producto::mostrarInformacionBasica() const {
    if (eliminado) {
        return;
    }

    cout << "ID: " << id
        << " | Codigo: " << codigo
        << " | Nombre: " << nombre
        << " | Precio: $" << precio
        << " | Stock: " << stock << endl;
}

void Producto::mostrarInformacionCompleta() const {
    cout << "ID                   : " << id << endl;
    cout << "Codigo               : " << codigo << endl;
    cout << "Nombre               : " << nombre << endl;
    cout << "Descripcion          : " << descripcion << endl;
    cout << "Precio               : $" << precio << endl;
    cout << "Stock                : " << stock << endl;
    cout << "ID Proveedor         : " << idProveedor << endl;
    cout << "Stock minimo         : " << stockMinimo << endl;
    cout << "Total vendido        : " << totalVendido << endl;
    cout << "Eliminado            : " << (eliminado ? "Si" : "No") << endl;
    cout << "Fecha creacion       : " << fechaCreacion << endl;
    cout << "Ultima modificacion  : " << fechaUltimaModificacion << endl;
}

int Producto::obtenerTamano() {
    return sizeof(Producto);
}