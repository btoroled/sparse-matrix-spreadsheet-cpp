#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <string>
#include "Node.h"

// La matriz dispersa almacena solo las celdas con contenido.
// Estructura: listas enlazadas cruzadas —> cada nodo está en una lista de fila y en una lista de columna.
class SparseMatrix {
public:
    RowHeader* rowHeaders;   // lista de cabeceras de fila
    ColHeader* colHeaders;   // lista de cabeceras de columna

    SparseMatrix();
    ~SparseMatrix();

    // Operaciones básicas de celda
    void setCell(int row, int col, const std::string& value);  // insertar o actualizar
    std::string getCell(int row, int col);                     // consultar (vacío si no existe)
    bool removeCell(int row, int col);                         // eliminar celda

    // Operaciones sobre filas y columnas
    void deleteRow(int row);
    void deleteCol(int col);
    void deleteRange(int r1, int c1, int r2, int c2);

    // Limpieza total
    void clear();

private:
    RowHeader* findRowHeader(int row);
    ColHeader* findColHeader(int col);
    RowHeader* getOrCreateRowHeader(int row);
    ColHeader* getOrCreateColHeader(int col);
};

#endif
