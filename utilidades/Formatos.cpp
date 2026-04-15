#include "Formatos.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cctype>

std::string Formatos::formatearFecha(std::time_t fecha) {
    std::tm* tiempoLocal = std::localtime(&fecha);
    if (tiempoLocal == nullptr) {
        return "Fecha invalida";
    }

    std::ostringstream salida;
    salida << std::setfill('0')
           << std::setw(2) << tiempoLocal->tm_mday << "/"
           << std::setw(2) << (tiempoLocal->tm_mon + 1) << "/"
           << (tiempoLocal->tm_year + 1900);

    return salida.str();
}

std::string Formatos::insertarSeparadoresMiles(const std::string& parteEntera) {
    std::string resultado;
    int contador = 0;

    for (int i = static_cast<int>(parteEntera.size()) - 1; i >= 0; i--) {
        resultado.insert(resultado.begin(), parteEntera[i]);
        contador++;

        if (contador == 3 && i > 0) {
            resultado.insert(resultado.begin(), ',');
            contador = 0;
        }
    }

    return resultado;
}

std::string Formatos::formatearMoneda(float monto) {
    std::ostringstream salida;
    salida << std::fixed << std::setprecision(2) << monto;

    std::string texto = salida.str();
    std::size_t punto = texto.find('.');

    std::string parteEntera = texto.substr(0, punto);
    std::string parteDecimal = (punto != std::string::npos) ? texto.substr(punto) : ".00";

    bool negativo = false;
    if (!parteEntera.empty() && parteEntera[0] == '-') {
        negativo = true;
        parteEntera = parteEntera.substr(1);
    }

    std::string conSeparadores = insertarSeparadoresMiles(parteEntera);

    if (negativo) {
        return "-$" + conSeparadores + parteDecimal;
    }

    return "$" + conSeparadores + parteDecimal;
}

void Formatos::convertirAMayusculas(char* cadena) {
    if (cadena == nullptr) {
        return;
    }

    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(cadena[i])));
    }
}

void Formatos::limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

void Formatos::pausar() {
    std::cout << "\nPresione ENTER para continuar...";
    std::cin.ignore(10000, '\n');
}