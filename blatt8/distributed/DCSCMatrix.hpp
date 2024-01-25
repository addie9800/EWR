#ifndef DISTRIBUTED_CSCMATRIX_HPP
#define DISTRIBUTED_CSCMATRIX_HPP

#include "../sparse/CSCMatrix.hpp"
#include "DVector.hpp"
#include <mpi.h>

class DCSCMatrix {
  // Änderung hier, da sonst Problem mit Compiler, da kein default Constructor existiert
  CSCMatrix data = CSCMatrix(1,1,{{0,0,1}});

public:
  // constructor: takes a list of triplets and stores them as distributed
  // CSCMatrix. Triplets that are not local to this matrix block are ignored.
  DCSCMatrix(size_t rows, size_t cols, std::vector<Triplet> triplets);

  DVector operator*(const DVector &rhs) const;
 ~DCSCMatrix(); 
};

#endif
