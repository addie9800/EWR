#ifndef DISTRIBUTED_CSCMATRIX_HPP
#define DISTRIBUTED_CSCMATRIX_HPP

#include "../sparse/CSCMatrix.hpp"
#include "DVector.hpp"
#include <mpi.h>

class DCSCMatrix {
  CSCMatrix data;

public:
  // constructor: takes a list of triplets and stores them as distributed
  // CSCMatrix. Triplets that are not local to this matrix block are ignored.
  DCSCMatrix(size_t rows, size_t cols, std::vector<Triplet> triplets);

  DVector operator*(const DVector &rhs) const;
};

#endif
