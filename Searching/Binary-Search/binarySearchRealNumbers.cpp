// Binary search on real numbers (bisection).
//
// There is no array to index, but the same idea works on a continuous range:
// keep an interval [lo, hi] that must contain the answer, evaluate the middle,
// and discard the half that cannot contain it. Stop after a fixed number of
// iterations or when the interval is smaller than the required precision.
//
// Example:
//   squareRoot(2)  = 1.414213562...
//   squareRoot(9)  = 3
//   squareRoot(0)  = 0
//
// The integer version of this problem is in squareRoot.cpp.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

// Bisection: 100 iterations shrink the interval by 2^100, far below double
// precision. This is the general pattern for binary search on real numbers.
double squareRootBisection(double x) {
    if (x < 0) {
        return -1;
    }
    if (x == 0) {
        return 0;
    }

    double lo = 0;
    double hi = max(1.0, x);

    for (int i = 0; i < 100; i++) {
        double mid = lo + (hi - lo) / 2;

        if (mid * mid < x) {
            lo = mid;
        } else {
            hi = mid;
        }
    }

    return (lo + hi) / 2;
}

// Newton's method converges faster for smooth functions, but it needs a
// derivative and a good starting point. Bisection is slower per digit yet
// always safe when the predicate is monotone.
double squareRootNewton(double x) {
    if (x < 0) {
        return -1;
    }
    if (x == 0) {
        return 0;
    }

    double guess = x;

    for (int i = 0; i < 50; i++) {
        guess = (guess + x / guess) / 2;
    }

    return guess;
}

int main() {
    cout << fixed << setprecision(9);

    cout << squareRootBisection(2) << endl;     // 1.414213562
    cout << squareRootBisection(9) << endl;     // 3.000000000
    cout << squareRootBisection(0) << endl;     // 0.000000000
    cout << squareRootBisection(0.25) << endl;  // 0.500000000
    cout << squareRootBisection(-4) << endl;    // -1.000000000

    cout << squareRootNewton(2) << endl;     // 1.414213562
    cout << squareRootNewton(1e12) << endl;  // 1000000.000000000

    double difference = fabs(squareRootBisection(2) - squareRootNewton(2));
    cout << (difference < 1e-9) << endl;  // 1

    return 0;
}

// Time complexity -> O(iterations), independent of the value of x
// Space complexity -> O(1)
