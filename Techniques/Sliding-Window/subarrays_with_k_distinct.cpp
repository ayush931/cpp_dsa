/**
 * Count subarrays containing EXACTLY k distinct integers.
 * Exactly(k) = atMost(k) - atMost(k-1). Exact equality is not itself a
 * monotone window condition. For atMost, all starts in [left,right] are valid,
 * contributing right-left+1. Expected O(n) time, O(n) space (hash worst
 * O(n^2)). Same reduction applies to counting binary subarrays with a given sum
 * or subarrays containing exactly k odd numbers, using their respective
 * budgets.
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

long long atMostDistinct(const vector<int> &a, int k) {
    if (k < 0) {
        return 0;
    }
    unordered_map<int, int> frequency;
    int left = 0;
    long long answer = 0;
    for (int right = 0; right < static_cast<int>(a.size()); ++right) {
        ++frequency[a[right]];
        while (static_cast<int>(frequency.size()) > k) {
            if (--frequency[a[left]] == 0) {
                frequency.erase(a[left]);
            }
            ++left;
        }
        answer += right - left + 1;
    }
    return answer;
}

long long exactlyKDistinct(const vector<int> &a, int k) {
    if (k <= 0) {
        return 0;  // only count non-empty subarrays
    }
    return atMostDistinct(a, k) - atMostDistinct(a, k - 1);
}

int main() {
    assert(exactlyKDistinct({1, 2, 1, 2, 3}, 2) == 7);
    assert(exactlyKDistinct({1, 1, 1}, 1) == 6);
    assert(exactlyKDistinct({1, 2}, 0) == 0);
    assert(exactlyKDistinct({}, 1) == 0);
    cout << exactlyKDistinct({1, 2, 1, 2, 3}, 2) << '\n';  // 7
}
