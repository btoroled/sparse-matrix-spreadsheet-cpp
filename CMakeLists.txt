cmake_minimum_required(VERSION 4.2)
project(sparse_matrix_spreadsheet_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(sparse_matrix_spreadsheet_cpp src/main.cpp
        src/sparseMatrix.cpp
        include/sparseMatrix.h)
