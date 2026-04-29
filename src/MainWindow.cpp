#include "../include/MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QHeaderView>
#include <QStatusBar>
#include <QString>
#include <sstream>

// Cuántas filas/columnas muestra la tabla por defecto
static const int VISIBLE_ROWS = 20;
static const int VISIBLE_COLS = 10;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Sparse Matrix Spreadsheet");
    setMinimumSize(900, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // Layout principal: tabla a la izquierda, panel de control a la derecha
    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    // --- Tabla visual ---
    table = new QTableWidget(VISIBLE_ROWS, VISIBLE_COLS, this);
    // Cabeceras de columna estilo hoja de cálculo: A, B, C...
    QStringList colLabels;
    for (int c = 0; c < VISIBLE_COLS; c++)
        colLabels << QString(QChar('A' + c));
    table->setHorizontalHeaderLabels(colLabels);
    // Cabeceras de fila: 1, 2, 3...
    QStringList rowLabels;
    for (int r = 0; r < VISIBLE_ROWS; r++)
        rowLabels << QString::number(r + 1);
    table->setVerticalHeaderLabels(rowLabels);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // solo lectura, edición via panel
    connect(table, &QTableWidget::cellClicked, this, &MainWindow::onCellSelected);
    mainLayout->addWidget(table, 3); // tabla ocupa 3/4 del espacio

    // --- Panel de control ---
    QVBoxLayout* panel = new QVBoxLayout();
    mainLayout->addLayout(panel, 1);

    // -- Grupo: celda individual --
    QGroupBox* cellGroup = new QGroupBox("Celda");
    QVBoxLayout* cellLayout = new QVBoxLayout(cellGroup);

    QHBoxLayout* coordLayout = new QHBoxLayout();
    coordLayout->addWidget(new QLabel("Fila:"));
    inputRow = new QSpinBox(); inputRow->setRange(1, VISIBLE_ROWS); inputRow->setValue(1);
    coordLayout->addWidget(inputRow);
    coordLayout->addWidget(new QLabel("Col:"));
    inputCol = new QSpinBox(); inputCol->setRange(1, VISIBLE_COLS); inputCol->setValue(1);
    coordLayout->addWidget(inputCol);
    cellLayout->addLayout(coordLayout);

    inputValue = new QLineEdit();
    inputValue->setPlaceholderText("Valor (ej: 42, hola, =A1+B1)");
    cellLayout->addWidget(inputValue);

    QPushButton* btnSet    = new QPushButton("Insertar / Actualizar");
    QPushButton* btnDelete = new QPushButton("Eliminar celda");
    cellLayout->addWidget(btnSet);
    cellLayout->addWidget(btnDelete);
    connect(btnSet,    &QPushButton::clicked, this, &MainWindow::onSetCell);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteCell);
    panel->addWidget(cellGroup);

    // -- Grupo: fila / columna --
    QGroupBox* rowColGroup = new QGroupBox("Fila / Columna");
    QVBoxLayout* rcLayout  = new QVBoxLayout(rowColGroup);

    QPushButton* btnDelRow = new QPushButton("Eliminar fila");
    QPushButton* btnDelCol = new QPushButton("Eliminar columna");
    rcLayout->addWidget(btnDelRow);
    rcLayout->addWidget(btnDelCol);
    connect(btnDelRow, &QPushButton::clicked, this, &MainWindow::onDeleteRow);
    connect(btnDelCol, &QPushButton::clicked, this, &MainWindow::onDeleteCol);

    // Agregaciones por fila/col
    QPushButton* btnSumRow = new QPushButton("SUMA fila");
    QPushButton* btnAvgRow = new QPushButton("PROMEDIO fila");
    QPushButton* btnSumCol = new QPushButton("SUMA columna");
    QPushButton* btnAvgCol = new QPushButton("PROMEDIO columna");
    rcLayout->addWidget(btnSumRow);
    rcLayout->addWidget(btnAvgRow);
    rcLayout->addWidget(btnSumCol);
    rcLayout->addWidget(btnAvgCol);
    connect(btnSumRow, &QPushButton::clicked, this, &MainWindow::onSumRow);
    connect(btnAvgRow, &QPushButton::clicked, this, &MainWindow::onAvgRow);
    connect(btnSumCol, &QPushButton::clicked, this, &MainWindow::onSumCol);
    connect(btnAvgCol, &QPushButton::clicked, this, &MainWindow::onAvgCol);
    panel->addWidget(rowColGroup);

    // -- Grupo: rango --
    QGroupBox* rangeGroup = new QGroupBox("Rango");
    QVBoxLayout* rangeLayout = new QVBoxLayout(rangeGroup);

    QHBoxLayout* r1c1 = new QHBoxLayout();
    r1c1->addWidget(new QLabel("F1:"));
    inputR1 = new QSpinBox(); inputR1->setRange(1, VISIBLE_ROWS); inputR1->setValue(1);
    r1c1->addWidget(inputR1);
    r1c1->addWidget(new QLabel("C1:"));
    inputC1 = new QSpinBox(); inputC1->setRange(1, VISIBLE_COLS); inputC1->setValue(1);
    r1c1->addWidget(inputC1);
    rangeLayout->addLayout(r1c1);

    QHBoxLayout* r2c2 = new QHBoxLayout();
    r2c2->addWidget(new QLabel("F2:"));
    inputR2 = new QSpinBox(); inputR2->setRange(1, VISIBLE_ROWS); inputR2->setValue(1);
    r2c2->addWidget(inputR2);
    r2c2->addWidget(new QLabel("C2:"));
    inputC2 = new QSpinBox(); inputC2->setRange(1, VISIBLE_COLS); inputC2->setValue(1);
    r2c2->addWidget(inputC2);
    rangeLayout->addLayout(r2c2);

    QPushButton* btnDelRange  = new QPushButton("Eliminar rango");
    QPushButton* btnSumRange  = new QPushButton("SUMA rango");
    QPushButton* btnAvgRange  = new QPushButton("PROMEDIO rango");
    QPushButton* btnMinRange  = new QPushButton("MÍNIMO rango");
    QPushButton* btnMaxRange  = new QPushButton("MÁXIMO rango");
    rangeLayout->addWidget(btnDelRange);
    rangeLayout->addWidget(btnSumRange);
    rangeLayout->addWidget(btnAvgRange);
    rangeLayout->addWidget(btnMinRange);
    rangeLayout->addWidget(btnMaxRange);
    connect(btnDelRange, &QPushButton::clicked, this, &MainWindow::onDeleteRange);
    connect(btnSumRange, &QPushButton::clicked, this, &MainWindow::onSumRange);
    connect(btnAvgRange, &QPushButton::clicked, this, &MainWindow::onAvgRange);
    connect(btnMinRange, &QPushButton::clicked, this, &MainWindow::onMinRange);
    connect(btnMaxRange, &QPushButton::clicked, this, &MainWindow::onMaxRange);
    panel->addWidget(rangeGroup);

    panel->addStretch();

    // --- Barra de estado ---
    statusLabel = new QLabel("Listo.");
    statusBar()->addWidget(statusLabel);
}

