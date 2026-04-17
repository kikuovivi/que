#include "persistencia/GestorArchivos.hpp"
#include "interfaz/Interfaz.hpp"
#include <iostream>

int main() {
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        std::cerr << "ERROR: No se pudieron inicializar los archivos binarios." << std::endl;
        return 1;
    }

    Interfaz::ejecutar();
    return 0;
}