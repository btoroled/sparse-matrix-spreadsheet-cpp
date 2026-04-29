#include <iostream>
#include <cassert>
#include <string>
#include "../include/SparseMatrix.h"
#include "../include/operations.h"

// Mini test runner
int passed = 0, failed = 0;

void check(const std::string& name, bool condition) {
    if (condition) {
        std::cout << "[OK] " << name << "\n";
        passed++;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        failed++;
    }
}

int main() {
    std::cout << "=== Test SparseMatrix ===\n\n";

    // --- inserción básica ---
    SparseMatrix m;
    m.setCell(0, 0, "42");
    check("insertar celda (0,0)", m.getCell(0, 0) == "42");

    // --- consulta de celda inexistente ---
    check("celda inexistente retorna vacío", m.getCell(5, 5) == "");

    // --- actualización (no debe crear nodo nuevo) ---
    m.setCell(0, 0, "99");
    check("actualizar celda existente", m.getCell(0, 0) == "99");

    // --- varias celdas en misma fila ---
    m.setCell(1, 0, "10");
    m.setCell(1, 1, "20");
    m.setCell(1, 2, "30");
    check("fila 1 col 0", m.getCell(1, 0) == "10");
    check("fila 1 col 1", m.getCell(1, 1) == "20");
    check("fila 1 col 2", m.getCell(1, 2) == "30");

    // --- varias celdas en misma columna ---
    m.setCell(0, 3, "100");
    m.setCell(2, 3, "200");
    m.setCell(4, 3, "300");
    check("col 3 fila 0", m.getCell(0, 3) == "100");
    check("col 3 fila 2", m.getCell(2, 3) == "200");
    check("col 3 fila 4", m.getCell(4, 3) == "300");

    // --- eliminar celda existente ---
    bool removed = m.removeCell(1, 1);
    check("removeCell retorna true si existía", removed);
    check("celda eliminada retorna vacío", m.getCell(1, 1) == "");
    // vecinos intactos
    check("fila 1 col 0 intacta tras eliminar (1,1)", m.getCell(1, 0) == "10");
    check("fila 1 col 2 intacta tras eliminar (1,1)", m.getCell(1, 2) == "30");

    // --- eliminar celda inexistente ---
    bool removedFake = m.removeCell(9, 9);
    check("removeCell retorna false si no existía", !removedFake);

    // --- isNumeric ---
    check("isNumeric('42')", isNumeric("42"));
    check("isNumeric('3.14')", isNumeric("3.14"));
    check("isNumeric('-5')", isNumeric("-5"));
    check("isNumeric('hola') = false", !isNumeric("hola"));
    check("isNumeric('') = false", !isNumeric(""));
    check("isNumeric('1.2.3') = false", !isNumeric("1.2.3"));

    // --- suma de fila ---
    // fila 1 tiene: col0=10, col2=30 (col1 fue eliminado)
    double s = sumRow(m, 1);
    check("sumRow fila 1 = 40", s == 40.0);

    // --- suma de columna ---
    // col 3: fila0=100, fila2=200, fila4=300
    double sc = sumCol(m, 3);
    check("sumCol col 3 = 600", sc == 600.0);

    // --- suma de rango ---
    // Insertar cuadrícula limpia 3x3
    SparseMatrix m2;
    m2.setCell(0, 0, "1");
    m2.setCell(0, 1, "2");
    m2.setCell(0, 2, "3");
    m2.setCell(1, 0, "4");
    m2.setCell(1, 1, "texto"); // no numérico
    m2.setCell(1, 2, "6");
    m2.setCell(2, 0, "7");
    m2.setCell(2, 1, "8");
    m2.setCell(2, 2, "9");

    check("sumRange(0,0,2,2) = 40 (ignora texto)", sumRange(m2, 0, 0, 2, 2) == 40.0);
    check("avgRange(0,0,2,2) = 5.0", avgRange(m2, 0, 0, 2, 2) == 5.0);
    check("minRange(0,0,2,2) = 1", minRange(m2, 0, 0, 2, 2) == 1.0);
    check("maxRange(0,0,2,2) = 9", maxRange(m2, 0, 0, 2, 2) == 9.0);
    check("countRange = 8 (excluye texto)", countRange(m2, 0, 0, 2, 2) == 8);

    // --- rango sin celdas numéricas ---
    SparseMatrix m3;
    m3.setCell(0, 0, "hola");
    check("sumRange sin numéricos = 0", sumRange(m3, 0, 0, 5, 5) == 0.0);
    check("avgRange sin numéricos = 0", avgRange(m3, 0, 0, 5, 5) == 0.0);

    // --- deleteRow ---
    SparseMatrix m4;
    m4.setCell(0, 0, "A");
    m4.setCell(0, 1, "B");
    m4.setCell(1, 0, "C");
    m4.deleteRow(0);
    check("deleteRow: (0,0) eliminado", m4.getCell(0, 0) == "");
    check("deleteRow: (0,1) eliminado", m4.getCell(0, 1) == "");
    check("deleteRow: (1,0) intacto", m4.getCell(1, 0) == "C");

    // --- deleteCol ---
    SparseMatrix m5;
    m5.setCell(0, 0, "X");
    m5.setCell(1, 0, "Y");
    m5.setCell(0, 1, "Z");
    m5.deleteCol(0);
    check("deleteCol: (0,0) eliminado", m5.getCell(0, 0) == "");
    check("deleteCol: (1,0) eliminado", m5.getCell(1, 0) == "");
    check("deleteCol: (0,1) intacto", m5.getCell(0, 1) == "Z");

    // --- deleteRange ---
    SparseMatrix m6;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            m6.setCell(r, c, std::to_string(r * 4 + c));
    m6.deleteRange(1, 1, 2, 2);
    check("deleteRange: (1,1) eliminado", m6.getCell(1, 1) == "");
    check("deleteRange: (2,2) eliminado", m6.getCell(2, 2) == "");
    check("deleteRange: (0,0) intacto", m6.getCell(0, 0) == "0");
    check("deleteRange: (3,3) intacto", m6.getCell(3, 3) == "15");

    // --- destructor (no crash) ---
    {
        SparseMatrix temp;
        temp.setCell(0, 0, "boom?");
        temp.setCell(1, 1, "no");
    } // destructor llamado aquí
    check("destructor sin crash", true);

    std::cout << "\n=== Resultado: " << passed << " OK, " << failed << " FAIL ===\n";
    return failed > 0 ? 1 : 0;
}
