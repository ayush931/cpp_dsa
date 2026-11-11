/**
 * Prime factorization -> express a number as a product of primes
 * Trial division -> O(sqrt(n))
 * Using the smallest prime factor array -> O(log n) per number
 */

#include <iostream>
#include <map>
#include <vector>
using namespace std;

vector<pair<int, int>> primeFactorization(int n) {
    vector<pair<int, int>> factors;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            int count = 0;
            while (n % i == 0) {
                n /= i;
                count++;
            }
            factors.push_back({i, count});
        }
    }

    if (n > 1) {
        factors.push_back({n, 1});
    }

    return factors;
};

vector<int> factorizeWithSPF(int n, vector<int> &spf) {
    vector<int> factors;

    while (n > 1) {
        factors.push_back(spf[n]);
        n /= spf[n];
    }

    return factors;
};

int main() {
    vector<pair<int, int>> factors = primeFactorization(360);

    for (auto &f : factors) {
        cout << f.first << "^" << f.second << " ";
    }
    cout << endl;

    int limit = 100;
    vector<int> spf(limit + 1);

    for (int i = 0; i <= limit; i++) {
        spf[i] = i;
    }

    for (int i = 2; i * i <= limit; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= limit; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    vector<int> result = factorizeWithSPF(84, spf);
    for (int prime : result) {
        cout << prime << " ";
    }
    cout << endl;

    return 0;
}

// 360 = 2^3 * 3^2 * 5
// 84 = 2 * 2 * 3 * 7
