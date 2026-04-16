#include "operacionesTransacciones.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Formatos.hpp"
#include "../productos/operacionesProductos.hpp"
#include "../clientes/operacionesClientes.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <ctime>

using namespace std;

namespace {
    void leerLinea(char* buffer, int tam) {
        cin.getline(buffer, tam);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

bool guardarTransaccion(Transaccion transaccion) {
    transaccion.setCancelada(false);
    return GestorArchivos::guardarRegistro<Transaccion>(Constantes::ARCHIVO_TRANSACCIONES, transaccion);
}

void listarTransacciones() {
    ifstream archivo(Constantes::ARCHIVO_TRANSACCIONES, ios::binary);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo de transacciones.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        cout << "ERROR: No se pudo leer el header de transacciones.\n";
        archivo.close();
        return;
    }

    if (header.registrosActivos == 0) {
        cout << "\nNo hay transacciones registradas.\n";
        archivo.close();
        return;
    }

    cout << "\n==================== LISTADO DE TRANSACCIONES ====================\n";

    Transaccion transaccion;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(GestorArchivos::obtenerOffset<Transaccion>(i), ios::beg);
        archivo.read(reinterpret_cast<char*>(&transaccion), sizeof(Transaccion));

        if (!archivo.good()) {
            cout << "ERROR: Fallo al leer un registro de transaccion.\n";
            archivo.close();
            return;
        }

        if (transaccion.isEliminado()) {
            continue;
        }

        cout << "\nID Transaccion : " << transaccion.getId() << "\n";
        cout << "Tipo           : " << transaccion.getTipo() << "\n";
        cout << "ID Relacionado : " << transaccion.getIdRelacionado() << "\n";
        cout << "Cantidad items : " << transaccion.getCantidadDetalles() << "\n";
        cout << "Total          : " << Formatos::formatearMoneda(transaccion.getTotal()) << "\n";

        for (int j = 0; j < transaccion.getCantidadDetalles(); j++) {
            DetalleTransaccion detalle = transaccion.getDetalle(j);
            cout << "  Producto ID  : " << detalle.idProducto << "\n";
            cout << "  Cantidad     : " << detalle.cantidad << "\n";
            cout << "  Precio Unit. : " << Formatos::formatearMoneda(detalle.precioUnitario) << "\n";
            cout << "  Subtotal     : " << Formatos::formatearMoneda(detalle.subtotal) << "\n";
        }
    }

    archivo.close();
}

void registrarVentaSimple() {
    char buffer[50];

    cout << "\n========== REGISTRAR VENTA SIMPLE ==========\n";

    cout << "ID del cliente: ";
    leerLinea(buffer, 50);
    int idCliente = atoi(buffer);

    int indiceCliente = buscarIndiceClientePorID(idCliente);
    Cliente cliente;
    if (indiceCliente == -1 || !leerClientePorIndice(indiceCliente, cliente)) {
        cout << "ERROR: No existe un cliente activo con ese ID.\n";
        return;
    }

    cout << "ID del producto: ";
    leerLinea(buffer, 50);
    int idProducto = atoi(buffer);

    int indiceProducto = buscarIndiceProductoPorID(idProducto);
    Producto producto;
    if (indiceProducto == -1 || !leerProductoPorIndice(indiceProducto, producto)) {
        cout << "ERROR: No existe un producto activo con ese ID.\n";
        return;
    }

    cout << "Cantidad a vender: ";
    leerLinea(buffer, 50);
    int cantidad = atoi(buffer);

    if (cantidad <= 0) {
        cout << "ERROR: La cantidad debe ser mayor a 0.\n";
        return;
    }

    if (producto.getStock() < cantidad) {
        cout << "ERROR: Stock insuficiente. Disponible: " << producto.getStock() << "\n";
        return;
    }

    float subtotal = cantidad * producto.getPrecio();

    cout << "\nResumen de venta:\n";
    cout << "Cliente   : " << cliente.getNombre() << " (ID: " << cliente.getId() << ")\n";
    cout << "Producto  : " << producto.getNombre() << " (ID: " << producto.getId() << ")\n";
    cout << "Cantidad  : " << cantidad << "\n";
    cout << "Precio    : " << Formatos::formatearMoneda(producto.getPrecio()) << "\n";
    cout << "Total     : " << Formatos::formatearMoneda(subtotal) << "\n";

    cout << "\nConfirmar venta? (S/N): ";
    leerLinea(buffer, 10);
    if (toupper(static_cast<unsigned char>(buffer[0])) != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    Transaccion transaccion;
    if (!transaccion.setTipo("VENTA")) {
        cout << "ERROR: No se pudo configurar el tipo de transaccion.\n";
        return;
    }

    if (!transaccion.setIdRelacionado(cliente.getId())) {
        cout << "ERROR: No se pudo configurar el ID relacionado.\n";
        return;
    }

    if (!transaccion.setCantidadDetalles(1)) {
        cout << "ERROR: No se pudo configurar la cantidad de detalles.\n";
        return;
    }

    if (!transaccion.configurarDetalle(0, producto.getId(), cantidad, producto.getPrecio(), subtotal)) {
        cout << "ERROR: No se pudo configurar el detalle de la transaccion.\n";
        return;
    }

    if (!transaccion.setTotal(subtotal)) {
        cout << "ERROR: No se pudo configurar el total de la transaccion.\n";
        return;
    }

    if (!transaccion.tieneDatosValidosBasicos()) {
        cout << "ERROR: La transaccion no cumple las validaciones basicas.\n";
        return;
    }

    if (!guardarTransaccion(transaccion)) {
        cout << "ERROR: No se pudo guardar la transaccion.\n";
        return;
    }

    if (!producto.setStock(producto.getStock() - cantidad)) {
        cout << "ERROR: No se pudo actualizar el stock del producto.\n";
        return;
    }

    if (!producto.setTotalVendido(producto.getTotalVendido() + cantidad)) {
        cout << "ERROR: No se pudo actualizar el total vendido del producto.\n";
        return;
    }

    producto.setFechaUltimaModificacion(time(nullptr));

    if (!actualizarProductoEnArchivo(indiceProducto, producto)) {
        cout << "ERROR: No se pudo actualizar el producto.\n";
        return;
    }

    if (!cliente.setTotalGastado(cliente.getTotalGastado() + subtotal)) {
        cout << "ERROR: No se pudo actualizar el total gastado del cliente.\n";
        return;
    }

    if (!cliente.setCantidadCompras(cliente.getCantidadCompras() + 1)) {
        cout << "ERROR: No se pudo actualizar la cantidad de compras del cliente.\n";
        return;
    }

    cliente.setFechaUltimaModificacion(time(nullptr));

    if (!actualizarClienteEnArchivo(indiceCliente, cliente)) {
        cout << "ERROR: No se pudo actualizar el cliente.\n";
        return;
    }

    cout << "Venta registrada correctamente.\n";
}