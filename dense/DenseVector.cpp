#include "DenseVector.hpp"

DenseVector::DenseVector(size_t size, double default_value){
    auto it = data.begin();
    data.insert(it, size ,default_value);
}

DenseVector::DenseVector(std::initializer_list<double> init){
    data = init;
}

DenseVector& DenseVector::operator+=(const DenseVector &rhs){
    if (rhs.size()!= data.size()){
        std::cout << "Sizes don't match, input unchanged!\n";
        return *this;
    }
    for (int i = 0; i < data.size(); i++){
        data[i] += rhs.data[i];
    }
    return *this;
}

DenseVector& DenseVector::operator*=(double rhs){
    for (int i = 0; i < data.size(); i++){
        data[i] *= rhs;
    }
    return *this;
}

DenseVector& DenseVector::operator/=(double rhs){
    return *this *= 1/rhs;
}

DenseVector& DenseVector::operator-=(const DenseVector &rhs){
    if (rhs.size()!= data.size()){
        std::cout << "Sizes don't match, input unchanged!\n";
        return *this;
    }
    for (int i = 0; i < data.size(); i++){
        data[i] -= rhs.data[i];
    }
    return *this;
}

double DenseVector::operator*(const DenseVector &rhs) const{
    if (rhs.size()!= data.size()){
        std::cout << "Sizes don't match, input unchanged!\n";
        return 0;
    }
    int tmp = 0;
    for (int i = 0; i < data.size(); i++){
        tmp += data[i] * rhs.data[i];
    }
    return tmp;
}

double& DenseVector::operator()(size_t idx){
    return data[idx];
}

double DenseVector::operator()(size_t idx) const{
    return data[idx];
}