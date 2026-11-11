/**
 * Lucas Theorem
 *
 * For a prime p, Lucas theorem computes a binomial coefficient modulo p from
 * the base-p digits of n and r:
 *
 *   C(n, r) mod p = product over digits C(n_i, r_i) mod p
 *
 * If any digit satisfies r_i > n_i, the result is zero. This lets us handle
 * n much larger than p, where factorial precomputation up to n is impossible.
 *
 * Time: O(p + log_p n), with factorial tables built once per call.
 * Space: O(p). Use a small prime p; O(p) storage is impractical for large p.
 *
 * A Pascal-triangle DP is included as the reference for small n.
 */

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

long long modularPower(long long base, long long exponent, long long modulus) {
    long long result = 1 % modulus;
    base %= modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= 2;
    }

    return result;
}

long long smallNcr(int n, int r, int prime, const vector<long long> &factorial,
                   const vector<long long> &inverseFactorial) {
    if (r < 0 || r > n) {
        return 0;
    }

    return factorial[n] * inverseFactorial[r] % prime *
           inverseFactorial[n - r] % prime;
}

long long lucasTheorem(long long n, long long r, int prime) {
    if (prime < 2) {
        throw invalid_argument("modulus must be prime");
    }
    for (int divisor = 2; divisor <= prime / divisor; divisor++) {
        if (prime % divisor == 0) {
            throw invalid_argument("modulus must be prime");
        }
    }
    if (n < 0 || r < 0 || r > n) {
        return 0;
    }

    // Build these tables once, rather than once per base-p digit.
    vector<long long> factorial(prime, 1);
    vector<long long> inverseFactorial(prime, 1);

    for (int i = 1; i < prime; i++) {
        factorial[i] = (factorial[i - 1] * i) % prime;
    }

    inverseFactorial[prime - 1] =
        modularPower(factorial[prime - 1], prime - 2, prime);

    for (int i = prime - 2; i >= 0; i--) {
        inverseFactorial[i] = (inverseFactorial[i + 1] * (i + 1)) % prime;
    }

    long long result = 1;

    while (n > 0 || r > 0) {
        int nDigit = static_cast<int>(n % prime);
        int rDigit = static_cast<int>(r % prime);

        if (rDigit > nDigit) {
            return 0;
        }

        result = (result * smallNcr(nDigit, rDigit, prime, factorial,
                                    inverseFactorial)) %
                 prime;
        n /= prime;
        r /= prime;
    }

    return result;
}

long long pascalNcr(int n, int r, int prime) {
    if (r < 0 || r > n) {
        return 0;
    }

    vector<long long> row(n + 1, 0);
    row[0] = 1;

    for (int i = 1; i <= n; i++) {
        for (int j = i; j >= 1; j--) {
            row[j] = (row[j] + row[j - 1]) % prime;
        }
    }

    return row[r];
}

int main() {
    vector<int> primes = {2, 3, 5, 7, 13};

    for (int prime : primes) {
        cout << "prime " << prime << ":" << endl;
        cout << "C(10, 5) = " << lucasTheorem(10, 5, prime)
             << ", Pascal = " << pascalNcr(10, 5, prime) << endl;
        cout << "C(100, 50) = " << lucasTheorem(100, 50, prime)
             << ", Pascal = " << pascalNcr(100, 50, prime) << endl;
        cout << "C(200, 100) = " << lucasTheorem(200, 100, prime)
             << ", Pascal = " << pascalNcr(200, 100, prime) << endl;
        cout << "---" << endl;
    }

    bool allMatch = true;

    for (int prime : primes) {
        for (int n = 0; n <= 120; n++) {
            for (int r = 0; r <= n; r++) {
                if (lucasTheorem(n, r, prime) != pascalNcr(n, r, prime)) {
                    allMatch = false;
                    cout << "mismatch: p=" << prime << " n=" << n << " r=" << r
                         << endl;
                    break;
                }
            }
            if (!allMatch) {
                break;
            }
        }
        if (!allMatch) {
            break;
        }
    }

    cout << "all small cases match Pascal's triangle: "
         << (allMatch ? "yes" : "no") << endl;

    long long hugeN = 1000000000000LL;
    long long hugeR = 123456789LL;
    cout << "C(10^12, 123456789) mod 13 = " << lucasTheorem(hugeN, hugeR, 13)
         << endl;

    cout << "C(0, 0) = " << lucasTheorem(0, 0, 7) << endl;
    cout << "C(5, 6) = " << lucasTheorem(5, 6, 7) << endl;
    cout << "C(5, -1) = " << lucasTheorem(5, -1, 7) << endl;

    return 0;
}

// Expected output (abridged):
// prime 2:
// C(10, 5) = 0, Pascal = 0
// C(100, 50) = 0, Pascal = 0
// C(200, 100) = 0, Pascal = 0
// ...
// all small cases match Pascal's triangle: yes
// C(10^12, 123456789) mod 13 = 0
// C(0, 0) = 1
// C(5, 6) = 0
// C(5, -1) = 0
//
// Special cases: r > n, r < 0, n = 0, a digit with r_i > n_i, and n much
// larger than the prime modulus.
