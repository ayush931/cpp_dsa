// Count sort: count how many times each value occurs, turn the counts into
// prefix sums, then place every value at its final position.
//
// The version below shifts values by the minimum, so it also works for
// negative numbers. It is stable because the output is filled from right to
// left. The cost depends on the range max - min, not just on n.

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void countSort(vector<int> &v) {
    int n = v.size();
    if (n == 0) {
        return;
    }

    // find the min and max element
    int min_ele = v[0];
    int max_ele = v[0];

    for (int i = 1; i < n; i++) {
        min_ele = min(min_ele, v[i]);
        max_ele = max(max_ele, v[i]);
    }

    long long range = (long long)max_ele - min_ele + 1;

    // create the freq array; index 0 represents min_ele
    vector<int> freq((size_t)range, 0);

    for (int i = 0; i < n; i++) {
        freq[v[i] - min_ele]++;
    }

    // calculate cumulative freq
    for (long long i = 1; i < range; i++) {
        freq[i] += freq[i - 1];
    }

    // calculate the sorted array
    vector<int> ans(n);

    for (int i = n - 1; i >= 0; i--) {
        ans[--freq[v[i] - min_ele]] = v[i];
    }

    // copy back the ans to original array
    for (int i = 0; i < n; i++) {
        v[i] = ans[i];
    }
}

int main() {
    int n;
    cout << "Enter the size of an array ";
    cin >> n;

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter the element of the array " << i + 1 << " ";
        cin >> a[i];
    }

    countSort(a);

    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}

// Sample input:  5
//                4 -2 4 0 -2
// Sample output: -2 -2 0 4 4
//
// Time complexity -> O(n + range), where range = max - min + 1
// Space complexity -> O(n + range)
//
// Special cases: empty array, all equal values, negative values, duplicates.
// Count sort is a poor choice when the range is much larger than n.
