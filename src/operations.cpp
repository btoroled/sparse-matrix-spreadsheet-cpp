#include "../include/operations.h"
#include <stdexcept>
#include <climits>
#include <cfloat>

// --- validación numérica ---

bool isNumeric(const std::string& s) {
    if (s.empty()) return false;
    bool hasDot = false;
    int start = 0;
    if (s[0] == '-' || s[0] == '+') start = 1;
    if (start == (int)s.size()) return false;
    for (int i = start; i < (int)s.size(); i++) {
        if (s[i] == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}

double toDouble(const std::string& s) {
    return std::stod(s);
}

// --- por fila ---
// Recorre la lista enlazada de la fila —> O(k_fila)

double sumRow(SparseMatrix& m, int row) {
    double total = 0.0;
    RowHeader* rh = m.rowHeaders;
    while (rh && rh->row != row) rh = rh->next;
    if (!rh) return 0.0;
    CellNode* cur = rh->first;
    while (cur) {
        if (isNumeric(cur->value)) total += toDouble(cur->value);
        cur = cur->right;
    }
    return total;
}

double avgRow(SparseMatrix& m, int row) {
    double total = 0.0;
    int count = 0;
    RowHeader* rh = m.rowHeaders;
    while (rh && rh->row != row) rh = rh->next;
    if (!rh) return 0.0;
    CellNode* cur = rh->first;
    while (cur) {
        if (isNumeric(cur->value)) { total += toDouble(cur->value); count++; }
        cur = cur->right;
    }
    return count > 0 ? total / count : 0.0;
}

// --- por columna ---
// Recorre la lista enlazada de la columna —> O(k_col)

double sumCol(SparseMatrix& m, int col) {
    double total = 0.0;
    ColHeader* ch = m.colHeaders;
    while (ch && ch->col != col) ch = ch->next;
    if (!ch) return 0.0;
    CellNode* cur = ch->first;
    while (cur) {
        if (isNumeric(cur->value)) total += toDouble(cur->value);
        cur = cur->down;
    }
    return total;
}

double avgCol(SparseMatrix& m, int col) {
    double total = 0.0;
    int count = 0;
    ColHeader* ch = m.colHeaders;
    while (ch && ch->col != col) ch = ch->next;
    if (!ch) return 0.0;
    CellNode* cur = ch->first;
    while (cur) {
        if (isNumeric(cur->value)) { total += toDouble(cur->value); count++; }
        cur = cur->down;
    }
    return count > 0 ? total / count : 0.0;
}

// --- por rango ---
// Itera solo sobre nodos existentes —> O(nodos_en_rango), no O(filas*cols)

double sumRange(SparseMatrix& m, int r1, int c1, int r2, int c2) {
    double total = 0.0;
    RowHeader* rh = m.rowHeaders;
    while (rh) {
        if (rh->row >= r1 && rh->row <= r2) {
            CellNode* cur = rh->first;
            while (cur && cur->col <= c2) {
                if (cur->col >= c1 && isNumeric(cur->value))
                    total += toDouble(cur->value);
                cur = cur->right;
            }
        }
        rh = rh->next;
    }
    return total;
}

int countRange(SparseMatrix& m, int r1, int c1, int r2, int c2) {
    int count = 0;
    RowHeader* rh = m.rowHeaders;
    while (rh) {
        if (rh->row >= r1 && rh->row <= r2) {
            CellNode* cur = rh->first;
            while (cur && cur->col <= c2) {
                if (cur->col >= c1 && isNumeric(cur->value)) count++;
                cur = cur->right;
            }
        }
        rh = rh->next;
    }
    return count;
}

double avgRange(SparseMatrix& m, int r1, int c1, int r2, int c2) {
    int count = countRange(m, r1, c1, r2, c2);
    if (count == 0) return 0.0;
    return sumRange(m, r1, c1, r2, c2) / count;
}

double minRange(SparseMatrix& m, int r1, int c1, int r2, int c2) {
    double result = DBL_MAX;
    bool found = false;
    RowHeader* rh = m.rowHeaders;
    while (rh) {
        if (rh->row >= r1 && rh->row <= r2) {
            CellNode* cur = rh->first;
            while (cur && cur->col <= c2) {
                if (cur->col >= c1 && isNumeric(cur->value)) {
                    double v = toDouble(cur->value);
                    if (v < result) result = v;
                    found = true;
                }
                cur = cur->right;
            }
        }
        rh = rh->next;
    }
    return found ? result : 0.0;
}

double maxRange(SparseMatrix& m, int r1, int c1, int r2, int c2) {
    double result = -DBL_MAX;
    bool found = false;
    RowHeader* rh = m.rowHeaders;
    while (rh) {
        if (rh->row >= r1 && rh->row <= r2) {
            CellNode* cur = rh->first;
            while (cur && cur->col <= c2) {
                if (cur->col >= c1 && isNumeric(cur->value)) {
                    double v = toDouble(cur->value);
                    if (v > result) result = v;
                    found = true;
                }
                cur = cur->right;
            }
        }
        rh = rh->next;
    }
    return found ? result : 0.0;
}
