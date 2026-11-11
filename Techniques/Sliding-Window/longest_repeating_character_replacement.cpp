/**
 * Longest substring made of one repeated byte after <= k replacements.
 * Window validity: length - largest frequency <= k. Recompute the frequency
 * maximum on shrink so every retained window is actually valid (an easier
 * invariant than the alternative stale-maximum optimization).
 * O(256*n) = O(n) time for byte strings; O(256) space. Not Unicode code points.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

int characterReplacement(const string &s, int k) {
    if (k < 0) {
        throw invalid_argument("k must be non-negative");
    }
    array<int, 256> count{};
    int left = 0, best = 0;
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        ++count[static_cast<unsigned char>(s[right])];
        while (right - left + 1 - *max_element(count.begin(), count.end()) >
               k) {
            --count[static_cast<unsigned char>(s[left++])];
        }
        best = max(best, right - left + 1);
    }
    return best;
}

int main() {
    assert(characterReplacement("AABABBA", 1) == 4);
    assert(characterReplacement("ABAB", 2) == 4);
    assert(characterReplacement("AAAB", 0) == 3);
    assert(characterReplacement("", 3) == 0);
    cout << characterReplacement("AABABBA", 1) << '\n';  // 4
}
