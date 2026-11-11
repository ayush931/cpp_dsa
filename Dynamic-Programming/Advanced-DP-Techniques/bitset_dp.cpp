/**
 * Bitset DP for subset sum
 *
 * The classic subset-sum DP uses a boolean array:
 *   dp[sum] = true if some subset adds up to sum
 *
 * The transition `dp |= dp << value` is a shift and OR. Packing 64 bits into
 * one word performs 64 transitions at once, so the complexity becomes
 * O(n * target / 64) instead of O(n * target).
 *
 * This file implements a small dynamic bitset and uses it for:
 *   - subset sum existence
 *   - counting distinct subset sums
 *   - partition into two equal-sum subsets
 *
 * Edge cases: empty input, target 0, target larger than the total sum, negative
 * numbers (not supported by this representation), duplicates, and overflow of
 * the sum.
 */

#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

class DynamicBitset {
    int words;
    vector<uint64_t> bits;

   public:
    DynamicBitset(int maximumValue) {
        words = maximumValue / 64 + 1;
        bits.assign(words, 0);
        bits[0] = 1;  // sum 0 is always reachable
    }

    void shiftOr(int shift) {
        if (shift <= 0) {
            return;
        }

        int wordShift = shift / 64;
        int bitShift = shift % 64;

        if (bitShift == 0) {
            for (int i = words - 1; i >= wordShift; i--) {
                bits[i] |= bits[i - wordShift];
            }
            return;
        }

        for (int i = words - 1; i > wordShift; i--) {
            uint64_t high = bits[i - wordShift] << bitShift;
            uint64_t low = bits[i - wordShift - 1] >> (64 - bitShift);
            bits[i] |= high | low;
        }

        bits[wordShift] |= bits[0] << bitShift;
    }

    bool test(int position) const {
        if (position < 0 || position / 64 >= words) {
            return false;
        }
        return (bits[position / 64] >> (position % 64)) & 1ULL;
    }

    int countReachable(int maximumValue) const {
        int count = 0;

        for (int value = 0; value <= maximumValue; value++) {
            if (test(value)) {
                count++;
            }
        }

        return count;
    }
};

bool subsetSum(vector<int> &nums, int target) {
    if (target < 0) {
        return false;
    }

    DynamicBitset dp(target);
    int total = 0;

    for (int value : nums) {
        if (value < 0) {
            return false;
        }
        if (value > target) {
            continue;
        }

        total += value;
        if (total > target) {
            total = target;
        }

        dp.shiftOr(value);
    }

    return dp.test(target);
}

int countDistinctSums(vector<int> &nums) {
    int total = 0;
    for (int value : nums) {
        total += value;
    }

    DynamicBitset dp(total);

    for (int value : nums) {
        dp.shiftOr(value);
    }

    return dp.countReachable(total);
}

bool canPartition(vector<int> &nums) {
    int total = 0;
    for (int value : nums) {
        total += value;
    }

    if (total % 2 != 0) {
        return false;
    }

    return subsetSum(nums, total / 2);
}

int main() {
    vector<int> nums = {3, 34, 4, 12, 5, 2};

    cout << subsetSum(nums, 9) << endl;    // 1 (4 + 5 or 3 + 2 + 4)
    cout << subsetSum(nums, 30) << endl;   // 1 (all elements)
    cout << subsetSum(nums, 100) << endl;  // 0

    cout << countDistinctSums(nums)
         << endl;  // 31 (sums 0..30 except some gaps)

    vector<int> partition = {1, 5, 11, 5};
    cout << canPartition(partition) << endl;  // 1 (1 + 5 + 5 = 11)

    vector<int> odd = {1, 2, 5};
    cout << canPartition(odd) << endl;  // 0 (total 8, but 1+2+5 cannot split)

    vector<int> empty;
    cout << subsetSum(empty, 0) << endl;  // 1
    cout << subsetSum(empty, 1) << endl;  // 0

    return 0;
}

// Special cases:
// - target 0 -> true (the empty subset)
// - target larger than the total sum -> false
// - negative values are rejected because the bitset index is non-negative
// - duplicate values are handled naturally by the shift-or transition
// - the bitset has target / 64 + 1 words
