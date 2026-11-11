/**
 * Maximum circular subarray sum. Non-empty subarray, at most n elements.
 * A wrapping answer is total - minimum non-wrapping subarray; otherwise
 * ordinary Kadane suffices. All-negative input must not choose an empty wrap.
 * O(n) time, O(1) space. Empty input has no answer (nullopt).
 * Sums are assumed to fit in long long.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <vector>
using namespace std;

optional<long long> maxCircularSum(const vector<int> &numbers) {
    if (numbers.empty()) {
        return nullopt;
    }
    long long total = numbers[0];
    long long maximumEndingHere = numbers[0], minimumEndingHere = numbers[0];
    long long maximumSum = numbers[0], minimumSum = numbers[0];
    for (size_t i = 1; i < numbers.size(); ++i) {
        maximumEndingHere =
            max<long long>(numbers[i], maximumEndingHere + numbers[i]);
        minimumEndingHere =
            min<long long>(numbers[i], minimumEndingHere + numbers[i]);
        maximumSum = max(maximumSum, maximumEndingHere);
        minimumSum = min(minimumSum, minimumEndingHere);
        total += numbers[i];
    }
    return maximumSum < 0 ? maximumSum : max(maximumSum, total - minimumSum);
}

int main() {
    assert(maxCircularSum({5, -3, 5}) == 10);
    assert(maxCircularSum({-3, -2, -5}) == -2);
    assert(maxCircularSum({0}) == 0);
    assert(maxCircularSum({3, -1, 2, -1}) == 4);
    assert(!maxCircularSum({}));
    cout << *maxCircularSum({5, -3, 5}) << '\n';  // 10
}
