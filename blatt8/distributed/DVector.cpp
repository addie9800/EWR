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
  if (vec.comm_rank == 0){
    os << "(";
    os << vec.data(0);
    for (int i = 1; i < vec.data.size(); i++){
      os << ", ";
      os << vec.data(i);
    } 
    int sqrt_size = int(std::sqrt(vec.comm_size));
    for (int i = 1; i < sqrt_size; i++){
      for (int j = 0; j < vec.data.size(); j++){
        int received = 0;
        MPI_Recv(&received, 1, MPI_INT, i * sqrt_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        os << ", ";
        os << received;
      } 
    } 
    os << ")^T\n";
  } else if (vec.comm_rank % int(std::sqrt(vec.comm_size)) == 0)
  {
    for (int i = 0; i < vec.data.size(); i++){
      int current = vec.data(i);
      MPI_Send(&current, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } 
    
  }
  

  return os;
}