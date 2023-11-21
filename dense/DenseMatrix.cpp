#include "DenseMatrix.hpp"

// Constructor using dimensions and a default value

DenseMatrix::DenseMatrix(size_t rows, size_t cols, double default_value){
    n_rows = rows;
    n_cols = cols;
    auto it = data.begin();
    data.insert(it, n_rows * n_cols ,default_value);
}

// Constructor using initialiser list

DenseMatrix::DenseMatrix(std::initializer_list<std::initializer_list<double>> init){
    n_rows = init.size();
    n_cols = init.begin()->size();
    auto it = data.begin();
    data.insert(it, n_rows * n_cols ,0);
    int i = 0;
	// the initialiser list is passed on to the method as {{row1},{row2}, ..., {rown}}. the tasks requires the index of our datatype to fill the vector by column. To achieve that, while still being cache efficient, we load each row and enter each value with a distance of n_rows apart, because that is equivalent to the number of values in each column.
    for (std::initializer_list<double> row : init) {
        int j = 0;
        for (double element : row) {
            data[j * n_rows + i] = element;
            j++;
        }
        i++;
    }
}

// Vector-Matrix multiplication using * operator

DenseVector DenseMatrix::operator*(const DenseVector &rhs) const{
	// Dimension Verification, returns unchanged vector on error
    if (rhs.size() != n_cols){
        std::cout << "Dimension mismatch!";
        return rhs;
    }
    
	// Since the vector is passed as constant, we need to initialize our result vector to 0
	
    DenseVector result(n_rows, 0);

	// perform standard matrix vector multiplication row-wise and updating the values in the result vector
	
    for (int i = 0; i < n_cols; i++) {
        for (int j = 0; j < n_rows; j++) {
            result(j) += data[i * n_rows + j] * rhs(i);
        }
    }

    return result;
}

// implementation of element access for dense Matrix

double DenseMatrix::operator()(size_t row, size_t col) const{
	// Dimension Verification, returns unchanged vector on error	
    if (row < 0 || n_rows - 1 < row || col < 0 || n_cols - 1 < col) {
        std::cout << "Dimension mismatch!";
        return 0;
    }
	// reversal of the positioning. since we will by columns and each column has length n_rows we get the postion within the vector by calculating column * n_rows + row (which basically tells us which value within the column is wanted.
    return data[col * n_rows + row];
}
