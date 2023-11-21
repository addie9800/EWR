#include "CSCMatrix.hpp"
#include <list> 
//#include "../dense/DenseMatrix.hpp"

// Constructor for the CSC Matrix with default value

CSCMatrix::CSCMatrix(size_t rows, size_t cols, double default_value) {
    n_rows = rows;
    n_cols = cols;
	if (default_value != 0){
		// if the default value is non-zero, fill the matrix as normal
		for (int i = 0; i < n_rows + 1; i++) {
			// each column is filled completex, hence the next columns starts with an offset of n_rows
			IC.insert(IC.end(), i * n_cols + 1);
		}
		

		for (int i = 0; i < n_rows; i ++) {
			// since each entty of the matrix is non-zero, all possible values of j need to be added to jr
			for (int j = 0; j < n_cols; j++) {
				JR.insert(JR.end(), j+1);
			}
		}
		// all values are identical, which is why we can add n_rows * n_cols times default_value to the Num array
		Num.insert(Num.begin(), n_rows * n_cols ,default_value);
	}else{
		// if the default value is zero, the matrix is empty as zero entries are suppressed in the CSC-format
		for (int i = 0; i < n_rows + 1; i++) {
			IC.insert(IC.end(), 1);
		}
	}

    
}

// for a given initializer list create a dense matrix and use the dense matrix constructor

CSCMatrix::CSCMatrix(std::initializer_list<std::initializer_list<double>> init){
    DenseMatrix tmp = init;
    *this = tmp;
}

CSCMatrix::CSCMatrix(const DenseMatrix &mat) {
    n_rows = mat.rows();
    n_cols = mat.cols();
	
	// set up a counter to count the number of entries to later reference in IC 
    int cnt = 0;

    for (int i = 0; i < n_rows; i++) {
		// cnt indicated the current length of JC/Num and where the (i+1)th row begins in the JC and Num array
        IC.insert(IC.end(), cnt+1);
        for (int j = 0; j < n_cols; j++) {
			// if the entry at i,j is not equal to 0, we need to save the value of j in jr and the corresponding value in Num
            if (mat(i,j) != 0) {
                JR.insert(JR.end(), j + 1);
                Num.insert(Num.end(), mat(i,j));
                cnt++;
            }
        }
    }
	// Insert "Null" Pointer at end to signify the end of the content
    IC.insert(IC.end(), cnt+1);
}

// Constructor using list of triplets

CSCMatrix::CSCMatrix(size_t rows, size_t cols, std::initializer_list<Triplet> triplet_init){
    n_rows = rows;
    n_cols = cols;

    for (int i = 0; i < n_rows + 1; i++) {
		// initialization of IC to the equivalent of an empty matrix (all columns data start at index 1 for num array length of 0)
        IC.insert(IC.end(), 1);
    }
	// Iteration over all triplets in initializer_list
    for (Triplet t : triplet_init){
		// if the value is zero it can be skipped, since zero values are omitted in this data type
        if (t.value == 0) {
            continue;
        }
		// Checking for input values to corresponding to previously set dimentions, if encountered the contructor exits with an error message
        if (t.row < 0 || n_rows - 1 < t.row || t.col < 0 || n_cols - 1 < t.col) {
            std::cout << "Dimension mismatch!";
            *this = {rows, cols, 0};
            break;
        }
		// Entering a value leads to a shift of the starting indices of all subsequent rows. Hence the values in IC are all incremented starting from the current positiom
        for (int i = t.row + 1; i < n_rows + 1; i++) {
            IC[i]++;
        }
		// If there are no previous entries in this row, so the entry can just be added to the end
        if (IC[t.row] == IC[t.row + 1] - 1) {
            JR.insert(JR.begin() + IC[t.row] - 1, t.col + 1);
            Num.insert(Num.begin() + IC[t.row] - 1, t.value);
        } else {
			// else you need to identify the correct place to insert the entry since for each row the entries are sorted in ascending order
            int index = 0;
            for (int i = IC[t.row] - 1; i < IC[t.row + 1] - 2; i++) {
                index = i;
                int tmp_col = JR[i] - 1;
				// Input verification. if a value is defined more than once, the contructor return with an error mes
                if (tmp_col == t.col) {
                    std::cout << "Value defined more than once!";
                    *this = {rows, cols, 0};
                    return;
                }
                if (tmp_col > t.col) {
					// when the column number is higher than the target column number we reduce the index by one, so that value will be inserted before
                    index--;
                    break;
                }
            }
			// insertion of value into jr and num
            JR.insert(JR.begin() + index + 1, t.col + 1);
            Num.insert(Num.begin() + index + 1, t.value);
        }
    }
}

// implementation of vector matrix multiplication

DenseVector CSCMatrix::operator*(const DenseVector &rhs) const {
	// Dimension Verification, returns unchanged vector on error
    if (rhs.size() != n_cols){
        std::cout << "Dimension mismatch!";
        return rhs;
    }
    // Since the operator is passed as a constant we need to initiaize a new result vector
    DenseVector result(n_rows, 0);
	// the matrix vector multiplication is again done by the default way, with the difference that many operations can be skipped due to most values within the matrix being zero.
    for (int i = 0; i < n_rows; i++) {
        for (int j = IC[i]-1; j < IC[i+1]-1; j++) {
            result(i) += rhs(JR[j]-1) * Num[j];
        }
    }

    return result;
}

CSCMatrix CSCMatrix::operator*(const CSCMatrix &rhs) const {
	// Dimension Verification, returns unchanged Matrix on error
    if (rhs.rows() != n_cols){
        std::cout << "Dimension mismatch!";
        return rhs;
    }
    // Since the operator is passed as a constant we need to initiaize a new result matrix
    DenseMatrix result(n_rows, rhs.cols(), 0);
	std::list<DenseVector> rhs_cols{};
    for (int i = 0; i < rhs.cols(); i++){
        DenseVector tmp(rhs.rows(), 0);
        // get the correct start index for jr from ic
        int col_index_start = IC[i] - 1;
        int diff = IC[i + 1] - 1 - col_index_start;
        // iterate over all relevant values in jr (before the new row starts)
        for (int j = 0; j < diff; j++) {
            int tmp_col = JR[col_index_start + j] - 1;
            if (tmp_col == i) {
                // copy the value of the matrix column to column vector
                tmp(i) = rhs.Num[col_index_start + j];
            }
        }
        rhs_cols.insert(rhs_cols.end(), tmp);
    }
    return result;
}

// implementation of access to matrix elements

double CSCMatrix::operator()(size_t row, size_t col) const {
	// Dimension Verification, returns 0 on error
    if (row < 0 || n_rows - 1 < row || col < 0 || n_cols - 1 < col) {
        std::cout << "Dimension mismatch!";
        return 0;
    }
	// get the correct start index for jr from ic
    int col_index_start = IC[row] - 1;
    int diff = IC[row + 1] - 1 - col_index_start;
	// iterate over all relevant values in jr (before the new row starts)
    for (int i = 0; i < diff; i++) {
        int tmp_col = JR[col_index_start + i] - 1;
        if (tmp_col == col) {
			//return the numerical value if it exists
            return Num[col_index_start + i];
        }
    }
    return 0;
}
