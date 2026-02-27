#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <ctime>
#include <cctype>
#include <cstdio>
#include <windows.h>
using namespace std;

struct Producto {
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    int idProveedor;
    float precio;
    int stock;
    char fechaRegistro[11];
    bool activo;
};

struct Proveedor {
    int id;
    char nombre[100];
    char rif[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
    bool activo;
};

struct Cliente {
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
    bool activo;
};

struct Transaccion {
    int id;
    char tipo[10];       
    int idProducto;
    int idRelacionado;  
    int cantidad;
    float precioUnitario;
    float total;
    char fecha[11];
    char descripcion[200];
    bool cancelada;
};

struct Tienda {
    char nombre[100];
    char rif[20];

    Producto* productos;
    int numProductos;
    int capacidadProductos;

    Proveedor* proveedores;
    int numProveedores;
    int capacidadProveedores;

    Cliente* clientes;
    int numClientes;
    int capacidadClientes;

    Transaccion* transacciones;
    int numTransacciones;
    int capacidadTransacciones;

    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

void obtenerFechaActual(char* buffer) {
    time_t t = time(nullptr);
    tm* local = localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d",
        local->tm_year + 1900,
        local->tm_mon + 1,
        local->tm_mday);
}

void convertirAMinusculas(char* cadena) {
    for (int i = 0; cadena[i]; i++)
        cadena[i] = tolower((unsigned char)cadena[i]);
}

bool contieneSubstring(const char* texto, const char* busqueda) {
    char t[512], b[512];
    strncpy(t, texto, 511); t[511] = '\0';
    strncpy(b, busqueda, 511); b[511] = '\0';
    convertirAMinusculas(t);
    convertirAMinusculas(b);
    return strstr(t, b) != nullptr;
}

bool validarEmail(const char* email) {
    const char* at = strchr(email, '@');
    if (!at) return false;
    const char* dot = strchr(at, '.');
    return dot != nullptr && dot > at + 1;
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit((unsigned char)fecha[i])) return false;
    }
    int y, m, d;
    sscanf(fecha, "%d-%d-%d", &y, &m, &d);
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    return true;
}

void leerLinea(char* buffer, int maxLen) {
    cin.getline(buffer, maxLen);
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

bool esCancelar(const char* str) {
    char tmp[512];
    strncpy(tmp, str, 511); tmp[511] = '\0';
    convertirAMinusculas(tmp);
    return (strcmp(tmp, "cancelar") == 0 || strcmp(tmp, "0") == 0);
}

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(10000, '\n');
}

void inicializarTienda(Tienda* t, const char* nombre, const char* rif) {
    strncpy(t->nombre, nombre, 99); t->nombre[99] = '\0';
    strncpy(t->rif, rif, 19);      t->rif[19] = '\0';

    t->capacidadProductos = 5;
    t->numProductos = 0;
    t->productos = new Producto[5];

    t->capacidadProveedores = 5;
    t->numProveedores = 0;
    t->proveedores = new Proveedor[5];

    t->capacidadClientes = 5;
    t->numClientes = 0;
    t->clientes = new Cliente[5];

    t->capacidadTransacciones = 5;
    t->numTransacciones = 0;
    t->transacciones = new Transaccion[5];

    t->siguienteIdProducto = 1;
    t->siguienteIdProveedor = 1;
    t->siguienteIdCliente = 1;
    t->siguienteIdTransaccion = 1;
}

void liberarTienda(Tienda* t) {
    delete[] t->productos;
    delete[] t->proveedores;
    delete[] t->clientes;
    delete[] t->transacciones;
    t->productos = nullptr;
    t->proveedores = nullptr;
    t->clientes = nullptr;
    t->transacciones = nullptr;
}

void redimensionarProductos(Tienda* t) {
    int nueva = t->capacidadProductos * 2;
    Producto* arr = new Producto[nueva];
    for (int i = 0; i < t->numProductos; i++) arr[i] = t->productos[i];
    delete[] t->productos;
    t->productos = arr;
    t->capacidadProductos = nueva;
}

void redimensionarProveedores(Tienda* t) {
    int nueva = t->capacidadProveedores * 2;
    Proveedor* arr = new Proveedor[nueva];
    for (int i = 0; i < t->numProveedores; i++) arr[i] = t->proveedores[i];
    delete[] t->proveedores;
    t->proveedores = arr;
    t->capacidadProveedores = nueva;
}

void redimensionarClientes(Tienda* t) {
    int nueva = t->capacidadClientes * 2;
    Cliente* arr = new Cliente[nueva];
    for (int i = 0; i < t->numClientes; i++) arr[i] = t->clientes[i];
    delete[] t->clientes;
    t->clientes = arr;
    t->capacidadClientes = nueva;
}

void redimensionarTransacciones(Tienda* t) {
    int nueva = t->capacidadTransacciones * 2;
    Transaccion* arr = new Transaccion[nueva];
    for (int i = 0; i < t->numTransacciones; i++) arr[i] = t->transacciones[i];
    delete[] t->transacciones;
    t->transacciones = arr;
    t->capacidadTransacciones = nueva;
}

int buscarProductoPorId(Tienda* t, int id) {
    for (int i = 0; i < t->numProductos; i++)
        if (t->productos[i].id == id && t->productos[i].activo) return i;
    return -1;
}

int buscarProveedorPorId(Tienda* t, int id) {
    for (int i = 0; i < t->numProveedores; i++)
        if (t->proveedores[i].id == id && t->proveedores[i].activo) return i;
    return -1;
}

int buscarClientePorId(Tienda* t, int id) {
    for (int i = 0; i < t->numClientes; i++)
        if (t->clientes[i].id == id && t->clientes[i].activo) return i;
    return -1;
}

bool existeProducto(Tienda* t, int id) { return buscarProductoPorId(t, id) != -1; }
bool existeProveedor(Tienda* t, int id) { return buscarProveedorPorId(t, id) != -1; }
bool existeCliente(Tienda* t, int id) { return buscarClientePorId(t, id) != -1; }

