# Sistema de Gestion de Inventario
---

## Descripcion del Proyecto

Sistema de gestion de inventario desarrollado en C++ que permite administrar productos, proveedores, clientes y transacciones comerciales de una tienda. 
---

## Instrucciones de Compilacion

### Visual Studio (MSVC)

1. Abrir Visual Studio
2. Crear un nuevo proyecto: **Archivo → Nuevo → Proyecto → Aplicacion de consola C++**
3. Reemplazar el contenido del `.cpp` generado con el archivo `que.cpp`
4. Compilar con **Ctrl + Shift + B** o pulsando en "Depurador local de Windows"
5. Verificar que no aparezcan errores en la ventana **Lista de errores**

> El archivo ya incluye `#define _CRT_SECURE_NO_WARNINGS` y `#include <windows.h>` necesarios para compilar en Visual Studio sin advertencias.
---

## Instrucciones de Ejecucion

### Visual Studio

Presionar **F5** para compilar y ejecutar con depurador, o **Ctrl + F5** para ejecutar sin depurador.

### Terminal

```bash
./inventario        # Linux / Mac
inventario.exe      # Windows CMD
```

Al iniciar, el sistema solicita el nombre y RIF de la tienda, luego muestra el menu principal.

---

## Estructura del Codigo

El proyecto consta de un unico archivo `que.cpp` organizado en las siguientes secciones:

```
que.cpp
|
+-- ESTRUCTURAS DE DATOS
|     Producto, Proveedor, Cliente, Transaccion, Tienda
|
+-- UTILIDADES
|     obtenerFechaActual(), validarEmail(), validarFecha()
|     contieneSubstring(), esCancelar(), leerLinea()
|
+-- INICIALIZACION Y LIBERACION
|     inicializarTienda(), liberarTienda()
|
+-- REDIMENSIONAMIENTO
|     redimensionarProductos(), redimensionarProveedores()
|     redimensionarClientes(), redimensionarTransacciones()
|
+-- BUSQUEDAS Y VALIDACIONES
|     buscarProductoPorId(), buscarProveedorPorId(), buscarClientePorId()
|     existeProducto(), existeProveedor(), existeCliente()
|     codigoDuplicado(), rifProveedorDuplicado(), cedulaDuplicada()
|
+-- CRUD PRODUCTOS
|     crearProducto(), listarProductos(), buscarProducto()
|     actualizarProducto(), actualizarStockProducto(), eliminarProducto()
|
+-- CRUD PROVEEDORES
|     crearProveedor(), listarProveedores(), buscarProveedor()
|     actualizarProveedor(), eliminarProveedor()
|
+-- CRUD CLIENTES
|     crearCliente(), listarClientes(), buscarCliente()
|     actualizarCliente(), eliminarCliente()
|
+-- TRANSACCIONES
|     registrarCompra(), registrarVenta(), listarTransacciones()
|     buscarTransacciones(), cancelarTransaccion()
|
+-- MENUS
|     menuProductos(), menuProveedores(), menuClientes(), menuTransacciones()
|
+-- main()
```

### Gestion de Memoria

Todos los arrays son dinamicos e inician con capacidad 5. Cuando se llena un array, se duplica su capacidad automaticamente:

```
numElementos == capacidad  →  nueva capacidad = capacidad * 2
                            →  new[], copiar datos, delete[] viejo
```

---

## Funcionalidades Implementadas

### Productos
- Registrar producto con codigo unico, proveedor valido, precio y stock
- Buscar por ID, nombre (parcial), codigo (parcial) o proveedor
- Actualizar campos individuales con menu de edicion
- Ajuste manual de stock con validacion de stock >= 0
- Listar todos en formato tabla
- Eliminar (marcado logico) con advertencia si tiene transacciones

### Proveedores
- Registrar con RIF unico y email validado
- Buscar por ID, nombre (parcial) o RIF
- Actualizar campos individuales
- Listar en formato tabla
- Eliminar solo si no tiene productos asociados

### Clientes
- Registrar con cedula/RIF unico y email validado
- Buscar por ID, nombre (parcial) o cedula
- Actualizar campos individuales
- Listar en formato tabla
- Eliminar (marcado logico)

### Transacciones
- Registrar compra: incrementa stock automaticamente
- Registrar venta: valida stock suficiente, decrementa stock automaticamente
- Buscar por ID, producto, cliente, proveedor, fecha o tipo
- Listar todas en formato tabla
- Cancelar/anular transaccion con reversion automatica de stock

### General
- Cancelacion en cualquier paso escribiendo `CANCELAR` o `0`
- Confirmacion (S/N) antes de guardar, actualizar, eliminar o cancelar
- Fecha de registro automatica tomada del sistema
- IDs autoincrementales para todas las entidades
- Validacion de email (debe contener `@` y `.`)

---

## Casos de Prueba Ejecutados

### Caso 1: Registro de Proveedor y Producto
```
1. Crear proveedor "Distribuidora XYZ" con RIF J-12345678-9
2. Crear producto "Laptop HP" con codigo PROD-001, asociado al proveedor anterior
3. Buscar producto por nombre parcial "lap" → encuentra "Laptop HP"
4. Listar productos → muestra tabla con el producto registrado
Resultado: sin errores
```

### Caso 2: Compra de Inventario
```
1. Registrar compra de 50 unidades de "Laptop HP" al proveedor
2. Verificar stock: paso de 0 a 50
3. Buscar transacciones por ID de producto → muestra la compra
Resultado: sin errores
```

### Caso 3: Venta con Validacion de Stock
```
1. Intentar vender 100 unidades → rechazado (stock insuficiente: disponible 50)
2. Vender 30 unidades al cliente registrado
3. Verificar stock: paso de 50 a 20
4. Buscar transacciones por cliente → muestra la venta
Resultado: sin errores
```

### Caso 4: Cancelacion de Transaccion
```
1. Cancelar la venta de 30 unidades
2. Verificar stock: volvio de 20 a 50
3. Transaccion marcada como [CANCELADA] en el listado
Resultado: sin errores
```

### Caso 5: Edicion Selectiva
```
1. Actualizar producto: cambiar solo precio y descripcion, resto sin cambios
2. Actualizar proveedor: cambiar solo telefono
3. Listar → refleja los cambios correctamente
Resultado: sin errores
```

### Caso 6: Busquedas Avanzadas
```
1. Buscar producto por nombre "lap" → encuentra "Laptop HP"
2. Listar productos de proveedor ID 1 → muestra solo sus productos
3. Buscar transacciones por fecha exacta → muestra coincidencias
Resultado: sin errores
```

### Caso 7: Cancelacion Durante Registro
```
1. Iniciar registro de producto, ingresar codigo y nombre
2. En campo descripcion escribir "CANCELAR"
3. Verificar que el producto NO fue creado (listado sin cambios)
Resultado: sin errores
```

---

## Notas Tecnicas

- Lenguaje: C++11
- Compilador probado: MSVC (Visual Studio) y g++ 11
- Gestion de memoria: manual con `new` / `delete[]`
- Sin uso de STL (vectores, strings): cumple requisito del proyecto
- Eliminacion logica: los registros se marcan con `activo = false` en lugar de borrarse fisicamente
- Codificacion: UTF-8 con soporte para consola Windows via `SetConsoleOutputCP(CP_UTF8)`
