/**
 * HyperLogLog
 *
 * HyperLogLog estimates the number of distinct elements in a stream using a
 * small, fixed amount of memory. It is used for unique visitors, distinct
 * search queries, and cardinality dashboards.
 *
 * Idea:
 *   - hash every element to 64 bits
 *   - use the first p bits as a register index (m = 2^p registers)
 *   - in the remaining bits, count the position of the first 1 (the rank)
 *   - each register keeps the maximum rank it has seen
 *   - the harmonic mean of 2^(-register) gives the estimate
 *
 * Brute force -> store every distinct value in a hash set -> O(n) memory
 * HyperLogLog -> O(2^p) memory, independent of n, with ~1.04 / sqrt(m) error
 *
 * This demo uses p = 10 (1024 registers, about 3% error).
 */

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

class HyperLogLog {
    int precision;
    int registerCount;
    vector<int> registers;

    uint64_t hashValue(const string &key) {
        // FNV-1a 64-bit, then a full avalanche. Without the finalizer,
        // structured keys such as "user:1", "user:2" share most high bits, so
        // they land in the same registers and the estimate collapses.
        uint64_t hash = 14695981039346656037ULL;

        for (unsigned char ch : key) {
            hash ^= ch;
            hash *= 1099511628211ULL;
        }

        hash ^= hash >> 33;
        hash *= 0xff51afd7ed558ccdULL;
        hash ^= hash >> 33;
        hash *= 0xc4ceb9fe1a85ec53ULL;
        hash ^= hash >> 33;

        return hash;
    }

   public:
    HyperLogLog(int p = 10) {
        precision = p;
        registerCount = 1 << p;
        registers.assign(registerCount, 0);
    }

    void add(const string &key) {
        uint64_t hash = hashValue(key);

        int index = (int)(hash >> (64 - precision));

        // Remove the index bits, then count leading zeros in the rest.
        uint64_t remaining = hash << precision;
        int rank;

        if (remaining == 0) {
            rank = 64 - precision + 1;
        } else {
            rank = __builtin_clzll(remaining) + 1;
        }

        if (rank > registers[index]) {
            registers[index] = rank;
        }
    }

    double estimate() {
        double alpha = 0.7213 / (1.0 + 1.079 / registerCount);
        double harmonicSum = 0.0;
        int zeroRegisters = 0;

        for (int value : registers) {
            harmonicSum += pow(2.0, -value);

            if (value == 0) {
                zeroRegisters++;
            }
        }

        double rawEstimate =
            alpha * registerCount * registerCount / harmonicSum;

        // Small-range correction: linear counting is more accurate when many
        // registers are still zero.
        if (rawEstimate <= 2.5 * registerCount && zeroRegisters > 0) {
            return registerCount * log((double)registerCount / zeroRegisters);
        }

        return rawEstimate;
    }

    int registerCountValue() {
        return registerCount;
    }
};

int main() {
    const int distinctCount = 20000;

    HyperLogLog hll(10);
    unordered_set<string> exact;

    for (int i = 0; i < distinctCount; i++) {
        string value = "user:" + to_string(i);
        hll.add(value);
        exact.insert(value);
    }

    double estimate = hll.estimate();
    double error = 100.0 * (estimate - exact.size()) / exact.size();

    cout << fixed << setprecision(2);
    cout << "exact: " << exact.size() << endl;
    cout << "hll:   " << estimate << endl;
    cout << "error: " << error << "%" << endl;

    HyperLogLog small(10);
    for (int i = 0; i < 100; i++) {
        small.add("item:" + to_string(i));
    }

    cout << "small exact: 100" << endl;
    cout << "small hll:   " << small.estimate() << endl;

    HyperLogLog empty(10);
    cout << "empty: " << empty.estimate() << endl;  // 0

    return 0;
}

/**
 * Time complexity -> O(1) per element, O(m) for the estimate
 * Space complexity -> O(m), where m = 2^p registers
 *
 * Error -> about 1.04 / sqrt(m)
 *
 * Special cases: empty stream (0), very small cardinalities (linear
 * counting), very large cardinalities (the raw estimate is fine), hash
 * collisions (different values can share a hash), a poor hash function
 * (correlated inputs produce biased estimates), p too small (large error).
 *
 * HyperLogLog is approximate: it cannot list the distinct values, only count
 * them. Use a hash set when exact answers or the actual values are needed.
 */
