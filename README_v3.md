# Proyecto 3 - POO y Modularización del Sistema de Inventario

## Universidad Rafael Urdaneta
**Asignatura:** Programación 2
**Profesor:** Ing. Victor Kneider
**Trimestre:** 2026-A

---

## Descripción

Este proyecto corresponde a la evolución del sistema de inventario desarrollado previamente en una versión monolítica.
En esta tercera fase se aplican principios de Programación Orientada a Objetos (POO), encapsulamiento, modularización por dominio y compilación multiarchivo.

El sistema mantiene persistencia binaria con acceso aleatorio mediante seekg() y seekp(), utilizando además una estructura ArchivoHeader para la administración de los archivos.

---

## Estructura del proyecto

inventario-p2/
├── clientes/
├── datos/
├── interfaz/
├── legacy/
├── persistencia/
├── productos/
├── proveedores/
├── tienda/
├── transacciones/
├── utilidades/
├── main.cpp
├── Makefile
├── README.md
├── README_V3.md
└── .gitignore

---

## Organización modular

### Persistencia
- GestorArchivos.hpp/.cpp
- inicialización del sistema de archivos binarios
- lectura y actualización de ArchivoHeader
- templates genéricos para guardar, leer y actualizar registros

### Utilidades
- Validaciones.hpp/.cpp
- Formatos.hpp/.cpp

### Productos
- Producto.hpp/.cpp
- operacionesProductos.hpp/.cpp

### Proveedores
- Proveedor.hpp/.cpp
- operacionesProveedores.hpp/.cpp

### Clientes
- Cliente.hpp/.cpp
- operacionesClientes.hpp/.cpp

### Transacciones
- Transaccion.hpp/.cpp
- operacionesTransacciones.hpp/.cpp

### Tienda
- Tienda.hpp/.cpp

### Interfaz
- Interfaz.hpp/.cpp
- navegación general y menús del sistema

---

## Funcionalidades disponibles

- Registro de proveedores
- Listado de proveedores
- Registro de productos
- Listado de productos
- Búsqueda de producto por ID
- Registro de clientes
- Listado de clientes
- Registro de venta simple
- Listado de transacciones
- Persistencia binaria modular

---

## Compilación

Compilar con MinGW Make:
mingw32-make

Limpiar archivos objeto y ejecutable:
mingw32-make clean

Ejecutar:
inventario_v3.exe

---

## Nota de diseño

La migración se realizó de forma incremental a partir de la base funcional del Proyecto 2.
Se preservó la persistencia binaria original mientras se transformaban las entidades en clases y se separaba la lógica por módulos.

La carpeta legacy/ conserva la referencia histórica de la fase previa (que_fase1.cpp), mientras que la ejecución principal del sistema queda delegada a main.cpp e Interfaz.

---

## Autor

Arturo Parra