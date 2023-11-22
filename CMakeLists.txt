cmake_minimum_required(VERSION 3.0.0)
project(Blatt3 VERSION 1.0.0 LANGUAGES C CXX)

add_executable(Blatt3 EWR/main.cpp EWR/dense/DenseVector.cpp EWR/dense/DenseMatrix.cpp EWR/sparse/CSCMatrix.cpp)
