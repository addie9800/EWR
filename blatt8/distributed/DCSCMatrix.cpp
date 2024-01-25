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
    }else if ((double (std::floor(std::sqrt(p)))) == std::sqrt(p))
    {
        throw std::invalid_argument("p should be quadratic!");
    }
    size_t s = size_t (rows / std::sqrt(p));
    // This needs to be double even though int should work to prevent bugs when dividing (int/int = int)!!!
    double sqrt_p = std::sqrt(p);
    std::vector<Triplet> subvector_triplets;
    for (Triplet triplet : triplets){
        if ((rank/sqrt_p == double (triplet.row)) || (std::floor(rank/sqrt_p) <= triplet.row <= std::floor(rank/sqrt_p) + 1)){
            if ((std::floor(rank/sqrt_p) == triplet.row && ((rank % (int (sqrt_p))) >= triplet.col))
             || (std::floor(rank/sqrt_p) + 1 == triplet.row && ((rank % (int (sqrt_p))) < triplet.col))){
                subvector_triplets.push_back({triplet.row % s, triplet.col % s, triplet.value});
            }
        }
    }
    data = CSCMatrix(s, s, subvector_triplets);
    std::cout << data;
} 

DCSCMatrix::~DCSCMatrix()
{
}
