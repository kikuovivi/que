# Sistema de Gestión de Inventario con Persistencia Binaria

## Descripción del Proyecto

Este proyecto corresponde a la **fase 2** del sistema de gestión de inventario de la materia **Programación 2**.

El sistema fue desarrollado en **C++** y evolucionó desde una versión inicial basada en estructuras cargadas en memoria (fase 1) hacia una versión con **persistencia en archivos binarios**, donde la información de proveedores, productos, clientes y transacciones se guarda directamente en disco.

El objetivo principal de esta fase fue implementar:

- persistencia binaria con `<fstream>`
- manejo de archivos independientes por entidad
- uso de **headers** administrativos por archivo
- IDs autoincrementales
- validaciones básicas de integridad entre archivos
- lectura y escritura de registros binarios
- actualización de registros directamente en archivo

---

## Estado Actual del Proyecto

La versión actual implementa una base funcional de la fase 2 con:

- registro y consulta de **proveedores**
- registro y consulta de **productos**
- registro y consulta de **clientes**
- registro de una **venta simple**
- actualización del **stock del producto**
- actualización del **gasto acumulado y cantidad de compras del cliente**
- almacenamiento de la venta en **transacciones.bin**

> Nota: esta versión prioriza una implementación funcional y defendible de la arquitectura de persistencia binaria. No todas las operaciones CRUD avanzadas quedaron completas para todas las entidades.

---

## Estructura del Proyecto

    proyecto-inventario-p2/
    ├── main.cpp
    ├── que.cpp
    ├── README.md
    ├── tienda.bin
    ├── proveedores.bin
    ├── productos.bin
    ├── clientes.bin
    └── transacciones.bin

### Archivos principales

#### `main.cpp`

Contiene la implementación principal de la **fase 2**, incluyendo:

- estructuras persistentes
- `ArchivoHeader`
- inicialización de archivos binarios
- CRUD inicial de proveedores
- CRUD inicial de productos
- CRUD inicial de clientes
- registro de venta simple
- actualización de stock y datos del cliente
- listado de transacciones

#### `que.cpp`

Archivo heredado de la **fase 1**.  
Se conserva como referencia de la versión anterior del proyecto, la cual estaba basada principalmente en arreglos dinámicos y manejo de datos en memoria RAM.

> Para la defensa de la fase 2, el archivo principal relevante es `main.cpp`.

---

## Archivos Binarios Utilizados

El sistema trabaja con un archivo binario independiente por entidad:

- `tienda.bin`
- `proveedores.bin`
- `productos.bin`
- `clientes.bin`
- `transacciones.bin`

Cada archivo inicia con un **header** administrativo.

---

## Estructura del Header

Cada archivo `.bin` comienza con la siguiente estructura:

    struct ArchivoHeader {
        int cantidadRegistros;
        int proximoID;
        int registrosActivos;
        int version;
    };

### Función del Header

El header permite conocer rápidamente:

- cuántos registros contiene el archivo
- cuál es el siguiente ID disponible
- cuántos registros activos existen
- la versión del archivo

Esto evita recorrer todo el archivo cada vez que se necesita esa información.

---

## Estructuras Persistentes

Las estructuras persistentes fueron diseñadas con tamaños fijos para poder almacenarse correctamente en binario.

### Entidades manejadas

- `Proveedor`
- `Producto`
- `Cliente`
- `Transaccion`
- `DetalleTransaccion`
- `Tienda`

Estas estructuras utilizan tipos apropiados para persistencia, por ejemplo:

- `int`
- `float`
- `char[]`
- `bool`
- `time_t`

---

## Funcionalidades Implementadas

### 1. Proveedores

- Registrar proveedor
- Validar RIF único
- Validar email
- Guardar proveedor en `proveedores.bin`
- Listar proveedores
- Buscar proveedor por ID

### 2. Productos

- Registrar producto
- Validar código único
- Validar que el proveedor exista
- Guardar producto en `productos.bin`
- Listar productos
- Buscar producto por ID
- Mostrar el proveedor asociado

### 3. Clientes

- Registrar cliente
- Validar cédula única
- Validar email
- Guardar cliente en `clientes.bin`
- Listar clientes

### 4. Transacciones

- Registrar una **venta simple**
- Validar existencia de cliente y producto
- Validar stock suficiente
- Guardar transacción en `transacciones.bin`
- Actualizar stock del producto
- Actualizar `totalGastado` y `cantidadCompras` del cliente
- Listar transacciones

---

## Flujo General de una Venta

La operación de venta simple sigue esta lógica:

1. Se busca el cliente en `clientes.bin`
2. Se busca el producto en `productos.bin`
3. Se valida que el producto tenga stock suficiente
4. Se construye la transacción
5. Se guarda la transacción en `transacciones.bin`
6. Se actualiza el producto en archivo:
   - se reduce el stock
   - se incrementa `totalVendido`
7. Se actualiza el cliente en archivo:
   - se incrementa `totalGastado`
   - se incrementa `cantidadCompras`

---

## Acceso Aleatorio

El sistema usa el concepto de **offset** para ubicar registros dentro de los archivos binarios.

La fórmula general utilizada es:

    sizeof(ArchivoHeader) + indice * sizeof(Estructura)

Esto permite:

- leer registros específicos
- actualizar registros existentes
- mantener una lógica de acceso directo dentro del archivo

---

## Compilación

### Con g++

    g++ main.cpp -o inventario

### Ejecución

En Windows:

    inventario.exe

En Linux / Git Bash:

    ./inventario

---

## Requisitos

- C++11 o superior
- Compilador compatible con `g++`
- Soporte para `<fstream>`

---

## Menú Principal

La versión actual incluye opciones para:

- registrar proveedor
- listar proveedores
- registrar producto
- listar productos
- registrar cliente
- listar clientes
- registrar venta simple
- listar transacciones
- mostrar headers de archivos

---

## Casos de Prueba Ejecutados

### Caso 1: Registro de proveedor

- registro correcto de proveedor
- validación de email
- rechazo de RIF duplicado

### Caso 2: Registro de producto

- rechazo si el proveedor no existe
- rechazo de código duplicado
- registro correcto del producto

### Caso 3: Registro de cliente

- validación de email
- validación de cédula única
- registro correcto del cliente

### Caso 4: Venta simple

- rechazo por stock insuficiente
- registro correcto de venta
- actualización de stock
- actualización del gasto del cliente
- almacenamiento correcto en `transacciones.bin`

---

## Notas Técnicas

- Lenguaje: **C++**
- Persistencia: **archivos binarios**
- Librería principal para archivos: `<fstream>`
- IDs autoincrementales manejados por `ArchivoHeader`
- La fase 2 ya no depende de arreglos dinámicos como almacenamiento principal
- Los datos se mantienen en disco entre ejecuciones
- La implementación actual prioriza estabilidad y claridad para la defensa académica

---

## Resumen de Evolución del Proyecto

### Fase 1

- sistema basado en memoria RAM
- arreglos dinámicos
- operaciones sobre estructuras en memoria
- archivo principal: `que.cpp`

### Fase 2

- sistema con persistencia binaria
- archivos separados por entidad
- headers administrativos
- registros persistentes en disco
- archivo principal: `main.cpp`

---

## Autoría y Organización del Trabajo

El repositorio conserva tanto el archivo de referencia de la fase 1 (`que.cpp`) como la implementación funcional de la fase 2 (`main.cpp`) para mostrar la evolución del proyecto.

La fase 2 se desarrolló de forma incremental, organizando la implementación por etapas:

1. base de persistencia y headers
2. proveedores binarios
3. productos binarios
4. clientes y ventas simples