#ifndef TIENDA_HPP
#define TIENDA_HPP

#include <ctime>

class Tienda {
private:
    char nombre[100];
    char rif[20];
    int totalProductos;
    int totalProveedores;
    int totalClientes;
    int totalTransacciones;
    bool eliminado;
    std::time_t fechaCreacion;
    std::time_t fechaUltimaModificacion;

public:
    Tienda();
    Tienda(const char* nombre, const char* rif);
    Tienda(const Tienda& otra) = default;
    ~Tienda() = default;

    const char* getNombre() const;
    const char* getRif() const;
    int getTotalProductos() const;
    int getTotalProveedores() const;
    int getTotalClientes() const;
    int getTotalTransacciones() const;
    bool isEliminado() const;
    std::time_t getFechaCreacion() const;
    std::time_t getFechaUltimaModificacion() const;

    bool setNombre(const char* nuevoNombre);
    bool setRif(const char* nuevoRif);
    bool setTotalProductos(int nuevoTotal);
    bool setTotalProveedores(int nuevoTotal);
    bool setTotalClientes(int nuevoTotal);
    bool setTotalTransacciones(int nuevoTotal);
    void setEliminado(bool estado);
    void setFechaCreacion(std::time_t fecha);
    void setFechaUltimaModificacion(std::time_t fecha);

    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();
};

#endif