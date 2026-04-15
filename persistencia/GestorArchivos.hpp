#ifndef GESTOR_ARCHIVOS_HPP
#define GESTOR_ARCHIVOS_HPP

#include "Constantes.hpp"
#include <fstream>
#include <ctime>
#include <type_traits>

struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};

class GestorArchivos {
public:
    static bool inicializarSistemaArchivos();
    static bool inicializarArchivo(const char* nombreArchivo);
    static ArchivoHeader leerHeader(const char* nombreArchivo);
    static bool actualizarHeader(const char* nombreArchivo, const ArchivoHeader& header);

    template <typename T>
    static long long obtenerOffset(int indice) {
        static_assert(std::is_standard_layout<T>::value,
            "La entidad debe tener standard layout para persistencia binaria.");
        return sizeof(ArchivoHeader) + static_cast<long long>(indice) * sizeof(T);
    }

    template <typename T>
    static bool guardarRegistro(const char* nombreArchivo, T& registro) {
        static_assert(std::is_trivially_copyable<T>::value,
            "La entidad debe ser trivially copyable para persistencia binaria.");
        static_assert(std::is_standard_layout<T>::value,
            "La entidad debe tener standard layout para persistencia binaria.");

        ArchivoHeader header = leerHeader(nombreArchivo);

        registro.setId(header.proximoID);
        registro.setEliminado(false);

        std::time_t ahora = std::time(nullptr);
        registro.setFechaCreacion(ahora);
        registro.setFechaUltimaModificacion(ahora);

        std::ofstream archivo(nombreArchivo, std::ios::binary | std::ios::app);
        if (!archivo.is_open()) {
            return false;
        }

        archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
        if (!archivo.good()) {
            archivo.close();
            return false;
        }

        archivo.close();

        header.cantidadRegistros++;
        header.registrosActivos++;
        header.proximoID++;

        return actualizarHeader(nombreArchivo, header);
    }

    template <typename T>
    static bool leerRegistroPorIndice(const char* nombreArchivo, int indice, T& registro) {
        static_assert(std::is_trivially_copyable<T>::value,
            "La entidad debe ser trivially copyable para persistencia binaria.");
        static_assert(std::is_standard_layout<T>::value,
            "La entidad debe tener standard layout para persistencia binaria.");

        std::ifstream archivo(nombreArchivo, std::ios::binary);
        if (!archivo.is_open()) {
            return false;
        }

        archivo.seekg(obtenerOffset<T>(indice), std::ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

        bool ok = archivo.good();
        archivo.close();
        return ok;
    }

    template <typename T>
    static bool actualizarRegistro(const char* nombreArchivo, int indice, const T& registro) {
        static_assert(std::is_trivially_copyable<T>::value,
            "La entidad debe ser trivially copyable para persistencia binaria.");
        static_assert(std::is_standard_layout<T>::value,
            "La entidad debe tener standard layout para persistencia binaria.");

        std::fstream archivo(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);
        if (!archivo.is_open()) {
            return false;
        }

        archivo.seekp(obtenerOffset<T>(indice), std::ios::beg);
        archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));

        bool ok = archivo.good();
        archivo.close();
        return ok;
    }

    template <typename T>
    static int buscarIndicePorID(const char* nombreArchivo, int idBuscado) {
        static_assert(std::is_trivially_copyable<T>::value,
            "La entidad debe ser trivially copyable para persistencia binaria.");
        static_assert(std::is_standard_layout<T>::value,
            "La entidad debe tener standard layout para persistencia binaria.");

        std::ifstream archivo(nombreArchivo, std::ios::binary);
        if (!archivo.is_open()) {
            return -1;
        }

        ArchivoHeader header;
        archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        if (!archivo.good()) {
            archivo.close();
            return -1;
        }

        T registro;
        for (int i = 0; i < header.cantidadRegistros; i++) {
            archivo.seekg(obtenerOffset<T>(i), std::ios::beg);
            archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

            if (!archivo.good()) {
                archivo.close();
                return -1;
            }

            if (!registro.isEliminado() && registro.getId() == idBuscado) {
                archivo.close();
                return i;
            }
        }

        archivo.close();
        return -1;
    }

    template <typename T>
    static bool leerRegistroPorID(const char* nombreArchivo, int idBuscado, T& registro) {
        int indice = buscarIndicePorID<T>(nombreArchivo, idBuscado);
        if (indice == -1) {
            return false;
        }

        return leerRegistroPorIndice<T>(nombreArchivo, indice, registro);
    }
};

#endif