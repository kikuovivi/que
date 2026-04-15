#include "GestorArchivos.hpp"

bool GestorArchivos::inicializarArchivo(const char* nombreArchivo) {
    std::ifstream archivoEntrada(nombreArchivo, std::ios::binary);
    if (archivoEntrada.good()) {
        archivoEntrada.close();
        return true;
    }
    archivoEntrada.close();

    std::ofstream archivoSalida(nombreArchivo, std::ios::binary);
    if (!archivoSalida.is_open()) {
        return false;
    }

    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoID = 1;
    header.registrosActivos = 0;
    header.version = 1;

    archivoSalida.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    bool ok = archivoSalida.good();
    archivoSalida.close();

    return ok;
}

ArchivoHeader GestorArchivos::leerHeader(const char* nombreArchivo) {
    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoID = 1;
    header.registrosActivos = 0;
    header.version = 1;

    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo.is_open()) {
        return header;
    }

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    return header;
}

bool GestorArchivos::actualizarHeader(const char* nombreArchivo, const ArchivoHeader& header) {
    std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.seekp(0, std::ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));

    bool ok = archivo.good();
    archivo.close();

    return ok;
}

bool GestorArchivos::inicializarSistemaArchivos() {
    bool ok = true;

    ok = ok && inicializarArchivo(Constantes::ARCHIVO_TIENDA);
    ok = ok && inicializarArchivo(Constantes::ARCHIVO_PRODUCTOS);
    ok = ok && inicializarArchivo(Constantes::ARCHIVO_PROVEEDORES);
    ok = ok && inicializarArchivo(Constantes::ARCHIVO_CLIENTES);
    ok = ok && inicializarArchivo(Constantes::ARCHIVO_TRANSACCIONES);

    return ok;
}