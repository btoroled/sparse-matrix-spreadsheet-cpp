#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include "SparseMatrix.h"

// Verifica si un string puede interpretarse como número
bool isNumeric(const std::string& s);

// Conversión segura
double toDouble(const std::string& s);

// Agregaciones por fila
double sumRow(SparseMatrix& m, int row);
double avgRow(SparseMatrix& m, int row);

// Agregaciones por columna
double sumCol(SparseMatrix& m, int col);
double avgCol(SparseMatrix& m, int col);

// Agregaciones por rango rectangular [r1,c1] a [r2,c2] inclusive
double sumRange(SparseMatrix& m, int r1, int c1, int r2, int c2);
double avgRange(SparseMatrix& m, int r1, int c1, int r2, int c2);
double minRange(SparseMatrix& m, int r1, int c1, int r2, int c2);
double maxRange(SparseMatrix& m, int r1, int c1, int r2, int c2);
int    countRange(SparseMatrix& m, int r1, int c1, int r2, int c2); // celdas numéricas

#endif
