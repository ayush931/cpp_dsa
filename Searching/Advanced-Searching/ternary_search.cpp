/**
 * Ternary search
 *
 * Ternary search finds the maximum of a unimodal function: one that increases
 * and then decreases. Each step evaluates two interior points m1 and m2:
 *
 *   if f(m1) < f(m2), the maximum cannot be left of m1
 *   otherwise, the maximum cannot be right of m2
 *
 * The search interval shrinks to two thirds of its size each step. For an
 * integer array, the loop stops when three or fewer candidates remain and the
 * last candidates are scanned directly.
 *
 * This file demonstrates both forms:
 *   - integer array, returning the index of the maximum
 *   - real-valued function, returning an approximate x
 *
 * Time        -> O(log n) for the integer form; O(log(1 / epsilon)) for the
 *                real form with a fixed number of iterations
 * Space       -> O(1)
 * Requirement -> the function must be unimodal on the search interval
 *
 * Linear scan and dense sampling are included as brute-force references.
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

int ternarySearchMaxIndex(const vector<int> &arr) {
    int n = arr.size();
    if (n == 0) {
        return -1;
    }

    int low = 0;
    int high = n - 1;

    while (high - low > 2) {
        int third = (high - low) / 3;
        int m1 = low + third;
        int m2 = high - third;

        if (arr[m1] < arr[m2]) {
            low = m1 + 1;
        } else {
            high = m2 - 1;
        }
    }

    int best = low;

    for (int i = low; i <= high; i++) {
        if (arr[i] > arr[best]) {
            best = i;
        }
    }

    return best;
}

double parabola(double x) {
    return -(x - 2.5) * (x - 2.5) + 4.0;
}

double ternarySearchMaxReal(double (*function)(double), double low, double high,
                            int iterations) {
    for (int i = 0; i < iterations; i++) {
        double m1 = low + (high - low) / 3.0;
        double m2 = high - (high - low) / 3.0;

        if (function(m1) < function(m2)) {
            low = m1;
        } else {
            high = m2;
        }
    }

    return (low + high) / 2.0;
}

int main() {
    vector<vector<int>> tests = {{1, 3, 5, 8, 12, 9, 6, 2},
                                 {1, 2, 3, 4, 5},
                                 {5, 4, 3, 2, 1},
                                 {7},
                                 {2, 2, 2, 3, 2, 2}};

    for (const vector<int> &arr : tests) {
        int index = ternarySearchMaxIndex(arr);

        // Brute-force reference.
        int bruteIndex = 0;

        for (int i = 1; i < (int)arr.size(); i++) {
            if (arr[i] > arr[bruteIndex]) {
                bruteIndex = i;
            }
        }

        cout << "integer max index: " << index
             << ", value: " << (index >= 0 ? arr[index] : -1)
             << ", brute index: " << bruteIndex
             << ", agree: " << (index == bruteIndex ? "yes" : "no") << endl;
    }

    cout << "empty array index: " << ternarySearchMaxIndex({}) << endl;

    double realMax = ternarySearchMaxReal(parabola, -10.0, 10.0, 200);

    // Dense sampling as the brute-force reference.
    double bestX = -10.0;
    double bestY = parabola(bestX);

    for (int i = 0; i <= 20000; i++) {
        double x = -10.0 + 20.0 * i / 20000.0;
        double y = parabola(x);

        if (y > bestY) {
            bestY = y;
            bestX = x;
        }
    }

    cout << fixed << setprecision(4);
    cout << "real max x: " << realMax << ", f(x): " << parabola(realMax)
         << endl;
    cout << "sampled max x: " << bestX << ", f(x): " << bestY << endl;

    return 0;
}

// Expected output:
// integer max index: 4, value: 12, brute index: 4, agree: yes
// integer max index: 4, value: 5, brute index: 4, agree: yes
// integer max index: 0, value: 5, brute index: 0, agree: yes
// integer max index: 0, value: 7, brute index: 0, agree: yes
// integer max index: 3, value: 3, brute index: 3, agree: yes
// empty array index: -1
// real max x: 2.5000, f(x): 4.0000
// sampled max x: 2.5000, f(x): 4.0000
//
// Special cases: empty array, one element, strictly increasing (maximum at the
// right end), strictly decreasing (maximum at the left end), a plateau, and a
// non-symmetric peak.
//
// With a plateau, several indices hold the maximum. Ternary search returns one
// of them; this implementation returns the leftmost maximum in the final
// three-element scan because the comparison uses strict greater-than.
