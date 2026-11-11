/**
 * Jump search
 *
 * Jump search works on a sorted array. It jumps ahead in blocks of about
 * sqrt(n) until it finds a block whose last element is not smaller than the
 * target, then performs a linear scan inside that block.
 *
 * Why sqrt(n)? With a block size b, the jump phase uses about n / b steps and
 * the linear phase uses about b steps. The total n / b + b is minimized at
 * b = sqrt(n), giving O(sqrt(n)).
 *
 * Time        -> O(sqrt n) comparisons
 * Space       -> O(1)
 * Requirement -> the array must be sorted
 *
 * Linear search is included as the brute-force reference. The implementation
 * returns the first occurrence when duplicates are present.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int linearSearch(const vector<int> &arr, int target) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

int jumpSearch(const vector<int> &arr, int target) {
    int n = arr.size();
    if (n == 0) {
        return -1;
    }

    int blockSize = (int)sqrt((double)n);
    if (blockSize < 1) {
        blockSize = 1;
    }

    int previous = 0;
    int current = min(blockSize, n) - 1;

    // Jump until the end of the current block reaches the target.
    while (current < n && arr[current] < target) {
        previous = current + 1;

        if (previous >= n) {
            return -1;
        }

        current = min(current + blockSize, n - 1);
    }

    // Linear scan inside [previous, current].
    for (int i = previous; i <= current; i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

int main() {
    vector<int> arr = {-9, -3, 0, 1, 4, 4, 4, 8, 12, 15, 21, 30};

    vector<int> targets = {4, -9, 30, 5, -20, 40};

    for (int target : targets) {
        int jump = jumpSearch(arr, target);
        int linear = linearSearch(arr, target);

        cout << "target " << target << ": jump = " << jump
             << ", linear = " << linear
             << ", agree = " << (jump == linear ? "yes" : "no") << endl;
    }

    vector<int> empty;
    cout << "empty array: " << jumpSearch(empty, 1) << endl;
    cout << "single hit: " << jumpSearch(vector<int>{7}, 7) << endl;
    cout << "single miss: " << jumpSearch(vector<int>{7}, 3) << endl;

    return 0;
}

// Expected output:
// target 4: jump = 4, linear = 4, agree = yes
// target -9: jump = 0, linear = 0, agree = yes
// target 30: jump = 11, linear = 11, agree = yes
// target 5: jump = -1, linear = -1, agree = yes
// target -20: jump = -1, linear = -1, agree = yes
// target 40: jump = -1, linear = -1, agree = yes
// empty array: -1
// single hit: 0
// single miss: -1
//
// Special cases: empty array, one element, target at the first or last index,
// target absent, target smaller than every element, target larger than every
// element, duplicates, and negative values.
//
// Jump search is a middle ground between linear search (good for tiny arrays)
// and binary search (better asymptotically but jumps around in memory). It is
// useful when random access is cheap but binary search's access pattern is not.
