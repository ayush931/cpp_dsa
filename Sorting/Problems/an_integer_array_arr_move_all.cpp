// Given an integer array arr, move all the 0's at the end of it while
// maintaining the relative order of the non-zero elements.

// Note that you must do this in-place without making a copy of the array.

#include <iostream>
#include <vector>
using namespace std;

// Two pointers: write index for the next non-zero element.
// Every element is visited once, so the time is O(n) and the space is O(1).
void swapZerosToEnd(vector<int> &v) {
    int write = 0;

    for (int i = 0; i < v.size(); i++) {
        if (v[i] != 0) {
            v[write] = v[i];
            write++;
        }
    }

    while (write < v.size()) {
        v[write] = 0;
        write++;
    }
}

int main() {
    int n;
    cin >> n;

    vector<int> v(n);

    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    swapZerosToEnd(v);

    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}

// Input:  8
//         0 1 0 3 12 0 5 0
// Output: 1 3 12 5 0 0 0 0

// space complexity -> O(1)
// Time complexity -> O(n)
// Special cases: all zeros, no zeros, empty array, a single element
