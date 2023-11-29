#ifndef SOLVERS_HPP
#define SOLVERS_HPP

#include "../dense/DenseVector.hpp"
#include "../sparse/CSCMatrix.hpp"

namespace solvers {

// solves the linear system Ax=b up to given tolerance
DenseVector conjugateGradient(const CSCMatrix &A, const DenseVector &b,
                              double tolerance = 1e-6);
} // namespace solvers

#endif