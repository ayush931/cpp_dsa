/**
 * Candy: every child gets >=1 candy; higher rating than a neighbor requires
 * strictly more candy than that neighbor. Equal ratings impose no inequality.
 * Left pass satisfies left neighbors. Right pass takes max with the right
 * requirement, preserving the first pass. O(n) time, O(n) space.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

long long minimumCandy(const vector<int> &ratings) {
    int n = static_cast<int>(ratings.size());
    vector<int> candy(n, 1);
    for (int i = 1; i < n; ++i) {
        if (ratings[i] > ratings[i - 1]) {
            candy[i] = candy[i - 1] + 1;
        }
    }
    for (int i = n - 2; i >= 0; --i) {
        if (ratings[i] > ratings[i + 1]) {
            candy[i] = max(candy[i], candy[i + 1] + 1);
        }
    }
    return accumulate(candy.begin(), candy.end(), 0LL);
}

int main() {
    assert(minimumCandy({1, 0, 2}) == 5);
    assert(minimumCandy({1, 2, 2}) == 4);
    assert(minimumCandy({1, 3, 4, 5, 2}) == 11);
    assert(minimumCandy({}) == 0);
    cout << minimumCandy({1, 0, 2}) << '\n';  // 5
}
