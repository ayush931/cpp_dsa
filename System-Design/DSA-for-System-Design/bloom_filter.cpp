/**
 * Bloom filter -> probabilistic set membership
 * add(x) -> set k bits chosen by k hash functions
 * contains(x) -> all k bits must be set
 *
 * No false negatives -> if the filter says "no", the element is definitely
 * absent False positives possible -> if the filter says "yes", the element may
 * be absent Cannot delete elements (use a counting Bloom filter for deletes)
 *
 * Space -> O(m) bits, independent of the element size
 * Insert / lookup -> O(k)
 * False positive rate ~ (1 - e^(-k * n / m))^k
 * Optimal k ~ (m / n) * ln(2)
 */

#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class BloomFilter {
    int bitCount;
    int hashCount;
    vector<bool> bits;

    unsigned int hashValue(string &key, int seed) {
        hash<string> hasher;
        return hasher(key + "#" + to_string(seed));
    }

   public:
    BloomFilter(int m, int k) {
        if (m <= 0 || k <= 0) {
            throw invalid_argument("positive bit and hash counts required");
        }
        bitCount = m;
        hashCount = k;
        bits.assign(m, false);
    }

    void add(string key) {
        for (int i = 0; i < hashCount; i++) {
            bits[hashValue(key, i) % bitCount] = true;
        }
    }

    bool contains(string key) {
        for (int i = 0; i < hashCount; i++) {
            if (!bits[hashValue(key, i) % bitCount]) {
                return false;
            }
        }

        return true;
    }

    double falsePositiveRate(int inserted) {
        if (inserted < 0) {
            throw invalid_argument("negative insertion count");
        }
        double exponent = -(double)hashCount * inserted / bitCount;
        double value = 1 - exp(exponent);
        return pow(value, hashCount);
    }
};

int main() {
    BloomFilter filter(1000, 5);

    vector<string> present = {"apple", "banana", "cherry"};
    vector<string> absent = {"grape", "mango", "kiwi", "melon"};

    for (string &word : present) {
        filter.add(word);
    }

    for (string &word : present) {
        cout << word << " " << filter.contains(word) << endl;  // all 1
    }

    for (string &word : absent) {
        cout << word << " " << filter.contains(word) << endl;  // usually 0
    }

    cout << filter.falsePositiveRate(3) << endl;

    return 0;
}

// Special cases: empty filter (everything absent), all bits set
// (everything reported present), m or k equal to 0, very large n
// Use cases: cache penetration protection, database key existence checks,
// spam filtering, avoiding expensive disk lookups
