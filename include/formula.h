#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include "SparseMatrix.h"

// Evalúa una expresión que empieza con '='.
// Soporta: =A1+B2, =A1*3, =10-B1, =A1/2
// Retorna el resultado como string, o "ERROR" si no se puede evaluar.
std::string evalFormula(const std::string& formula, SparseMatrix& m);

// Convierte referencia estilo "A1" a (row, col) base-0.
// Retorna false si el formato es inválido.
bool parseRef(const std::string& ref, int& row, int& col);

#endif
