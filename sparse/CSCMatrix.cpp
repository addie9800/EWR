#include "CSCMatrix.hpp"
//#include "../dense/DenseMatrix.hpp"

CSCMatrix::CSCMatrix(size_t rows, size_t cols, double default_value) {
    n_rows = rows;
    n_cols = cols;

    for (int i = 0; i < n_rows + 1; i++) {
        IC.insert(IC.end(), i * n_cols + 1);
    }

    for (int i = 0; i < n_rows; i ++) {
        for (int j = 0; j < n_cols; j++) {
            JR.insert(JR.end(), j+1);
        }
    }

    Num.insert(Num.begin(), n_rows * n_cols ,default_value);
}

CSCMatrix::CSCMatrix(std::initializer_list<std::initializer_list<double>> init){
    DenseMatrix tmp = init;
    *this = tmp;
}

CSCMatrix::CSCMatrix(const DenseMatrix &mat) {
    n_rows = mat.rows();
    n_cols = mat.cols();

    int cnt = 0;

    for (int i = 0; i < n_rows; i++) {
        IC.insert(IC.end(), cnt+1);
        for (int j = 0; j < n_cols; j++) {
            if (mat(i,j) != 0) {
                JR.insert(JR.end(), j + 1);
                Num.insert(Num.end(), mat(i,j));
                cnt++;
            }
        }
    }

    IC.insert(IC.end(), cnt+1);
}

CSCMatrix::CSCMatrix(size_t rows, size_t cols, std::initializer_list<Triplet> triplet_init){
    n_rows = rows;
    n_cols = cols;

    for (int i = 0; i < n_rows + 1; i++) {
        IC.insert(IC.end(), 1);
    }

    for (Triplet t : triplet_init){
        if (t.value == 0) {
            continue;
        }
        if (t.row < 0 || n_rows -1 < t.row || t.col < 0 || n_cols - 1 < t.col) {
            std::cout << "Dimension mismatch!";
            *this = {rows, cols, 0};
            break;
        }

        for (int i = t.row + 1; i < n_rows + 1; i++) {
            IC[i]++;
        }

        if (IC[t.row] == IC[t.row + 1] - 1) {
            JR.insert(JR.begin() + IC[t.row] - 1, t.col + 1);
            Num.insert(Num.begin() + IC[t.row] - 1, t.value);
        } else {
            int index = 0;
            for (int i = IC[t.row] - 1; i < IC[t.row + 1] - 2; i++) {
                index = i;
                int tmp_col = JR[i] - 1;
                if (tmp_col == t.col) {
                    std::cout << "Value defined more than once!";
                    *this = {rows, cols, 0};
                    return;
                }
                if (tmp_col > t.col) {
                    index--;
                    break;
                }
            }
            JR.insert(JR.begin() + index + 1, t.col + 1);
            Num.insert(Num.begin() + index + 1, t.value);
        }
    }
}

DenseVector CSCMatrix::operator*(const DenseVector &rhs) const {
    if (rhs.size() != n_cols){
        std::cout << "Dimension mismatch!";
        return rhs;
    }
    
    DenseVector result(n_rows, 0);

    for (int i = 0; i < n_rows; i++) {
        for (int j = IC[i]-1; j < IC[i+1]-1; j++) {
            result(i) += rhs(JR[j]-1) * Num[j];
        }
    }

    return result;
}

double CSCMatrix::operator()(size_t row, size_t col) const {
    if (row < 0 || n_rows - 1 < row || col < 0 || n_cols - 1 < col) {
        std::cout << "Dimension mismatch!";
        return 0;
    }

    int col_index_start = IC[row] - 1;
    int diff = IC[row + 1] - 1 - col_index_start;
    for (int i = 0; i < diff; i++) {
        int tmp_col = JR[col_index_start + i] - 1;
        if (tmp_col == col) {
            return Num[col_index_start + i];
        }
        if (tmp_col > col) {
            break;
        }
    }
    return 0;
}