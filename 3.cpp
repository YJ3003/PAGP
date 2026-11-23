#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <omp.h>

using namespace std;

const int BLOCK = 32768;

// Generate primes up to sqrt(N)
vector<int> getPrimes(int n) {
    vector<bool> mark(n + 1, false);
    vector<int> primes;

    for (int i = 2; (long long)i * i <= n; i++)
        if (!mark[i])
            for (long long j = (long long)i * i; j <= n; j += i)
                mark[j] = true;

    for (int i = 2; i <= n; i++)
        if (!mark[i])
            primes.push_back(i);

    return primes;
}

// 1. Cache-unfriendly Sieve
long long unfriendlySieve(int n) {
    double t = omp_get_wtime();

    vector<bool> comp(n + 1, false);

    for (int i = 2; (long long)i * i <= n; i++)
        if (!comp[i])
            for (long long j = (long long)i * i; j <= n; j += i)
                comp[j] = true;

    long long count = 0;

    for (int i = 2; i <= n; i++)
        if (!comp[i])
            count++;

    cout << "Time: " << omp_get_wtime() - t << " seconds\n";
    return count;
}

// 2. Cache-friendly Segmented Sieve
long long friendlySieve(int n) {
    double t = omp_get_wtime();

    vector<int> primes = getPrimes(sqrt(n));
    long long count = 0;

    for (int low = 2; low <= n; low += BLOCK) {
        int high = min(low + BLOCK - 1, n);
        vector<bool> mark(high - low + 1, false);

        for (int p : primes) {
            long long start = max((long long)p * p,
                ((long long)low + p - 1) / p * p);

            for (long long j = start; j <= high; j += p)
                mark[j - low] = true;
        }

        for (int i = 0; i < (int)mark.size(); i++)
            if (!mark[i])
                count++;
    }

    cout << "Time: " << omp_get_wtime() - t << " seconds\n";
    return count;
}

// 3. Parallel Cache-friendly Sieve
long long parallelySieve(int n) {
    double t = omp_get_wtime();

    vector<int> primes = getPrimes(sqrt(n));
    long long count = 0;

    int blocks = (n - 1 + BLOCK - 1) / BLOCK;

    #pragma omp parallel for reduction(+:count) schedule(static)
    for (int b = 0; b < blocks; b++) {
        int low = 2 + b * BLOCK;
        int high = min(low + BLOCK - 1, n);

        vector<bool> mark(high - low + 1, false);

        for (int p : primes) {
            long long start = max((long long)p * p,
                ((long long)low + p - 1) / p * p);

            for (long long j = start; j <= high; j += p)
                mark[j - low] = true;
        }

        for (int i = 0; i < (int)mark.size(); i++)
            if (!mark[i])
                count++;
    }

    cout << "Time: " << omp_get_wtime() - t << " seconds\n";
    return count;
}

int main() {
    int n, th;

    cout << "Enter N: ";
    cin >> n;

    cout << "Enter number of threads: ";
    cin >> th;

    if (n < 0 || th <= 0) {
        cout << "Invalid input\n";
        return 0;
    }

    omp_set_num_threads(th);

    cout << "\nCache-unfriendly: "
         << unfriendlySieve(n) << endl;

    cout << "\nCache-friendly: "
         << friendlySieve(n) << endl;

    cout << "\nParallel Sieve: "
         << parallelySieve(n) << endl;

    return 0;
}
