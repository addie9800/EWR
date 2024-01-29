#include "DVector.hpp"
#include <cmath>

DVector::DVector(std::vector<double> init) {
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

  if (init.size() % int (std::sqrt(comm_size)) != 0)
  {
      throw std::invalid_argument("n should be multiple of sqrt(p)!");
  }else if (std::floor(std::sqrt(comm_size)) != std::sqrt(comm_size))
  {
      throw std::invalid_argument("p should be quadratic! p is " + std::to_string(comm_size));
  }

  int k = init.size()/std::sqrt(comm_size);
  offset = (int (comm_rank % int (std::sqrt(comm_size)))) * k;

  std::vector<double> local_init(init.begin() + offset, init.begin() + offset + k);

  data = DenseVector(local_init);
  // std::cout << "Proc. " << comm_rank << "has vec " << data << "\n";
}

std::ostream &operator<<(std::ostream &os, const DVector &vec) {
  if (vec.comm_rank == 0){
    // Since it's easier to coordinate, the printing is being done in the root process
    os << "(";
    os << vec.data(0);
    for (int i = 1; i < vec.data.size(); i++){
      // printing of local data
      os << ", ";
      os << vec.data(i);
    } 
    int sqrt_size = int(std::sqrt(vec.comm_size));
    for (int i = 1; i < sqrt_size; i++){
      for (int j = 0; j < vec.data.size(); j++){
        // receives and prints the entries in ascending order from the subprocesses
        int received = 0;
        MPI_Recv(&received, 1, MPI_INT, i , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        os << ", ";
        os << received;
      } 
    } 
    os << ")^T\n";
  } else if (vec.comm_rank / int(std::sqrt(vec.comm_size)) == 0)
  {
    for (int i = 0; i < vec.data.size(); i++){
      // send the local data to the root process
      int current = vec.data(i);
      MPI_Send(&current, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } 
    
  }
  

  return os;
}