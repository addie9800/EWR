#include <iostream>

#include "dense/DenseMatrix.hpp"
#include "dense/DenseVector.hpp"
#include "sparse/CSCMatrix.hpp"
#include "solvers/solvers.hpp"
#include <chrono>
using namespace std::chrono;

int main(int, char **) {
  CSCMatrix A("../sparse-systems/system-03/", 5000);
  DenseVector b("../sparse-systems/system-03/");

  /*
  int wdh = 100;
  double time = 0.0;

  std::cout << "iteration, time\n";

  for (int i = 0; i < wdh; i++) {
    auto start = high_resolution_clock::now();
    auto x = solvers::conjugateGradient(A, b);
    auto stop = high_resolution_clock::now();
    double duration = duration_cast<microseconds>(stop - start).count();
    time += duration;
    std::cout << i << "," << duration << "\n";
  }

  std::cout << "Average: " << time/wdh;*/

  auto x = solvers::conjugateGradient(A, b);
  
  //std::cout << "CG result: " << x << "\n";


  return 0;
}