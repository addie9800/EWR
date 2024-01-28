#include "DVector.hpp"
#include <cmath>

DVector::DVector(std::vector<double> init) {
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

  int k = init.size()/std::sqrt(comm_size);
  offset = (int (comm_rank / std::sqrt(comm_size))) * k;

  std::vector<double> local_init(init.begin() + offset, init.begin() + offset + k);

  data = DenseVector(local_init);
  // std::cout << "Proc. " << comm_rank << "has vec " << data << "\n";
}

std::ostream &operator<<(std::ostream &os, const DVector &vec) {
  // TODO: implement print for DVector. Printing should include global indices
  // for each element in the vector
  return os;
}