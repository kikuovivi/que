#include "Validaciones.hpp"
#include <cstring>
#include <cctype>

bool Validaciones::validarEmail(const char* email) {
    if (email == nullptr || std::strlen(email) == 0) {
        return false;
    }

    const char* arroba = std::strchr(email, '@');
    if (!arroba) {
        return false;
    }

    const char* punto = std::strchr(arroba, '.');
    if (!punto) {
        return false;
    }

    return punto > arroba + 1;
}

bool Validaciones::soloDigitos(const char* texto) {
    if (texto == nullptr || std::strlen(texto) == 0) {
        return false;
    }

    for (int i = 0; texto[i] != '\0'; i++) {
        if (!std::isdigit(static_cast<unsigned char>(texto[i]))) {
            return false;
        }
    }

    return true;
}

bool Validaciones::validarRIF(const char* rif) {
    if (rif == nullptr || std::strlen(rif) < 2) {
        return false;
    }

    char inicial = static_cast<char>(std::toupper(static_cast<unsigned char>(rif[0])));
    if (inicial != 'V' && inicial != 'E' && inicial != 'J' &&
        inicial != 'G' && inicial != 'P') {
        return false;
    }

    int inicioNumeros = 1;
    if (rif[1] == '-' || rif[1] == ' ') {
        inicioNumeros = 2;
    }

    int cantidadDigitos = 0;
    for (int i = inicioNumeros; rif[i] != '\0'; i++) {
        if (rif[i] == '-' || rif[i] == ' ') {
            continue;
        }

        if (!std::isdigit(static_cast<unsigned char>(rif[i]))) {
            return false;
        }

        cantidadDigitos++;
    }

    return cantidadDigitos >= 7 && cantidadDigitos <= 10;
}

bool Validaciones::validarTelefono(const char* telefono) {
    if (telefono == nullptr || std::strlen(telefono) < 7) {
        return false;
    }

    int cantidadDigitos = 0;

    for (int i = 0; telefono[i] != '\0'; i++) {
        char c = telefono[i];

        if (std::isdigit(static_cast<unsigned char>(c))) {
            cantidadDigitos++;
            continue;
        }

        if (c == '+' || c == '-' || c == '(' || c == ')' || c == ' ') {
            continue;
        }

        return false;
    }

    return cantidadDigitos >= 7;
}

bool Validaciones::validarRango(int valor, int minimo, int maximo) {
    return valor >= minimo && valor <= maximo;
}