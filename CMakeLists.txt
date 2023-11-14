cmake_minimum_required(VERSION 3.0.0)
project(Blatt3 VERSION 1.0.0 LANGUAGES C CXX)

add_executable(Blatt3 main.cpp dense/DenseVector.cpp dense/DenseMatrix.cpp sparse/CSCMatrix.cpp)
