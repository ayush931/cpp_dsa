/**
 * Monotonic stack -> maintains elements in sorted order
 * Used for next greater element, previous smaller element and histogram
 * problems Every element is pushed and popped at most once, so the total time
 * is O(n)
 *
 * Monotonic queue -> maintains a deque in decreasing order
 * Used for sliding window maximum and first negative in every window
 */

#include <deque>
#include <iostream>
#include <vector>
using namespace std;

vector<int> nextGreaterBrute(vector<int> &arr) {
    int n = arr.size();
    vector<int> result(n, -1);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[i]) {
                result[i] = arr[j];
                break;
            }
        }
    }

    return result;
};

vector<int> nextGreater(vector<int> &arr) {
    int n = arr.size();
    vector<int> result(n, -1);
    vector<int> st;

    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && st.back() <= arr[i]) {
            st.pop_back();
        }

        if (!st.empty()) {
            result[i] = st.back();
        }
        st.push_back(arr[i]);
    }

    return result;
};

vector<int> firstNegativeBrute(vector<int> &arr, int k) {
    vector<int> result;

    for (int i = 0; i + k <= arr.size(); i++) {
        int found = 0;

        for (int j = i; j < i + k; j++) {
            if (arr[j] < 0) {
                found = arr[j];
                break;
            }
        }

        result.push_back(found);
    }

    return result;
};

vector<int> firstNegative(vector<int> &arr, int k) {
    deque<int> dq;
    vector<int> result;

    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < 0) {
            dq.push_back(i);
        }

        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }

        if (i >= k - 1) {
            result.push_back(dq.empty() ? 0 : arr[dq.front()]);
        }
    }

    return result;
};

int main() {
    vector<int> arr = {2, 5, 3, 7, 1, 9};

    vector<int> brute = nextGreaterBrute(arr);
    vector<int> optimized = nextGreater(arr);

    for (int value : brute) {
        cout << value << " ";
    }
    cout << endl;

    for (int value : optimized) {
        cout << value << " ";
    }
    cout << endl;

    vector<int> nums = {-1, 2, -3, 4, -5, 6};
    vector<int> windows = firstNegative(nums, 3);

    for (int value : windows) {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}

// Special cases: empty array, all equal elements, increasing/decreasing array,
// window size larger than array, no negative number in a window
