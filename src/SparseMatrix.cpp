#include "../include/SparseMatrix.h"
#include <iostream>

SparseMatrix::SparseMatrix() {
    rowHeaders = nullptr;
    colHeaders = nullptr;
}

SparseMatrix::~SparseMatrix() {
    clear();
}

// --- helpers privados ---

RowHeader* SparseMatrix::findRowHeader(int row) {
    RowHeader* rh = rowHeaders;
    while (rh && rh->row != row) rh = rh->next;
    return rh;
}

ColHeader* SparseMatrix::findColHeader(int col) {
    ColHeader* ch = colHeaders;
    while (ch && ch->col != col) ch = ch->next;
    return ch;
}

// Busca o crea cabecera de fila, manteniendo orden ascendente
RowHeader* SparseMatrix::getOrCreateRowHeader(int row) {
    // Caso: lista vacía o nuevo mínimo
    if (!rowHeaders || rowHeaders->row > row) {
        RowHeader* rh = new RowHeader{row, nullptr, rowHeaders};
        rowHeaders = rh;
        return rh;
    }
    RowHeader* cur = rowHeaders;
    while (cur->next && cur->next->row < row) cur = cur->next;
    if (cur->row == row) return cur;
    if (cur->next && cur->next->row == row) return cur->next;
    // insertar entre cur y cur->next
    RowHeader* rh = new RowHeader{row, nullptr, cur->next};
    cur->next = rh;
    return rh;
}

// Busca o crea cabecera de columna, manteniendo orden ascendente
ColHeader* SparseMatrix::getOrCreateColHeader(int col) {
    if (!colHeaders || colHeaders->col > col) {
        ColHeader* ch = new ColHeader{col, nullptr, colHeaders};
        colHeaders = ch;
        return ch;
    }
    ColHeader* cur = colHeaders;
    while (cur->next && cur->next->col < col) cur = cur->next;
    if (cur->col == col) return cur;
    if (cur->next && cur->next->col == col) return cur->next;
    ColHeader* ch = new ColHeader{col, nullptr, cur->next};
    cur->next = ch;
    return ch;
}

// --- operaciones públicas ---

// O(k_fila + k_col): recorre los nodos de la fila y de la columna para insertar en orden
void SparseMatrix::setCell(int row, int col, const std::string& value) {
    // Primero: ¿ya existe el nodo?
    RowHeader* rh = findRowHeader(row);
    if (rh) {
        CellNode* cur = rh->first;
        while (cur && cur->col != col) cur = cur->right;
        if (cur) {
            cur->value = value; // actualizar, sin crear nodo nuevo
            return;
        }
    }

    // No existe —> crear nodo nuevo
    CellNode* node = new CellNode{row, col, value, nullptr, nullptr};

    // Insertar en lista de fila (orden por columna)
    RowHeader* rowH = getOrCreateRowHeader(row);
    if (!rowH->first || rowH->first->col > col) {
        node->right = rowH->first;
        rowH->first = node;
    } else {
        CellNode* cur = rowH->first;
        while (cur->right && cur->right->col < col) cur = cur->right;
        node->right = cur->right;
        cur->right = node;
    }

    // Insertar en lista de columna (orden por fila)
    ColHeader* colH = getOrCreateColHeader(col);
    if (!colH->first || colH->first->row > row) {
        node->down = colH->first;
        colH->first = node;
    } else {
        CellNode* cur = colH->first;
        while (cur->down && cur->down->row < row) cur = cur->down;
        node->down = cur->down;
        cur->down = node;
    }
}

// O(k_fila): recorre la lista de la fila hasta encontrar la columna
std::string SparseMatrix::getCell(int row, int col) {
    RowHeader* rh = findRowHeader(row);
    if (!rh) return "";
    CellNode* cur = rh->first;
    while (cur && cur->col != col) cur = cur->right;
    return cur ? cur->value : "";
}

