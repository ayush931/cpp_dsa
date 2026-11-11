/**
 * Euler's totient function -> count of numbers from 1 to n that are coprime to
 * n phi(n) = n * product(1 - 1 / p) for every distinct prime p dividing n
 *
 * Chinese Remainder Theorem (CRT)
 * Find x such that x % m[i] = r[i] for pairwise coprime moduli
 * x = sum(r[i] * M[i] * inverse(M[i], m[i])) mod product(M)
 */

#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int eulerTotient(int n) {
    int result = n;

    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) {
                n /= p;
            }
            result -= result / p;
        }
    }

    if (n > 1) {
        result -= result / n;
    }

    return result;
};

vector<int> totientSieve(int n) {
    vector<int> phi(n + 1);

    for (int i = 0; i <= n; i++) {
        phi[i] = i;
    }

    for (int p = 2; p <= n; p++) {
        if (phi[p] == p) {  // p is prime
            for (int multiple = p; multiple <= n; multiple += p) {
                phi[multiple] -= phi[multiple] / p;
            }
        }
    }

    return phi;
};

long long extendedGcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long x1, y1;
    long long g = extendedGcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
};

long long modularInverse(long long a, long long mod) {
    long long x, y;
    long long g = extendedGcd(a, mod, x, y);

    if (g != 1) {
        return -1;  // inverse does not exist
    }

    return (x % mod + mod) % mod;
};

long long chineseRemainder(vector<long long> &remainders,
                           vector<long long> &moduli) {
    long long product = 1;

    for (long long mod : moduli) {
        product *= mod;
    }

    long long result = 0;

    for (int i = 0; i < moduli.size(); i++) {
        long long partial = product / moduli[i];
        long long inverse = modularInverse(partial % moduli[i], moduli[i]);

        result =
            (result + remainders[i] * partial % product * inverse) % product;
    }

    return (result + product) % product;
};

int main() {
    cout << eulerTotient(36) << endl;  // 12

    vector<int> phi = totientSieve(10);
    for (int i = 1; i <= 10; i++) {
        cout << phi[i] << " ";
    }
    cout << endl;

    vector<long long> remainders = {2, 3, 2};
    vector<long long> moduli = {3, 5, 7};
    cout << chineseRemainder(remainders, moduli) << endl;  // 23

    return 0;
}

// Special cases: n = 1 (phi = 1), prime n (phi = n - 1),
// non-coprime moduli (CRT needs a generalized version),
// negative remainders, product overflow (use __int128)
