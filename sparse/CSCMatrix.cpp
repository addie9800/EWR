#include "CSCMatrix.hpp"
#include <list> 
//#include "../dense/DenseMatrix.hpp"

#include <algorithm>
#include <fstream>

template <class Type> size_t length_of_file(const std::string &path) {
  std::fstream stream(path, std::ios::binary | std::ios::in | std::ios::ate);
  size_t length = stream.tellg() / sizeof(Type);
  stream.close();
  return length;
}

// Reads a binary file that stores a sequence of values with type 'Type' and
// returns them as a std::vector<Type>
template <class Type> std::vector<Type> read_vec(const std::string &path) {
  const size_t size = length_of_file<Type>(path);
  std::vector<Type> to_read(size);

  std::ifstream read_stream(path, std::ios::binary);
  read_stream.read(reinterpret_cast<char *>(&to_read[0]), size * sizeof(Type));
  read_stream.close();

  return to_read;
}

CSCMatrix::CSCMatrix(std::string path, size_t n)
    : n_rows(n), n_cols(n), IC(n_cols + 1) {

  auto rows_data = read_vec<uint64_t>(path + "/rows.bin");
  auto cols_data = read_vec<uint64_t>(path + "/cols.bin");
  auto vals_data = read_vec<double>(path + "/vals.bin");

  JR.reserve(vals_data.size());
  Num.reserve(vals_data.size());

  // count number of elements for each column
  for (auto &col : cols_data)
    ++IC[col];

  // compute prefix sums. IC now contains correct data
  for (size_t i = 0, prefix_sum = 0; i < cols(); ++i) {
    auto nnz_ith_col = IC[i];
    IC[i] = prefix_sum;
    prefix_sum += nnz_ith_col;
  }
  IC.back() = vals_data.size(); // last entry of IC

  // compute row indices for each entry
  for (size_t i = 0; i < vals_data.size(); ++i) {
    // for (auto &trip : triplet_init) {
    auto col_start = IC[cols_data.at(i)];

    JR[col_start] = rows_data.at(i);
    Num[col_start] = vals_data.at(i);

    // just for now: increase the start index of the current column so we know
    // where to insert the next value.
    // we will fix IC after this loop.
    ++IC[cols_data.at(i)];
  }

  // fix IC
  for (size_t i = 0, first_idx_of_col = 0; i <= cols(); ++i) {
    size_t next_col = IC[i];
    IC[i] = first_idx_of_col;
    first_idx_of_col = next_col;
  }
}

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
    /*for (int i = 0; i < n_rows; i++) {
        for (int j = IC[i]-1; j < IC[i+1]-1; j++) {
            result(i) += rhs(JR[j]-1) * Num[j];
        }
    }*/ // ALTE MATRIX-VEKTOR MULTIPLIKATION

    for (size_t j = 0; j < cols(); ++j) {
        for (size_t d = IC[j]; d < IC[j + 1]; ++d) {
        result(JR[d]) += Num[d] * rhs(j);
        }
    }

    return result;
}

// Constructor using a list of triplets
// This extra constructor is necessary, because we want to be able to construct a CSCMatrix from a "normal" list and not only a initializer list.
// This is because one cannot iteratively construct an initializer list.

CSCMatrix::CSCMatrix(size_t rows, size_t cols, std::list<Triplet> triplet_init){
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

// implementation of matrix matrix multiplication

CSCMatrix CSCMatrix::operator*(const CSCMatrix &rhs) const {
	// Dimension Verification, returns unchanged Matrix on error
    if (rhs.rows() != n_cols){
        std::cout << "Dimension mismatch!";
        return rhs;
    }
    // First, we split the matrix into its columns. Before that the list of columns is just a list of empty vectors
    DenseVector rhs_cols[rhs.cols()] = {DenseVector(rhs.rows(), 0)};
    std::fill_n(rhs_cols, rhs.cols(), DenseVector(rhs.rows(), 0));
    // Then we copy the values from the rhs matrix to the list of vectors
    for (int i = 0; i < rhs.rows(); i++){
        int col_index_start = rhs.IC[i] - 1;
        int diff = rhs.IC[i + 1] - 1 - col_index_start;
        // iterate over all relevant values in jr (before the new row starts)
        for (int j = i; j < i + diff; j++) {
            rhs_cols[rhs.JR[j] - 1](i) = rhs(i, rhs.JR[j] - 1);
        }
    }
    // All the column vectors are multiplied with the lhs matrix to get a list of column vectors which span the result matrix.
    // These vectors are then transformed into Triplets, which are used to create the result matrix.
    std::list<Triplet> result_list{};
    for (int i = 0; i < rhs.cols(); i++){
        DenseVector current = rhs_cols[i]; 
        // Creating the result vector by matrix vector multiplication
        current = *this * current;
        rhs_cols[i] = current;
        // Transforming the result vector into Triplets and adding them to the list of Triplets
        for (int j = 0; j < current.size(); j++){
            if (current(j) != 0){
                Triplet trip{static_cast<size_t> (j), static_cast<size_t>(i), current(j)};
                result_list.insert(result_list.end(), trip);
            } 
        } 
    }
    // Since the operator is passed as a constant we need to initiaize a new result matrix
    CSCMatrix result(n_rows, rhs.cols(), result_list);
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