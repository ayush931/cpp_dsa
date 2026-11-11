/**
 * DP with matrix exponentiation
 *
 * A linear recurrence with constant coefficients can be evaluated in
 * O(k^3 log n) time with matrix exponentiation, where k is the order of the
 * recurrence.
 *
 * Example: Fibonacci
 *   F(n) = F(n - 1) + F(n - 2)
 *   [F(n)  ]   [1 1] [F(n-1)]
 *   [F(n-1)] = [1 0] [F(n-2)]
 *
 * The same technique handles tilings, paths in a fixed graph, and any DP whose
 * transition does not depend on the step number.
 *
 * Edge cases: n = 0, n = 1, negative n (not supported), modulo arithmetic, and
 * overflow when the modulus is large.
 */

#include <iostream>
#include <vector>
using namespace std;

using Matrix = vector<vector<long long>>;

const long long MOD = 1000000007;

Matrix multiply(Matrix &a, Matrix &b) {
    int n = a.size();
    int m = b[0].size();
    int p = b.size();

    Matrix result(n, vector<long long>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < p; k++) {
            if (a[i][k] == 0) {
                continue;
            }
            for (int j = 0; j < m; j++) {
                result[i][j] = (result[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }

    return result;
}

Matrix power(Matrix base, long long exponent) {
    int n = base.size();
    Matrix result(n, vector<long long>(n, 0));

    for (int i = 0; i < n; i++) {
        result[i][i] = 1;
    }

    while (exponent > 0) {
        if (exponent & 1) {
            result = multiply(result, base);
        }
        base = multiply(base, base);
        exponent >>= 1;
    }

    return result;
}

long long fibonacci(long long n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    Matrix transition = {{1, 1}, {1, 0}};

    Matrix result = power(transition, n - 1);
    return result[0][0];
}

long long tribonacci(long long n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 1;
    }

    Matrix transition = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}};

    Matrix result = power(transition, n - 2);
    return (result[0][0] + result[0][1]) % MOD;
}

long long dominoTilings(int n) {
    // Number of ways to tile a 2 x n board with 1 x 2 dominoes = F(n + 1).
    return fibonacci(n + 1);
}

int main() {
    cout << fibonacci(10) << endl;     // 55
    cout << fibonacci(50) << endl;     // 12586269025 mod 1e9+7 = 586268941
    cout << tribonacci(10) << endl;    // 149
    cout << dominoTilings(5) << endl;  // 8

    cout << fibonacci(0) << endl;  // 0
    cout << fibonacci(1) << endl;  // 1

    return 0;
}

// Special cases:
// - n = 0 and n = 1 are handled directly to avoid negative exponents
// - all arithmetic is modulo 1e9+7 to prevent overflow
// - a 1x1 transition matrix works for a first-order recurrence
// - matrix exponentiation is only valid when the transition is step-independent
