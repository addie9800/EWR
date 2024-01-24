#ifndef DISTRIBUTED_VECTOR_HPP
#define DISTRIBUTED_VECTOR_HPP

#include "../dense/DenseVector.hpp"
#include <mpi.h>
#include <vector>

class DVector {
  int comm_size, comm_rank;
  size_t offset;
  DenseVector data;

public:
  // constructor: takes a list of values and stores them as distributed vector.
  // the list is sliced and only the local part is stored.
  DVector(std::vector<double> init);
  // output vector
  friend std::ostream &operator<<(std::ostream &os, const DVector &vec);
};

#endif
