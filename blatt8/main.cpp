#include <mpi.h>
#include <numeric>

#include <DCSCMatrix.hpp>
#include <DVector.hpp>
#include <cmath>

int main(int argc, char *argv[]) {

  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // (MPI debugging)
  bool wait = false;
  while (wait)
    ;

  const size_t n =
      std::sqrt(size) * 3; // matrix size depends on number of processes

  // create matrix data - this example is a matrix with 1 on the main diagonal
  // and 2 where i and j are multiple of 4.
  std::vector<Triplet> triplets;
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (i == j){ 
        triplets.push_back({i, i, 1});
      } 
      if (i % 4 == 0 && j % 4 == 0 && i != j){ 
        triplets.push_back({i, j, 2});
      } 
    }
  }
  if (rank == 0){
    std::cout << rank << " Matrix " << CSCMatrix{n,n,triplets} << "\n"; 
  } 
  // create vector data (0, 1, 2, 3, 4, ..., n-1)
  std::vector<double> b_vals(n);
  std::iota(b_vals.begin(), b_vals.end(), 0);

  // create distributed matrix/vector objects
  DCSCMatrix A{n, n, triplets};
  //DVector b(b_vals);

  // SpMV and print result
  //auto c = A * b;
  //std::cout << c;

  // expected result:
  // for p=4: (8 1 2 3 4 5)
  // for p=9: (24 1 2 3 20 5 6 7 16)
  // for p=16:(24 1 2 3 20 5 6 7 16 9 10 11)

  MPI_Finalize();

  return 0;
}
