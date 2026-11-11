#include <bits/stdc++.h>
using namespace std;

int binarySearch(vector<int> &input, int target) {
    // define search space

    int lo = 0;                 // start of the search space
    int hi = input.size() - 1;  // end of the search space

    while (lo <= hi) {
        // calc midpoint for the search space
        // int mid = (lo + hi) / 2;
        int mid = lo + (hi - lo) / 2;  // optimized
        if (input[mid] == target) {
            return mid;
        } else if (input[mid] < target) {
            // discard the left of the mid
            lo = mid + 1;
        } else {
            // discard the right of the mid
            hi = mid - 1;
        }
    }

    return -1;
}

int main() {
    vector<int> input{2, 4, 5, 7, 15, 24, 45, 50};
    int target = 15;

    cout << binarySearch(input, target) << "\n";
    return 0;
}

// Time complexity -> O(log n)
// Space complexity -> O(1)