// O(k_fila + k_col): ajusta punteros en ambas listas antes de liberar
bool SparseMatrix::removeCell(int row, int col) {
    RowHeader* rh = findRowHeader(row);
    if (!rh) return false;

    // Desvincular de la lista de fila
    CellNode* target = nullptr;
    if (rh->first && rh->first->col == col) {
        target = rh->first;
        rh->first = target->right;
    } else {
        CellNode* cur = rh->first;
        while (cur && cur->right && cur->right->col != col) cur = cur->right;
        if (!cur || !cur->right) return false;
        target = cur->right;
        cur->right = target->right;
    }

    // Desvincular de la lista de columna
    ColHeader* ch = findColHeader(col);
    if (ch) {
        if (ch->first == target) {
            ch->first = target->down;
        } else {
            CellNode* cur = ch->first;
            while (cur && cur->down != target) cur = cur->down;
            if (cur) cur->down = target->down;
        }
    }

    delete target;
    return true;
}

// O(k_fila): elimina todos los nodos de una fila, actualizando punteros de columna
void SparseMatrix::deleteRow(int row) {
    RowHeader* rh = findRowHeader(row);
    if (!rh) return;

    CellNode* cur = rh->first;
    while (cur) {
        CellNode* next = cur->right;
        // desvincular de su columna
        ColHeader* ch = findColHeader(cur->col);
        if (ch) {
            if (ch->first == cur) {
                ch->first = cur->down;
            } else {
                CellNode* c = ch->first;
                while (c && c->down != cur) c = c->down;
                if (c) c->down = cur->down;
            }
        }
        delete cur;
        cur = next;
    }

    // Eliminar la cabecera de fila de la lista
    if (rowHeaders == rh) {
        rowHeaders = rh->next;
    } else {
        RowHeader* r = rowHeaders;
        while (r && r->next != rh) r = r->next;
        if (r) r->next = rh->next;
    }
    delete rh;
}

// O(k_col): elimina todos los nodos de una columna, actualizando punteros de fila
void SparseMatrix::deleteCol(int col) {
    ColHeader* ch = findColHeader(col);
    if (!ch) return;

    CellNode* cur = ch->first;
    while (cur) {
        CellNode* next = cur->down;
        // desvincular de su fila
        RowHeader* rh = findRowHeader(cur->row);
        if (rh) {
            if (rh->first == cur) {
                rh->first = cur->right;
            } else {
                CellNode* c = rh->first;
                while (c && c->right != cur) c = c->right;
                if (c) c->right = cur->right;
            }
        }
        delete cur;
        cur = next;
    }

    // Eliminar la cabecera de columna
    if (colHeaders == ch) {
        colHeaders = ch->next;
    } else {
        ColHeader* c = colHeaders;
        while (c && c->next != ch) c = c->next;
        if (c) c->next = ch->next;
    }
    delete ch;
}

// O(filas_en_rango * k_fila): aplica removeCell a cada celda del rango
void SparseMatrix::deleteRange(int r1, int c1, int r2, int c2) {
    RowHeader* rh = rowHeaders;
    while (rh) {
        if (rh->row >= r1 && rh->row <= r2) {
            // recolectar columnas a eliminar (no modificar la lista mientras la recorremos)
            CellNode* cur = rh->first;
            while (cur) {
                CellNode* next = cur->right;
                if (cur->col >= c1 && cur->col <= c2)
                    removeCell(rh->row, cur->col);
                cur = next;
            }
        }
        rh = rh->next;
    }
}

// Libera toda la memoria: recorre nodo a nodo por filas
void SparseMatrix::clear() {
    RowHeader* rh = rowHeaders;
    while (rh) {
        CellNode* cell = rh->first;
        while (cell) {
            CellNode* next = cell->right;
            delete cell;
            cell = next;
        }
        RowHeader* nextRH = rh->next;
        delete rh;
        rh = nextRH;
    }
    rowHeaders = nullptr;

    ColHeader* ch = colHeaders;
    while (ch) {
        ColHeader* next = ch->next;
        delete ch; // los CellNode ya fueron liberados arriba
        ch = next;
    }
    colHeaders = nullptr;
}
