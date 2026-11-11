/**
 * Jump Game II: minimum jumps to the last index; -1 if unreachable.
 * Non-negative jump lengths. Treat reachable index ranges as BFS layers:
 * scanning one layer determines the farthest boundary of the next layer.
 * Commit a jump only when the current boundary is reached.
 * O(n) time, O(1) space. Empty/single-element input needs zero jumps.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int minimumJumps(const vector<int> &numbers) {
    int n = static_cast<int>(numbers.size());
    int jumps = 0, boundary = 0, farthest = 0;
    for (int index = 0; index < n - 1; ++index) {
        long long reach = static_cast<long long>(index) + numbers[index];
        farthest = static_cast<int>(
            max<long long>(farthest, min<long long>(n - 1, reach)));
        if (index == boundary) {
            if (farthest == boundary) {
                return -1;
            }
            ++jumps;
            boundary = farthest;
            if (boundary == n - 1) {
                return jumps;
            }
        }
    }
    return jumps;
}

int main() {
    assert(minimumJumps({2, 3, 1, 1, 4}) == 2);
    assert(minimumJumps({3, 2, 1, 0, 4}) == -1);
    assert(minimumJumps({0}) == 0);
    assert(minimumJumps({}) == 0);
    assert(minimumJumps({1, 1, 1}) == 2);
    cout << minimumJumps({2, 3, 1, 1, 4}) << '\n';  // 2
}
