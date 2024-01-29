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
        // A subvector with the relevant entries for each process is created
        if (rank % (int (sqrt_p)) == std::floor(triplet.col / s) && std::floor(rank / sqrt_p) == std::floor(triplet.row / s)){
            subvector_triplets.push_back({triplet.row % s, triplet.col % s, triplet.value});
        }
    }
    data = CSCMatrix(s, s, subvector_triplets);
} 

DVector DCSCMatrix::operator*(const DVector &rhs) const {
    int comm_size;
    int comm_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    // Calculation of local products
    DenseVector local_result = data * rhs.data;
    // Choose a manager process for each row
    int local_manager = comm_rank - (comm_rank % int(std::sqrt(comm_size)));
    std::vector<double> result(rhs.data.size() * int(std::sqrt(comm_size)));
    if (local_manager != comm_rank) {
        // the local worker processes send their results to the local manager
        for (int i = 0; i < local_result.size(); i++){
            int current = local_result(i);
            MPI_Send(&current, 1, MPI_INT, local_manager, 0, MPI_COMM_WORLD);
        }
    } else {
        // the local manager receives the results from the local workers
        for (int i = 0; i < local_result.size(); i++){
            for (int j = 1; j < int(std::sqrt(comm_size)); j++) {
                int current_recv = 0;
                MPI_Recv(&current_recv, 1, MPI_INT, comm_rank + j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_result(i) += current_recv;
            }
        }
        // and send the aggregated results to the root process
        if (comm_rank != 0) {
            for (int i = 0; i < local_result.size(); i++){
                int current = local_result(i);
                MPI_Send(&current, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        } else {
            // the root process stores all of the local results in a vector of length n
            for (int i = 0; i < local_result.size(); i++) {
                result[i] = local_result(i);
            }

            for (int j = 1; j < int(std::sqrt(comm_size)); j++) {
                int current_rank = j * int(std::sqrt(comm_size));
                for (int i = 0; i < local_result.size(); i++){
                    int current_recv = 0;
                    MPI_Recv(&current_recv, 1, MPI_INT, current_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    result[local_result.size() * j + i] = current_recv;
                }
            }
            // Since every Process needs to return a DVector, the result is communicated with the other processes
            for (int j = 0; j < result.size(); j++){
                int current = result.at(j);
                for (int i = 0; i < comm_size; i++) {
                    MPI_Send(&current, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            } 
        }
    }
    if(comm_rank > 0){
        // The non root processes receive the results of the computation
        for (int j = 0; j < result.size(); j++){
            int current_recv = 0;
            MPI_Recv(&current_recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            result[j] = current_recv;
        } 
    }
    // return of final result 
    return DVector(result);
}
DCSCMatrix::~DCSCMatrix()
{
}
