#include "DenseMatrix.hpp"

DenseMatrix::DenseMatrix(size_t rows, size_t cols, double default_value){
    n_rows = rows;
    n_cols = cols;
    auto it = data.begin();
    data.insert(it, n_rows * n_cols ,default_value);
}

DenseMatrix::DenseMatrix(std::initializer_list<std::initializer_list<double>> init){
    n_rows = init.size();
    n_cols = init.begin()->size();
    auto it = data.begin();
    data.insert(it, n_rows * n_cols ,0);
    int i = 0;
    for (std::initializer_list<double> row : init) {
        int j = 0;
        for (double element : row) {
            data[j * n_rows + i] = element;
            j++;
        }
        i++;
    }
}

DenseVector DenseMatrix::operator*(const DenseVector &rhs) const{
    if (rhs.size() != n_cols){
        std::cout << "Dimension mismatch!";
        return rhs;
    }
    
    DenseVector result(n_rows, 0);

    for (int i = 0; i < n_cols; i++) {
        for (int j = 0; j < n_rows; j++) {
            result(j) += data[i * n_rows + j] * rhs(i);
        }
    }

    return result;
}

double DenseMatrix::operator()(size_t row, size_t col) const{
    if (row < 0 || n_rows - 1 < row || col < 0 || n_cols - 1 < col) {
        std::cout << "Dimension mismatch!";
        return 0;
    }
    return data[col * n_rows + row];
}