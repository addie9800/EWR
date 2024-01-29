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

DVector DCSCMatrix::operator*(const DVector &rhs) const {
	/*// Dimension Verification, returns unchanged vector on error
    if (rhs.size() != cols){
        throw std::invalid_argument("Dimension mismatch!");
        return rhs;
    }*/
    int comm_size;
    int comm_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    DenseVector local_result = data * rhs.data;
    int local_manager = comm_rank - (comm_rank % int(std::sqrt(comm_size)));

    if (local_manager != comm_rank) {
        for (int i = 0; i < local_result.size(); i++){
            int current = local_result(i);
            MPI_Send(&current, 1, MPI_INT, local_manager, 0, MPI_COMM_WORLD);
        }
    } else {
        for (int i = 0; i < local_result.size(); i++){
            for (int j = 1; j < int(std::sqrt(comm_size)); j++) {
                int current_recv = 0;
                MPI_Recv(&current_recv, 1, MPI_INT, comm_rank + j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_result(i) += current_recv;
            }
        }

        for (int i = 0; i < local_result.size(); i++){
            int current = local_result(i);
            MPI_Send(&current, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        if (comm_rank == 0) {
            std::vector<double> result(local_result.size() * int(std::sqrt(comm_size)));

            for (int j = 0; j < int(std::sqrt(comm_size)); j++) {
                int current_rank = j * int(std::sqrt(comm_size));
                for (int i = 0; i < local_result.size(); i++){
                    int current_recv = 0;
                    MPI_Recv(&current_recv, 1, MPI_INT, current_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    result[current_rank + i] = current_recv;
                }
            }
            return DVector(result);
        }
    }
}

DCSCMatrix::~DCSCMatrix()
{
}
