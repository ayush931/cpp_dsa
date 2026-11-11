/**
 * Minimum length of a subarray with sum >= positive target.
 * Contract: all numbers are non-negative. Shrinking cannot increase the sum,
 * so a moving left boundary works. Negative numbers break this invariant:
 * see Stacks-and-Queues/Deque/shortest_subarray_at_least_k.cpp for that case.
 * O(n) time, O(1) space; return 0 when no qualifying subarray exists.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

int minSubarrayLength(const vector<int> &a, long long target) {
    if (target <= 0) {
        throw invalid_argument("target must be positive");
    }
    if (any_of(a.begin(), a.end(), [](int x) { return x < 0; })) {
        throw invalid_argument("window requires non-negative numbers");
    }
    int left = 0, best = static_cast<int>(a.size()) + 1;
    long long sum = 0;
    for (int right = 0; right < static_cast<int>(a.size()); ++right) {
        sum += a[right];
        while (sum >= target) {
            best = min(best, right - left + 1);
            sum -= a[left++];
        }
    }
    return best > static_cast<int>(a.size()) ? 0 : best;
}

int main() {
    assert(minSubarrayLength({2, 3, 1, 2, 4, 3}, 7) == 2);
    assert(minSubarrayLength({0, 0, 5}, 5) == 1);
    assert(minSubarrayLength({1, 1}, 3) == 0);
    assert(minSubarrayLength({}, 1) == 0);
    cout << minSubarrayLength({2, 3, 1, 2, 4, 3}, 7) << '\n';  // 2
}
