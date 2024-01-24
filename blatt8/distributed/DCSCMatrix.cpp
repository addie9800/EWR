#include "DCSCMatrix.hpp"
#include <cmath>

class DCSCMatrix
{
private:
    /* data */
public:
    DCSCMatrix(/* args */);
    ~DCSCMatrix();
};

DCSCMatrix::DCSCMatrix(/* args */)
{
}

DCSCMatrix::DCSCMatrix(size_t rows, size_t cols, std::vector<Triplet> triplets){
    // Wir gehen an dieser Stelle zunächst davon aus, dass die Anzahl der Prozesse p bekannst ist.
    // Dabei kann davon ausgegangen werden, dass p eine Quadratzahl ist.
    int p = 25;
    int p_sqrt = std::sqrt(p);


} 

DCSCMatrix::~DCSCMatrix()
{
}
