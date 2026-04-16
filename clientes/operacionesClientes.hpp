#ifndef OPERACIONES_CLIENTES_HPP
#define OPERACIONES_CLIENTES_HPP

#include "Cliente.hpp"

bool cedulaClienteDuplicada(const char* cedulaBuscada);
int buscarIndiceClientePorID(int idBuscado);
bool leerClientePorIndice(int indice, Cliente& cliente);
bool leerClientePorID(int idBuscado, Cliente& cliente);
bool actualizarClienteEnArchivo(int indice, const Cliente& cliente);

void registrarCliente();
void listarClientes();

#endif