bool codigoDuplicado(Tienda* t, const char* codigo) {
    for (int i = 0; i < t->numProductos; i++)
        if (t->productos[i].activo && strcmp(t->productos[i].codigo, codigo) == 0)
            return true;
    return false;
}

bool rifProveedorDuplicado(Tienda* t, const char* rif) {
    for (int i = 0; i < t->numProveedores; i++)
        if (t->proveedores[i].activo && strcmp(t->proveedores[i].rif, rif) == 0)
            return true;
    return false;
}

bool cedulaDuplicada(Tienda* t, const char* cedula) {
    for (int i = 0; i < t->numClientes; i++)
        if (t->clientes[i].activo && strcmp(t->clientes[i].cedula, cedula) == 0)
            return true;
    return false;
}

const char* nombreProveedor(Tienda* t, int id) {
    int idx = buscarProveedorPorId(t, id);
    if (idx == -1) return "Desconocido";
    return t->proveedores[idx].nombre;
}

void mostrarProducto(Tienda* t, int idx) {
    Producto& p = t->productos[idx];
    cout << "  ID         : " << p.id << "\n";
    cout << "  Codigo     : " << p.codigo << "\n";
    cout << "  Nombre     : " << p.nombre << "\n";
    cout << "  Descripcion: " << p.descripcion << "\n";
    cout << "  Proveedor  : " << nombreProveedor(t, p.idProveedor) << " (ID: " << p.idProveedor << ")\n";
    cout << "  Precio     : " << p.precio << "\n";
    cout << "  Stock      : " << p.stock << "\n";
    cout << "  Fecha      : " << p.fechaRegistro << "\n";
}

void mostrarProveedor(int idx, Proveedor* arr) {
    Proveedor& p = arr[idx];
    cout << "  ID         : " << p.id << "\n";
    cout << "  Nombre     : " << p.nombre << "\n";
    cout << "  RIF        : " << p.rif << "\n";
    cout << "  Telefono   : " << p.telefono << "\n";
    cout << "  Email      : " << p.email << "\n";
    cout << "  Direccion  : " << p.direccion << "\n";
    cout << "  Fecha      : " << p.fechaRegistro << "\n";
}

void mostrarCliente(int idx, Cliente* arr) {
    Cliente& c = arr[idx];
    cout << "  ID         : " << c.id << "\n";
    cout << "  Nombre     : " << c.nombre << "\n";
    cout << "  Cedula/RIF : " << c.cedula << "\n";
    cout << "  Telefono   : " << c.telefono << "\n";
    cout << "  Email      : " << c.email << "\n";
    cout << "  Direccion  : " << c.direccion << "\n";
    cout << "  Fecha      : " << c.fechaRegistro << "\n";
}

void mostrarTransaccion(Tienda* t, int idx) {
    Transaccion& tr = t->transacciones[idx];
    cout << "\n+-----------------------------------------------------------+\n";
    cout << "|              DETALLE DE TRANSACCION                      |\n";
    cout << "+-----------------------------------------------------------+\n";
    cout << "| ID           : " << tr.id;
    if (tr.cancelada) cout << " [CANCELADA]";
    cout << "\n";
    cout << "| Tipo         : " << tr.tipo << "\n";
    int ip = buscarProductoPorId(t, tr.idProducto);
    cout << "| Producto     : " << (ip >= 0 ? t->productos[ip].nombre : "N/A") << " (ID: " << tr.idProducto << ")\n";
    if (strcmp(tr.tipo, "VENTA") == 0) {
        int ic = buscarClientePorId(t, tr.idRelacionado);
        cout << "| Cliente      : " << (ic >= 0 ? t->clientes[ic].nombre : "N/A") << " (ID: " << tr.idRelacionado << ")\n";
    }
    else {
        int iv = buscarProveedorPorId(t, tr.idRelacionado);
        cout << "| Proveedor    : " << (iv >= 0 ? t->proveedores[iv].nombre : "N/A") << " (ID: " << tr.idRelacionado << ")\n";
    }
    cout << "| Cantidad     : " << tr.cantidad << "\n";
    printf("| Precio Unit. : $%.2f\n", tr.precioUnitario);
    printf("| Total        : $%.2f\n", tr.total);
    cout << "| Fecha        : " << tr.fecha << "\n";
    cout << "| Descripcion  : " << tr.descripcion << "\n";
    cout << "+-----------------------------------------------------------+\n";
}

