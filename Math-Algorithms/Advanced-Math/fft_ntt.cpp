/**
 * Polynomial Multiplication: FFT and NTT
 *
 * Given two polynomials
 *   A(x) = a0 + a1*x + ... + an*x^n
 *   B(x) = b0 + b1*x + ... + bm*x^m
 * the product has coefficients
 *   c[k] = sum over i + j = k of a[i] * b[j]
 *
 * The naive method is O(n * m). Both FFT and NTT use the same divide-and-
 * conquer idea:
 *   1. pad both coefficient arrays to a power of two
 *   2. evaluate both polynomials at that many roots of unity
 *   3. multiply the values pointwise
 *   4. interpolate back with the inverse transform
 *
 * FFT uses complex roots of unity -> O(n log n), but floating-point rounding
 * limits the safe coefficient size.
 *
 * NTT uses roots of unity modulo a prime p where p - 1 is divisible by a large
 * power of two. The common choice is
 *   p = 998244353 = 119 * 2^23 + 1, primitive root g = 3
 * It is exact modulo p and avoids rounding, but the result wraps modulo p.
 *
 * For coefficients whose true product is below the modulus, NTT returns the
 * exact integer coefficients. For larger products, use several NTT primes and
 * the Chinese Remainder Theorem, or use FFT with a split-coefficient trick.
 */

#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>
using namespace std;

const double PI = acos(-1.0);
const long long MOD = 998244353;
const long long PRIMITIVE_ROOT = 3;

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

// In-place iterative Cooley-Tukey FFT.
void fft(vector<complex<double>> &values, bool invert) {
    int n = values.size();

    // Bit-reversal permutation.
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;

        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;

        if (i < j) {
            swap(values[i], values[j]);
        }
    }

    for (int length = 2; length <= n; length <<= 1) {
        double angle = 2 * PI / length * (invert ? -1 : 1);
        complex<double> root(cos(angle), sin(angle));

        for (int start = 0; start < n; start += length) {
            complex<double> factor(1, 0);

            for (int offset = 0; offset < length / 2; offset++) {
                complex<double> even = values[start + offset];
                complex<double> odd =
                    values[start + offset + length / 2] * factor;

                values[start + offset] = even + odd;
                values[start + offset + length / 2] = even - odd;
                factor *= root;
            }
        }
    }

    if (invert) {
        for (complex<double> &value : values) {
            value /= n;
        }
    }
}

// In-place iterative NTT modulo MOD.
void ntt(vector<long long> &values, bool invert) {
    int n = values.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;

        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;

        if (i < j) {
            swap(values[i], values[j]);
        }
    }

    for (int length = 2; length <= n; length <<= 1) {
        long long root = modPower(PRIMITIVE_ROOT, (MOD - 1) / length);

        if (invert) {
            root = modPower(root, MOD - 2);
        }

        for (int start = 0; start < n; start += length) {
            long long factor = 1;

            for (int offset = 0; offset < length / 2; offset++) {
                long long even = values[start + offset];
                long long odd =
                    values[start + offset + length / 2] * factor % MOD;

                values[start + offset] = (even + odd) % MOD;
                values[start + offset + length / 2] = (even - odd + MOD) % MOD;
                factor = factor * root % MOD;
            }
        }
    }

    if (invert) {
        long long inverseN = modPower(n, MOD - 2);

        for (long long &value : values) {
            value = value * inverseN % MOD;
        }
    }
}

int nextPowerOfTwo(int value) {
    int result = 1;

    while (result < value) {
        result <<= 1;
    }

    return result;
}

vector<long long> multiplyNaive(const vector<long long> &a,
                                const vector<long long> &b) {
    if (a.empty() || b.empty()) {
        return {};
    }

    vector<long long> result(a.size() + b.size() - 1, 0);

    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            result[i + j] += a[i] * b[j];
        }
    }

    return result;
}

