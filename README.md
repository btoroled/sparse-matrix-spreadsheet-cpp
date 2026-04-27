# Sparse Matrix Spreadsheet

Hoja de cálculo simple implementada con una matriz dispersa usando listas enlazadas cruzadas en C++17.

## Integrantes

- Godoy Torres, Diego
- Medina Gago, Leonardo
- Toro Leddihn, Benjamin


## Descripción

Una hoja de cálculo tiene miles de celdas pero solo una fracción tiene contenido. Usar un arreglo bidimensional desperdiciaría memoria para las celdas vacías. Esta implementación usa una **lista enlazada cruzada**: solo existen los nodos de las celdas con contenido, y cada nodo apunta al siguiente en su fila y al siguiente en su columna.

## Estructura de datos

```
RowHeader  → CellNode → CellNode → nullptr
               ↓            ↓
ColHeader  CellNode      CellNode
               ↓
            nullptr
```

Cada `CellNode` almacena: `row`, `col`, `value`, `right` (siguiente en fila), `down` (siguiente en columna).

## Compilación

**Requisitos:** C++17, CMake ≥ 3.16, Qt5 Widgets.

```bash
# Ubuntu/Debian
sudo apt install qtbase5-dev cmake build-essential

# Compilar
mkdir build && cd build
cmake ..
make

# Ejecutar la aplicación
./spreadsheet

# Ejecutar tests de consola
./tests
```

## Funcionalidades

**Operaciones de celda:** insertar, consultar, modificar, eliminar.

**Operaciones de fila/columna:** eliminar fila completa, eliminar columna completa, eliminar rango rectangular.

**Agregaciones:** SUMA, PROMEDIO, MÍNIMO y MÁXIMO sobre filas, columnas y rangos.

**Fórmulas:** expresiones aritméticas con `=`, por ejemplo `=A1+B2`, `=A1*3`, `=10-B1`.

**Casos borde manejados:** celda inexistente, fila/columna vacía, rango sin valores numéricos, texto en celdas numéricas.

## Análisis de complejidad

Ver `docs/complexity.md`.
