/**
 * Longest contiguous subarray with sum k, including negative values.
 * Store the EARLIEST index for each prefix sum; this maximizes the distance
 * to a later prefix with difference k. A frequency map solves COUNT instead.
 * Expected O(n) time (O(n^2) hash worst case), O(n) space. Prefix arithmetic
 * including prefix-k must fit long long. Answer 0 means no non-empty match.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int longestSubarraySumK(const vector<int> &a, long long k) {
    unordered_map<long long, int> first{{0, -1}};
    long long prefix = 0;
    int best = 0;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        prefix += a[i];
        auto found = first.find(prefix - k);
        if (found != first.end()) {
            best = max(best, i - found->second);
        }
        first.emplace(prefix, i);  // does not overwrite an earlier occurrence
    }
    return best;
}

int main() {
    assert(longestSubarraySumK({1, -1, 5, -2, 3}, 3) == 4);
    assert(longestSubarraySumK({0, 0, 0}, 0) == 3);
    assert(longestSubarraySumK({2, -2, 2, -2}, 0) == 4);
    assert(longestSubarraySumK({}, 7) == 0);
    cout << longestSubarraySumK({1, -1, 5, -2, 3}, 3) << '\n';  // 4
}
