#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

// ==============================
// ESTRUCTURAS BASE DE FASE 2
// ==============================

struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};

struct Producto {
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    float precio;
    int stock;
    int idProveedor;
    int stockMinimo;
    int totalVendido;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Proveedor {
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

struct Cliente {
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    float totalGastado;
    int cantidadCompras;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct DetalleTransaccion {
    int idProducto;
    int cantidad;
    float precioUnitario;
    float subtotal;
};

struct Transaccion {
    int id;
    char tipo[10]; // "COMPRA" o "VENTA"
    int idRelacionado; // proveedor o cliente
    int cantidadDetalles;
    DetalleTransaccion detalles[20];
    float total;
    bool cancelada;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Tienda {
    char nombre[100];
    char rif[20];
    int totalProductos;
    int totalProveedores;
    int totalClientes;
    int totalTransacciones;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

// ==============================
// NOMBRES DE ARCHIVOS
// ==============================

const char* ARCHIVO_TIENDA = "tienda.bin";
const char* ARCHIVO_PRODUCTOS = "productos.bin";
const char* ARCHIVO_PROVEEDORES = "proveedores.bin";
const char* ARCHIVO_CLIENTES = "clientes.bin";
const char* ARCHIVO_TRANSACCIONES = "transacciones.bin";

// ==============================
// FUNCIONES DE HEADER
// ==============================

bool inicializarArchivo(const char* nombreArchivo) {
    ifstream archivoEntrada(nombreArchivo, ios::binary);
    if (archivoEntrada.good()) {
        archivoEntrada.close();
        return true;
    }
    archivoEntrada.close();

    ofstream archivoSalida(nombreArchivo, ios::binary);
    if (!archivoSalida.is_open()) {
        return false;
    }

    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoID = 1;
    header.registrosActivos = 0;
    header.version = 1;

    archivoSalida.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivoSalida.close();

    return true;
}

ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoID = 1;
    header.registrosActivos = 0;
    header.version = 1;

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        return header;
    }

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    return header;
}

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    return true;
}

// ==============================
// INICIALIZACION GENERAL
// ==============================

bool inicializarSistemaArchivos() {
    bool ok = true;

    ok = ok && inicializarArchivo(ARCHIVO_TIENDA);
    ok = ok && inicializarArchivo(ARCHIVO_PRODUCTOS);
    ok = ok && inicializarArchivo(ARCHIVO_PROVEEDORES);
    ok = ok && inicializarArchivo(ARCHIVO_CLIENTES);
    ok = ok && inicializarArchivo(ARCHIVO_TRANSACCIONES);

    return ok;
}

// ==============================
// UTILIDADES DE PRUEBA
// ==============================

void mostrarHeader(const char* nombreArchivo) {
    ArchivoHeader header = leerHeader(nombreArchivo);

    cout << "\nArchivo: " << nombreArchivo << "\n";
    cout << "Cantidad de registros : " << header.cantidadRegistros << "\n";
    cout << "Proximo ID            : " << header.proximoID << "\n";
    cout << "Registros activos     : " << header.registrosActivos << "\n";
    cout << "Version               : " << header.version << "\n";
}

// ==============================
// MAIN
// ==============================

int main() {
    if (!inicializarSistemaArchivos()) {
        cout << "ERROR: No se pudieron inicializar los archivos binarios.\n";
        return 1;
    }

    cout << "Sistema de archivos binarios inicializado correctamente.\n";

    mostrarHeader(ARCHIVO_TIENDA);
    mostrarHeader(ARCHIVO_PRODUCTOS);
    mostrarHeader(ARCHIVO_PROVEEDORES);
    mostrarHeader(ARCHIVO_CLIENTES);
    mostrarHeader(ARCHIVO_TRANSACCIONES);

    return 0;
}