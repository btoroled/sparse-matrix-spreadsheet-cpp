cmake_minimum_required(VERSION 3.16)
project(sparse_matrix_spreadsheet_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)   # necesario para que Qt procese las macros Q_OBJECT

find_package(Qt5 REQUIRED COMPONENTS Widgets)

# --- Aplicación principal (GUI) ---
add_executable(spreadsheet
    src/main.cpp
    src/MainWindow.cpp
    src/SparseMatrix.cpp
    src/operations.cpp
    src/formula.cpp
    include/MainWindow.h
    include/SparseMatrix.h
    include/Node.h
    include/operations.h
    include/formula.h
)
target_include_directories(spreadsheet PRIVATE include)
target_link_libraries(spreadsheet Qt5::Widgets)

# --- Tests de consola (sin Qt, compilan solos) ---
add_executable(tests
    src/test_sparse.cpp
    src/SparseMatrix.cpp
    src/operations.cpp
    include/SparseMatrix.h
    include/Node.h
    include/operations.h
)
target_include_directories(tests PRIVATE include)
