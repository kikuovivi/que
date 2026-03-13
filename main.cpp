#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#define _CRT_SECURE_NO_WARNINGS

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
    char tipo[10];
    int idRelacionado;
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
// FUNCIONES AUXILIARES
// ==============================

void leerLinea(char* buffer, int tam) {
    cin.getline(buffer, tam);
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

bool validarEmail(const char* email) {
    const char* arroba = strchr(email, '@');
    if (!arroba) return false;

    const char* punto = strchr(arroba, '.');
    if (!punto) return false;

    return punto > arroba + 1;
}

void mostrarFechaHora(time_t fecha) {
    tm* tiempoLocal = localtime(&fecha);
    cout << (tiempoLocal->tm_year + 1900) << "-";
    if (tiempoLocal->tm_mon + 1 < 10) cout << "0";
    cout << (tiempoLocal->tm_mon + 1) << "-";
    if (tiempoLocal->tm_mday < 10) cout << "0";
    cout << tiempoLocal->tm_mday << " ";
    if (tiempoLocal->tm_hour < 10) cout << "0";
    cout << tiempoLocal->tm_hour << ":";
    if (tiempoLocal->tm_min < 10) cout << "0";
    cout << tiempoLocal->tm_min << ":";
    if (tiempoLocal->tm_sec < 10) cout << "0";
    cout << tiempoLocal->tm_sec;
}

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
// FUNCIONES DE PROVEEDORES
// ==============================

long long obtenerOffsetProveedor(int indice) {
    return sizeof(ArchivoHeader) + static_cast<long long>(indice) * sizeof(Proveedor);
}

bool rifProveedorDuplicado(const char* rifBuscado) {
    ifstream archivo(ARCHIVO_PROVEEDORES, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Proveedor proveedor;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(obtenerOffsetProveedor(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));

        if (!proveedor.eliminado && strcmp(proveedor.rif, rifBuscado) == 0) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

bool guardarProveedor(Proveedor proveedor) {
    ArchivoHeader header = leerHeader(ARCHIVO_PROVEEDORES);

    proveedor.id = header.proximoID;
    proveedor.eliminado = false;
    proveedor.fechaCreacion = time(nullptr);
    proveedor.fechaUltimaModificacion = proveedor.fechaCreacion;

    ofstream archivo(ARCHIVO_PROVEEDORES, ios::binary | ios::app);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.write(reinterpret_cast<const char*>(&proveedor), sizeof(Proveedor));
    archivo.close();

    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID++;

    return actualizarHeader(ARCHIVO_PROVEEDORES, header);
}

int buscarIndiceProveedorPorID(int idBuscado) {
    ifstream archivo(ARCHIVO_PROVEEDORES, ios::binary);
    if (!archivo.is_open()) {
        return -1;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Proveedor proveedor;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(obtenerOffsetProveedor(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));

        if (!proveedor.eliminado && proveedor.id == idBuscado) {
            archivo.close();
            return i;
        }
    }

    archivo.close();
    return -1;
}

bool leerProveedorPorIndice(int indice, Proveedor& proveedor) {
    ifstream archivo(ARCHIVO_PROVEEDORES, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.seekg(obtenerOffsetProveedor(indice), ios::beg);
    archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));
    archivo.close();

    return true;
}

bool leerProveedorPorID(int idBuscado, Proveedor& proveedor) {
    int indice = buscarIndiceProveedorPorID(idBuscado);
    if (indice == -1) {
        return false;
    }
    return leerProveedorPorIndice(indice, proveedor);
}

void registrarProveedor() {
    Proveedor proveedor;
    char buffer[300];

    cout << "\n========== REGISTRAR PROVEEDOR ==========\n";

    cout << "Nombre: ";
    leerLinea(proveedor.nombre, 100);
    if (strlen(proveedor.nombre) == 0) {
        cout << "ERROR: El nombre no puede estar vacio.\n";
        return;
    }

    cout << "RIF: ";
    leerLinea(proveedor.rif, 20);
    if (strlen(proveedor.rif) == 0) {
        cout << "ERROR: El RIF no puede estar vacio.\n";
        return;
    }

    if (rifProveedorDuplicado(proveedor.rif)) {
        cout << "ERROR: Ya existe un proveedor con ese RIF.\n";
        return;
    }

    cout << "Telefono: ";
    leerLinea(proveedor.telefono, 20);

    cout << "Email: ";
    leerLinea(proveedor.email, 100);
    if (!validarEmail(proveedor.email)) {
        cout << "ERROR: El email no tiene un formato valido.\n";
        return;
    }

    cout << "Direccion: ";
    leerLinea(proveedor.direccion, 200);

    cout << "\nResumen del proveedor:\n";
    cout << "Nombre    : " << proveedor.nombre << "\n";
    cout << "RIF       : " << proveedor.rif << "\n";
    cout << "Telefono  : " << proveedor.telefono << "\n";
    cout << "Email     : " << proveedor.email << "\n";
    cout << "Direccion : " << proveedor.direccion << "\n";

    cout << "\nConfirmar guardado? (S/N): ";
    leerLinea(buffer, 10);

    if (toupper(buffer[0]) != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (guardarProveedor(proveedor)) {
        cout << "Proveedor guardado correctamente.\n";
    } else {
        cout << "ERROR: No se pudo guardar el proveedor.\n";
    }
}

void listarProveedores() {
    ifstream archivo(ARCHIVO_PROVEEDORES, ios::binary);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo de proveedores.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    if (header.registrosActivos == 0) {
        cout << "\nNo hay proveedores registrados.\n";
        archivo.close();
        return;
    }

    cout << "\n==================== LISTADO DE PROVEEDORES ====================\n";

    Proveedor proveedor;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(obtenerOffsetProveedor(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));

        if (proveedor.eliminado) {
            continue;
        }

        cout << "\nID        : " << proveedor.id << "\n";
        cout << "Nombre    : " << proveedor.nombre << "\n";
        cout << "RIF       : " << proveedor.rif << "\n";
        cout << "Telefono  : " << proveedor.telefono << "\n";
        cout << "Email     : " << proveedor.email << "\n";
        cout << "Direccion : " << proveedor.direccion << "\n";
        cout << "Creado    : ";
        mostrarFechaHora(proveedor.fechaCreacion);
        cout << "\n";
    }

    archivo.close();
}

void buscarProveedorPorID() {
    char buffer[50];
    cout << "\nID del proveedor a buscar: ";
    leerLinea(buffer, 50);

    int idBuscado = atoi(buffer);
    int indice = buscarIndiceProveedorPorID(idBuscado);

    if (indice == -1) {
        cout << "ERROR: No existe un proveedor activo con ese ID.\n";
        return;
    }

    Proveedor proveedor;
    if (!leerProveedorPorIndice(indice, proveedor)) {
        cout << "ERROR: No se pudo leer el proveedor.\n";
        return;
    }

    cout << "\n========== PROVEEDOR ENCONTRADO ==========\n";
    cout << "ID        : " << proveedor.id << "\n";
    cout << "Nombre    : " << proveedor.nombre << "\n";
    cout << "RIF       : " << proveedor.rif << "\n";
    cout << "Telefono  : " << proveedor.telefono << "\n";
    cout << "Email     : " << proveedor.email << "\n";
    cout << "Direccion : " << proveedor.direccion << "\n";
    cout << "Creado    : ";
    mostrarFechaHora(proveedor.fechaCreacion);
    cout << "\n";
}

// ==============================
// FUNCIONES DE PRODUCTOS
// ==============================

long long obtenerOffsetProducto(int indice) {
    return sizeof(ArchivoHeader) + static_cast<long long>(indice) * sizeof(Producto);
}

bool codigoProductoDuplicado(const char* codigoBuscado) {
    ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Producto producto;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(obtenerOffsetProducto(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));

        if (!producto.eliminado && strcmp(producto.codigo, codigoBuscado) == 0) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

bool guardarProducto(Producto producto) {
    ArchivoHeader header = leerHeader(ARCHIVO_PRODUCTOS);

    producto.id = header.proximoID;
    producto.eliminado = false;
    producto.totalVendido = 0;
    producto.fechaCreacion = time(nullptr);
    producto.fechaUltimaModificacion = producto.fechaCreacion;

    ofstream archivo(ARCHIVO_PRODUCTOS, ios::binary | ios::app);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.write(reinterpret_cast<const char*>(&producto), sizeof(Producto));
    archivo.close();

    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID++;

    return actualizarHeader(ARCHIVO_PRODUCTOS, header);
}

int buscarIndiceProductoPorID(int idBuscado) {
    ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
    if (!archivo.is_open()) {
        return -1;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Producto producto;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(obtenerOffsetProducto(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));

        if (!producto.eliminado && producto.id == idBuscado) {
            archivo.close();
            return i;
        }
    }

    archivo.close();
    return -1;
}

bool leerProductoPorIndice(int indice, Producto& producto) {
    ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.seekg(obtenerOffsetProducto(indice), ios::beg);
    archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
    archivo.close();

    return true;
}

void registrarProducto() {
    Producto producto;
    char buffer[300];

    cout << "\n========== REGISTRAR PRODUCTO ==========\n";

    cout << "Codigo: ";
    leerLinea(producto.codigo, 20);
    if (strlen(producto.codigo) == 0) {
        cout << "ERROR: El codigo no puede estar vacio.\n";
        return;
    }

    if (codigoProductoDuplicado(producto.codigo)) {
        cout << "ERROR: Ya existe un producto con ese codigo.\n";
        return;
    }

    cout << "Nombre: ";
    leerLinea(producto.nombre, 100);
    if (strlen(producto.nombre) == 0) {
        cout << "ERROR: El nombre no puede estar vacio.\n";
        return;
    }

    cout << "Descripcion: ";
    leerLinea(producto.descripcion, 200);

    cout << "ID del proveedor: ";
    leerLinea(buffer, 20);
    producto.idProveedor = atoi(buffer);

    Proveedor proveedor;
    if (!leerProveedorPorID(producto.idProveedor, proveedor)) {
        cout << "ERROR: No existe un proveedor activo con ese ID.\n";
        return;
    }

    cout << "Precio: ";
    leerLinea(buffer, 30);
    producto.precio = static_cast<float>(atof(buffer));
    if (producto.precio <= 0) {
        cout << "ERROR: El precio debe ser mayor a 0.\n";
        return;
    }

    cout << "Stock inicial: ";
    leerLinea(buffer, 30);
    producto.stock = atoi(buffer);
    if (producto.stock < 0) {
        cout << "ERROR: El stock no puede ser negativo.\n";
        return;
    }

    cout << "Stock minimo: ";
    leerLinea(buffer, 30);
    producto.stockMinimo = atoi(buffer);
    if (producto.stockMinimo < 0) {
        cout << "ERROR: El stock minimo no puede ser negativo.\n";
        return;
    }

    cout << "\nResumen del producto:\n";
    cout << "Codigo       : " << producto.codigo << "\n";
    cout << "Nombre       : " << producto.nombre << "\n";
    cout << "Descripcion  : " << producto.descripcion << "\n";
    cout << "Proveedor    : " << proveedor.nombre << " (ID: " << proveedor.id << ")\n";
    cout << "Precio       : " << producto.precio << "\n";
    cout << "Stock        : " << producto.stock << "\n";
    cout << "Stock minimo : " << producto.stockMinimo << "\n";

    cout << "\nConfirmar guardado? (S/N): ";
    leerLinea(buffer, 10);

    if (toupper(buffer[0]) != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (guardarProducto(producto)) {
        cout << "Producto guardado correctamente.\n";
    } else {
        cout << "ERROR: No se pudo guardar el producto.\n";
    }
}

void listarProductos() {
    ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo de productos.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    if (header.registrosActivos == 0) {
        cout << "\nNo hay productos registrados.\n";
        archivo.close();
        return;
    }

    cout << "\n==================== LISTADO DE PRODUCTOS ====================\n";

    Producto producto;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(obtenerOffsetProducto(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));

        if (producto.eliminado) {
            continue;
        }

        Proveedor proveedor;
        bool proveedorExiste = leerProveedorPorID(producto.idProveedor, proveedor);

        cout << "\nID           : " << producto.id << "\n";
        cout << "Codigo       : " << producto.codigo << "\n";
        cout << "Nombre       : " << producto.nombre << "\n";
        cout << "Descripcion  : " << producto.descripcion << "\n";
        cout << "Precio       : " << producto.precio << "\n";
        cout << "Stock        : " << producto.stock << "\n";
        cout << "Stock minimo : " << producto.stockMinimo << "\n";
        cout << "Proveedor    : ";
        if (proveedorExiste) {
            cout << proveedor.nombre << " (ID: " << proveedor.id << ")\n";
        } else {
            cout << "No encontrado (ID: " << producto.idProveedor << ")\n";
        }
        cout << "Creado       : ";
        mostrarFechaHora(producto.fechaCreacion);
        cout << "\n";
    }

    archivo.close();
}

void buscarProductoPorID() {
    char buffer[50];
    cout << "\nID del producto a buscar: ";
    leerLinea(buffer, 50);

    int idBuscado = atoi(buffer);
    int indice = buscarIndiceProductoPorID(idBuscado);

    if (indice == -1) {
        cout << "ERROR: No existe un producto activo con ese ID.\n";
        return;
    }

    Producto producto;
    if (!leerProductoPorIndice(indice, producto)) {
        cout << "ERROR: No se pudo leer el producto.\n";
        return;
    }

    Proveedor proveedor;
    bool proveedorExiste = leerProveedorPorID(producto.idProveedor, proveedor);

    cout << "\n========== PRODUCTO ENCONTRADO ==========\n";
    cout << "ID           : " << producto.id << "\n";
    cout << "Codigo       : " << producto.codigo << "\n";
    cout << "Nombre       : " << producto.nombre << "\n";
    cout << "Descripcion  : " << producto.descripcion << "\n";
    cout << "Precio       : " << producto.precio << "\n";
    cout << "Stock        : " << producto.stock << "\n";
    cout << "Stock minimo : " << producto.stockMinimo << "\n";
    cout << "Proveedor    : ";
    if (proveedorExiste) {
        cout << proveedor.nombre << " (ID: " << proveedor.id << ")\n";
    } else {
        cout << "No encontrado (ID: " << producto.idProveedor << ")\n";
    }
    cout << "Creado       : ";
    mostrarFechaHora(producto.fechaCreacion);
    cout << "\n";
}

// ==============================
// FUNCIONES DE PRUEBA
// ==============================

void mostrarHeader(const char* nombreArchivo) {
    ArchivoHeader header = leerHeader(nombreArchivo);

    cout << "\nArchivo: " << nombreArchivo << "\n";
    cout << "Cantidad de registros : " << header.cantidadRegistros << "\n";
    cout << "Proximo ID            : " << header.proximoID << "\n";
    cout << "Registros activos     : " << header.registrosActivos << "\n";
    cout << "Version               : " << header.version << "\n";
}

void menuPrincipal() {
    char buffer[20];
    int opcion = -1;

    do {
        cout << "\n==================== MENU PRINCIPAL ====================\n";
        cout << "1. Registrar proveedor\n";
        cout << "2. Listar proveedores\n";
        cout << "3. Buscar proveedor por ID\n";
        cout << "4. Mostrar header de proveedores\n";
        cout << "5. Registrar producto\n";
        cout << "6. Listar productos\n";
        cout << "7. Buscar producto por ID\n";
        cout << "8. Mostrar header de productos\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        leerLinea(buffer, 20);

        opcion = atoi(buffer);

        switch (opcion) {
            case 1:
                registrarProveedor();
                break;
            case 2:
                listarProveedores();
                break;
            case 3:
                buscarProveedorPorID();
                break;
            case 4:
                mostrarHeader(ARCHIVO_PROVEEDORES);
                break;
            case 5:
                registrarProducto();
                break;
            case 6:
                listarProductos();
                break;
            case 7:
                buscarProductoPorID();
                break;
            case 8:
                mostrarHeader(ARCHIVO_PRODUCTOS);
                break;
            case 0:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "ERROR: Opcion invalida.\n";
        }

    } while (opcion != 0);
}

// ==============================
// MAIN
// ==============================

int main() {
    if (!inicializarSistemaArchivos()) {
        cout << "ERROR: No se pudieron inicializar los archivos binarios.\n";
        return 1;
    }

    menuPrincipal();

    return 0;
}
