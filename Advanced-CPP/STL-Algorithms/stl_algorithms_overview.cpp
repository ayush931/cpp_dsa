/**
 * STL algorithms -> ready-made functions that operate on ranges
 * A range is passed as [begin, end), where end points one past the last element
 * Most algorithms are in <algorithm>, numeric algorithms are in <numeric>
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {5, 2, 8, 1, 9, 3, 2};

    sort(v.begin(), v.end());
    sort(v.begin(), v.end(), greater<int>());

    reverse(v.begin(), v.end());

    cout << *min_element(v.begin(), v.end()) << endl;
    cout << *max_element(v.begin(), v.end()) << endl;

    cout << count(v.begin(), v.end(), 2) << endl;

    auto it = find(v.begin(), v.end(), 8);
    if (it != v.end()) {
        cout << "Found at index " << it - v.begin() << endl;
    }

    cout << accumulate(v.begin(), v.end(), 0) << endl;

    vector<int> v2 = {3, 1, 2, 1, 3};
    stable_sort(v2.begin(),
                v2.end());  // keeps the relative order of equal elements
    for (int x : v2) {
        cout << x << " ";
    }
    cout << endl;

    fill(v2.begin(), v2.end(), 7);
    cout << v2[0] << endl;

    return 0;
}

// Time complexity:-
// sort / stable_sort -> O(n log n)
// reverse, count, find, min_element, max_element, accumulate, fill -> O(n)
