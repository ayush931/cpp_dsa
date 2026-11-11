/**
 * Target Sum: count assignments of + or - to every non-negative integer.
 * If positive subset sums to P, P-(total-P)=target, so P=(total+target)/2.
 * Count index-distinct subsets with descending 0/1-knapsack updates. A zero
 * doubles every count: +0 and -0 are distinct assignments.
 * O(n*P) time, O(P) space (pseudo-polynomial). Educational contract:
 * total <= 1000 and n <= 60, so storage is bounded and counts fit long long.
 */

#include <cassert>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>
using namespace std;

long long targetSumWays(const vector<int> &numbers, long long target) {
    if (numbers.size() > 60) {
        throw invalid_argument("at most 60 values");
    }
    for (int value : numbers) {
        if (value < 0) {
            throw invalid_argument("non-negative values required");
        }
    }
    long long total = accumulate(numbers.begin(), numbers.end(), 0LL);
    if (total > 1000) {
        throw invalid_argument("total must be <= 1000");
    }
    if (target < -total || target > total || (total + target) % 2 != 0) {
        return 0;
    }
    int positive = static_cast<int>((total + target) / 2);
    vector<long long> ways(positive + 1);
    ways[0] = 1;
    for (int value : numbers) {
        for (int sum = positive; sum >= value; --sum) {
            ways[sum] += ways[sum - value];
        }
    }
    return ways[positive];
}

int main() {
    assert(targetSumWays({1, 1, 1, 1, 1}, 3) == 5);
    assert(targetSumWays({0, 0, 1}, 1) == 4);
    assert(targetSumWays({}, 0) == 1);  // one empty assignment
    assert(targetSumWays({1}, 2) == 0);
    assert(targetSumWays({1, 2}, -3) == 1);
    cout << targetSumWays({1, 1, 1, 1, 1}, 3) << '\n';  // 5
}
