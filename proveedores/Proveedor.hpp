#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include <ctime>

class Proveedor {
private:
    int id;
    char nombre[100];
    char rif[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    bool eliminado;
    std::time_t fechaCreacion;
    std::time_t fechaUltimaModificacion;

public:
    Proveedor();
    Proveedor(const char* nombre,
              const char* rif,
              const char* telefono,
              const char* email,
              const char* direccion);
    Proveedor(const Proveedor& otro) = default;
    ~Proveedor() = default;

    int getId() const;
    const char* getNombre() const;
    const char* getRif() const;
    const char* getTelefono() const;
    const char* getEmail() const;
    const char* getDireccion() const;
    bool isEliminado() const;
    std::time_t getFechaCreacion() const;
    std::time_t getFechaUltimaModificacion() const;

    void setId(int nuevoId);
    bool setNombre(const char* nuevoNombre);
    bool setRif(const char* nuevoRif);
    bool setTelefono(const char* nuevoTelefono);
    bool setEmail(const char* nuevoEmail);
    bool setDireccion(const char* nuevaDireccion);
    void setEliminado(bool estado);
    void setFechaCreacion(std::time_t fecha);
    void setFechaUltimaModificacion(std::time_t fecha);

    bool tieneDatosValidosBasicos() const;

    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static int obtenerTamano();
};

#endif