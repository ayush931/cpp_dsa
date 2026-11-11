/**
 * Sliding Window Maximum
 *
 * Given an array nums and a window size k, return the maximum of every
 * contiguous window of size k as the window moves from left to right.
 *
 * Example:
 * Input:  nums = [1, 3, -1, -3, 5, 3, 6, 7], k = 3
 * Output: [3, 3, 5, 5, 6, 7]
 *
 * Brute force: scan all k elements of every window -> O(n * k) time
 * Optimized: keep indices in a decreasing deque -> O(n) time, O(k) space
 */

#include <deque>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: recompute the maximum for every window.
vector<int> maxWindowBrute(const vector<int> &arr, int k) {
    int n = arr.size();
    vector<int> result;
    if (k <= 0 || k > n) {
        return result;
    }

    for (int start = 0; start + k <= n; start++) {
        int best = arr[start];

        for (int i = start + 1; i < start + k; i++) {
            if (arr[i] > best) {
                best = arr[i];
            }
        }

        result.push_back(best);
    }

    return result;
}

// Monotonic deque: the front is always the index of the current maximum.
vector<int> maxWindow(const vector<int> &arr, int k) {
    int n = arr.size();
    vector<int> result;
    if (k <= 0 || k > n) {
        return result;
    }

    deque<int> candidates;

    for (int i = 0; i < n; i++) {
        // Remove smaller elements: they can never be the maximum while arr[i]
        // lives.
        while (!candidates.empty() && arr[candidates.back()] <= arr[i]) {
            candidates.pop_back();
        }

        candidates.push_back(i);

        // Drop the index that just left the window.
        if (candidates.front() <= i - k) {
            candidates.pop_front();
        }

        // The first window is complete once i reaches k - 1.
        if (i >= k - 1) {
            result.push_back(arr[candidates.front()]);
        }
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
    vector<int> first = {1, 3, -1, -3, 5, 3, 6, 7};

    print(maxWindowBrute(first, 3));  // [3, 3, 5, 5, 6, 7]
    print(maxWindow(first, 3));       // [3, 3, 5, 5, 6, 7]

    print(maxWindow(first, 1));                    // [1, 3, -1, -3, 5, 3, 6, 7]
    print(maxWindow(first, 8));                    // [7]
    print(maxWindow(first, 9));                    // []
    print(maxWindow(vector<int>{}, 3));            // []
    print(maxWindow(vector<int>{4, 2, 1}, 2));     // [4, 2]
    print(maxWindow(vector<int>{2, 2, 2, 2}, 2));  // [2, 2, 2]

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n * k)
 *   deque -> O(n) because every index enters and leaves the deque at most once
 * Space complexity:
 *   brute -> O(1) extra (excluding the output)
 *   deque -> O(k)
 *
 * Special cases: k <= 0, k larger than the array, a single element, duplicate
 * values, an increasing array (the deque keeps only the newest index), and a
 * decreasing array (the front expires on every step).
 */
