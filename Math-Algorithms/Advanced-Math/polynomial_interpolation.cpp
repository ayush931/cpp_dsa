/**
 * Lagrange polynomial interpolation
 *
 * Given n + 1 distinct points (x_i, y_i), there is exactly one polynomial of
 * degree at most n that passes through all of them. Lagrange interpolation
 * evaluates that polynomial at a query x without building its coefficients:
 *
 *   P(x) = sum over i of y_i * product over j != i of (x - x_j) / (x_i - x_j)
 *
 * Time -> O(n^2) per evaluation
 * Space -> O(1) extra
 *
 * This implementation uses modulo 1e9+7, so division is modular inverse.
 * For consecutive x values, a finite-difference method can be faster.
 *
 * Edge cases: one point, duplicate x values (invalid input), x equal to a known
 * point, and a query outside the range of the known points.
 */

#include <iostream>
#include <vector>
using namespace std;

const long long MOD = 1000000007;

long long modPower(long long base, long long exponent) {
    long long result = 1;
    base %= MOD;

    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }

    return result;
}

long long modInverse(long long value) {
    return modPower(value, MOD - 2);
}

long long interpolate(vector<pair<long long, long long>> &points, long long x) {
    int n = points.size();
    long long answer = 0;

    for (int i = 0; i < n; i++) {
        long long numerator = 1;
        long long denominator = 1;

        for (int j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }

            numerator = numerator * ((x - points[j].first + MOD) % MOD) % MOD;
            denominator = denominator *
                          ((points[i].first - points[j].first + MOD) % MOD) %
                          MOD;
        }

        if (denominator == 0) {
            return -1;  // duplicate x values
        }

        long long term = points[i].second * numerator % MOD;
        term = term * modInverse(denominator) % MOD;
        answer = (answer + term) % MOD;
    }

    return answer;
}

int main() {
    // Points on P(x) = x^2 + 1.
    vector<pair<long long, long long>> points = {{0, 1}, {1, 2}, {2, 5}};

    cout << interpolate(points, 3) << endl;   // 10
    cout << interpolate(points, 0) << endl;   // 1
    cout << interpolate(points, 10) << endl;  // 101

    vector<pair<long long, long long>> single = {{5, 42}};
    cout << interpolate(single, 100) << endl;  // 42

    vector<pair<long long, long long>> duplicate = {{1, 2}, {1, 3}};
    cout << interpolate(duplicate, 0) << endl;  // -1

    return 0;
}

// Special cases:
// - one point -> the constant polynomial y_0
// - duplicate x values -> division by zero, returns -1
// - x equal to a known point -> the product for every other term is zero,
//   so the answer is exactly y_i
// - negative coordinates are normalized with (+ MOD) % MOD
