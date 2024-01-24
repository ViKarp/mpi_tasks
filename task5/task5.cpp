#include <iostream>
#include <vector>
#include <mpi.h>
#include <cmath>
#include <numeric>
#include <functional>
#include <chrono>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int p = size;
    int q = static_cast<int>(sqrt(p));

    if (p != q * q) {
        if (rank == 0) {
            std::cerr << "Количество процессов должно быть квадратом целого числа." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    MPI_Comm grid_comm;
    int dims[2] = {q, q};
    int periods[2] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &grid_comm);
    int coords[2];
    MPI_Cart_coords(grid_comm, rank, 2, coords);

    MPI_Comm row_comm, col_comm;
    MPI_Comm_split(grid_comm, coords[0], coords[1], &row_comm);
    MPI_Comm_split(grid_comm, coords[1], coords[0], &col_comm);

    std::vector<int> local_vector(q, rank);
    auto start_generation = std::chrono::high_resolution_clock::now();
    int local_sum = std::accumulate(local_vector.begin(), local_vector.end(), 0);
    auto end_generation = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> generation_duration = end_generation - start_generation;

    auto start_allreduce = std::chrono::high_resolution_clock::now();
    int global_sum;
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, grid_comm);
    auto end_allreduce = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> allreduce_duration = end_allreduce - start_allreduce;
    
    int row_sum;
    auto start_reduce = std::chrono::high_resolution_clock::now();
    MPI_Reduce(&local_sum, &row_sum, 1, MPI_INT, MPI_SUM, 0, row_comm);
    auto end_reduce = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> reduce_duration = end_reduce - start_reduce;
    if (rank == 0) {
        std::cout << "Сумма элементов вектора для всех процессов: " << global_sum << std::endl;
        std::cout << "Сумма элементов вектора для процессов в одной строке: " << row_sum << std::endl;

        // Вывод времени выполнения каждого этапа
        std::cout << "Время генерации вектора: " << generation_duration.count() << " сек." << std::endl;
        std::cout << "Время выполнения Allreduce: " << allreduce_duration.count() << " сек." << std::endl;
        std::cout << "Время выполнения Reduce: " << reduce_duration.count() << " сек." << std::endl;
    }

    MPI_Comm_free(&grid_comm);
    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&col_comm);

    MPI_Finalize();
    return 0;
}

