//
// Created by Benjamin Toro Leddihn on 25/04/26.
//

#ifndef SPARSE_MATRIX_SPREADSHEET_CPP_NODE_H
#define SPARSE_MATRIX_SPREADSHEET_CPP_NODE_H

template <typename T>
struct CellNode {
    int column;
    int row;
    string value;
    CellNode<T> *right;
    CellNode<T> *down;
};



#endif //SPARSE_MATRIX_SPREADSHEET_CPP_NODE_H
