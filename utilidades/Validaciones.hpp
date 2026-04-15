#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

class Validaciones {
public:
    static bool validarEmail(const char* email);
    static bool validarRIF(const char* rif);
    static bool validarTelefono(const char* telefono);
    static bool validarRango(int valor, int minimo, int maximo);

private:
    static bool soloDigitos(const char* texto);
};

#endif