#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <ctime>
using namespace std;

int main() {
    int n, count = 0;

    // Input: number of random points
    cout << "Enter number of intervals: ";
    cin >> n;

    // Start timer
    double t = omp_get_wtime();

    // Create parallel threads
    #pragma omp parallel
    {
        // Separate seed for each thread
        unsigned int seed = time(NULL) + omp_get_thread_num();

        // Divide iterations and combine counts
        #pragma omp for reduction(+:count)
        for (int i = 0; i < n; i++) {

            // Generate random coordinates
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            // Check if point is inside circle
            if (x*x + y*y <= 1)
                count++;
        }
    }

    // Calculate elapsed time
    t = omp_get_wtime() - t;

    // Display results
    cout << "Number of threads: " << omp_get_max_threads() << endl;
    cout << "Number of intervals: " << n << endl;
    cout << "Estimated PI: " << 4.0 * count / n << endl;
    cout << "Time taken: " << t << " seconds" << endl;

    return 0;
}