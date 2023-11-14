#ifndef DENSE_MATRIX_HPP
#define DENSE_MATRIX_HPP

#include <vector>

#include "DenseVector.hpp"

class DenseMatrix {
  // stores data in a single vector in col-major order
  size_t n_rows, n_cols;
  std::vector<double> data;

public:
  // Aufgabe 1.2
  // constructors
  DenseMatrix(size_t rows, size_t cols, double default_value);
  // takes a list of lists to initialize the matrix. You may restrict the input
  // to expect 'correct' matrices i.e. each inner list has the same size.
  DenseMatrix(std::initializer_list<std::initializer_list<double>> init);

  // basic linear algebra operations
  DenseVector operator*(const DenseVector &rhs) const;

  // value access
  double operator()(size_t row, size_t col) const;

  // size of matrix
  size_t rows() const { return n_rows; }
  size_t cols() const { return n_cols; }
};

/////// derived functions (already implemented)

// output matrix
inline std::ostream &operator<<(std::ostream &os, const DenseMatrix &mat) {
  for (size_t i = 0; i < mat.rows(); ++i) {
    if (i != 0)
      os << "\n";
    os << "(";
    for (size_t j = 0; j < mat.cols(); ++j) {
      if (j != 0)
        os << ", ";
      os << mat(i, j);
    }
    os << ")";
  }
  return os;
}

#endif