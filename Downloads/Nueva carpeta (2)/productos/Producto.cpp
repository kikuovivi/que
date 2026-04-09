#include "Producto.hpp"
#include <iostream>
#include <cstring>

using namespace std;

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
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = fechaCreacion;
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
time_t Producto::getFechaCreacion() const { return fechaCreacion; }
time_t Producto::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }


void Producto::setId(int nuevoId) {
    id = nuevoId;
}

void Producto::setCodigo(const char* nuevoCodigo) {
    strncpy(codigo, nuevoCodigo, sizeof(codigo) - 1);
    codigo[sizeof(codigo) - 1] = '\0'; 
}

void Producto::setNombre(const char* nuevoNombre) {
    strncpy(nombre, nuevoNombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
}

void Producto::setDescripcion(const char* nuevaDescripcion) {
    strncpy(descripcion, nuevaDescripcion, sizeof(descripcion) - 1);
    descripcion[sizeof(descripcion) - 1] = '\0';
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

void Producto::setIdProveedor(int nuevoIdProveedor) {
    idProveedor = nuevoIdProveedor;
}

bool Producto::setStockMinimo(int nuevoStockMinimo) {
    if (nuevoStockMinimo < 0) {
        return false;
    }
    stockMinimo = nuevoStockMinimo;
    return true;
}

void Producto::setTotalVendido(int nuevoTotal) {
    totalVendido = nuevoTotal;
}

void Producto::setEliminado(bool estado) {
    eliminado = estado;
}

void Producto::setFechaCreacion(time_t fecha) {
    fechaCreacion = fecha;
}

void Producto::setFechaUltimaModificacion(time_t fecha) {
    fechaUltimaModificacion = fecha;
}

void Producto::mostrarInformacionBasica() const {
    if (eliminado) return;

    cout << "ID: " << id
        << " | Codigo: " << codigo
        << " | Nombre: " << nombre
        << " | Precio: $" << precio
        << " | Stock: " << stock << endl;
}

int Producto::obtenerTamano() {
    return sizeof(Producto);
}