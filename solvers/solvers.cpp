#include "solvers.hpp"
#include <cmath>

DenseVector solvers::conjugateGradient(const CSCMatrix &A, const DenseVector &b, double tolerance){
    DenseVector x = DenseVector(A.cols(), 0);
    // Dimension Verification, returns 0 vector on error
    if (A.rows() != b.size()) {
        std::cout << "Dimension mismatch!";
        return x;
    }

    // Initialization of variables
    DenseVector p = b - (A * x);
    DenseVector r = p;
    double alpha = r * r;
    double alphaold = alpha; 
    // alphaold is need as we want to find out, how alpha changed in each iteration
    int t = 0;

    DenseVector v = x;
    double lambda = 0;

    // While the residuum is not zero, i.e. bigger than the tolerance, we have to improve the solution
    while (alpha > tolerance) {
        v = A * p;
        lambda = alpha / (v * p);
        x = x + (p * lambda);
        r = r - (v * lambda);
        alpha = r*r;
        p = r + (p * (alpha / alphaold));
        alphaold = alpha;

        // Writes the current itration and residuum to the output
        std::cout << "Iteration: " << t++ << "; Error: " << sqrt(alpha) << "\n"; // TODO: Muss hier sqrt(alpha) oder alpha sein?
    }

    return x;
} 