#ifndef OPERACIONES_PRODUCTOS_HPP
#define OPERACIONES_PRODUCTOS_HPP

#include "Producto.hpp"

bool codigoProductoDuplicado(const char* codigoBuscado);
int buscarIndiceProductoPorID(int idBuscado);
bool leerProductoPorIndice(int indice, Producto& producto);
bool leerProductoPorID(int idBuscado, Producto& producto);
bool actualizarProductoEnArchivo(int indice, const Producto& producto);

void registrarProducto();
void listarProductos();
void buscarProductoPorID();

#endif