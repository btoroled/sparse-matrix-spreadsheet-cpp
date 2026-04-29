#ifndef NODE_H
#define NODE_H

#include <string>

// Un nodo representa una celda no-vacía en la hoja de cálculo.
// Solo existimos si tenemos contenido —> las celdas vacías no gastan memoria.
struct CellNode {
    int row;
    int col;
    std::string value;
    CellNode* right; // siguiente nodo en la misma fila
    CellNode* down;  // siguiente nodo en la misma columna
};

// Cabecera de fila: punto de entrada para recorrer todos los nodos de una fila
struct RowHeader {
    int row;
    CellNode* first; // primer nodo de la fila
    RowHeader* next; // siguiente cabecera de fila
};

// Cabecera de columna: punto de entrada para recorrer todos los nodos de una columna
struct ColHeader {
    int col;
    CellNode* first; // primer nodo de la columna
    ColHeader* next; // siguiente cabecera de columna
};

#endif
