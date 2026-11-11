// Given an array of integers of size n. Answer q queries where you need to
// print the sum of values in a given range of indices from l to r (both
// included). Note:- The values of l and r in queries follow 1-based indexing.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int size;
    cout << "Enter the size of an array: " << endl;
    cin >> size;

    vector<int> v(size + 1, 0);

    cout << "Enter the element of an array: " << endl;

    for (int i = 1; i <= size; i++) {
        cin >> v[i];
    }

    // calculate prefix sum array

    for (int i = 1; i <= size; i++) {
        v[i] = v[i] + v[i - 1];
    }

    int q;
    cout << "Enter the no. of queries: " << endl;
    cin >> q;

    while (q--) {
        cout << "Enter the range from l to r: " << endl;

        int l, r;
        cin >> l >> r;

        int ans = 0;

        ans = v[r] - v[l - 1];
        cout << "Sum of the given range: " << ans << endl;
    }

    return 0;
}