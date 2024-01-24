#include "DenseVector.hpp"

#include <fstream>

DenseVector::DenseVector(size_t size, double default_value)
    : data(size, default_value) {}

DenseVector::DenseVector(std::initializer_list<double> init) : data(init) {}
DenseVector::DenseVector(std::vector<double> init) : data(init) {}

// value access and modification: get value at index idx.
double &DenseVector::operator()(size_t idx) { return data.at(idx); }
double DenseVector::operator()(size_t idx) const { return data.at(idx); }
