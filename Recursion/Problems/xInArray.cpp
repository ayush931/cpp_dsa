// Given an array of n integers and a target value x. Print whether x exists in
// the array or not. constraints:- 0 <= n <= 1e6, -1e8 <= x <= 1e8, -1e8 <= a[i]
// <= 1e8 The recursive version uses O(n) stack space, so use an iterative loop
// for very large n.

#include <iostream>
using namespace std;

// f represents whether x is present in the range [i, n - 1] or not?

bool f(int *arr, int n, int i, int x) {
    // base case
    if (i == n) {
        // array is exhausted
        return false;
    }
    return (arr[i] == x) || f(arr, n, i + 1, x);
}

int main() {
    int arr[] = {5, 4, 1, 8, 6, -9, 8, 2, 3, 5};
    int n = 10;
    int x = 8;

    bool result = f(arr, n, 0, x);

    if (result) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}