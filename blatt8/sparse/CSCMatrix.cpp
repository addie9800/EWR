#include "CSCMatrix.hpp"

#include <algorithm>
#include <fstream>

DenseVector CSCMatrix::operator*(const DenseVector &rhs) const {
  if (cols() != rhs.size())
    throw std::logic_error("matrix and vector dimensions do not fit!");

  DenseVector result(rows(), 0);

  for (size_t j = 0; j < cols(); ++j) {
    for (size_t d = IC[j]; d < IC[j + 1]; ++d) {
      size_t i = JR[d];
      double aij = Num[d];
      result(i) += aij * rhs(j);
    }
  }
  return result;
}

double CSCMatrix::operator()(size_t row, size_t col) const {
  // search col for correct row
  for (size_t d = IC[col]; d < IC[col + 1]; ++d)
    if (JR[d] == row)
      return Num[d];
  return 0.;
}

CSCMatrix::CSCMatrix(size_t n_rows, size_t n_cols,
                     std::vector<Triplet> triplet_init)
    : n_rows(n_rows), n_cols(n_cols), Num(triplet_init.size()),
      JR(triplet_init.size()), IC(n_cols + 1) {

  // count number of elements for each column
  for (auto &trip : triplet_init)
    ++IC.at(trip.col);

  // compute prefix sums. IC now contains correct data
  for (size_t i = 0, prefix_sum = 0; i < cols(); ++i) {
    auto nnz_ith_col = IC.at(i);
    IC.at(i) = prefix_sum;
    prefix_sum += nnz_ith_col;
  }
  IC.back() = triplet_init.size(); // last entry of IC

  // compute row indices for each entry
  for (auto &trip : triplet_init) {
    auto col_start = IC.at(trip.col);

    JR.at(col_start) = trip.row;
    Num.at(col_start) = trip.value;

    // just for now: increase the start index of the current column so we know
    // where to insert the next value.
    // we will fix IC after this loop.
    ++IC.at(trip.col);
  }

  // fix IC
  for (size_t i = 0, first_idx_of_col = 0; i <= cols(); ++i) {
    size_t next_col = IC.at(i);
    IC.at(i) = first_idx_of_col;
    first_idx_of_col = next_col;
  }
}