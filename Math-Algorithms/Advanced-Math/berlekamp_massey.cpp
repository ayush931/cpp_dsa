/**
 * Berlekamp-Massey algorithm
 *
 * Given the first terms of a sequence, Berlekamp-Massey finds the shortest
 * linear recurrence with constant coefficients that generates it:
 *
 *   s[n] = c[1] * s[n - 1] + c[2] * s[n - 2] + ... + c[L] * s[n - L]
 *
 * The algorithm works over a field; this implementation uses modulo 1e9+7.
 * The nth term can then be computed with Kitamasa's polynomial exponentiation
 * in O(L^2 log n) time.
 *
 * Time to find the recurrence -> O(n^2)
 * Space -> O(n)
 *
 * Edge cases: a constant sequence (L = 1), an all-zero sequence (L = 0),
 * a sequence shorter than the recurrence order, and a non-prime modulus
 * (modular inverses may not exist).
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

// Returns c with c[0] = 1 and s[n] = sum(c[i] * s[n - i]) for i = 1..L.
vector<long long> berlekampMassey(vector<long long> &sequence) {
    int n = sequence.size();
    int order = 0;
    int shift = 0;

    vector<long long> current(n + 1, 0);
    vector<long long> previous(n + 1, 0);
    vector<long long> saved;

    current[0] = 1;
    previous[0] = 1;

    long long lastDiscrepancy = 1;

    for (int i = 0; i < n; i++) {
        shift++;

        long long discrepancy = sequence[i];
        for (int j = 1; j <= order; j++) {
            discrepancy = (discrepancy + current[j] * sequence[i - j]) % MOD;
        }

        if (discrepancy == 0) {
            continue;
        }

        saved = current;
        long long coefficient = discrepancy * modInverse(lastDiscrepancy) % MOD;

        for (int j = shift; j <= n; j++) {
            current[j] = (current[j] - coefficient * previous[j - shift]) % MOD;
            if (current[j] < 0) {
                current[j] += MOD;
            }
        }

        if (2 * order <= i) {
            order = i + 1 - order;
            previous = saved;
            lastDiscrepancy = discrepancy;
            shift = 0;
        }
    }

    current.resize(order + 1);

    // Convert from s[n] + sum(c[i] * s[n-i]) = 0 to the recurrence form.
    for (int i = 1; i <= order; i++) {
        current[i] = (MOD - current[i]) % MOD;
    }

    return current;
}

vector<long long> combine(vector<long long> &a, vector<long long> &b,
                          vector<long long> &recurrence) {
    int order = recurrence.size() - 1;
    vector<long long> result(2 * order - 1, 0);

    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            result[i + j] = (result[i + j] + a[i] * b[j]) % MOD;
        }
    }

    for (int i = 2 * order - 2; i >= order; i--) {
        for (int j = 1; j <= order; j++) {
            result[i - j] = (result[i - j] + result[i] * recurrence[j]) % MOD;
        }
    }

    result.resize(order);
    return result;
}

long long nthTerm(vector<long long> &initial, vector<long long> &recurrence,
                  long long n) {
    int order = recurrence.size() - 1;
    if (order == 0) {
        return 0;
    }
    if (n < (long long)initial.size()) {
        return initial[n];
    }

    vector<long long> result(order, 0);
    vector<long long> base(order, 0);

    result[0] = 1;

    if (order == 1) {
        base[0] = recurrence[1];
    } else {
        base[1] = 1;
    }

    while (n > 0) {
        if (n & 1) {
            result = combine(result, base, recurrence);
        }
        base = combine(base, base, recurrence);
        n >>= 1;
    }

    long long answer = 0;
    for (int i = 0; i < order; i++) {
        answer = (answer + result[i] * initial[i]) % MOD;
    }

    return answer;
}

int main() {
    vector<long long> fibonacci = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    vector<long long> recurrence = berlekampMassey(fibonacci);

    cout << recurrence.size() - 1 << endl;               // 2
    cout << nthTerm(fibonacci, recurrence, 10) << endl;  // 55

    vector<long long> powers = {1, 2, 4, 8, 16, 32};
    vector<long long> powerRecurrence = berlekampMassey(powers);
    cout << powerRecurrence.size() - 1 << endl;            // 1
    cout << nthTerm(powers, powerRecurrence, 10) << endl;  // 1024

    vector<long long> zeros = {0, 0, 0, 0};
    vector<long long> zeroRecurrence = berlekampMassey(zeros);
    cout << zeroRecurrence.size() - 1 << endl;  // 0

    return 0;
}

// Special cases:
// - an all-zero sequence has order 0
// - a constant sequence c has recurrence s[n] = s[n-1]
// - if n is smaller than the number of known terms, return the known term
// - the modulus must be prime for modInverse to work
