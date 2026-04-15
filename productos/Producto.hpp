#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include <ctime>

class Producto {
private:
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
    std::time_t fechaCreacion;
    std::time_t fechaUltimaModificacion;

public:
    Producto();
    Producto(const char* codigo,
            const char* nombre,
            const char* descripcion,
            float precio,
            int stock,
            int idProveedor,
            int stockMinimo);
    Producto(const Producto& otro) = default;
    ~Producto() = default;

    int getId() const;
    const char* getCodigo() const;
    const char* getNombre() const;
    const char* getDescripcion() const;
    float getPrecio() const;
    int getStock() const;
    int getIdProveedor() const;
    int getStockMinimo() const;
    int getTotalVendido() const;
    bool isEliminado() const;
    std::time_t getFechaCreacion() const;
    std::time_t getFechaUltimaModificacion() const;

    void setId(int nuevoId);
    bool setCodigo(const char* nuevoCodigo);
    bool setNombre(const char* nuevoNombre);
    bool setDescripcion(const char* nuevaDescripcion);
    bool setPrecio(float nuevoPrecio);
    bool setStock(int nuevoStock);
    bool setIdProveedor(int nuevoIdProveedor);
    bool setStockMinimo(int nuevoStockMinimo);
    bool setTotalVendido(int nuevoTotal);
    void setEliminado(bool estado);
    void setFechaCreacion(std::time_t fecha);
    void setFechaUltimaModificacion(std::time_t fecha);

    bool tieneDatosValidosBasicos() const;
    bool tieneStockCritico() const;

    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();
};

#endif