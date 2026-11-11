/**
 * Sum the minimum of every non-empty subarray, modulo 1,000,000,007.
 * Contribution pattern: index i owns (i-left)*(right-i) subarrays, where
 * left is the previous STRICTLY smaller index and right the next smaller
 * OR EQUAL index. Asymmetric equality assigns duplicate minima exactly once.
 * O(n) time, O(n) space. Contract: non-negative int values.
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

long long sumSubarrayMinimums(const vector<int> &numbers) {
    constexpr long long MOD = 1000000007;
    int n = static_cast<int>(numbers.size());
    vector<int> increasing;  // indices; values strictly increasing
    long long answer = 0;
    for (int right = 0; right <= n; ++right) {
        // right==n flushes the stack; short-circuit avoids numbers[n].
        while (!increasing.empty() &&
               (right == n || numbers[increasing.back()] >= numbers[right])) {
            int index = increasing.back();
            increasing.pop_back();
            int left = increasing.empty() ? -1 : increasing.back();
            long long choices =
                static_cast<long long>(index - left) * (right - index);
            answer = (answer + choices % MOD * numbers[index]) % MOD;
        }
        if (right < n) {
            increasing.push_back(right);
        }
    }
    return answer;
}

int main() {
    assert(sumSubarrayMinimums({3, 1, 2, 4}) == 17);
    assert(sumSubarrayMinimums({2, 2}) == 6);
    assert(sumSubarrayMinimums({0, 0}) == 0);
    assert(sumSubarrayMinimums({}) == 0);
    cout << sumSubarrayMinimums({3, 1, 2, 4}) << '\n';  // 17
}
