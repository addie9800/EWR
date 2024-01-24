#ifndef DENSE_VECTOR_HPP
#define DENSE_VECTOR_HPP

#include <iostream>
#include <vector>

class DenseVector {
  std::vector<double> data;

public:
  // Aufgabe 1.1
  // constructor: create vector with given size, each entry is set to
  // default_value.
  DenseVector(size_t size, double default_value);
  // constructor: vector is initialized to the values in init
  DenseVector(std::initializer_list<double> init);
  DenseVector(std::vector<double> init);

  // value access and modification: get value at index idx.
  double &operator()(size_t idx); // this allows assignment: Vector(1) = 2;
  double operator()(size_t idx) const;

  // size of vector
  inline size_t size() const { return data.size(); }
};

/////// derived functions (already implemented)

// output vector
inline std::ostream &operator<<(std::ostream &os, const DenseVector &vec) {
  os << "(";
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i != 0)
      os << ", ";
    os << vec(i);
  }
  os << ")^T";
  return os;
}

#endif
