// Given a sorted array of integers 'a' in non-decreasing order, find the first
// and last position of a target element. Follow 0-based indexing.
// If the target is not present, return [-1, -1].
//
// Example:
//   a = [1, 2, 2, 2, 3, 4], target = 2 -> [1, 3]
//   a = [1, 2, 3],          target = 5 -> [-1, -1]

#include <bits/stdc++.h>
using namespace std;

// First index i such that input[i] >= target, or n if there is no such index.
int lowerBound(vector<int> &input, int target) {
    int lo = 0;
    int hi = input.size();  // half-open search space [lo, hi)

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (input[mid] >= target) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return lo;
}

// First index i such that input[i] > target, or n if there is no such index.
int upperBound(vector<int> &input, int target) {
    int lo = 0;
    int hi = input.size();

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (input[mid] > target) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return lo;
}

int main() {
    int n;
    cin >> n;

    vector<int> input(n);

    for (int i = 0; i < n; i++) {
        cin >> input[i];
    }

    int target;
    cin >> target;

    int lb = lowerBound(input, target);
    int ub = upperBound(input, target);

    // lb == n means every element is smaller than target, so input[lb] would
    // read past the end of the array.
    if (lb == n || input[lb] != target) {
        cout << -1 << " " << -1 << endl;
    } else {
        cout << lb << " " << ub - 1 << endl;
    }

    return 0;
}

// Time complexity -> O(log n)
// Space complexity -> O(1)
