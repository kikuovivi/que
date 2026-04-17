CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

TARGET = inventario_v3

SOURCES = main.cpp \
	persistencia/GestorArchivos.cpp \
	utilidades/Validaciones.cpp \
	utilidades/Formatos.cpp \
	tienda/Tienda.cpp \
	interfaz/Interfaz.cpp \
	proveedores/Proveedor.cpp \
	proveedores/operacionesProveedores.cpp \
	clientes/Cliente.cpp \
	clientes/operacionesClientes.cpp \
	productos/Producto.cpp \
	productos/operacionesProductos.cpp \
	transacciones/Transaccion.cpp \
	transacciones/operacionesTransacciones.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	./$(TARGET)