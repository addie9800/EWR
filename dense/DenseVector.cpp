#include "DenseVector.hpp"

#include <fstream>

DenseVector::DenseVector(std::string path) {
  path += "/b.bin";
  std::fstream stream(path, std::ios::binary | std::ios::in | std::ios::ate);
  const size_t size = stream.tellg() / sizeof(double);
  stream.close();
  data.resize(size);

  std::ifstream read_stream(path, std::ios::binary);
  read_stream.read(reinterpret_cast<char *>(&data[0]), size * sizeof(double));
  read_stream.close();
}

// Constructor with default value

DenseVector::DenseVector(size_t size, double default_value){
    auto it = data.begin();
    data.insert(it, size, default_value);
}

// Constructor with initializer list

DenseVector::DenseVector(std::initializer_list<double> init){
    data = init;
}

// elementwise addition using += operator 

DenseVector& DenseVector::operator+=(const DenseVector &rhs){
	// Dimension Verification, returns unchanged vector on error
    if (rhs.size()!= data.size()){
        std::cout << "Sizes don't match, input unchanged!\n";
        return *this;
    }
    for (int i = 0; i < data.size(); i++){
        data[i] += rhs.data[i];
    }
    return *this;
}

// scalar multiplication using *= operator

DenseVector& DenseVector::operator*=(double rhs){
    for (int i = 0; i < data.size(); i++){
        data[i] *= rhs;
    }
    return *this;
}

// scalar division using /= operator. implementaion using previously implemented *= operator and 1/rhs (division of doubles)

DenseVector& DenseVector::operator/=(double rhs){
    return *this *= 1/rhs;
}

// elementwise subtraction using -= operator

DenseVector& DenseVector::operator-=(const DenseVector &rhs){
	// Dimension Verification, returns unchanged vector on error
    if (rhs.size()!= data.size()){
        std::cout << "Sizes don't match, input unchanged!\n";
        return *this;
    }
    for (int i = 0; i < data.size(); i++){
        data[i] -= rhs.data[i];
    }
    return *this;
}

// computation of the scalar product of two vectors

double DenseVector::operator*(const DenseVector &rhs) const{
	// Dimension Verification, returns unchanged vector on error
    if (rhs.size()!= data.size()){
        std::cout << "Sizes don't match, input unchanged!\n";
        return 0;
    }
    double tmp = 0.0;
    for (int i = 0; i < data.size(); i++){
        tmp += data[i] * rhs.data[i];
    }
    return tmp;
}

// implementation of read-write element access

double& DenseVector::operator()(size_t idx){
    return data[idx];
}

// implementation of read-only element access

double DenseVector::operator()(size_t idx) const{
    return data[idx];
}
