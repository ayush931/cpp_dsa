/**
 * Difference array -> supports range additions in O(1)
 * Add value to [l, r] -> diff[l] += value, diff[r + 1] -= value
 * Rebuild the array with a prefix sum -> O(n)
 * Useful when there are many range updates and one final read
 *
 * 2-D difference array -> add value to a rectangle in O(1)
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> applyRangeUpdates(int n, vector<vector<int>> &updates) {
    vector<int> diff(n + 1, 0);

    for (auto &update : updates) {
        int left = update[0];
        int right = update[1];
        int value = update[2];

        diff[left] += value;
        diff[right + 1] -= value;
    }

    vector<int> arr(n, 0);
    int running = 0;

    for (int i = 0; i < n; i++) {
        running += diff[i];
        arr[i] = running;
    }

    return arr;
};

int main() {
    int n = 6;
    vector<vector<int>> updates = {{1, 3, 10}, {0, 2, 5}, {4, 5, -3}};

    vector<int> arr = applyRangeUpdates(n, updates);

    for (int value : arr) {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}

// Result -> 5 15 15 10 -3 -3
// Special cases: right + 1 == n (diff size n + 1 handles it), overlapping
// updates
