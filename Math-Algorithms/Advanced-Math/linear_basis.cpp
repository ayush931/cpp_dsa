/**
 * XOR Linear Basis
 *
 * A linear basis is a set of vectors (here: 64-bit integers) that spans the
 * same XOR space as a given set of numbers, but has at most one vector per
 * highest bit. It is Gaussian elimination over GF(2), where addition is XOR.
 *
 * Insert(x):
 *   for bit from high to low:
 *     if x has this bit:
 *       if basis[bit] is empty, store x there and stop
 *       else x ^= basis[bit]
 *   if x becomes 0, it was already in the span
 *
 * The basis can answer:
 *   - rank: the number of independent vectors
 *   - contains(x): is x representable as a subset XOR?
 *   - maxXor(): the largest subset XOR
 *   - minPositiveXor(): the smallest nonzero subset XOR
 *   - kthSmallest(k): the k-th smallest subset XOR (1-indexed)
 *
 * Complexity -> O(64) per insertion, O(64) per query, O(64^2) to build the
 * reduced basis for kthSmallest. Space -> O(64).
 *
 * The empty subset always produces XOR 0, so 0 is always representable.
 */

#include <iostream>
#include <vector>
using namespace std;

class XorBasis {
    static const int MAX_BIT = 62;  // fits in a signed 64-bit integer
    vector<long long> basis;

   public:
    XorBasis() : basis(MAX_BIT + 1, 0) {}

    bool insert(long long value) {
        if (value < 0) {
            return false;  // negative values are not supported
        }

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            if (!(value & (1LL << bit))) {
                continue;
            }

            if (basis[bit] == 0) {
                basis[bit] = value;
                return true;  // value increased the rank
            }

            value ^= basis[bit];
        }

