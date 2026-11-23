
#include <stdio.h>
#define N 10
#define ITER 100

int main() {
    int i, j, k;
    double A[N][N] = {0}, B[N][N] = {0};

    // Boundary conditions
    for (i = 0; i < N; i++)
        A[0][i] = A[N-1][i] = A[i][0] = A[i][N-1] = 100;

    #pragma acc data copy(A) create(B)
    {
        for (k = 0; k < ITER; k++) {

            #pragma acc parallel loop collapse(2)
            for (i = 1; i < N-1; i++)
                for (j = 1; j < N-1; j++)
                    B[i][j] = 0.25 * (A[i-1][j] + A[i+1][j]
                                    + A[i][j-1] + A[i][j+1]);

            #pragma acc parallel loop collapse(2)
            for (i = 1; i < N-1; i++)
                for (j = 1; j < N-1; j++)
                    A[i][j] = B[i][j];
        }
    }

    printf("Final Matrix:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%.2f ", A[i][j]);
        printf("\n");
    }

    return 0;
}
