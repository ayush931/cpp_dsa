/**
 * Combinatorics -> counting arrangements and selections
 * nPr = n! / (n - r)!
 * nCr = n! / (r! * (n - r)!)
 * Pascal's rule -> nCr = (n - 1)Cr + (n - 1)C(r - 1)
 * Catalan number -> C(n) = (2n)C(n) / (n + 1)
 */

#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1e9 + 7;

long long factorial(int n) {
    long long result = 1;

    for (int i = 2; i <= n; i++) {
        result = (result * i) % MOD;
    }

    return result;
};

long long modularExponentiation(long long base, long long exp) {
    long long result = 1;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp /= 2;
    }

    return result;
};

long long nCr(int n, int r) {
    if (r < 0 || r > n) {
        return 0;
    }

    long long numerator = factorial(n);
    long long denominator = (factorial(r) * factorial(n - r)) % MOD;

    return (numerator * modularExponentiation(denominator, MOD - 2)) % MOD;
};

vector<vector<long long>> pascalTriangle(int n) {
    vector<vector<long long>> triangle(n);

    for (int i = 0; i < n; i++) {
        triangle[i].resize(i + 1, 1);

        for (int j = 1; j < i; j++) {
            triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
        }
    }

    return triangle;
};

long long catalan(int n) {
    return (nCr(2 * n, n) * modularExponentiation(n + 1, MOD - 2)) % MOD;
};

int main() {
    cout << factorial(5) << endl;  // 120
    cout << nCr(5, 2) << endl;     // 10

    vector<vector<long long>> triangle = pascalTriangle(5);
    for (auto &row : triangle) {
        for (long long value : row) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << catalan(4) << endl;  // 14

    return 0;
}

// Catalan numbers -> 1, 1, 2, 5, 14, 42, ...
// They count valid parenthesis sequences, binary search trees and more
// nCr above uses factorials, so it needs n < MOD; for larger n use Lucas
// theorem (see 96_Math_Algorithms/08_segmented_sieve_and_inverse.cpp)
