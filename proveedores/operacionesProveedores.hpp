#ifndef OPERACIONES_PROVEEDORES_HPP
#define OPERACIONES_PROVEEDORES_HPP

#include "Proveedor.hpp"

bool rifProveedorDuplicado(const char* rifBuscado);
int buscarIndiceProveedorPorID(int idBuscado);
bool leerProveedorPorIndice(int indice, Proveedor& proveedor);
bool leerProveedorPorID(int idBuscado, Proveedor& proveedor);

void registrarProveedor();
void listarProveedores();

#endif