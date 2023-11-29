#include <iostream>

#include <CSCMatrix.hpp>
#include <DenseMatrix.hpp>
#include <DenseVector.hpp>
#include <solvers.hpp>

int main(int, char **) {
  CSCMatrix A("../sparse-systems/system-01/", 100);
  DenseVector b("../sparse-systems/system-01/");

  auto x = solvers::conjugateGradient(A, b);

  std::cout << "CG result: " << x << "\n";

  return 0;
}
