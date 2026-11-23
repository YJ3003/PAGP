#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define SIZE 4
#define BUFSIZE 32

char *m[SIZE] = {"", "HELLO", "CSE", "RVCE"};

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    char msg[BUFSIZE];

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != SIZE) {
        if (rank == 0)
            printf("Run with -np 4\n");

        MPI_Finalize();
        return 1;
    }

    if (rank != 0) {
        strcpy(msg, m[rank]);
        MPI_Send(msg, BUFSIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else {
        for (int i = 1; i < SIZE; i++) {
            MPI_Recv(msg, BUFSIZE, MPI_CHAR, i, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            printf("Received %s from process %d\n", msg, i);
        }
    }

    MPI_Finalize();
    return 0;
}
