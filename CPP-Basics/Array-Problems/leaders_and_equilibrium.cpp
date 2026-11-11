// Leaders in an array and equilibrium index
//
// Leader: an element that is strictly greater than every element to its
// right. The rightmost element is always a leader.
//
// Example:
//   {16, 17, 4, 3, 5, 2} -> leaders 17, 5, 2
//   {1, 2, 3, 4}         -> leader 4
//
// Brute force: for each element, scan everything to its right.
// O(n^2) time, O(1) extra space (not counting the answer).
//
// Optimized: scan from right to left and remember the largest value seen so
// far. An element is a leader when it is greater than that maximum. Reverse
// the collected values to restore left-to-right order.
// O(n) time, O(n) space for the answer.
//
// Equilibrium index: an index i such that the sum of elements before i equals
// the sum of elements after i. The element at i itself is not included.
//
// Example:
//   {1, 3, 5, 2, 2} -> index 2 (1 + 3 == 2 + 2)
//   {1, 2, 3}       -> -1
//
// Brute force: compute the left and right sums for every index -> O(n^2).
// Optimized: keep a running left sum and subtract from the total -> O(n).
//
// Edge cases:
//   - empty array: no leaders, equilibrium -1
//   - one element: it is a leader and index 0 is an equilibrium because both
//     sides are empty and sum to 0
//   - duplicate values: a value equal to the maximum on its right is not a
//     leader under the strict definition
//   - all equal values: only the last element is a leader
//   - negative values are fine for both problems
//   - use long long for sums when values can be large
//
// Complexity:
//   leaders brute      -> O(n^2) time, O(1) extra space
//   leaders optimized  -> O(n) time, O(n) space for the result
//   equilibrium brute  -> O(n^2) time, O(1) space
//   equilibrium fast   -> O(n) time, O(1) space

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<int> leadersBrute(const vector<int> &values) {
    vector<int> result;

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        bool isLeader = true;

        for (int j = i + 1; j < static_cast<int>(values.size()); j++) {
            if (values[j] > values[i]) {
                isLeader = false;
                break;
            }
        }

        if (isLeader) {
            result.push_back(values[i]);
        }
    }

    return result;
}

vector<int> leaders(const vector<int> &values) {
    vector<int> result;
    int maximumOnRight = -1;
    bool hasMaximum = false;

    for (int i = static_cast<int>(values.size()) - 1; i >= 0; i--) {
        if (!hasMaximum || values[i] > maximumOnRight) {
            result.push_back(values[i]);
            maximumOnRight = values[i];
            hasMaximum = true;
        }
    }

    reverse(result.begin(), result.end());
    return result;
}

int equilibriumBrute(const vector<int> &values) {
    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        long long leftSum = 0;
        long long rightSum = 0;

        for (int j = 0; j < i; j++) {
            leftSum += values[j];
        }

        for (int j = i + 1; j < static_cast<int>(values.size()); j++) {
            rightSum += values[j];
        }

        if (leftSum == rightSum) {
            return i;
        }
    }

    return -1;
}

int equilibrium(const vector<int> &values) {
    long long total = 0;

    for (int value : values) {
        total += value;
    }

    long long leftSum = 0;

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        long long rightSum = total - leftSum - values[i];

        if (leftSum == rightSum) {
            return i;
        }

        leftSum += values[i];
    }

    return -1;
}

void print(const vector<int> &values) {
    if (values.empty()) {
        cout << "(none)";
    }

    for (int value : values) {
        cout << value << " ";
    }

    cout << endl;
}

int main() {
    vector<int> first = {16, 17, 4, 3, 5, 2};
    print(leadersBrute(first));  // 17 5 2
    print(leaders(first));       // 17 5 2

    vector<int> second = {1, 2, 3, 4};
    print(leaders(second));  // 4

    vector<int> equal = {5, 5, 5};
    print(leaders(equal));  // 5

    cout << equilibriumBrute({1, 3, 5, 2, 2}) << " "
         << equilibrium({1, 3, 5, 2, 2}) << endl;  // 2 2

    cout << equilibrium({1, 2, 3}) << " " << equilibrium({1, 2, 3})
         << endl;  // -1 -1

    cout << equilibrium({5}) << " " << equilibrium({5}) << endl;  // 0 0

    cout << equilibrium({}) << " " << equilibrium({}) << endl;  // -1 -1

    return 0;
}

// Output:
// 17 5 2
// 17 5 2
// 4
// 5
// 2 2
// -1 -1
// 0 0
// -1 -1
//
// The leader scan is a suffix-maximum problem. The equilibrium scan is a
// prefix-sum problem. Both patterns appear often: keep one running value
// instead of recomputing a whole side for every index.
