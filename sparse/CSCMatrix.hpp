#ifndef CSCMATRIX_HPP
#define CSCMATRIX_HPP

#include <vector>
#include <list>

#include "../dense/DenseMatrix.hpp"
#include "../dense/DenseVector.hpp"

struct Triplet {
  size_t row;
  size_t col;
  double value;
};

class CSCMatrix {
  size_t n_rows, n_cols;
  std::vector<size_t> IC;
  std::vector<size_t> JR;
  std::vector<double> Num;

public:
  // Aufgabe 2
  // constructors
  CSCMatrix(size_t rows, size_t cols, double default_value);
  CSCMatrix(std::initializer_list<std::initializer_list<double>> init);
  CSCMatrix(const DenseMatrix &mat);
  CSCMatrix(size_t rows, size_t cols,
            std::initializer_list<Triplet> triplet_init);
  CSCMatrix(size_t rows, size_t cols,
            std::list<Triplet> triplet_init);

  // basic linear algebra operations
  DenseVector operator*(const DenseVector &rhs) const;

  // Blatt 4: SpGEMM
  CSCMatrix operator*(const CSCMatrix &rhs) const;

  // value access
  double operator()(size_t row, size_t col) const;

  // size of matrix
  size_t rows() const { return n_rows; }
  size_t cols() const { return n_cols; }
};

/////// derived functions (already implemented)

// output matrix
inline std::ostream &operator<<(std::ostream &os, const CSCMatrix &mat) {
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