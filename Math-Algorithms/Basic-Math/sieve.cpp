/**
 * Sieve of Eratosthenes -> finds all prime numbers up to n
 * Mark multiples of every prime starting from its square
 * Time complexity -> O(n log log n)
 * Space complexity -> O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

vector<bool> sieve(int n) {
    vector<bool> isPrime(n + 1, true);

    isPrime[0] = false;
    if (n >= 1) {
        isPrime[1] = false;
    }

    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    return isPrime;
};

// Smallest prime factor of every number up to n
vector<int> smallestPrimeFactor(int n) {
    vector<int> spf(n + 1);

    for (int i = 0; i <= n; i++) {
        spf[i] = i;
    }

    for (int i = 2; i * i <= n; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= n; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }

    return spf;
};

int main() {
    int n = 30;

    vector<bool> isPrime = sieve(n);

    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            cout << i << " ";
        }
    }
    cout << endl;

    vector<int> spf = smallestPrimeFactor(n);
    cout << spf[12] << " " << spf[25] << " " << spf[29] << endl;  // 2 5 29

    return 0;
}
