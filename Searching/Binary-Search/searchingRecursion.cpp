#include <bits/stdc++.h>
using namespace std;

int binarySearchRecursion(vector<int> &input, int target, int lo, int hi) {
    if (lo > hi) {
        return -1;
    }

    // int mid = (lo + hi) / 2;
    int mid = lo + (hi - lo) / 2;  // optimized

    if (input[mid] == target) {
        return mid;
    }
    if (input[mid] < target) {
        return binarySearchRecursion(input, target, mid + 1, hi);
    } else {
        return binarySearchRecursion(input, target, lo, mid - 1);
    }
}

int main() {
    int n;
    cin >> n;

    vector<int> input;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        input.push_back(x);
    }

    int target;
    cin >> target;

    cout << binarySearchRecursion(input, target, 0, n - 1) << "\n";
    return 0;
}

// Time complexity -> O(logn)
// Space complexity -> O(logn)