void crearProducto(Tienda* t) {
    char resp[10];
    cout << "\n¿Desea registrar un nuevo producto? (S/N): ";
    leerLinea(resp, 10);
    if (toupper(resp[0]) != 'S') return;

    Producto p;
    p.activo = true;
    char buf[512];

    while (true) {
        cout << "Codigo (o CANCELAR): ";
        leerLinea(p.codigo, 20);
        if (esCancelar(p.codigo)) { cout << "Operacion cancelada.\n"; return; }
        if (strlen(p.codigo) == 0) { cout << "ERROR: El codigo no puede estar vacio.\n"; continue; }
        if (codigoDuplicado(t, p.codigo)) { cout << "ERROR: El codigo '" << p.codigo << "' ya esta registrado.\n"; continue; }
        break;
    }

    cout << "Nombre (o CANCELAR): ";
    leerLinea(p.nombre, 100);
    if (esCancelar(p.nombre)) { cout << "Operacion cancelada.\n"; return; }

    cout << "Descripcion (o CANCELAR): ";
    leerLinea(p.descripcion, 200);
    if (esCancelar(p.descripcion)) { cout << "Operacion cancelada.\n"; return; }

    while (true) {
        cout << "ID de Proveedor (o CANCELAR): ";
        leerLinea(buf, 20);
        if (esCancelar(buf)) { cout << "Operacion cancelada.\n"; return; }
        p.idProveedor = atoi(buf);
        if (!existeProveedor(t, p.idProveedor)) {
            cout << "ERROR: El proveedor con ID " << p.idProveedor << " no existe.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Precio (o CANCELAR): ";
        leerLinea(buf, 20);
        if (esCancelar(buf)) { cout << "Operacion cancelada.\n"; return; }
        p.precio = (float)atof(buf);
        if (p.precio <= 0) { cout << "ERROR: El precio debe ser mayor a 0.\n"; continue; }
        break;
    }

    while (true) {
        cout << "Stock inicial (o CANCELAR): ";
        leerLinea(buf, 20);
        if (esCancelar(buf)) { cout << "Operacion cancelada.\n"; return; }
        p.stock = atoi(buf);
        if (p.stock < 0) { cout << "ERROR: El stock no puede ser negativo.\n"; continue; }
        break;
    }

    obtenerFechaActual(p.fechaRegistro);
    p.id = t->siguienteIdProducto;

    cout << "\n--- Resumen del nuevo producto ---\n";
    cout << "  Codigo    : " << p.codigo << "\n";
    cout << "  Nombre    : " << p.nombre << "\n";
    cout << "  Proveedor : " << nombreProveedor(t, p.idProveedor) << "\n";
    printf("  Precio    : %.2f\n", p.precio);
    cout << "  Stock     : " << p.stock << "\n";

    cout << "\n¿Guardar producto? (S/N): ";
    leerLinea(resp, 10);
    if (toupper(resp[0]) != 'S') { cout << "Producto no guardado.\n"; return; }

    if (t->numProductos == t->capacidadProductos) redimensionarProductos(t);
    t->productos[t->numProductos++] = p;
    t->siguienteIdProducto++;
    cout << "Producto registrado con ID " << p.id << ".\n";
}

void listarProductos(Tienda* t) {
    if (t->numProductos == 0) { cout << "No hay productos registrados.\n"; return; }
    cout << "\n+--------------------------------------------------------------------------+\n";
    cout << "|                         LISTADO DE PRODUCTOS                            |\n";
    cout << "+----+----------+----------------------+----------------+-------+--------+\n";
    cout << "| ID |  Codigo  |       Nombre         |   Proveedor    |Precio | Stock  |\n";
    cout << "+----+----------+----------------------+----------------+-------+--------+\n";
    for (int i = 0; i < t->numProductos; i++) {
        if (!t->productos[i].activo) continue;
        Producto& p = t->productos[i];
        printf("|%3d | %-8s | %-20s | %-14s |%6.2f |%7d |\n",
            p.id, p.codigo, p.nombre, nombreProveedor(t, p.idProveedor),
            p.precio, p.stock);
    }
    cout << "+----+----------+----------------------+----------------+-------+--------+\n";
}

void buscarProducto(Tienda* t) {
    cout << "\n--- Buscar Producto ---\n";
    cout << "1. Buscar por ID\n2. Buscar por nombre\n3. Buscar por codigo\n4. Listar por proveedor\n0. Cancelar\nOpcion: ";
    char buf[512];
    leerLinea(buf, 10);
    int op = atoi(buf);

    if (op == 0) return;

    if (op == 1) {
        cout << "ID: ";
        leerLinea(buf, 20);
        int id = atoi(buf);
        int idx = buscarProductoPorId(t, id);
        if (idx == -1) { cout << "ERROR: Producto con ID " << id << " no encontrado.\n"; return; }
        mostrarProducto(t, idx);
    }
    else if (op == 2 || op == 3) {
        cout << (op == 2 ? "Nombre" : "Codigo") << " (parcial): ";
        leerLinea(buf, 100);
        bool encontrado = false;
        for (int i = 0; i < t->numProductos; i++) {
            if (!t->productos[i].activo) continue;
            const char* campo = (op == 2) ? t->productos[i].nombre : t->productos[i].codigo;
            if (contieneSubstring(campo, buf)) {
                cout << "\n";
                mostrarProducto(t, i);
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron resultados.\n";
    }
    else if (op == 4) {
        cout << "ID de Proveedor: ";
        leerLinea(buf, 20);
        int idProv = atoi(buf);
        bool encontrado = false;
        for (int i = 0; i < t->numProductos; i++) {
            if (!t->productos[i].activo) continue;
            if (t->productos[i].idProveedor == idProv) {
                cout << "\n";
                mostrarProducto(t, i);
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No hay productos para ese proveedor.\n";
    }
}

void actualizarProducto(Tienda* t) {
    char buf[512];
    cout << "ID del producto a actualizar: ";
    leerLinea(buf, 20);
    int id = atoi(buf);
    int idx = buscarProductoPorId(t, id);
    if (idx == -1) { cout << "ERROR: Producto con ID " << id << " no encontrado.\n"; return; }

    Producto& p = t->productos[idx];
    int op;
    do {
        cout << "\n--- Datos actuales ---\n";
        mostrarProducto(t, idx);
        cout << "\n¿Que desea editar?\n";
        cout << "1. Codigo\n2. Nombre\n3. Descripcion\n4. Proveedor\n5. Precio\n6. Stock\n7. Guardar cambios\n0. Cancelar sin guardar\nOpcion: ";
        leerLinea(buf, 10);
        op = atoi(buf);

        if (op == 1) {
            cout << "Nuevo codigo: ";
            leerLinea(buf, 20);
            if (strlen(buf) > 0) {
                if (codigoDuplicado(t, buf) && strcmp(buf, p.codigo) != 0)
                    cout << "ERROR: Codigo duplicado.\n";
                else strncpy(p.codigo, buf, 19);
            }
        }
        else if (op == 2) {
            cout << "Nuevo nombre: ";
            leerLinea(p.nombre, 100);
        }
        else if (op == 3) {
            cout << "Nueva descripcion: ";
            leerLinea(p.descripcion, 200);
        }
        else if (op == 4) {
            cout << "Nuevo ID de proveedor: ";
            leerLinea(buf, 20);
            int idProv = atoi(buf);
            if (!existeProveedor(t, idProv)) cout << "ERROR: Proveedor no existe.\n";
            else p.idProveedor = idProv;
        }
        else if (op == 5) {
            cout << "Nuevo precio: ";
            leerLinea(buf, 20);
            float pr = (float)atof(buf);
            if (pr <= 0) cout << "ERROR: Precio invalido.\n";
            else p.precio = pr;
        }
        else if (op == 6) {
            cout << "Nuevo stock: ";
            leerLinea(buf, 20);
            int s = atoi(buf);
            if (s < 0) cout << "ERROR: Stock negativo.\n";
            else p.stock = s;
        }
    } while (op != 7 && op != 0);

    if (op == 7) {
        char resp[10];
        cout << "¿Confirmar cambios? (S/N): ";
        leerLinea(resp, 10);
        if (toupper(resp[0]) == 'S') cout << "Producto actualizado.\n";
        else cout << "Cambios descartados.\n";
    }
    else {
        cout << "Edicion cancelada.\n";
    }
}

void actualizarStockProducto(Tienda* t) {
    char buf[512];
    cout << "ID del producto: ";
    leerLinea(buf, 20);
    int id = atoi(buf);
    int idx = buscarProductoPorId(t, id);
    if (idx == -1) { cout << "ERROR: Producto no encontrado.\n"; return; }

    cout << "Stock actual: " << t->productos[idx].stock << "\n";
    cout << "Ajuste (+/-): ";
    leerLinea(buf, 20);
    int ajuste = atoi(buf);
    int nuevo = t->productos[idx].stock + ajuste;
    if (nuevo < 0) { cout << "ERROR: Stock resultante negativo.\n"; return; }

    cout << "Nuevo stock sera: " << nuevo << ". Confirmar? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) == 'S') {
        t->productos[idx].stock = nuevo;
        cout << "Stock actualizado.\n";
    }
    else {
        cout << "Cambio cancelado.\n";
    }
}

void eliminarProducto(Tienda* t) {
    char buf[512];
    cout << "ID del producto a eliminar: ";
    leerLinea(buf, 20);
    int id = atoi(buf);
    int idx = buscarProductoPorId(t, id);
    if (idx == -1) { cout << "ERROR: Producto no encontrado.\n"; return; }

    mostrarProducto(t, idx);

    int cont = 0;
    for (int i = 0; i < t->numTransacciones; i++)
        if (t->transacciones[i].idProducto == id && !t->transacciones[i].cancelada) cont++;
    if (cont > 0) cout << "ADVERTENCIA: Este producto tiene " << cont << " transacciones asociadas.\n";

    cout << "¿Confirmar eliminacion? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) == 'S') {
        t->productos[idx].activo = false;
        cout << "Producto eliminado.\n";
    }
    else {
        cout << "Eliminacion cancelada.\n";
    }
}

void crearProveedor(Tienda* t) {
    char resp[10];
    cout << "\n¿Desea registrar un nuevo proveedor? (S/N): ";
    leerLinea(resp, 10);
    if (toupper(resp[0]) != 'S') return;

    Proveedor p;
    p.activo = true;
    char buf[512];

    cout << "Nombre (o CANCELAR): ";
    leerLinea(p.nombre, 100);
    if (esCancelar(p.nombre)) { cout << "Cancelado.\n"; return; }

    while (true) {
        cout << "RIF (o CANCELAR): ";
        leerLinea(p.rif, 20);
        if (esCancelar(p.rif)) { cout << "Cancelado.\n"; return; }
        if (rifProveedorDuplicado(t, p.rif)) { cout << "ERROR: RIF duplicado.\n"; continue; }
        break;
    }

    cout << "Telefono (o CANCELAR): ";
    leerLinea(p.telefono, 20);
    if (esCancelar(p.telefono)) { cout << "Cancelado.\n"; return; }

    while (true) {
        cout << "Email (o CANCELAR): ";
        leerLinea(p.email, 100);
        if (esCancelar(p.email)) { cout << "Cancelado.\n"; return; }
        if (!validarEmail(p.email)) { cout << "ERROR: Email invalido.\n"; continue; }
        break;
    }

    cout << "Direccion (o CANCELAR): ";
    leerLinea(p.direccion, 200);
    if (esCancelar(p.direccion)) { cout << "Cancelado.\n"; return; }

    obtenerFechaActual(p.fechaRegistro);
    p.id = t->siguienteIdProveedor;

    cout << "\n--- Resumen ---\n";
    cout << "  Nombre: " << p.nombre << "\n  RIF: " << p.rif << "\n  Email: " << p.email << "\n";

    cout << "¿Guardar? (S/N): ";
    leerLinea(resp, 10);
    if (toupper(resp[0]) != 'S') { cout << "No guardado.\n"; return; }

    if (t->numProveedores == t->capacidadProveedores) redimensionarProveedores(t);
    t->proveedores[t->numProveedores++] = p;
    t->siguienteIdProveedor++;
    cout << "Proveedor registrado con ID " << p.id << ".\n";
}

void listarProveedores(Tienda* t) {
    if (t->numProveedores == 0) { cout << "No hay proveedores registrados.\n"; return; }
    cout << "\n+---+----------------------+-----------+----------------------+\n";
    cout << "|ID |       Nombre         |    RIF    |       Email          |\n";
    cout << "+---+----------------------+-----------+----------------------+\n";
    for (int i = 0; i < t->numProveedores; i++) {
        if (!t->proveedores[i].activo) continue;
        Proveedor& p = t->proveedores[i];
        printf("|%2d | %-20s | %-9s | %-20s |\n", p.id, p.nombre, p.rif, p.email);
    }
    cout << "+---+----------------------+-----------+----------------------+\n";
}

void buscarProveedor(Tienda* t) {
    cout << "\n1. Buscar por ID\n2. Buscar por nombre\n3. Buscar por RIF\n0. Cancelar\nOpcion: ";
    char buf[512];
    leerLinea(buf, 10);
    int op = atoi(buf);
    if (op == 0) return;

    if (op == 1) {
        cout << "ID: ";
        leerLinea(buf, 20);
        int idx = buscarProveedorPorId(t, atoi(buf));
        if (idx == -1) { cout << "No encontrado.\n"; return; }
        mostrarProveedor(idx, t->proveedores);
    }
    else if (op == 2 || op == 3) {
        cout << (op == 2 ? "Nombre" : "RIF") << " (parcial): ";
        leerLinea(buf, 100);
        bool encontrado = false;
        for (int i = 0; i < t->numProveedores; i++) {
            if (!t->proveedores[i].activo) continue;
            const char* campo = (op == 2) ? t->proveedores[i].nombre : t->proveedores[i].rif;
            if (contieneSubstring(campo, buf)) {
                mostrarProveedor(i, t->proveedores);
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron resultados.\n";
    }
}

void actualizarProveedor(Tienda* t) {
    char buf[512];
    cout << "ID del proveedor: ";
    leerLinea(buf, 20);
    int idx = buscarProveedorPorId(t, atoi(buf));
    if (idx == -1) { cout << "ERROR: Proveedor no encontrado.\n"; return; }

    Proveedor& p = t->proveedores[idx];
    cout << "\n1. Nombre\n2. RIF\n3. Telefono\n4. Email\n5. Direccion\n0. Cancelar\nCampo a editar: ";
    leerLinea(buf, 10);
    int op = atoi(buf);

    if (op == 0) return;
    if (op == 1) { cout << "Nuevo nombre: "; leerLinea(p.nombre, 100); }
    else if (op == 2) { cout << "Nuevo RIF: "; leerLinea(p.rif, 20); }
    else if (op == 3) { cout << "Nuevo telefono: "; leerLinea(p.telefono, 20); }
    else if (op == 4) {
        while (true) {
            cout << "Nuevo email: ";
            leerLinea(p.email, 100);
            if (validarEmail(p.email)) break;
            cout << "ERROR: Email invalido.\n";
        }
    }
    else if (op == 5) { cout << "Nueva direccion: "; leerLinea(p.direccion, 200); }

    cout << "¿Guardar cambios? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) == 'S') cout << "Proveedor actualizado.\n";
    else cout << "Cambios descartados.\n";
}

void eliminarProveedor(Tienda* t) {
    char buf[512];
    cout << "ID del proveedor a eliminar: ";
    leerLinea(buf, 20);
    int id = atoi(buf);
    int idx = buscarProveedorPorId(t, id);
    if (idx == -1) { cout << "ERROR: Proveedor no encontrado.\n"; return; }

    int cont = 0;
    for (int i = 0; i < t->numProductos; i++)
        if (t->productos[i].activo && t->productos[i].idProveedor == id) cont++;

    if (cont > 0) {
        cout << "ADVERTENCIA: Este proveedor tiene " << cont << " productos asociados. No se puede eliminar.\n";
        return;
    }

    mostrarProveedor(idx, t->proveedores);
    cout << "¿Confirmar eliminacion? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) == 'S') { t->proveedores[idx].activo = false; cout << "Proveedor eliminado.\n"; }
    else cout << "Cancelado.\n";
}

void crearCliente(Tienda* t) {
    char resp[10];
    cout << "\n¿Desea registrar un nuevo cliente? (S/N): ";
    leerLinea(resp, 10);
    if (toupper(resp[0]) != 'S') return;

    Cliente c;
    c.activo = true;
    char buf[512];

    cout << "Nombre (o CANCELAR): ";
    leerLinea(c.nombre, 100);
    if (esCancelar(c.nombre)) { cout << "Cancelado.\n"; return; }

    while (true) {
        cout << "Cedula/RIF (o CANCELAR): ";
        leerLinea(c.cedula, 20);
        if (esCancelar(c.cedula)) { cout << "Cancelado.\n"; return; }
        if (cedulaDuplicada(t, c.cedula)) { cout << "ERROR: Cedula/RIF duplicado.\n"; continue; }
        break;
    }

    cout << "Telefono (o CANCELAR): ";
    leerLinea(c.telefono, 20);
    if (esCancelar(c.telefono)) { cout << "Cancelado.\n"; return; }

    while (true) {
        cout << "Email (o CANCELAR): ";
        leerLinea(c.email, 100);
        if (esCancelar(c.email)) { cout << "Cancelado.\n"; return; }
        if (!validarEmail(c.email)) { cout << "ERROR: Email invalido.\n"; continue; }
        break;
    }

    cout << "Direccion (o CANCELAR): ";
    leerLinea(c.direccion, 200);
    if (esCancelar(c.direccion)) { cout << "Cancelado.\n"; return; }

    obtenerFechaActual(c.fechaRegistro);
    c.id = t->siguienteIdCliente;

    cout << "\n--- Resumen ---\n";
    cout << "  Nombre: " << c.nombre << "\n  Cedula: " << c.cedula << "\n";

    cout << "¿Guardar? (S/N): ";
    leerLinea(resp, 10);
    if (toupper(resp[0]) != 'S') { cout << "No guardado.\n"; return; }

    if (t->numClientes == t->capacidadClientes) redimensionarClientes(t);
    t->clientes[t->numClientes++] = c;
    t->siguienteIdCliente++;
    cout << "Cliente registrado con ID " << c.id << ".\n";
}

void listarClientes(Tienda* t) {
    if (t->numClientes == 0) { cout << "No hay clientes registrados.\n"; return; }
    cout << "\n+---+----------------------+------------+----------------------+\n";
    cout << "|ID |       Nombre         |  Cedula    |       Email          |\n";
    cout << "+---+----------------------+------------+----------------------+\n";
    for (int i = 0; i < t->numClientes; i++) {
        if (!t->clientes[i].activo) continue;
        Cliente& c = t->clientes[i];
        printf("|%2d | %-20s | %-10s | %-20s |\n", c.id, c.nombre, c.cedula, c.email);
    }
    cout << "+---+----------------------+------------+----------------------+\n";
}

void buscarCliente(Tienda* t) {
    cout << "\n1. Buscar por ID\n2. Buscar por nombre\n3. Buscar por cedula\n0. Cancelar\nOpcion: ";
    char buf[512];
    leerLinea(buf, 10);
    int op = atoi(buf);
    if (op == 0) return;

    if (op == 1) {
        cout << "ID: ";
        leerLinea(buf, 20);
        int idx = buscarClientePorId(t, atoi(buf));
        if (idx == -1) { cout << "No encontrado.\n"; return; }
        mostrarCliente(idx, t->clientes);
    }
    else if (op == 2 || op == 3) {
        cout << (op == 2 ? "Nombre" : "Cedula") << " (parcial): ";
        leerLinea(buf, 100);
        bool encontrado = false;
        for (int i = 0; i < t->numClientes; i++) {
            if (!t->clientes[i].activo) continue;
            const char* campo = (op == 2) ? t->clientes[i].nombre : t->clientes[i].cedula;
            if (contieneSubstring(campo, buf)) {
                mostrarCliente(i, t->clientes);
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron resultados.\n";
    }
}

void actualizarCliente(Tienda* t) {
    char buf[512];
    cout << "ID del cliente: ";
    leerLinea(buf, 20);
    int idx = buscarClientePorId(t, atoi(buf));
    if (idx == -1) { cout << "ERROR: Cliente no encontrado.\n"; return; }

    Cliente& c = t->clientes[idx];
    cout << "\n1. Nombre\n2. Cedula\n3. Telefono\n4. Email\n5. Direccion\n0. Cancelar\nCampo: ";
    leerLinea(buf, 10);
    int op = atoi(buf);
    if (op == 0) return;
    if (op == 1) { cout << "Nuevo nombre: "; leerLinea(c.nombre, 100); }
    else if (op == 2) { cout << "Nueva cedula: "; leerLinea(c.cedula, 20); }
    else if (op == 3) { cout << "Nuevo telefono: "; leerLinea(c.telefono, 20); }
    else if (op == 4) {
        while (true) { cout << "Nuevo email: "; leerLinea(c.email, 100); if (validarEmail(c.email)) break; cout << "Email invalido.\n"; }
    }
    else if (op == 5) { cout << "Nueva direccion: "; leerLinea(c.direccion, 200); }

    cout << "¿Guardar? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) == 'S') cout << "Cliente actualizado.\n";
    else cout << "Cambios descartados.\n";
}

void eliminarCliente(Tienda* t) {
    char buf[512];
    cout << "ID del cliente a eliminar: ";
    leerLinea(buf, 20);
    int id = atoi(buf);
    int idx = buscarClientePorId(t, id);
    if (idx == -1) { cout << "ERROR: Cliente no encontrado.\n"; return; }
    mostrarCliente(idx, t->clientes);
    cout << "¿Confirmar eliminacion? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) == 'S') { t->clientes[idx].activo = false; cout << "Cliente eliminado.\n"; }
    else cout << "Cancelado.\n";
}

void registrarCompra(Tienda* t) {
    char buf[512];

    cout << "ID del producto: ";
    leerLinea(buf, 20);
    int idProd = atoi(buf);
    int idxProd = buscarProductoPorId(t, idProd);
    if (idxProd == -1) { cout << "ERROR: Producto no encontrado.\n"; return; }

    cout << "ID del proveedor: ";
    leerLinea(buf, 20);
    int idProv = atoi(buf);
    if (!existeProveedor(t, idProv)) { cout << "ERROR: Proveedor no encontrado.\n"; return; }

    cout << "Cantidad: ";
    leerLinea(buf, 20);
    int cant = atoi(buf);
    if (cant <= 0) { cout << "ERROR: Cantidad invalida.\n"; return; }

    cout << "Precio unitario de compra: ";
    leerLinea(buf, 20);
    float precio = (float)atof(buf);
    if (precio <= 0) { cout << "ERROR: Precio invalido.\n"; return; }

    cout << "Descripcion (opcional): ";
    char desc[200];
    leerLinea(desc, 200);

    float total = cant * precio;
    printf("\n--- Resumen de Compra ---\n");
    printf("  Producto : %s\n", t->productos[idxProd].nombre);
    printf("  Proveedor: %s\n", nombreProveedor(t, idProv));
    printf("  Cantidad : %d\n", cant);
    printf("  Precio   : %.2f\n", precio);
    printf("  Total    : %.2f\n", total);

    cout << "¿Confirmar compra? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) != 'S') { cout << "Compra cancelada.\n"; return; }

    Transaccion tr;
    tr.id = t->siguienteIdTransaccion++;
    strcpy(tr.tipo, "COMPRA");
    tr.idProducto = idProd;
    tr.idRelacionado = idProv;
    tr.cantidad = cant;
    tr.precioUnitario = precio;
    tr.total = total;
    obtenerFechaActual(tr.fecha);
    strncpy(tr.descripcion, desc, 199);
    tr.cancelada = false;

    t->productos[idxProd].stock += cant;

    if (t->numTransacciones == t->capacidadTransacciones) redimensionarTransacciones(t);
    t->transacciones[t->numTransacciones++] = tr;

    cout << "Compra registrada. Stock actualizado a " << t->productos[idxProd].stock << ".\n";
}

void registrarVenta(Tienda* t) {
    char buf[512];

    cout << "ID del producto: ";
    leerLinea(buf, 20);
    int idProd = atoi(buf);
    int idxProd = buscarProductoPorId(t, idProd);
    if (idxProd == -1) { cout << "ERROR: Producto no encontrado.\n"; return; }
    cout << "Stock disponible: " << t->productos[idxProd].stock << "\n";

    cout << "ID del cliente: ";
    leerLinea(buf, 20);
    int idCli = atoi(buf);
    if (!existeCliente(t, idCli)) { cout << "ERROR: Cliente no encontrado.\n"; return; }

    cout << "Cantidad: ";
    leerLinea(buf, 20);
    int cant = atoi(buf);
    if (cant <= 0) { cout << "ERROR: Cantidad invalida.\n"; return; }
    if (cant > t->productos[idxProd].stock) {
        cout << "ERROR: Stock insuficiente. Disponible: " << t->productos[idxProd].stock
            << ", Solicitado: " << cant << "\n";
        return;
    }

    cout << "Precio unitario (Enter para usar precio del producto " << t->productos[idxProd].precio << "): ";
    leerLinea(buf, 20);
    float precio = (strlen(buf) == 0) ? t->productos[idxProd].precio : (float)atof(buf);
    if (precio <= 0) precio = t->productos[idxProd].precio;

    cout << "Descripcion (opcional): ";
    char desc[200];
    leerLinea(desc, 200);

    float total = cant * precio;
    printf("\n--- Resumen de Venta ---\n");
    printf("  Producto : %s\n", t->productos[idxProd].nombre);
    int ic = buscarClientePorId(t, idCli);
    printf("  Cliente  : %s\n", t->clientes[ic].nombre);
    printf("  Cantidad : %d\n", cant);
    printf("  Precio   : %.2f\n", precio);
    printf("  Total    : %.2f\n", total);

    cout << "¿Confirmar venta? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) != 'S') { cout << "Venta cancelada.\n"; return; }

    Transaccion tr;
    tr.id = t->siguienteIdTransaccion++;
    strcpy(tr.tipo, "VENTA");
    tr.idProducto = idProd;
    tr.idRelacionado = idCli;
    tr.cantidad = cant;
    tr.precioUnitario = precio;
    tr.total = total;
    obtenerFechaActual(tr.fecha);
    strncpy(tr.descripcion, desc, 199);
    tr.cancelada = false;

    t->productos[idxProd].stock -= cant;

    if (t->numTransacciones == t->capacidadTransacciones) redimensionarTransacciones(t);
    t->transacciones[t->numTransacciones++] = tr;

    cout << "Venta registrada. Stock actualizado a " << t->productos[idxProd].stock << ".\n";
}

void listarTransacciones(Tienda* t) {
    if (t->numTransacciones == 0) { cout << "No hay transacciones.\n"; return; }
    cout << "\n+---+--------+------------------+------+----------+------------+\n";
    cout << "|ID |  Tipo  |     Producto     |Cantd |  Total   |   Fecha    |\n";
    cout << "+---+--------+------------------+------+----------+------------+\n";
    for (int i = 0; i < t->numTransacciones; i++) {
        Transaccion& tr = t->transacciones[i];
        int ip = buscarProductoPorId(t, tr.idProducto);
        const char* nomProd = (ip >= 0) ? t->productos[ip].nombre : "N/A";
        const char* estado = tr.cancelada ? "[CANC]" : "";
        printf("|%2d | %-6s | %-16s |%5d |%9.2f | %-10s | %s\n",
            tr.id, tr.tipo, nomProd, tr.cantidad, tr.total, tr.fecha, estado);
    }
    cout << "+---+--------+------------------+------+----------+------------+\n";
}

void buscarTransacciones(Tienda* t) {
    cout << "\n1. Por ID\n2. Por producto\n3. Por cliente\n4. Por proveedor\n5. Por fecha\n6. Por tipo\n0. Cancelar\nOpcion: ";
    char buf[512];
    leerLinea(buf, 10);
    int op = atoi(buf);
    if (op == 0) return;

    bool encontrado = false;
    if (op == 1) {
        cout << "ID transaccion: ";
        leerLinea(buf, 20);
        int id = atoi(buf);
        for (int i = 0; i < t->numTransacciones; i++)
            if (t->transacciones[i].id == id) { mostrarTransaccion(t, i); encontrado = true; }
    }
    else if (op == 2) {
        cout << "ID producto: ";
        leerLinea(buf, 20);
        int id = atoi(buf);
        for (int i = 0; i < t->numTransacciones; i++)
            if (t->transacciones[i].idProducto == id) { mostrarTransaccion(t, i); encontrado = true; }
    }
    else if (op == 3) {
        cout << "ID cliente: ";
        leerLinea(buf, 20);
        int id = atoi(buf);
        for (int i = 0; i < t->numTransacciones; i++)
            if (strcmp(t->transacciones[i].tipo, "VENTA") == 0 && t->transacciones[i].idRelacionado == id)
            {
                mostrarTransaccion(t, i); encontrado = true;
            }
    }
    else if (op == 4) {
        cout << "ID proveedor: ";
        leerLinea(buf, 20);
        int id = atoi(buf);
        for (int i = 0; i < t->numTransacciones; i++)
            if (strcmp(t->transacciones[i].tipo, "COMPRA") == 0 && t->transacciones[i].idRelacionado == id)
            {
                mostrarTransaccion(t, i); encontrado = true;
            }
    }
    else if (op == 5) {
        cout << "Fecha (YYYY-MM-DD): ";
        leerLinea(buf, 20);
        for (int i = 0; i < t->numTransacciones; i++)
            if (strcmp(t->transacciones[i].fecha, buf) == 0) { mostrarTransaccion(t, i); encontrado = true; }
    }
    else if (op == 6) {
        cout << "Tipo (COMPRA/VENTA): ";
        leerLinea(buf, 20);
        for (int i = 0; i < strlen(buf); i++) buf[i] = toupper((unsigned char)buf[i]);
        for (int i = 0; i < t->numTransacciones; i++)
            if (strcmp(t->transacciones[i].tipo, buf) == 0) { mostrarTransaccion(t, i); encontrado = true; }
    }

    if (!encontrado) cout << "No se encontraron transacciones.\n";
}

void cancelarTransaccion(Tienda* t) {
    char buf[512];
    cout << "ID de la transaccion a cancelar: ";
    leerLinea(buf, 20);
    int id = atoi(buf);

    int idx = -1;
    for (int i = 0; i < t->numTransacciones; i++)
        if (t->transacciones[i].id == id) { idx = i; break; }

    if (idx == -1) { cout << "ERROR: Transaccion no encontrada.\n"; return; }
    if (t->transacciones[idx].cancelada) { cout << "ERROR: La transaccion ya esta cancelada.\n"; return; }

    mostrarTransaccion(t, idx);
    Transaccion& tr = t->transacciones[idx];
    int idxProd = buscarProductoPorId(t, tr.idProducto);

    if (strcmp(tr.tipo, "COMPRA") == 0) {
        cout << "ADVERTENCIA: Se restaran " << tr.cantidad << " unidades del stock.\n";
        if (idxProd >= 0 && t->productos[idxProd].stock < tr.cantidad) {
            cout << "ADVERTENCIA: Stock insuficiente para revertir la compra.\n";
        }
    }
    else {
        cout << "ADVERTENCIA: Se sumaran " << tr.cantidad << " unidades al stock.\n";
    }

    cout << "¿Confirmar cancelacion? (S/N): ";
    leerLinea(buf, 10);
    if (toupper(buf[0]) != 'S') { cout << "Cancelacion abortada.\n"; return; }

    tr.cancelada = true;
    if (idxProd >= 0) {
        if (strcmp(tr.tipo, "COMPRA") == 0) t->productos[idxProd].stock -= tr.cantidad;
        else t->productos[idxProd].stock += tr.cantidad;
    }
    cout << "Transaccion " << id << " cancelada. Stock ajustado.\n";
}

void menuProductos(Tienda* t) {
    char buf[10];
    int op;
    do {
        cout << "\n+-------------------------------------------+\n";
        cout << "|        GESTION DE PRODUCTOS              |\n";
        cout << "+-------------------------------------------+\n";
        cout << "1. Registrar nuevo producto\n";
        cout << "2. Buscar producto\n";
        cout << "3. Actualizar producto\n";
        cout << "4. Actualizar stock manualmente\n";
        cout << "5. Listar todos los productos\n";
        cout << "6. Eliminar producto\n";
        cout << "0. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        leerLinea(buf, 10);
        op = atoi(buf);

        switch (op) {
        case 1: crearProducto(t); break;
        case 2: buscarProducto(t); break;
        case 3: actualizarProducto(t); break;
        case 4: actualizarStockProducto(t); break;
        case 5: listarProductos(t); break;
        case 6: eliminarProducto(t); break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

void menuProveedores(Tienda* t) {
    char buf[10];
    int op;
    do {
        cout << "\n+-------------------------------------------+\n";
        cout << "|       GESTION DE PROVEEDORES             |\n";
        cout << "+-------------------------------------------+\n";
        cout << "1. Registrar proveedor\n2. Buscar proveedor\n3. Actualizar proveedor\n";
        cout << "4. Listar proveedores\n5. Eliminar proveedor\n0. Volver\nOpcion: ";
        leerLinea(buf, 10);
        op = atoi(buf);
        switch (op) {
        case 1: crearProveedor(t); break;
        case 2: buscarProveedor(t); break;
        case 3: actualizarProveedor(t); break;
        case 4: listarProveedores(t); break;
        case 5: eliminarProveedor(t); break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

void menuClientes(Tienda* t) {
    char buf[10];
    int op;
    do {
        cout << "\n+-------------------------------------------+\n";
        cout << "|         GESTION DE CLIENTES              |\n";
        cout << "+-------------------------------------------+\n";
        cout << "1. Registrar cliente\n2. Buscar cliente\n3. Actualizar cliente\n";
        cout << "4. Listar clientes\n5. Eliminar cliente\n0. Volver\nOpcion: ";
        leerLinea(buf, 10);
        op = atoi(buf);
        switch (op) {
        case 1: crearCliente(t); break;
        case 2: buscarCliente(t); break;
        case 3: actualizarCliente(t); break;
        case 4: listarClientes(t); break;
        case 5: eliminarCliente(t); break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

void menuTransacciones(Tienda* t) {
    char buf[10];
    int op;
    do {
        cout << "\n+-------------------------------------------+\n";
        cout << "|       GESTION DE TRANSACCIONES           |\n";
        cout << "+-------------------------------------------+\n";
        cout << "1. Registrar compra (a proveedor)\n2. Registrar venta (a cliente)\n";
        cout << "3. Buscar transacciones\n4. Listar transacciones\n5. Cancelar transaccion\n0. Volver\nOpcion: ";
        leerLinea(buf, 10);
        op = atoi(buf);
        switch (op) {
        case 1: registrarCompra(t); break;
        case 2: registrarVenta(t); break;
        case 3: buscarTransacciones(t); break;
        case 4: listarTransacciones(t); break;
        case 5: cancelarTransaccion(t); break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Tienda tienda;
    char nombre[100], rif[20];

    cout << "+-------------------------------------------+\n";
    cout << "|   SISTEMA DE GESTION DE INVENTARIO       |\n";
    cout << "+-------------------------------------------+\n\n";
    cout << "Nombre de la tienda: ";
    leerLinea(nombre, 100);
    cout << "RIF de la tienda: ";
    leerLinea(rif, 20);

    inicializarTienda(&tienda, nombre, rif);

    char buf[10];
    int op;
    do {
        cout << "\n+-------------------------------------------+\n";
        printf("|   SISTEMA DE GESTION DE INVENTARIO       |\n");
        printf("|   Tienda: %-32s|\n", tienda.nombre);
        cout << "+-------------------------------------------+\n\n";
        cout << "1. Gestion de Productos\n";
        cout << "2. Gestion de Proveedores\n";
        cout << "3. Gestion de Clientes\n";
        cout << "4. Gestion de Transacciones\n";
        cout << "5. Salir\n\n";
        cout << "Seleccione una opcion: ";
        leerLinea(buf, 10);
        op = atoi(buf);

        switch (op) {
        case 1: menuProductos(&tienda); break;
        case 2: menuProveedores(&tienda); break;
        case 3: menuClientes(&tienda); break;
        case 4: menuTransacciones(&tienda); break;
        case 5: {
            cout << "¿Desea salir? (S/N): ";
            leerLinea(buf, 10);
            if (toupper(buf[0]) != 'S') op = 0;
            break;
        }
        }
    } while (op != 5);

    liberarTienda(&tienda);
    cout << "¡Hasta luego!\n";
    return 0;

}
