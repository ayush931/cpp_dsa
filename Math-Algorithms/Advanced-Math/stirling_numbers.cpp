/**
 * Stirling Numbers
 *
 * Stirling numbers of the second kind S(n, k) count the ways to partition a
 * set of n labelled elements into exactly k non-empty unlabelled blocks.
 *
 *   S(n, k) = S(n - 1, k - 1) + k * S(n - 1, k)
 *
 * Stirling numbers of the first kind c(n, k) count permutations of n elements
 * with exactly k cycles.
 *
 *   c(n, k) = c(n - 1, k - 1) + (n - 1) * c(n - 1, k)
 *
 * The signed first-kind numbers s(n, k) = (-1)^(n-k) c(n, k) are the
 * coefficients that express falling factorials in powers of x.
 *
 * Bell numbers B(n) = sum_k S(n, k) count all set partitions.
 *
 * Brute-force enumerations are included for small n and verify the tables.
 * This long long demonstration accepts n <= 20 to avoid signed overflow.
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>
using namespace std;

long long stirlingSecondKind(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return 0;
    }
    if (n > 20) {
        throw invalid_argument(
            "use BigInteger or modular arithmetic for n > 20");
    }

    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    dp[0][0] = 1;

    for (int elements = 1; elements <= n; elements++) {
        for (int blocks = 1; blocks <= elements; blocks++) {
            dp[elements][blocks] = dp[elements - 1][blocks - 1] +
                                   blocks * dp[elements - 1][blocks];
        }
    }

    return dp[n][k];
}

long long stirlingFirstKindUnsigned(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return 0;
    }
    if (n > 20) {
        throw invalid_argument(
            "use BigInteger or modular arithmetic for n > 20");
    }

    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    dp[0][0] = 1;

    for (int elements = 1; elements <= n; elements++) {
        for (int cycles = 1; cycles <= elements; cycles++) {
            dp[elements][cycles] = dp[elements - 1][cycles - 1] +
                                   (elements - 1) * dp[elements - 1][cycles];
        }
    }

    return dp[n][k];
}

long long stirlingFirstKindSigned(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return 0;
    }
    long long value = stirlingFirstKindUnsigned(n, k);
    if ((n - k) % 2 == 1) {
        value = -value;
    }
    return value;
}

long long bellNumber(int n) {
    long long total = 0;

    for (int k = 0; k <= n; k++) {
        total += stirlingSecondKind(n, k);
    }

    return total;
}

void countPartitions(int index, int n, int targetBlocks, int blocks,
                     long long &count) {
    if (index == n) {
        if (blocks == targetBlocks) {
            count++;
        }
        return;
    }

    for (int block = 0; block < blocks; block++) {
        countPartitions(index + 1, n, targetBlocks, blocks, count);
    }

    countPartitions(index + 1, n, targetBlocks, blocks + 1, count);
}

long long bruteSecondKind(int n, int k) {
    long long count = 0;
    countPartitions(0, n, k, 0, count);
    return count;
}

long long bruteFirstKind(int n, int k) {
    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 0);

    long long count = 0;

    do {
        vector<bool> visited(n, false);
        int cycles = 0;

        for (int start = 0; start < n; start++) {
            if (visited[start]) {
                continue;
            }

            cycles++;
            int current = start;

            while (!visited[current]) {
                visited[current] = true;
                current = permutation[current];
            }
        }

        if (cycles == k) {
            count++;
        }
    } while (next_permutation(permutation.begin(), permutation.end()));

    return count;
}

void printSecondKindTable(int n) {
    for (int row = 0; row <= n; row++) {
        cout << "n=" << row << ": ";

        for (int k = 0; k <= row; k++) {
            if (k > 0) {
                cout << " ";
            }
            cout << stirlingSecondKind(row, k);
        }

        cout << endl;
    }
}

void printFirstKindTable(int n) {
    for (int row = 0; row <= n; row++) {
        cout << "n=" << row << ": ";

        for (int k = 0; k <= row; k++) {
            if (k > 0) {
                cout << " ";
            }
            cout << stirlingFirstKindUnsigned(row, k);
        }

        cout << endl;
    }
}

int main() {
    cout << "Stirling numbers of the second kind:" << endl;
    printSecondKindTable(7);

    cout << "Stirling numbers of the first kind (unsigned):" << endl;
    printFirstKindTable(7);

    cout << "Bell numbers: ";

    for (int n = 0; n <= 8; n++) {
        if (n > 0) {
            cout << " ";
        }
        cout << bellNumber(n);
    }

    cout << endl;

    bool secondKindMatches = true;
    bool firstKindMatches = true;

    for (int n = 0; n <= 7; n++) {
        for (int k = 0; k <= n; k++) {
            if (stirlingSecondKind(n, k) != bruteSecondKind(n, k)) {
                secondKindMatches = false;
            }

            if (stirlingFirstKindUnsigned(n, k) != bruteFirstKind(n, k)) {
                firstKindMatches = false;
            }
        }
    }

    cout << "second kind matches brute force: "
         << (secondKindMatches ? "yes" : "no") << endl;
    cout << "first kind matches brute force: "
         << (firstKindMatches ? "yes" : "no") << endl;

    cout << "signed first kind s(5, 2): " << stirlingFirstKindSigned(5, 2)
         << " (unsigned " << stirlingFirstKindUnsigned(5, 2) << ")" << endl;

    return 0;
}

// Expected output (abridged):
// Stirling numbers of the second kind:
// n=0: 1
// n=1: 0 1
// n=2: 0 1 1
// n=3: 0 1 3 1
// n=4: 0 1 7 6 1
// n=5: 0 1 15 25 10 1
// ...
// Bell numbers: 1 1 2 5 15 52 203 877 4140
// second kind matches brute force: yes
// first kind matches brute force: yes
// signed first kind s(5, 2): -50 (unsigned 50)
//
// Special cases: n = 0, k = 0, k > n, and the signed first-kind relation
// s(n, k) = (-1)^(n-k) c(n, k).
