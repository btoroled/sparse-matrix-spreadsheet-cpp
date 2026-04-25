//
// Created by Benjamin Toro Leddihn on 25/04/26.
//

#include "../include//SparseMatrix.h"
#include <iostream>
#include "../include/Node.h"

SparseMatrix::SparseMatrix() {
    rowHeader = NULL;
    columnHeader = NULL;
}


CellNode* SparseMatrix::findRow(int row) {
    return NULL;
}

CellNode* SparseMatrix::findColumn(int column) {
    return NULL;
}

CellNode* SparseMatrix::getOrCreateRow(int row) {
    return NULL;
}

CellNode* SparseMatrix::getOrCreateColumn(int column) {
    return NULL;
}

CellNode* SparseMatrix::findNodeInRow(int row) {
    return NULL;
}

CellNode* SparseMatrix::findNodeInColumn(int column) {
    return NULL;
}

void SparseMatrix::setCell(int row, int column, std::string value) {

}

void SparseMatrix::insertCell(int row, int column, std::string value) {

}

void SparseMatrix::insertColumn() {

}

std::string getCell(int row, int column) {
    return "";
}


bool SparseMatrix::removeCell(int row, int column) {
    return true;
}



