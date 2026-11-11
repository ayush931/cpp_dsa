/**
 * Mobius Inversion and the Mobius Function
 *
 * The Mobius function mu(n) is:
 *   mu(1) = 1
 *   mu(n) = 0        if n has a squared prime factor
 *   mu(n) = (-1)^k   if n is a product of k distinct primes
 *
 * Mobius inversion turns a sum over divisors into a sum over multiples:
 *
 *   if F(n) = sum_{d | n} f(d), then f(n) = sum_{d | n} mu(d) F(n / d)
 *
 * Two classic applications are shown:
 *   - number of ordered coprime pairs in [1, n]^2:
 *       sum_{d=1..n} mu(d) * floor(n / d)^2
 *   - number of square-free integers up to n:
 *       sum_{d=1..sqrt(n)} mu(d) * floor(n / d^2)
 *
 * Euler's totient is also computed with a linear sieve and used to sum
 * gcd(a, b) over all ordered pairs.
 *
 * Brute-force checks are included for small n.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

struct SieveResult {
    vector<int> mobius;
    vector<int> totient;
    vector<int> primes;
};

SieveResult linearSieve(int limit) {
    SieveResult result;
    result.mobius.assign(limit + 1, 0);
    result.totient.assign(limit + 1, 0);
    vector<bool> composite(limit + 1, false);

    if (limit >= 1) {
        result.mobius[1] = 1;
        result.totient[1] = 1;
    }

    for (int value = 2; value <= limit; value++) {
        if (!composite[value]) {
            result.primes.push_back(value);
            result.mobius[value] = -1;
            result.totient[value] = value - 1;
        }

        for (int prime : result.primes) {
            long long next = 1LL * value * prime;
            if (next > limit) {
                break;
            }

            composite[next] = true;

            if (value % prime == 0) {
                result.mobius[next] = 0;
                result.totient[next] = result.totient[value] * prime;
                break;
            }

            result.mobius[next] = -result.mobius[value];
            result.totient[next] = result.totient[value] * (prime - 1);
        }
    }

    return result;
}

long long countCoprimePairs(int n, const vector<int> &mobius) {
    long long total = 0;

    for (int divisor = 1; divisor <= n; divisor++) {
        long long quotient = n / divisor;
        total += 1LL * mobius[divisor] * quotient * quotient;
    }

    return total;
}

long long countSquareFree(int n, const vector<int> &mobius) {
    long long total = 0;
    int limit = static_cast<int>(sqrt(n));

    for (int divisor = 1; divisor <= limit; divisor++) {
        long long quotient = n / (1LL * divisor * divisor);
        total += 1LL * mobius[divisor] * quotient;
    }

    return total;
}

long long sumOfGcdOverOrderedPairs(int n, const vector<int> &totient) {
    long long total = 0;

    for (int divisor = 1; divisor <= n; divisor++) {
        long long quotient = n / divisor;
        total += 1LL * totient[divisor] * quotient * quotient;
    }

    return total;
}

long long bruteCoprimePairs(int n) {
    long long total = 0;

    for (int a = 1; a <= n; a++) {
        for (int b = 1; b <= n; b++) {
            if (gcd(a, b) == 1) {
                total++;
            }
        }
    }

    return total;
}

long long bruteSquareFree(int n) {
    long long total = 0;

    for (int value = 1; value <= n; value++) {
        int remaining = value;
        bool squareFree = true;

        for (int divisor = 2; divisor * divisor <= remaining; divisor++) {
            int exponent = 0;
            while (remaining % divisor == 0) {
                remaining /= divisor;
                exponent++;
            }
            if (exponent >= 2) {
                squareFree = false;
                break;
            }
        }

        if (squareFree) {
            total++;
        }
    }

    return total;
}

long long bruteGcdSum(int n) {
    long long total = 0;

    for (int a = 1; a <= n; a++) {
        for (int b = 1; b <= n; b++) {
            total += gcd(a, b);
        }
    }

    return total;
}

int main() {
    int limit = 100;
    SieveResult sieve = linearSieve(limit);

    cout << "mu[1..20]: ";

    for (int value = 1; value <= 20; value++) {
        if (value > 1) {
            cout << " ";
        }
        cout << sieve.mobius[value];
    }

    cout << endl;

    bool coprimeMatches = true;
    bool squareFreeMatches = true;
    bool gcdMatches = true;

    for (int n = 1; n <= 60; n++) {
        if (countCoprimePairs(n, sieve.mobius) != bruteCoprimePairs(n)) {
            coprimeMatches = false;
        }

        if (countSquareFree(n, sieve.mobius) != bruteSquareFree(n)) {
            squareFreeMatches = false;
        }

        if (sumOfGcdOverOrderedPairs(n, sieve.totient) != bruteGcdSum(n)) {
            gcdMatches = false;
        }
    }

    cout << "coprime pair formula matches brute force: "
         << (coprimeMatches ? "yes" : "no") << endl;
    cout << "square-free formula matches brute force: "
         << (squareFreeMatches ? "yes" : "no") << endl;
    cout << "gcd sum formula matches brute force: "
         << (gcdMatches ? "yes" : "no") << endl;

    cout << "coprime ordered pairs up to 10: "
         << countCoprimePairs(10, sieve.mobius) << endl;
    cout << "square-free numbers up to 100: "
         << countSquareFree(100, sieve.mobius) << endl;
    cout << "sum of gcd over ordered pairs up to 10: "
         << sumOfGcdOverOrderedPairs(10, sieve.totient) << endl;

    cout << "first 10 totients: ";

    for (int value = 1; value <= 10; value++) {
        if (value > 1) {
            cout << " ";
        }
        cout << sieve.totient[value];
    }

    cout << endl;

    return 0;
}

// Expected output (abridged):
// mu[1..20]: 1 -1 -1 0 -1 1 -1 0 0 1 -1 0 -1 1 1 0 -1 0 -1 0
// coprime pair formula matches brute force: yes
// square-free formula matches brute force: yes
// gcd sum formula matches brute force: yes
// coprime ordered pairs up to 10: 63
// square-free numbers up to 100: 61
// sum of gcd over ordered pairs up to 10: 189
// first 10 totients: 1 1 2 2 4 2 6 4 6 4
//
// Special cases: n = 1, prime n, highly composite n, and numbers containing a
// squared prime factor (mu = 0).
