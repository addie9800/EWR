#include <iostream>

#include "dense/DenseMatrix.hpp"
#include "dense/DenseVector.hpp"
#include "sparse/CSCMatrix.hpp"
#include "solvers/solvers.hpp"


int main(int, char **) {
  // FRÜHERE BLÄTTER

  /* DenseVector v{1, 2, 3};
  DenseVector w{-1, 0, 1};
  
  // Vector operations:
  v *= 2;
  std::cout << "v = " << v << "\n";         // (2, 4, 6)
  std::cout << "3w = " << 3 * w << "\n";    // (-3, 0, 3)
  std::cout << "w/2 = " << w / 2 << "\n";   // (-0.5, 0, 0.5)
  std::cout << "v + w = " << v + w << "\n"; // (1, 4, 7)
  std::cout << "<v,w> = " << v * w << "\n"; // 4
  v(2) = 42;
  std::cout << "modified v = " << v << "\n"; // (2, 4, 42)*/

  /*DenseMatrix A = {{1, 2}, {3, 4}};
  // 1, 2
  // 3, 4
  std::cout << "A:\n" << A << "\n";

  CSCMatrix D = A;

  
  DenseVector u = {-1, 1};
  std::cout << "Au = " << D * u << "\n"; // (1, 1) */


  // CSCMatrix B(2, 2, {{0, 0, 4}, {1, 1, 10}, {0, 1, 3}, {1,0,1}});
  // 2 3
  // 0 0
  // 0 -4
  //CSCMatrix A(2,2, {{0,1,1},{1,0,1}});
  //CSCMatrix B(2,2,{{0,0,1},{1,0,2}});
  //std::cout << "A:\n" << A << "\n";
  // std::cout << "B:\n" << B << "\n";
  // std::cout << "B^2:\n" << B*B << "\n";
  //std::cout << "AB:\n" << A*B << "\n";int main(int, char **) 


  // AB HIER BLATT 5

  /*
  // Tests
  DenseVector u = {-23.345, 65.88};

  CSCMatrix A = {{0.3343, 3.1414}, {3.1414, 123.456}};

  CSCMatrix T = A;
  DenseVector t = u;

  std::cout << "Test Matrix: \n" << T << "\n";
  std::cout << "Test Vector: " << t << "\n";

  auto x = solvers::conjugateGradient(T, t);

  std::cout << "CG result: " << x << "\n";
  std::cout << "Matrix-Vector Product: " << T*x << "\n"; //*/

  ///*
  CSCMatrix A("../sparse-systems/system-03/", 5000);
  DenseVector b("../sparse-systems/system-03/");

  auto x = solvers::conjugateGradient(A, b);

  //std::cout << "CG result: " << x << "\n";//*/


  return 0;
}
