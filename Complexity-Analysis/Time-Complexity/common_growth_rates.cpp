// Common growth rates
//
// From fastest to slowest:
//   O(1)        constant      hash lookup, array index
//   O(log n)    logarithmic   binary search, balanced tree lookup
//   O(n)        linear        single pass, counting
//   O(n log n)  linearithmic  merge sort, heap sort
//   O(n^2)      quadratic     nested loops, bubble sort
//   O(n^3)      cubic         triple loops, Floyd-Warshall
//   O(2^n)      exponential   subsets, naive recursion
//   O(n!)       factorial     permutations
//
// Constants and lower-order terms are dropped: 3n + 5 is O(n), and
// n^2 + 100n is O(n^2). For small n a "worse" algorithm can still win, but
// asymptotically the growth rate decides.

#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

long long constantWork(long long n) {
    return 1;
}

long long logarithmicWork(long long n) {
    long long steps = 0;

    while (n > 1) {
        n /= 2;
        steps++;
    }

    return steps;
}

long long linearWork(long long n) {
    return n;
}

long long linearithmicWork(long long n) {
    return n * logarithmicWork(n);
}

long long quadraticWork(long long n) {
    return n * n;
}

long long cubicWork(long long n) {
    return n * n * n;
}

// Use long double so large powers do not overflow immediately.
long double exponentialWork(long long n) {
    return pow(2.0L, (long double)n);
}

long double factorialWork(long long n) {
    long double result = 1;

    for (long long i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}

int main() {
    cout << fixed << setprecision(0);
    cout << "n\tO(1)\tO(log n)\tO(n)\tO(n log n)\tO(n^2)\tO(n^3)\tO(2^n)"
         << endl;

    for (long long n = 4; n <= 32; n *= 2) {
        cout << n << "\t" << constantWork(n) << "\t" << logarithmicWork(n)
             << "\t\t" << linearWork(n) << "\t" << linearithmicWork(n) << "\t\t"
             << quadraticWork(n) << "\t" << cubicWork(n) << "\t"
             << exponentialWork(n) << endl;
    }

    cout << endl;
    cout << "factorial(10) = " << factorialWork(10) << endl;  // 3628800
    cout << "factorial(20) = " << factorialWork(20)
         << endl;  // 2432902008176640000

    // Rough feasibility at one billion operations per second:
    //   n = 10^6 -> O(n), O(n log n) fine; O(n^2) too slow
    //   n = 10^3 -> O(n^2) fine; O(2^n) impossible
    //   n = 20   -> O(2^n) borderline; O(n!) impossible
    return 0;
}

// Time complexity -> the functions above are intentionally simple so the
// growth rates are easy to compare.
// Space complexity -> O(1) for every function; only the output values grow.
//
// Special cases: n = 0 (log 0 is undefined; define the loop to handle it),
// n = 1 (every function returns a small constant), overflow when squaring or
// exponentiating large values, and floating-point rounding in long double.
//
// Practical rule of thumb: with n up to 10^5, aim for O(n log n) or
// better. With n up to 10^3, O(n^2) is often acceptable. With n up to 20,
// exponential algorithms are possible.
