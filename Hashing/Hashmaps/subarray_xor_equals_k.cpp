/**
 * Count subarrays with XOR k. XOR is its own inverse:
 * prefix[r] ^ prefix[l] = k iff prefix[l] = prefix[r] ^ k.
 * Seed empty prefix 0 once; query before updating to exclude empty subarrays.
 * Expected O(n) time (hash worst O(n^2)), O(n) space. Unsigned 32-bit values.
 */

#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

long long subarrayXor(const vector<uint32_t> &a, uint32_t k) {
    unordered_map<uint32_t, long long> frequency{{0, 1}};
    uint32_t prefix = 0;
    long long answer = 0;
    for (auto x : a) {
        prefix ^= x;
        auto it = frequency.find(prefix ^ k);
        if (it != frequency.end()) {
            answer += it->second;
        }
        ++frequency[prefix];
    }
    return answer;
}

int main() {
    assert(subarrayXor({4, 2, 2, 6, 4}, 6) == 4);
    assert(subarrayXor({0, 0, 0}, 0) == 6);
    assert(subarrayXor({}, 0) == 0);
    cout << subarrayXor({4, 2, 2, 6, 4}, 6) << '\n';  // 4
}
