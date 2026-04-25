//
// Created by Benjamin Toro Leddihn on 25/04/26.
//

#ifndef SPARSE_MATRIX_SPREADSHEET_CPP_NODE_H
#define SPARSE_MATRIX_SPREADSHEET_CPP_NODE_H

struct CellNode {
    int column;
    int row;
    std::string value;
    CellNode* right;
    CellNode* down;
};

struct RowHeader {
    int row;
    CellNode* firstCell;
    RowHeader* next;
};


struct ColumnHeader {
    int column;
    CellNode* firstCell;
    ColumnHeader* next;
};

#endif //SPARSE_MATRIX_SPREADSHEET_CPP_NODE_H
