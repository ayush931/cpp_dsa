/**
 * Modular arithmetic -> keeps numbers inside a fixed range
 * (a + b) % m = ((a % m) + (b % m)) % m
 * (a - b) % m = ((a % m) - (b % m) + m) % m
 * (a * b) % m = ((a % m) * (b % m)) % m
 * Division is done by multiplying with the modular inverse
 *
 * Fast exponentiation -> a^b in O(log b)
 * Modular inverse using Fermat -> a^(m - 2) % m, when m is prime
 */

#include <iostream>
using namespace std;

long long binaryExponentiation(long long base, long long exp) {
    long long result = 1;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }

    return result;
};

long long modularExponentiation(long long base, long long exp, long long mod) {
    long long result = 1;

    base %= mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }

    return result;
};

long long modularInverse(long long a, long long prime) {
    return modularExponentiation(a, prime - 2, prime);
};

int main() {
    cout << binaryExponentiation(2, 10) << endl;         // 1024
    cout << modularExponentiation(2, 10, 1000) << endl;  // 24
    cout << modularInverse(3, 7) << endl;  // 5, because 3 * 5 % 7 = 1

    long long a = 1e18;
    long long b = 1e18;
    long long mod = 1e9 + 7;

    cout << ((a % mod) * (b % mod)) % mod << endl;

    return 0;
}

// Fermat's little theorem -> a^(p - 1) = 1 (mod p), when p is prime and a is
// not divisible by p
