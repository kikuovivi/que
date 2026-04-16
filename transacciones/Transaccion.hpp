#ifndef TRANSACCION_HPP
#define TRANSACCION_HPP

#include <ctime>

struct DetalleTransaccion {
    int idProducto;
    int cantidad;
    float precioUnitario;
    float subtotal;
};

class Transaccion {
private:
    int id;
    char tipo[10];
    int idRelacionado;
    int cantidadDetalles;
    DetalleTransaccion detalles[20];
    float total;
    bool cancelada;
    bool eliminado;
    std::time_t fechaCreacion;
    std::time_t fechaUltimaModificacion;

public:
    Transaccion();
    Transaccion(const char* tipo, int idRelacionado);
    Transaccion(const Transaccion& otra) = default;
    ~Transaccion() = default;

    int getId() const;
    const char* getTipo() const;
    int getIdRelacionado() const;
    int getCantidadDetalles() const;
    DetalleTransaccion getDetalle(int indice) const;
    float getTotal() const;
    bool isCancelada() const;
    bool isEliminado() const;
    std::time_t getFechaCreacion() const;
    std::time_t getFechaUltimaModificacion() const;

    void setId(int nuevoId);
    bool setTipo(const char* nuevoTipo);
    bool setIdRelacionado(int nuevoIdRelacionado);
    bool setCantidadDetalles(int nuevaCantidadDetalles);
    bool configurarDetalle(int indice, int idProducto, int cantidad, float precioUnitario, float subtotal);
    bool setTotal(float nuevoTotal);
    void setCancelada(bool estado);
    void setEliminado(bool estado);
    void setFechaCreacion(std::time_t fecha);
    void setFechaUltimaModificacion(std::time_t fecha);

    bool tieneDatosValidosBasicos() const;

    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();
};

#endif