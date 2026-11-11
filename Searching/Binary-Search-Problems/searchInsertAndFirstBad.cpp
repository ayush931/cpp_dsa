// Two binary search variants that both reduce to lower_bound.
//
// 1. Search insert position
//    Given a sorted array of distinct integers and a target, return the index
//    where the target is found. If it is absent, return the index where it
//    would be inserted to keep the array sorted.
//      [1, 3, 5, 6], target 5 -> 2
//      [1, 3, 5, 6], target 2 -> 1
//      [1, 3, 5, 6], target 7 -> 4
//
// 2. First bad version
//    Versions 1..n are good up to some point and bad after it. The predicate
//    isBad(version) is monotone: false, false, ..., true, true. Find the first
//    bad version with O(log n) calls.
//      n = 10, first bad = 4 -> 4
//
// Brute force: scan every index / version -> O(n).
// Optimized: binary search the first index satisfying the predicate -> O(log
// n).

#include <iostream>
#include <vector>
using namespace std;

int searchInsertBrute(const vector<int> &nums, int target) {
    for (int i = 0; i < (int)nums.size(); i++) {
        if (nums[i] >= target) {
            return i;
        }
    }
    return nums.size();
}

int searchInsert(const vector<int> &nums, int target) {
    int lo = 0;
    int hi = nums.size();

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        if (nums[mid] >= target) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return lo;
}

// The predicate used by the demo. In a real API this would be provided.
int firstBad = 1;

bool isBadVersion(int version) {
    return version >= firstBad;
}

int firstBadVersionBrute(int n, bool (*isBad)(int)) {
    for (int version = 1; version <= n; version++) {
        if (isBad(version)) {
            return version;
        }
    }
    return -1;
}

int firstBadVersion(int n, bool (*isBad)(int)) {
    if (n < 1 || !isBad(n)) {
        return -1;  // no bad version exists
    }

    int lo = 1;
    int hi = n;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;  // avoids overflow of (lo + hi)

        if (isBad(mid)) {
            hi = mid;  // mid is bad, the first bad is at mid or earlier
        } else {
            lo = mid + 1;  // mid is good, the first bad is later
        }
    }

    return lo;
}

int main() {
    vector<int> nums = {1, 3, 5, 6};

    cout << searchInsert(nums, 5) << endl;  // 2
    cout << searchInsert(nums, 2) << endl;  // 1
    cout << searchInsert(nums, 7) << endl;  // 4
    cout << searchInsert(nums, 0) << endl;  // 0

    cout << searchInsertBrute(nums, 5) << " " << searchInsertBrute(nums, 2)
         << " " << searchInsertBrute(nums, 7) << " "
         << searchInsertBrute(nums, 0) << endl;
    // 2 1 4 0

    firstBad = 4;
    cout << firstBadVersion(10, isBadVersion) << endl;       // 4
    cout << firstBadVersionBrute(10, isBadVersion) << endl;  // 4

    firstBad = 1;
    cout << firstBadVersion(10, isBadVersion) << endl;  // 1

    firstBad = 10;
    cout << firstBadVersion(10, isBadVersion) << endl;  // 10

    vector<int> empty;
    cout << searchInsert(empty, 3) << endl;  // 0

    return 0;
}

// Time complexity -> O(log n) for both optimized functions
// Space complexity -> O(1)
