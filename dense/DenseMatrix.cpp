#include "DenseMatrix.hpp"

DenseMatrix::DenseMatrix(size_t rows, size_t cols, double default_value){
    n_rows = rows;
    n_cols = cols;
    auto it = data.begin();
    data.insert(it, n_rows * n_cols ,default_value);
}

DenseMatrix::DenseMatrix(std::initializer_list<std::initializer_list<double>> init){
    n_rows = 0;
    n_cols = init.size();
    for (std::initializer_list<double> i : init){
        if (n_rows == 0) {
            n_rows = i.size();
        }
        data.insert(data.end(), i.begin(), i.end());
    }
}

DenseVector DenseMatrix::operator*(const DenseVector &rhs) const{
    if (rhs.size() != n_cols){
        std::cout << "Dimension mismatch!";
        return rhs;
    }
    DenseVector result(n_rows, 0);
    double tmp;
    for (int i = 0; i < n_rows; i++){
        
        result[i] = 
    }
}

double DenseMatrix::operator()(size_t row, size_t col) const{
    return data[(col - 1) * n_rows + row - 1];
}