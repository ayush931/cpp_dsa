/**
 * binary_search -> returns true if the target exists, range must be sorted
 * lower_bound  -> first position where the value can be inserted without
 * breaking order (>= target) upper_bound  -> first position where the value is
 * greater than target (> target) equal_range  -> pair of lower_bound and
 * upper_bound All of them take O(log n) comparisons
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {1, 2, 2, 2, 4, 5, 7};

    cout << binary_search(v.begin(), v.end(), 4) << endl;  // 1
    cout << binary_search(v.begin(), v.end(), 3) << endl;  // 0

    cout << lower_bound(v.begin(), v.end(), 2) - v.begin() << endl;  // 1
    cout << upper_bound(v.begin(), v.end(), 2) - v.begin() << endl;  // 4

    cout << upper_bound(v.begin(), v.end(), 2) -
                lower_bound(v.begin(), v.end(), 2)
         << endl;  // count of 2

    pair<vector<int>::iterator, vector<int>::iterator> range =
        equal_range(v.begin(), v.end(), 2);
    cout << range.first - v.begin() << " " << range.second - v.begin() << endl;

    int arr[] = {10, 20, 30, 40};
    cout << lower_bound(arr, arr + 4, 25) - arr << endl;  // 2

    return 0;
}

// lower_bound and upper_bound work on any sorted range, not only vectors
