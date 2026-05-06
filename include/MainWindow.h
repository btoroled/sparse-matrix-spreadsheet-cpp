#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include "SparseMatrix.h"
#include "operations.h"
#include "formula.h"

// Ventana principal de la hoja de cálculo.
// La tabla visual muestra solo las celdas que existen en la SparseMatrix.
// Los botones llaman directamente a los métodos del backend.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    // Celda
    void onSetCell();
    void onDeleteCell();
    void onCellSelected(int row, int col);

    // Fila / columna
    void onDeleteRow();
    void onDeleteCol();
    void onDeleteRange();

    // Agregaciones
    void onSumRow();
    void onAvgRow();
    void onSumCol();
    void onAvgCol();
    void onSumRange();
    void onAvgRange();
    void onMinRange();
    void onMaxRange();

private:
    SparseMatrix matrix;

    // Tabla visual
    QTableWidget* table;

    // Inputs de coordenada principal
    QSpinBox*  inputRow;
    QSpinBox*  inputCol;
    QLineEdit* inputValue;

    // Inputs para rango
    QSpinBox* inputR1;
    QSpinBox* inputC1;
    QSpinBox* inputR2;
    QSpinBox* inputC2;

    // Barra de estado inferior
    QLabel* statusLabel;

    // Reconstruye la tabla a partir de la SparseMatrix
    void refreshTable();

    // Muestra un mensaje en la barra de estado
    void setStatus(const QString& msg);
};

#endif
