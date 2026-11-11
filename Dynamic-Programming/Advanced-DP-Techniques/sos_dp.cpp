/**
 * Sum Over Subsets (SOS) DP
 *
 * Given f[mask] for every mask in [0, 2^n), compute
 *
 *   F[mask] = sum of f[submask] for every submask of mask
 *
 * The naive method checks every pair (mask, submask), which is O(3^n). SOS DP
 * does it in O(n * 2^n) by adding one bit at a time:
 *
 *   for each bit b:
 *     for each mask:
 *       if mask has bit b:
 *         F[mask] += F[mask without bit b]
 *
 * After processing bit b, F[mask] contains the sum over all submasks that may
 * differ from mask only in bits 0..b. After every bit, that is every submask.
 *
 * The same loop with the condition reversed computes superset sums:
 *
 *   G[mask] = sum of f[supermask] for every supermask of mask
 *
 * Applications:
 *   - counting pairs with (a & b) == 0
 *   - subset-sum queries over bitmasks
 *   - OR / AND convolution (with an extra transform step)
 *   - probability that a random subset is contained in a set
 *   - bitmask DP where a state depends on all of its submasks
 */

#include <iostream>
#include <vector>
using namespace std;

vector<long long> subsetSums(vector<long long> values, int bits) {
    int size = 1 << bits;
    vector<long long> result = values;

    for (int bit = 0; bit < bits; bit++) {
        for (int mask = 0; mask < size; mask++) {
            if (mask & (1 << bit)) {
                result[mask] += result[mask ^ (1 << bit)];
            }
        }
    }

    return result;
}

vector<long long> supersetSums(vector<long long> values, int bits) {
    int size = 1 << bits;
    vector<long long> result = values;

    for (int bit = 0; bit < bits; bit++) {
        for (int mask = 0; mask < size; mask++) {
            if (!(mask & (1 << bit))) {
                result[mask] += result[mask | (1 << bit)];
            }
        }
    }

    return result;
}

// Counts unordered pairs i < j with (numbers[i] & numbers[j]) == 0.
// The SOS table counts, for every mask, how many values are submasks of it.
// For a value a, the compatible values are exactly the submasks of ~a.
long long countPairsWithAndZero(vector<int> &numbers, int bits) {
    int size = 1 << bits;
    vector<long long> frequency(size, 0);

    for (int value : numbers) {
        frequency[value]++;
    }

    vector<long long> submaskCount = subsetSums(frequency, bits);

    int full = size - 1;
    long long orderedPairs = 0;
    long long zeroCount = 0;

    for (int value : numbers) {
        orderedPairs += submaskCount[full ^ value];

        // A zero element is compatible with itself; every other element is not.
        if (value == 0) {
            zeroCount++;
        }
    }

    // Every i != j pair was counted once from each side.
    return (orderedPairs - zeroCount) / 2;
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
    // f[0] = 1, f[1] = 2, f[2] = 3, f[3] = 4
    vector<long long> values = {1, 2, 3, 4};

    vector<long long> subsets = subsetSums(values, 2);
    vector<long long> supersets = supersetSums(values, 2);

    print(subsets);    // [1, 3, 4, 10]
    print(supersets);  // [10, 6, 7, 4]

    // For mask 3, every mask is a submask: 1 + 2 + 3 + 4 = 10.
    // For mask 0, only mask 0 is a submask: 1.
    // For supersets, mask 0 is contained in every mask: 1 + 2 + 3 + 4 = 10.
    // For mask 3, only mask 3 is a superset: 4.

    vector<int> first = {1, 2, 3};
    vector<int> second = {0, 1, 2};
    vector<int> zeros = {0, 0, 0};
    vector<int> disjoint = {1, 2, 4, 7};
    vector<int> none;
    vector<int> singleZero = {0};

    cout << countPairsWithAndZero(first, 2) << endl;   // 1: (1,2)
    cout << countPairsWithAndZero(second, 2) << endl;  // 3: (0,1), (0,2), (1,2)
    cout << countPairsWithAndZero(zeros, 2) << endl;   // 3: every pair
    cout << countPairsWithAndZero(disjoint, 3)
         << endl;                                    // 3: (1,2), (1,4), (2,4)
    cout << countPairsWithAndZero(none, 3) << endl;  // 0
    cout << countPairsWithAndZero(singleZero, 2) << endl;  // 0

    // A direct O(n^2) check for the first example.
    int brute = 0;

    for (int i = 0; i < first.size(); i++) {
        for (int j = i + 1; j < first.size(); j++) {
            if ((first[i] & first[j]) == 0) {
                brute++;
            }
        }
    }

    cout << brute << endl;  // 1

    return 0;
}

/**
 * Time complexity -> O(bits * 2^bits) for both transforms; the pair count is
 * O(n) after the table is built
 * Space complexity -> O(2^bits) for the frequency and SOS arrays
 *
 * Special cases:
 *   - bits = 0: one mask, F[0] = f[0]
 *   - all values zero: every pair has AND 0, so n * (n - 1) / 2
 *   - a value with bits outside the chosen range would index out of bounds;
 *     choose bits large enough for the maximum value
 *   - duplicate values are counted separately, because they are different
 *     array elements
 *   - the pair count uses long long: n can be large and the answer can be
 *     quadratic
 *
 * The subtraction of zeroCount is easy to miss. For a non-zero value a,
 * a is not a submask of ~a, so the self-pair is not counted. For a = 0, ~0 is
 * the full mask and 0 is a submask of it, so the self-pair is counted and must
 * be removed before dividing by two.
 *
 * SOS DP is the bitmask analogue of a prefix sum. The outer loop is the
 * dimension (the bit), and the inner loop is the position (the mask), exactly
 * like a 2D prefix sum over n dimensions.
 */
