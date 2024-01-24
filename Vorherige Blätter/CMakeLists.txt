cmake_minimum_required(VERSION 3.17)
set (CMAKE_CXX_STANDARD 17)
project(SciComp VERSION 1.0.0 LANGUAGES CXX)

add_library(SciCompAlgebra dense/DenseVector.cpp dense/DenseMatrix.cpp sparse/CSCMatrix.cpp solvers/solvers.cpp)

add_executable(Blatt5 main.cpp)
target_link_libraries(Blatt5 SciCompAlgebra)
target_include_directories(Blatt5 PUBLIC dense sparse solvers)
