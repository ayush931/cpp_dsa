/**
 * 4Sum: unique value quadruplets, using four different indices.
 * Sort a copy; fix two values and use opposing pointers on the remaining
 * suffix. Skip duplicates at every level. O(n^3) time, O(n) copy plus output.
 * Promote before adding: four int values can overflow a 32-bit sum.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

vector<array<int, 4>> fourSum(vector<int> a, long long target) {
    sort(a.begin(), a.end());
    vector<array<int, 4>> result;
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n - 3; ++i) {
        if (i && a[i] == a[i - 1]) {
            continue;
        }
        for (int j = i + 1; j < n - 2; ++j) {
            if (j > i + 1 && a[j] == a[j - 1]) {
                continue;
            }
            int left = j + 1, right = n - 1;
            while (left < right) {
                long long sum =
                    static_cast<long long>(a[i]) + a[j] + a[left] + a[right];
                if (sum < target) {
                    ++left;
                } else if (sum > target) {
                    --right;
                } else {
                    result.push_back({a[i], a[j], a[left], a[right]});
                    int lv = a[left], rv = a[right];
                    while (left < right && a[left] == lv) {
                        ++left;
                    }
                    while (left < right && a[right] == rv) {
                        --right;
                    }
                }
            }
        }
    }
    return result;
}

int main() {
    assert(fourSum({1, 0, -1, 0, -2, 2}, 0).size() == 3);
    assert(fourSum({2, 2, 2, 2, 2}, 8).size() == 1);
    assert(fourSum({INT_MAX, INT_MAX, INT_MAX, INT_MAX}, -4).empty());
    assert(fourSum({}, 0).empty());
    cout << fourSum({1, 0, -1, 0, -2, 2}, 0).size() << '\n';  // 3
}
