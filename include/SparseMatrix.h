//
// Created by Benjamin Toro Leddihn on 25/04/26.
//

#ifndef SPARSE_MATRIX_SPREADSHEET_CPP_SPARSEMATRIX_H
#define SPARSE_MATRIX_SPREADSHEET_CPP_SPARSEMATRIX_H

#include <string>
#include "Node.h"

class SparseMatrix {
    RowHeader* rowHeader;
    ColumnHeader* columnHeader;
public:
    SparseMatrix() {}

    CellNode* findRow(int row);
    CellNode* findColumn(int column);
    CellNode* getOrCreateRow(int row);
    CellNode* getOrCreateColumn(int column);
    CellNode* findNodeInRow();
    CellNode* findNodeInColumn();;
    void setCell(int row, int column, std::string value);
    void insertCell(int row, int column, std::string value);
    void insertColumn();
    std::string getCell(int row, int column);
    bool removeCell(int row, int column);
    void clear();

};




#endif //SPARSE_MATRIX_SPREADSHEET_CPP_SPARSEMATRIX_H
