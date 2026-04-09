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
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Producto();
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
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    void setId(int nuevoId);
    void setCodigo(const char* nuevoCodigo);
    void setNombre(const char* nuevoNombre);
    void setDescripcion(const char* nuevaDescripcion);
    bool setPrecio(float nuevoPrecio);       
    bool setStock(int nuevoStock);           
    void setIdProveedor(int nuevoIdProveedor);
    bool setStockMinimo(int nuevoStockMinimo);
    void setTotalVendido(int nuevoTotal);
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    void mostrarInformacionBasica() const;
    static int obtenerTamano();
};

#endif