// Reconstruye la QTableWidget leyendo la SparseMatrix.
// Solo itera sobre nodos existentes —> O(n).
void MainWindow::refreshTable() {
    // Limpiar tabla
    for (int r = 0; r < VISIBLE_ROWS; r++)
        for (int c = 0; c < VISIBLE_COLS; c++)
            table->setItem(r, c, nullptr);

    // Recorrer nodos de la matriz dispersa
    RowHeader* rh = matrix.rowHeaders;
    while (rh) {
        if (rh->row < VISIBLE_ROWS) {
            CellNode* cur = rh->first;
            while (cur) {
                if (cur->col < VISIBLE_COLS) {
                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(cur->value));
                    item->setBackground(Qt::yellow); // celda ocupada resaltada
                    table->setItem(rh->row, cur->col, item);
                }
                cur = cur->right;
            }
        }
        rh = rh->next;
    }
}

void MainWindow::setStatus(const QString& msg) {
    statusLabel->setText(msg);
}

// Convierte inputs base-1 a base-0 para la SparseMatrix
void MainWindow::onSetCell() {
    int row = inputRow->value() - 1;
    int col = inputCol->value() - 1;
    std::string val = inputValue->text().toStdString();

    if (val.empty()) { setStatus("Error: valor vacío."); return; }

    // Si empieza con '=' es fórmula — evaluar y guardar el resultado
    if (val[0] == '=') {
        std::string result = evalFormula(val, matrix);
        matrix.setCell(row, col, result);
        setStatus(QString("Fórmula evaluada: %1 → %2")
            .arg(QString::fromStdString(val))
            .arg(QString::fromStdString(result)));
    } else {
        matrix.setCell(row, col, val);
        setStatus(QString("Celda (%1,%2) = %3")
            .arg(row+1).arg(col+1)
            .arg(QString::fromStdString(val)));
    }
    refreshTable();
}

