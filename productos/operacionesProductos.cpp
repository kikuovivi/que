#include "operacionesProductos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Formatos.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <ctime>

using namespace std;

namespace {
    struct ProveedorLegacy {
        int id;
        char nombre[100];
        char rif[20];
        char telefono[20];
        char email[100];
        char direccion[200];
        bool eliminado;
        time_t fechaCreacion;
        time_t fechaUltimaModificacion;
    };

    void leerLinea(char* buffer, int tam) {
        cin.getline(buffer, tam);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    long long obtenerOffsetProveedorLegacy(int indice) {
        return sizeof(ArchivoHeader) + static_cast<long long>(indice) * sizeof(ProveedorLegacy);
    }

    bool leerProveedorPorIDLegacy(int idBuscado, ProveedorLegacy& proveedor) {
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

        for (int i = 0; i < header.cantidadRegistros; i++) {
            archivo.seekg(obtenerOffsetProveedorLegacy(i), ios::beg);
            archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(ProveedorLegacy));

            if (!archivo.good()) {
                archivo.close();
                return false;
            }

            if (!proveedor.eliminado && proveedor.id == idBuscado) {
                archivo.close();
                return true;
            }
        }

        archivo.close();
        return false;
    }
}

bool codigoProductoDuplicado(const char* codigoBuscado) {
    ifstream archivo(Constantes::ARCHIVO_PRODUCTOS, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        archivo.close();
        return false;
    }

    Producto producto;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(GestorArchivos::obtenerOffset<Producto>(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));

        if (!archivo.good()) {
            archivo.close();
            return false;
        }

        if (!producto.isEliminado() && strcmp(producto.getCodigo(), codigoBuscado) == 0) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

int buscarIndiceProductoPorID(int idBuscado) {
    return GestorArchivos::buscarIndicePorID<Producto>(Constantes::ARCHIVO_PRODUCTOS, idBuscado);
}

bool leerProductoPorIndice(int indice, Producto& producto) {
    return GestorArchivos::leerRegistroPorIndice<Producto>(Constantes::ARCHIVO_PRODUCTOS, indice, producto);
}

bool leerProductoPorID(int idBuscado, Producto& producto) {
    return GestorArchivos::leerRegistroPorID<Producto>(Constantes::ARCHIVO_PRODUCTOS, idBuscado, producto);
}

bool actualizarProductoEnArchivo(int indice, const Producto& producto) {
    return GestorArchivos::actualizarRegistro<Producto>(Constantes::ARCHIVO_PRODUCTOS, indice, producto);
}

void registrarProducto() {
    Producto producto;
    char buffer[300];

    cout << "\n========== REGISTRAR PRODUCTO ==========\n";

    cout << "Codigo: ";
    leerLinea(buffer, 20);
    if (!producto.setCodigo(buffer)) {
        cout << "ERROR: El codigo no puede estar vacio o excede el tamano permitido.\n";
        return;
    }

    if (codigoProductoDuplicado(producto.getCodigo())) {
        cout << "ERROR: Ya existe un producto con ese codigo.\n";
        return;
    }

    cout << "Nombre: ";
    leerLinea(buffer, 100);
    if (!producto.setNombre(buffer)) {
        cout << "ERROR: El nombre no puede estar vacio o excede el tamano permitido.\n";
        return;
    }

    cout << "Descripcion: ";
    leerLinea(buffer, 200);
    if (!producto.setDescripcion(buffer)) {
        cout << "ERROR: La descripcion excede el tamano permitido.\n";
        return;
    }

    cout << "ID del proveedor: ";
    leerLinea(buffer, 20);
    int idProveedor = atoi(buffer);

    if (!producto.setIdProveedor(idProveedor)) {
        cout << "ERROR: El ID del proveedor debe ser mayor a 0.\n";
        return;
    }

    ProveedorLegacy proveedor;
    if (!leerProveedorPorIDLegacy(producto.getIdProveedor(), proveedor)) {
        cout << "ERROR: No existe un proveedor activo con ese ID.\n";
        return;
    }

    cout << "Precio: ";
    leerLinea(buffer, 30);
    float precio = static_cast<float>(atof(buffer));
    if (!producto.setPrecio(precio)) {
        cout << "ERROR: El precio debe ser mayor a 0.\n";
        return;
    }

    cout << "Stock inicial: ";
    leerLinea(buffer, 30);
    int stock = atoi(buffer);
    if (!producto.setStock(stock)) {
        cout << "ERROR: El stock no puede ser negativo.\n";
        return;
    }

    cout << "Stock minimo: ";
    leerLinea(buffer, 30);
    int stockMinimo = atoi(buffer);
    if (!producto.setStockMinimo(stockMinimo)) {
        cout << "ERROR: El stock minimo no puede ser negativo.\n";
        return;
    }

    if (!producto.setTotalVendido(0)) {
        cout << "ERROR: No se pudo inicializar el total vendido.\n";
        return;
    }

    if (!producto.tieneDatosValidosBasicos()) {
        cout << "ERROR: El producto no cumple las validaciones basicas.\n";
        return;
    }

    cout << "\nConfirmar guardado? (S/N): ";
    leerLinea(buffer, 10);
    if (toupper(static_cast<unsigned char>(buffer[0])) != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (GestorArchivos::guardarRegistro<Producto>(Constantes::ARCHIVO_PRODUCTOS, producto)) {
        cout << "Producto guardado correctamente.\n";
    } else {
        cout << "ERROR: No se pudo guardar el producto.\n";
    }
}

void listarProductos() {
    ifstream archivo(Constantes::ARCHIVO_PRODUCTOS, ios::binary);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo de productos.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        cout << "ERROR: No se pudo leer el header de productos.\n";
        archivo.close();
        return;
    }

    if (header.registrosActivos == 0) {
        cout << "\nNo hay productos registrados.\n";
        archivo.close();
        return;
    }

    cout << "\n==================== LISTADO DE PRODUCTOS ====================\n";

    Producto producto;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(GestorArchivos::obtenerOffset<Producto>(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));

        if (!archivo.good()) {
            cout << "ERROR: Fallo al leer un registro de producto.\n";
            archivo.close();
            return;
        }

        if (producto.isEliminado()) {
            continue;
        }

        ProveedorLegacy proveedor;
        bool proveedorExiste = leerProveedorPorIDLegacy(producto.getIdProveedor(), proveedor);

        cout << "\nID           : " << producto.getId() << "\n";
        cout << "Codigo       : " << producto.getCodigo() << "\n";
        cout << "Nombre       : " << producto.getNombre() << "\n";
        cout << "Precio       : " << Formatos::formatearMoneda(producto.getPrecio()) << "\n";
        cout << "Stock        : " << producto.getStock() << "\n";
        cout << "Total vendido: " << producto.getTotalVendido() << "\n";
        cout << "Proveedor    : ";
        if (proveedorExiste) {
            cout << proveedor.nombre << " (ID: " << proveedor.id << ")\n";
        } else {
            cout << "No encontrado (ID: " << producto.getIdProveedor() << ")\n";
        }
    }

    archivo.close();
}

void buscarProductoPorID() {
    char buffer[50];

    cout << "\nID del producto a buscar: ";
    leerLinea(buffer, 50);
    int idBuscado = atoi(buffer);

    Producto producto;
    if (!leerProductoPorID(idBuscado, producto)) {
        cout << "ERROR: No existe un producto activo con ese ID.\n";
        return;
    }

    ProveedorLegacy proveedor;
    bool proveedorExiste = leerProveedorPorIDLegacy(producto.getIdProveedor(), proveedor);

    cout << "\n========== PRODUCTO ENCONTRADO ==========\n";
    cout << "ID           : " << producto.getId() << "\n";
    cout << "Codigo       : " << producto.getCodigo() << "\n";
    cout << "Nombre       : " << producto.getNombre() << "\n";
    cout << "Precio       : " << Formatos::formatearMoneda(producto.getPrecio()) << "\n";
    cout << "Stock        : " << producto.getStock() << "\n";
    cout << "Proveedor    : ";
    if (proveedorExiste) {
        cout << proveedor.nombre << " (ID: " << proveedor.id << ")\n";
    } else {
        cout << "No encontrado (ID: " << producto.getIdProveedor() << ")\n";
    }
}