        return false;  // value is a linear combination of existing vectors
    }

    bool contains(long long value) const {
        if (value < 0) {
            return false;  // negative values are not supported
        }

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            if (!(value & (1LL << bit))) {
                continue;
            }
            if (basis[bit] == 0) {
                return false;
            }
            value ^= basis[bit];
        }

        return true;
    }

    int rank() const {
        int result = 0;

        for (long long value : basis) {
            if (value != 0) {
                result++;
            }
        }

        return result;
    }

    long long maxXor() const {
        long long result = 0;

        for (int bit = MAX_BIT; bit >= 0; bit--) {
            if ((result ^ basis[bit]) > result) {
                result ^= basis[bit];
            }
        }

        return result;
    }

    long long minPositiveXor() const {
        for (int bit = 0; bit <= MAX_BIT; bit++) {
            if (basis[bit] != 0) {
                return basis[bit];
            }
        }

        return 0;  // only the empty subset is available
    }

    // Reduced row echelon form: each pivot bit appears in exactly one basis
    // vector. The vectors are returned in increasing pivot order.
    vector<long long> reducedBasis() const {
        vector<long long> reduced = basis;

        for (int bit = 0; bit <= MAX_BIT; bit++) {
            if (reduced[bit] == 0) {
                continue;
            }

            for (int higher = bit + 1; higher <= MAX_BIT; higher++) {
                if (reduced[higher] & (1LL << bit)) {
                    reduced[higher] ^= reduced[bit];
                }
            }
        }

        vector<long long> result;

        for (int bit = 0; bit <= MAX_BIT; bit++) {
            if (reduced[bit] != 0) {
                result.push_back(reduced[bit]);
            }
        }

        return result;
    }

    // 1-indexed: k = 1 is the empty subset (0), k = 2 is the smallest nonzero
    // subset XOR, and so on. Returns -1 if k is out of range.
    long long kthSmallest(long long k) const {
        vector<long long> reduced = reducedBasis();
        int count = reduced.size();

        if (k <= 0) {
            return -1;
        }

        // There are 2^count subset XORs. Avoid shifting by 64 or more.
        if (count < 63 && k > (1LL << count)) {
            return -1;
        }

        k--;  // convert to 0-indexed
        long long result = 0;

        for (int i = 0; i < count; i++) {
            if ((k >> i) & 1) {
                result ^= reduced[i];
            }
        }

        return result;
    }
};

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
    XorBasis basis;

    cout << basis.insert(3) << endl;  // 1
    cout << basis.insert(5) << endl;  // 1
    cout << basis.insert(6) << endl;  // 0 (6 = 3 xor 5)
    cout << basis.insert(9) << endl;  // 1

    cout << basis.rank() << endl;            // 3
    cout << basis.maxXor() << endl;          // 15 (3 xor 5 xor 9)
    cout << basis.minPositiveXor() << endl;  // 3
    cout << basis.contains(6) << endl;       // 1
    cout << basis.contains(7) << endl;       // 0
    cout << basis.contains(0) << endl;       // 1 (empty subset)

    vector<long long> all;

    for (int k = 1; k <= 8; k++) {
        all.push_back(basis.kthSmallest(k));
    }

    print(all);  // [0, 3, 5, 6, 9, 10, 12, 15]

    // Powers of two span every value from 0 to 7.
    XorBasis powers;

    for (int value : {1, 2, 4}) {
        powers.insert(value);
    }

    vector<long long> range;

    for (int k = 1; k <= 8; k++) {
        range.push_back(powers.kthSmallest(k));
    }

    print(range);                     // [0, 1, 2, 3, 4, 5, 6, 7]
    cout << powers.maxXor() << endl;  // 7

    // Duplicate values do not increase the rank.
    XorBasis duplicate;
    duplicate.insert(5);
    duplicate.insert(5);

    cout << duplicate.rank() << endl;            // 1
    cout << duplicate.maxXor() << endl;          // 5
    cout << duplicate.minPositiveXor() << endl;  // 5

    // An empty basis.
    XorBasis empty;
    cout << empty.rank() << " " << empty.maxXor() << " "
         << empty.minPositiveXor() << endl;
    // 0 0 0
    cout << empty.kthSmallest(1) << " " << empty.kthSmallest(2) << endl;
    // 0 -1

    // Large values around 2^40 and 2^41.
    XorBasis large;
    large.insert(1LL << 40);
    large.insert(1LL << 41);

    cout << large.rank() << " " << large.maxXor() << endl;
    // 2 3298534883328 (2^41 + 2^40)

    // Out-of-range k.
    cout << basis.kthSmallest(0) << " " << basis.kthSmallest(9) << endl;
    // -1 -1

    return 0;
}

/**
 * Time complexity:
 *   insert -> O(64)
 *   contains -> O(64)
 *   maxXor -> O(64)
 *   reducedBasis -> O(64^2)
 *   kthSmallest -> O(64^2) including the reduced basis
 * Space complexity -> O(64)
 *
 * Special cases:
 *   - an empty set: rank 0, max XOR 0, min positive XOR 0, only k = 1 works
 *   - duplicate values: the second insertion returns false
 *   - a value that is a combination of existing values: insertion returns
 *     false, but contains() returns true
 *   - zero: always representable by the empty subset; inserting 0 returns
 *     false
 *   - values with bit 63 set: this implementation uses signed long long and
 *     stops at bit 62; use unsigned long long and MAX_BIT = 63 for the full
 *     unsigned range
 *   - negative values are not supported: insert() and contains() return false
 *     for them, because the sign bit breaks the high-bit ordering
 *   - kthSmallest: there are 2^rank subset XORs; k must be in [1, 2^rank]
 *
 * Why maxXor works: process bits from high to low. If XORing the basis vector
 * for the current bit makes the result larger, do it. The highest differing
 * bit dominates every lower bit, so this greedy choice is optimal.
 *
 * Why kthSmallest works: after reducing the basis, each pivot bit appears in
 * exactly one vector. The subset XORs, sorted ascending, correspond exactly to
 * the binary representations of 0..2^rank-1, with bit i selecting the i-th
 * reduced vector. That is why k - 1 is used as a bitmask.
 */
