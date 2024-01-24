#include <iostream>
#include <mpi.h>
#include <chrono>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int trials = 10000;
    int max_length = 100000000;

    for (int length = 10; length <= max_length; length *= 10) {
        vector<char> send_buffer(length);
        vector<char> receive_buffer(length);

        chrono::time_point<chrono::system_clock, chrono::nanoseconds> start_time;

        if (rank == 0) {
            start_time = chrono::high_resolution_clock::now();
        }

        for (int j = 0; j < trials; ++j) {
            if (rank == 0) {
                MPI_Send(send_buffer.data(), length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(receive_buffer.data(), length, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else if (rank == 1) {
                MPI_Recv(receive_buffer.data(), length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(send_buffer.data(), length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }

        if (rank == 0) {
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time) / trials;

            cout << duration.count() << ",";
        }
    }

    MPI_Finalize();
    return 0;
}

