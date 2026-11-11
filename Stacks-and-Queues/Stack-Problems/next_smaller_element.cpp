/**
 * Next smaller element
 *
 * For every element, find the first element to its right that is strictly
 * smaller. If no such element exists, the answer is -1.
 *
 * Example:
 * Input:  [4, 8, 5, 2, 25]
 * Output: [2, 5, 2, -1, -1]
 *
 * Input:  [2, 2, 3]
 * Output: [-1, -1, -1]   (equal values are not smaller)
 *
 * Brute force: scan right from every index -> O(n^2) time, O(1) extra space
 * Optimized: monotonic increasing stack of indices -> O(n) time, O(n) space
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

vector<int> nextSmallerBrute(const vector<int> &arr) {
    int n = arr.size();
    vector<int> result(n, -1);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[i]) {
                result[i] = arr[j];
                break;
            }
        }
    }

    return result;
}

vector<int> nextSmaller(const vector<int> &arr) {
    int n = arr.size();
    vector<int> result(n, -1);
    stack<int> pending;  // indices with no smaller element found yet

    for (int i = 0; i < n; i++) {
        while (!pending.empty() && arr[i] < arr[pending.top()]) {
            result[pending.top()] = arr[i];
            pending.pop();
        }

        pending.push(i);
    }

    return result;
}

void print(const vector<int> &values) {
    cout << "[";
    for (int i = 0; i < (int)values.size(); i++) {
        cout << values[i];
        if (i + 1 < (int)values.size()) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

int main() {
    vector<vector<int>> tests = {
        {4, 8, 5, 2, 25}, {2, 2, 3}, {1, 2, 3, 4}, {5}, {}};

    print(nextSmallerBrute(tests[0]));  // [2, 5, 2, -1, -1]
    print(nextSmaller(tests[0]));       // [2, 5, 2, -1, -1]

    print(nextSmaller(tests[1]));  // [-1, -1, -1]
    print(nextSmaller(tests[2]));  // [-1, -1, -1, -1]
    print(nextSmaller(tests[3]));  // [-1]
    print(nextSmaller(tests[4]));  // []

    return 0;
}

/**
 * The stack keeps a non-decreasing sequence of values from bottom to top.
 * When a smaller value arrives, it is the next smaller element for every
 * larger value on top of the stack. Each index is pushed and popped once.
 *
 * Special cases: empty array, one element, duplicates, strictly increasing
 * input, and strictly decreasing input.
 */