void MainWindow::onDeleteCell() {
    int row = inputRow->value() - 1;
    int col = inputCol->value() - 1;
    bool removed = matrix.removeCell(row, col);
    setStatus(removed
        ? QString("Celda (%1,%2) eliminada.").arg(row+1).arg(col+1)
        : QString("Celda (%1,%2) no existía.").arg(row+1).arg(col+1));
    refreshTable();
}

// Al hacer click en una celda de la tabla, actualiza los inputs de coordenada
void MainWindow::onCellSelected(int row, int col) {
    inputRow->setValue(row + 1);
    inputCol->setValue(col + 1);
    std::string val = matrix.getCell(row, col);
    inputValue->setText(QString::fromStdString(val));
    setStatus(val.empty()
        ? QString("Celda (%1,%2) vacía.").arg(row+1).arg(col+1)
        : QString("Celda (%1,%2) = %3").arg(row+1).arg(col+1).arg(QString::fromStdString(val)));
}

void MainWindow::onDeleteRow() {
    int row = inputRow->value() - 1;
    matrix.deleteRow(row);
    setStatus(QString("Fila %1 eliminada.").arg(row+1));
    refreshTable();
}

void MainWindow::onDeleteCol() {
    int col = inputCol->value() - 1;
    matrix.deleteCol(col);
    setStatus(QString("Columna %1 eliminada.").arg(col+1));
    refreshTable();
}

void MainWindow::onDeleteRange() {
    int r1 = inputR1->value()-1, c1 = inputC1->value()-1;
    int r2 = inputR2->value()-1, c2 = inputC2->value()-1;
    matrix.deleteRange(r1, c1, r2, c2);
    setStatus(QString("Rango (%1,%2):(%3,%4) eliminado.").arg(r1+1).arg(c1+1).arg(r2+1).arg(c2+1));
    refreshTable();
}

void MainWindow::onSumRow() {
    int row = inputRow->value() - 1;
    double res = sumRow(matrix, row);
    setStatus(QString("SUMA fila %1 = %2").arg(row+1).arg(res));
}

void MainWindow::onAvgRow() {
    int row = inputRow->value() - 1;
    double res = avgRow(matrix, row);
    setStatus(QString("PROMEDIO fila %1 = %2").arg(row+1).arg(res));
}

void MainWindow::onSumCol() {
    int col = inputCol->value() - 1;
    double res = sumCol(matrix, col);
    setStatus(QString("SUMA columna %1 = %2").arg(col+1).arg(res));
}

void MainWindow::onAvgCol() {
    int col = inputCol->value() - 1;
    double res = avgCol(matrix, col);
    setStatus(QString("PROMEDIO columna %1 = %2").arg(col+1).arg(res));
}

void MainWindow::onSumRange() {
    int r1=inputR1->value()-1, c1=inputC1->value()-1;
    int r2=inputR2->value()-1, c2=inputC2->value()-1;
    double res = sumRange(matrix, r1, c1, r2, c2);
    setStatus(QString("SUMA rango (%1,%2):(%3,%4) = %5").arg(r1+1).arg(c1+1).arg(r2+1).arg(c2+1).arg(res));
}

void MainWindow::onAvgRange() {
    int r1=inputR1->value()-1, c1=inputC1->value()-1;
    int r2=inputR2->value()-1, c2=inputC2->value()-1;
    double res = avgRange(matrix, r1, c1, r2, c2);
    setStatus(QString("PROMEDIO rango (%1,%2):(%3,%4) = %5").arg(r1+1).arg(c1+1).arg(r2+1).arg(c2+1).arg(res));
}

void MainWindow::onMinRange() {
    int r1=inputR1->value()-1, c1=inputC1->value()-1;
    int r2=inputR2->value()-1, c2=inputC2->value()-1;
    double res = minRange(matrix, r1, c1, r2, c2);
    setStatus(QString("MÍNIMO rango (%1,%2):(%3,%4) = %5").arg(r1+1).arg(c1+1).arg(r2+1).arg(c2+1).arg(res));
}

void MainWindow::onMaxRange() {
    int r1=inputR1->value()-1, c1=inputC1->value()-1;
    int r2=inputR2->value()-1, c2=inputC2->value()-1;
    double res = maxRange(matrix, r1, c1, r2, c2);
    setStatus(QString("MÁXIMO rango (%1,%2):(%3,%4) = %5").arg(r1+1).arg(c1+1).arg(r2+1).arg(c2+1).arg(res));
}
