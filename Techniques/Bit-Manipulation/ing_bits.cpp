/**
 * Counting Bits
 *
 * Given an integer n, return an array ans of length n + 1 such that ans[i] is
 * the number of 1 bits in the binary representation of i.
 *
 * Example:
 * Input:  n = 5
 * Output: [0, 1, 1, 2, 1, 2]
 *
 * Constraints: 0 <= n <= 10^5.
 */

#include <iostream>
#include <vector>
using namespace std;

int countBitsOfNumber(int value) {
    int count = 0;

    while (value > 0) {
        count += value & 1;
        value >>= 1;
    }

    return count;
}

// Brute force: count every number separately -> O(n log n)
vector<int> countBitsBrute(int n) {
    vector<int> result(n + 1);

    for (int i = 0; i <= n; i++) {
        result[i] = countBitsOfNumber(i);
    }

    return result;
}

// DP: dp[i] = dp[i >> 1] + (i & 1) -> O(n)
vector<int> countBits(int n) {
    vector<int> result(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        result[i] = result[i >> 1] + (i & 1);
    }

    return result;
}

// Alternative DP: remove the lowest set bit -> O(n)
vector<int> countBitsLowestBit(int n) {
    vector<int> result(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        result[i] = result[i & (i - 1)] + 1;
    }

    return result;
}

void print(vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> brute = countBitsBrute(5);
    print(brute);  // 0 1 1 2 1 2

    vector<int> optimized = countBits(5);
    print(optimized);  // 0 1 1 2 1 2

    vector<int> lowest = countBitsLowestBit(5);
    print(lowest);  // 0 1 1 2 1 2

    vector<int> zero = countBits(0);
    print(zero);  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n log n)
 *   DP -> O(n) time, O(n) space for the result
 *
 * Special cases: n = 0 (only 0), n = 1, powers of two (one set bit), numbers
 * with all bits set, negative n (not allowed), very large n (the DP avoids
 * repeated bit counting).
 */
