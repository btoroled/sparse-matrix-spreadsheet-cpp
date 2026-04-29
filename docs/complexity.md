# Análisis de Complejidad — Sparse Matrix Spreadsheet

## Estructura elegida: Lista enlazada cruzada

Cada celda no vacía es un `CellNode` con punteros `right` (siguiente en su fila) y `down` (siguiente en su columna). Hay cabeceras separadas para filas (`RowHeader`) y columnas (`ColHeader`).

**Por qué no una matriz densa:** una hoja de cálculo con 1000×1000 celdas usaría 1M de nodos aunque solo 100 tengan contenido. La lista enlazada cruzada usa exactamente `n` nodos, donde `n` es el número de celdas con contenido.

---

## Notación

- `n` = número total de celdas con contenido
- `k_f` = número de celdas en una fila concreta
- `k_c` = número de celdas en una columna concreta
- `k_r` = número de celdas en un rango rectangular

---

## Complejidad temporal por operación

| Operación | Tiempo | Razonamiento |
|---|---|---|
| `setCell(row, col, value)` | O(k_f + k_c) | Recorre la fila para insertar en orden y la columna para insertar en orden. Si ya existe, solo O(k_f) para encontrarlo y actualizar. |
| `getCell(row, col)` | O(k_f) | Encuentra la cabecera de fila en O(filas activas), luego recorre la lista de esa fila hasta encontrar la columna. |
| `removeCell(row, col)` | O(k_f + k_c) | Debe desvincular el nodo de su lista de fila y de su lista de columna — dos recorridos. |
| `deleteRow(row)` | O(k_f * k_c_avg) | Por cada nodo de la fila, desvincula de su columna: O(k_c) por nodo. |
| `deleteCol(col)` | O(k_c * k_f_avg) | Simétrico a deleteRow. |
| `deleteRange(r1,c1,r2,c2)` | O(k_r * (k_f + k_c)) | Aplica removeCell a cada celda del rango. |
| `sumRow / avgRow` | O(k_f) | Un único recorrido por la lista de la fila. |
| `sumCol / avgCol` | O(k_c) | Un único recorrido por la lista de la columna. |
| `sumRange / avgRange / minRange / maxRange` | O(k_r) | Recorre solo los nodos existentes dentro del rango, sin visitar celdas vacías. |
| `clear` | O(n) | Libera todos los nodos una vez. |

**Ventaja clave frente a matriz densa:** en una matriz densa `sumRange` costaría O(filas × cols) aunque el rango esté casi vacío. Aquí cuesta O(k_r), proporcional a cuántas celdas *tienen* contenido en ese rango.

---

## Complejidad espacial

| Componente | Espacio |
|---|---|
| Nodos de celda | O(n) |
| Cabeceras de fila | O(filas activas) |
| Cabeceras de columna | O(columnas activas) |
| **Total** | **O(n)** |

Una matriz densa de 1000×1000 con 100 celdas ocupadas usaría **10,000×** más memoria que esta estructura.

---

## Casos borde manejados

- `getCell` en celda inexistente -> retorna `""` sin crash.
- `removeCell` en celda inexistente -> retorna `false` sin crash.
- `deleteRow` / `deleteCol` en fila/columna vacía -> no hace nada.
- Agregaciones sobre rango sin valores numéricos -> retorna 0.
- Celdas con texto -> `isNumeric` las filtra antes de cualquier operación aritmética.
