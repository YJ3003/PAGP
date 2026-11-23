#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int n, th;

    cout << "Enter matrix size: ";
    cin >> n;

    cout << "Enter number of threads: ";
    cin >> th;

    int *A = new int[n*n];
    int *B = new int[n*n];
    int *C = new int[n*n]();

    for (int i = 0; i < n*n; i++) {
        A[i] = 1;
        B[i] = 2;
    }

    omp_set_num_threads(th);

    double t = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[n*i+j] += A[n*i+k] * B[n*k+j];

    t = omp_get_wtime() - t;

    cout << "Output matrix:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << C[n*i+j] << " ";
        cout << endl;
    }

    cout << "Time taken: " << t << " seconds" << endl;

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}