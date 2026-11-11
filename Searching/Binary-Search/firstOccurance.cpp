// Find the first occurance of a given element x, given that the given array is
// sorted. If no occurance of x is found then return -1.

#include <bits/stdc++.h>
using namespace std;

int firstOccurance(vector<int> &input, int target) {
    int lo = 0;
    int hi = input.size() - 1;
    int ans = -1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (input[mid] == target) {
            ans = mid;
            hi = mid - 1;
        } else if (input[mid] > target) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    return ans;
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

    cout << firstOccurance(input, target) << endl;

    return 0;
}

// Time complexity -> O(logn)
// Space complexity -> O(1)