/**
 * Russian Doll Envelopes: longest nesting chain, strictly increasing width
 * AND height; rotation disallowed. Sort width ascending, equal-width height
 * DESCENDING, then compute strict LIS on heights with lower_bound.
 * Descending ties prevent selecting two envelopes of equal width.
 * O(n log n) time, O(n) space including the by-value copy.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int maxEnvelopes(vector<array<int, 2>> envelopes) {
    sort(envelopes.begin(), envelopes.end(), [](const auto &a, const auto &b) {
        if (a[0] != b[0]) {
            return a[0] < b[0];
        }
        return a[1] > b[1];
    });
    vector<int> tails;
    for (const auto &envelope : envelopes) {
        auto position = lower_bound(tails.begin(), tails.end(), envelope[1]);
        if (position == tails.end()) {
            tails.push_back(envelope[1]);
        } else {
            *position = envelope[1];
        }
    }
    return static_cast<int>(tails.size());
}

int main() {
    assert(maxEnvelopes({{5, 4}, {6, 4}, {6, 7}, {2, 3}}) == 3);
    assert(maxEnvelopes({{1, 1}, {1, 2}, {1, 3}}) == 1);
    assert(maxEnvelopes({{1, 1}, {1, 1}}) == 1);
    assert(maxEnvelopes({}) == 0);
    cout << maxEnvelopes({{5, 4}, {6, 4}, {6, 7}, {2, 3}}) << '\n';  // 3
}
