#include "DCSCMatrix.hpp"
#include <cmath>

DCSCMatrix::DCSCMatrix(size_t rows, size_t cols, std::vector<Triplet> triplets){
    // Es kann davon ausgegangen werden, dass p eine Quadratzahl ist und n teilt
    int p;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rows != cols)
    {
        throw std::invalid_argument("Matrix should be quadratic!");
    }else if (rows % int (std::sqrt(p)) != 0)
    {
        throw std::invalid_argument("n should be multiple of sqrt(p)!");
    }else if (std::floor(std::sqrt(p)) != std::sqrt(p))
    {
        throw std::invalid_argument("p should be quadratic! p is " + std::to_string(p));
    }
    size_t s = size_t (rows / std::sqrt(p));
    // This needs to be double even though int should work to prevent bugs when dividing (int/int = int)!!!
    double sqrt_p = std::sqrt(p);
    std::vector<Triplet> subvector_triplets;
    for (Triplet triplet : triplets){
        if (rank % (int (sqrt_p)) == std::floor(triplet.col / s) && std::floor(rank / sqrt_p) == std::floor(triplet.row / s)){
            subvector_triplets.push_back({triplet.row % s, triplet.col % s, triplet.value});
        }
    }
    data = CSCMatrix(s, s, subvector_triplets);
    std::cout << data;
} 
/*
DVector DCSCMatrix::operator*(const DVector &rhs) const {
    //TODO
}*/

DCSCMatrix::~DCSCMatrix()
{
}