vector<long long> multiplyFFT(const vector<long long> &a,
                              const vector<long long> &b) {
    if (a.empty() || b.empty()) {
        return {};
    }

    int resultSize = a.size() + b.size() - 1;
    int n = nextPowerOfTwo(resultSize);

    vector<complex<double>> first(n, 0), second(n, 0);

    for (int i = 0; i < a.size(); i++) {
        first[i] = a[i];
    }
    for (int i = 0; i < b.size(); i++) {
        second[i] = b[i];
    }

    fft(first, false);
    fft(second, false);

    for (int i = 0; i < n; i++) {
        first[i] *= second[i];
    }

    fft(first, true);

    vector<long long> result(resultSize, 0);

    for (int i = 0; i < resultSize; i++) {
        result[i] = llround(first[i].real());
    }

    return result;
}

vector<long long> multiplyNTT(const vector<long long> &a,
                              const vector<long long> &b) {
    if (a.empty() || b.empty()) {
        return {};
    }

    int resultSize = a.size() + b.size() - 1;
    int n = nextPowerOfTwo(resultSize);

    vector<long long> first(n, 0), second(n, 0);

    for (int i = 0; i < a.size(); i++) {
        first[i] = ((a[i] % MOD) + MOD) % MOD;
    }

    for (int i = 0; i < b.size(); i++) {
        second[i] = ((b[i] % MOD) + MOD) % MOD;
    }

    ntt(first, false);
    ntt(second, false);

    for (int i = 0; i < n; i++) {
        first[i] = first[i] * second[i] % MOD;
    }

    ntt(first, true);
    first.resize(resultSize);

    return first;
}

void print(const vector<long long> &values) {
    cout << "[";

    for (int i = 0; i < values.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << values[i];
    }

    cout << "]" << endl;
}

int main() {
    vector<long long> first = {1, 2, 3, 4};
    vector<long long> second = {5, 6, 7};

    print(multiplyNaive(first, second));  // [5, 16, 34, 52, 45, 28]
    print(multiplyFFT(first, second));    // [5, 16, 34, 52, 45, 28]
    print(multiplyNTT(first, second));    // [5, 16, 34, 52, 45, 28]

    // (1 + x)^2 = 1 + 2x + x^2.
    print(multiplyFFT({1, 1}, {1, 1}));  // [1, 2, 1]

    // Small coefficients stay exact in both transforms.
    print(multiplyNaive({123, 456}, {789, 321}));
    print(multiplyFFT({123, 456}, {789, 321}));
    print(multiplyNTT({123, 456}, {789, 321}));
    // [97047, 399267, 146376]

    // A zero polynomial.
    print(multiplyNTT({0}, {1, 2, 3}));  // [0, 0, 0, 0]

    // Empty input.
    print(multiplyNTT({}, {1, 2}));  // []

    // Large coefficients: NTT wraps modulo 998244353.
    vector<long long> largeA = {1000000000, 2000000000};
    vector<long long> largeB = {3000000000LL, 4000000000LL};

    print(multiplyNTT(largeA, largeB));
    // The true coefficients are 3e18, 1e19, and 8e18; NTT prints them modulo
    // 998244353, which is why the result is not the integer product.

    return 0;
}

/**
 * Time complexity -> O(N log N) for FFT and NTT, where N is the padded
 * transform length; O(n * m) for the naive method, where n and m are the
 * numbers of coefficients
 * Space complexity -> O(N) for the padded arrays and the result
 *
 * Special cases:
 *   - empty input -> empty result
 *   - a zero polynomial -> all-zero coefficients
 *   - a single coefficient -> scalar multiplication
 *   - a result shorter than a power of two -> pad with zeros
 *   - FFT rounding: coefficients above about 10^15 can lose precision; split
 *     large coefficients into high and low parts or use NTT
 *   - NTT modulus: 998244353 supports transforms up to length 2^23; a longer
 *     polynomial needs a larger NTT-friendly prime or a CRT combination
 *   - negative coefficients: reduced modulo MOD before NTT; the result is in
 *     [0, MOD)
 *
 * FFT and NTT are the core of fast convolution. They are used for polynomial
 * multiplication, big-integer multiplication, string matching with wildcards,
 * combinatorial generating functions, and convolution-based DP.
 *
 * The inverse transform uses the inverse root and divides by n. For NTT, the
 * division is multiplication by the modular inverse of n.
 */
