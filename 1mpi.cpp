#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

#define ROOT 0

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;

    if (rank == ROOT) {
        cout << "Enter the number of intervals: " << endl;
        cin >> n;
    }

    MPI_Bcast(&n, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    if (n <= 0) {
        if (rank == ROOT)
            cout << "Invalid number of intervals!" << endl;

        MPI_Finalize();
        return 0;
    }

    double t;

    if (rank == ROOT)
        t = MPI_Wtime();

    int count = 0, total_count = 0;

    unsigned int seed = time(NULL) + rank;

    int local_n = n / size;
    int remainder = n % size;

    if (rank < remainder)
        local_n++;

    for (int i = 0; i < local_n; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;

        if (x*x + y*y <= 1.0)
            count++;
    }

    MPI_Reduce(&count, &total_count, 1, MPI_INT,
               MPI_SUM, ROOT, MPI_COMM_WORLD);

    if (rank == ROOT) {
        t = MPI_Wtime() - t;

        double pi = 4.0 * total_count / n;

        cout << "Number of processes: " << size << endl;
        cout << "Number of intervals: " << n << endl;
        cout << "Estimated value of PI: " << pi << endl;
        cout << "Time taken: " << t << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
