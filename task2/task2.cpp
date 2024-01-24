#include <iostream>
#include <vector>
#include <mpi.h>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dist(1, 100);
vector<int> create_vector(int size, int seed){
    std::minstd_rand simple_rand;
    simple_rand.seed(seed);
    vector<int> v(size);
    for(int i = 1;i<=size;i++){
        int a = abs(int(simple_rand() % 100 + 1));
        v[i-1] = a;
    }
    return v;
}
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int vector_size = 10000; vector_size <= 100000000; vector_size *= 10) {
        vector<int> vector_first, vector_second;

        chrono::time_point<chrono::system_clock, chrono::nanoseconds> start_time;
        if (rank == 0) {
	    vector_first = create_vector(vector_size, 0);
    	    vector_second = create_vector(vector_size, 1);
            start_time = chrono::high_resolution_clock::now();
        }

        int local_size = vector_size / size;
        vector<int> local_vector_first(local_size), local_vector_second(local_size);

        MPI_Scatter(vector_first.data(), local_size, MPI_INT, local_vector_first.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(vector_second.data(), local_size, MPI_INT, local_vector_second.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

        int local_dot_product = 0;
        for (int i = 0; i < local_size; ++i) {
            local_dot_product += local_vector_first[i] * local_vector_second[i];
        }

        int global_dot_product;
        MPI_Reduce(&local_dot_product, &global_dot_product, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

            cout << duration.count() << endl;
        }
    }

    MPI_Finalize();
    return 0;
}

