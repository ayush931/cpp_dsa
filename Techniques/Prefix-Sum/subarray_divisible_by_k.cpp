/**
 * Count non-empty subarrays whose sum is divisible by positive k.
 * Equal prefix remainders delimit a divisible sum. Seed remainder 0 once
 * for the empty prefix, and query BEFORE inserting the current prefix.
 * Normalize C++ negative remainders. Expected O(n) time, O(min(n,k)) space;
 * unordered_map has O(n^2) worst-case total time. Counts fit in long long.
 */

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
using namespace std;

long long subarraysDivByK(const vector<int> &a, int k) {
    if (k <= 0) {
        throw invalid_argument("k must be positive");
    }
    unordered_map<int, long long> frequency{{0, 1}};
    long long answer = 0;
    int remainder = 0;
    for (int x : a) {
        remainder =
            static_cast<int>((static_cast<long long>(remainder) + x) % k);
        if (remainder < 0) {
            remainder += k;
        }
        answer += frequency[remainder];
        ++frequency[remainder];
    }
    return answer;
}

int main() {
    assert(subarraysDivByK({4, 5, 0, -2, -3, 1}, 5) == 7);
    assert(subarraysDivByK({-1, 1}, 2) == 1);
    assert(subarraysDivByK({0, 0}, 3) == 3);
    assert(subarraysDivByK({}, 1) == 0);
    cout << subarraysDivByK({4, 5, 0, -2, -3, 1}, 5) << '\n';  // 7
}
