#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <string>
#include <ctime>

class Formatos {
public:
    static std::string formatearFecha(std::time_t fecha);
    static std::string formatearMoneda(float monto);
    static void convertirAMayusculas(char* cadena);
    static void limpiarBuffer();
    static void pausar();

private:
    static std::string insertarSeparadoresMiles(const std::string& parteEntera);
};

#endif