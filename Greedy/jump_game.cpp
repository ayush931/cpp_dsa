/**
 * Jump Game
 *
 * You are given an integer array nums. You are initially at index 0, and each
 * element nums[i] is the maximum jump length from that position. Return true
 * if you can reach the last index, or false otherwise.
 *
 * Example:
 * Input:  [2, 3, 1, 1, 4]
 * Output: true
 *
 * Input:  [3, 2, 1, 0, 4]
 * Output: false
 *
 * Constraints: 1 <= nums.length <= 10^4, 0 <= nums[i] <= 10^5.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: DP, try every earlier position -> O(n^2)
bool canJumpBrute(vector<int> &nums) {
    int n = nums.size();
    vector<bool> reachable(n, false);
    reachable[0] = true;

    for (int i = 0; i < n; i++) {
        if (!reachable[i]) {
            continue;
        }

        for (int jump = 1; jump <= nums[i] && i + jump < n; jump++) {
            reachable[i + jump] = true;
        }
    }

    return reachable[n - 1];
}

// Greedy: track the farthest index reachable so far -> O(n), O(1) space
bool canJump(vector<int> &nums) {
    int farthest = 0;

    for (int i = 0; i < nums.size(); i++) {
        // If this index is beyond anything reachable, we are stuck.
        if (i > farthest) {
            return false;
        }

        farthest = max(farthest, i + nums[i]);

        if (farthest >= nums.size() - 1) {
            return true;
        }
    }

    return true;
}

int main() {
    vector<int> first = {2, 3, 1, 1, 4};
    vector<int> second = {3, 2, 1, 0, 4};
    vector<int> third = {0};
    vector<int> fourth = {0, 1};
    vector<int> fifth = {2, 0, 0};

    cout << canJumpBrute(first) << " " << canJump(first) << endl;    // 1 1
    cout << canJumpBrute(second) << " " << canJump(second) << endl;  // 0 0
    cout << canJump(third) << endl;   // 1 (already at the last index)
    cout << canJump(fourth) << endl;  // 0 (cannot move)
    cout << canJump(fifth) << endl;   // 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2) time, O(n) space
 *   greedy -> O(n) time, O(1) space
 *
 * Special cases: one element (true), first jump is 0 (false unless n = 1),
 * zeros in the middle, a jump that overshoots the end, all zeros, large jump
 * values.
 */
