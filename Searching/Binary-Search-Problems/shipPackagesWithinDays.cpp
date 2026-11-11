// Ship Packages Within D Days
//
// A conveyor belt has packages that must be shipped in the given order. Each
// day a ship loads packages from the front of the belt, as long as their total
// weight does not exceed the ship's capacity. Return the minimum capacity that
// ships all packages within d days.
//
// Example:
//   weights = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10], d = 5  -> 15
//   weights = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10], d = 1  -> 55
//   weights = [3, 2, 2, 4, 1, 4], d = 3                -> 6
//
// The capacity must be at least the heaviest package and at most the total
// weight. daysNeeded(capacity) is non-increasing, so binary search the answer.
//
// Brute force: try every capacity in that range -> O(n * totalWeight).
// Optimized: binary search the capacity -> O(n * log totalWeight).

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int daysNeeded(const vector<int> &weights, int capacity) {
    int days = 1;
    int currentLoad = 0;

    for (int weight : weights) {
        if (currentLoad + weight > capacity) {
            days++;
            currentLoad = weight;
        } else {
            currentLoad += weight;
        }
    }

    return days;
}

int shipWithinDaysBrute(const vector<int> &weights, int d) {
    if (weights.empty()) {
        return 0;
    }

    int lo = *max_element(weights.begin(), weights.end());
    int hi = accumulate(weights.begin(), weights.end(), 0);

    for (int capacity = lo; capacity <= hi; capacity++) {
        if (daysNeeded(weights, capacity) <= d) {
            return capacity;
        }
    }

    return -1;
}

int shipWithinDays(const vector<int> &weights, int d) {
    if (weights.empty()) {
        return 0;
    }
    if (d < 1) {
        return -1;
    }

    int lo = *max_element(weights.begin(), weights.end());
    int hi = accumulate(weights.begin(), weights.end(), 0);

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        if (daysNeeded(weights, mid) <= d) {
            hi = mid;  // mid works, try a smaller capacity
        } else {
            lo = mid + 1;  // mid is too small
        }
    }

    return lo;
}

int main() {
    vector<int> weights = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << shipWithinDays(weights, 5) << endl;       // 15
    cout << shipWithinDaysBrute(weights, 5) << endl;  // 15
    cout << shipWithinDays(weights, 1) << endl;       // 55

    vector<int> second = {3, 2, 2, 4, 1, 4};
    cout << shipWithinDays(second, 3) << endl;  // 6
    cout << shipWithinDays(second, 2) << endl;  // 10

    vector<int> one = {10};
    cout << shipWithinDays(one, 1) << endl;  // 10

    vector<int> empty;
    cout << shipWithinDays(empty, 3) << endl;  // 0

    return 0;
}

// Time complexity -> O(n log totalWeight) time, O(1) extra space
// Brute force -> O(n * totalWeight) time, O(1) extra space
