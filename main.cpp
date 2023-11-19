#include <iostream>

#include "dense/DenseMatrix.hpp"
#include "dense/DenseVector.hpp"
#include "sparse/CSCMatrix.hpp"
//TODO: Diese Import Statements ersetzen: Keine gute Lösung; ohne sie werden die implementierten Funktionen nicht geladen

int main(int, char **) {
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


  CSCMatrix B(3, 2, {{0, 0, 2}, {2, 1, -4}, {0, 1, 3}, {2,0,1}});
  // 2 3
  // 0 0
  // 0 -4
  std::cout << "B:\n" << B << "\n";
}
