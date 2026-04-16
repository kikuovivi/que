#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <ctime>

class Cliente {
private:
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    float totalGastado;
    int cantidadCompras;
    bool eliminado;
    std::time_t fechaCreacion;
    std::time_t fechaUltimaModificacion;

public:
    Cliente();
    Cliente(const char* nombre,
            const char* cedula,
            const char* telefono,
            const char* email,
            const char* direccion);
    Cliente(const Cliente& otro) = default;
    ~Cliente() = default;

    int getId() const;
    const char* getNombre() const;
    const char* getCedula() const;
    const char* getTelefono() const;
    const char* getEmail() const;
    const char* getDireccion() const;
    float getTotalGastado() const;
    int getCantidadCompras() const;
    bool isEliminado() const;
    std::time_t getFechaCreacion() const;
    std::time_t getFechaUltimaModificacion() const;

    void setId(int nuevoId);
    bool setNombre(const char* nuevoNombre);
    bool setCedula(const char* nuevaCedula);
    bool setTelefono(const char* nuevoTelefono);
    bool setEmail(const char* nuevoEmail);
    bool setDireccion(const char* nuevaDireccion);
    bool setTotalGastado(float nuevoTotalGastado);
    bool setCantidadCompras(int nuevaCantidadCompras);
    void setEliminado(bool estado);
    void setFechaCreacion(std::time_t fecha);
    void setFechaUltimaModificacion(std::time_t fecha);

    bool tieneDatosValidosBasicos() const;

    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();
};

#endif