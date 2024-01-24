cmake_minimum_required(VERSION 3.17...3.22)
set(CMAKE_CXX_STANDARD 17)
project(SciComp VERSION 1.0.0 LANGUAGES CXX)

add_library(SciCompAlgebra dense/DenseVector.cpp sparse/CSCMatrix.cpp distributed/DVector.cpp distributed/DCSCMatrix.cpp)

find_package(MPI REQUIRED)
target_link_libraries(SciCompAlgebra PUBLIC MPI::MPI_CXX)

add_executable(Blatt8 main.cpp)
target_link_libraries(Blatt8 SciCompAlgebra)
target_include_directories(Blatt8 PUBLIC dense sparse distributed)
