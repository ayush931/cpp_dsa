/**
 * Miller-Rabin -> probabilistic primality test
 * Deterministic for 64-bit integers with the bases 2, 3, 5, 7, 11, 13, 17, 19,
 * 23, 29, 31, 37 Time -> O(k * log^3 n)
 *
 * Pollard's rho -> factorizes a composite number in expected O(n^(1/4))
 * Used with Miller-Rabin to factorize large 64-bit numbers
 */

#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

u64 modularMultiply(u64 a, u64 b, u64 mod) {
    return (u128)a * b % mod;
};

u64 modularPower(u64 base, u64 exp, u64 mod) {
    u64 result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1) {
            result = modularMultiply(result, base, mod);
        }
        base = modularMultiply(base, base, mod);
        exp >>= 1;
    }

    return result;
};

bool isPrime(u64 n) {
    if (n < 2) {
        return false;
    }

    for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
                  29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) {
            return n == p;
        }
    }

    u64 d = n - 1;
    int s = 0;

    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    for (u64 a : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
                  29ULL, 31ULL, 37ULL}) {
        u64 x = modularPower(a, d, n);

        if (x == 1 || x == n - 1) {
            continue;
        }

        bool composite = true;

        for (int i = 1; i < s; i++) {
            x = modularMultiply(x, x, n);

            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite) {
            return false;
        }
    }

    return true;
};

u64 pollardRho(u64 n) {
    if (n % 2 == 0) {
        return 2;
    }

    u64 x = rand() % (n - 2) + 2;
    u64 y = x;
    u64 c = rand() % (n - 1) + 1;
    u64 divisor = 1;

    while (divisor == 1) {
        x = (modularMultiply(x, x, n) + c) % n;
        y = (modularMultiply(y, y, n) + c) % n;
        y = (modularMultiply(y, y, n) + c) % n;

        divisor = gcd(x > y ? x - y : y - x, n);
    }

    return divisor;
};

void factorize(u64 n, vector<u64> &factors) {
    if (n == 1) {
        return;
    }

    if (isPrime(n)) {
        factors.push_back(n);
        return;
    }

    u64 divisor = pollardRho(n);
    factorize(divisor, factors);
    factorize(n / divisor, factors);
};

int main() {
    cout << isPrime(97) << endl;          // 1
    cout << isPrime(1000000007) << endl;  // 1
    cout << isPrime(1000000008) << endl;  // 0

    vector<u64> factors;
    factorize(600851475143ULL, factors);

    for (u64 factor : factors) {
        cout << factor << " ";
    }
    cout << endl;

    return 0;
}

// Special cases: n < 2, even numbers, perfect powers, Carmichael numbers,
// 64-bit overflow in multiplication (use __int128)
