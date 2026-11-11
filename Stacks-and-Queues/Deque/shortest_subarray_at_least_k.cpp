/**
 * Shortest non-empty subarray with sum >= k, allowing NEGATIVE numbers.
 * Prefix sums + deque of indices with strictly increasing prefix values.
 * Front: pop qualifying starts; a later end cannot improve their lengths.
 * Back: discard dominated starts; a later, smaller prefix is always better.
 * Each index enters/exits once: O(n) time, O(n) space. Return -1 if absent.
 * Prefix sums and their differences must fit long long.
 */

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

int shortestSubarray(const vector<int> &numbers, long long k) {
    int n = static_cast<int>(numbers.size()), best = n + 1;
    vector<long long> prefix(n + 1);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + numbers[i];
    }
    deque<int> candidates;
    for (int end = 0; end <= n; ++end) {
        while (!candidates.empty() &&
               prefix[end] - prefix[candidates.front()] >= k) {
            best = min(best, end - candidates.front());
            candidates.pop_front();
        }
        while (!candidates.empty() &&
               prefix[candidates.back()] >= prefix[end]) {
            candidates.pop_back();
        }
        candidates.push_back(
            end);  // insert after queries: excludes empty subarray
    }
    return best == n + 1 ? -1 : best;
}

int main() {
    assert(shortestSubarray({2, -1, 2}, 3) == 3);
    assert(shortestSubarray({84, -37, 32, 40, 95}, 167) == 3);
    assert(shortestSubarray({1, 2}, 4) == -1);
    assert(shortestSubarray({-1}, -1) == 1);
    assert(shortestSubarray({}, 0) == -1);
    cout << shortestSubarray({2, -1, 2}, 3) << '\n';  // 3
}
