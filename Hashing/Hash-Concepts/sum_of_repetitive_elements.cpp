/**
 * Sum of Repetitive elements

 * You are given an integer n, representing the number of elements. Then, you
 will be given n elements. You have to return the sum of repetitive elements
 i.e. the elements that appear more than one time.

 * Input: n = 7, Element = {1, 1, 2, 1, 3, 3, 3}
 *
 * Output: 4, The repetitive elements are 1, 3 and the sum is 4.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> input(n);

    for (int i = 0; i < n; i++) {
        cin >> input[i];
    }

    map<int, int> m;

    for (int i = 0; i < n; i++) {
        // storing frequency of every element in input array
        m[input[i]]++;
    }

    // [1, 2] [7, 1]
    /**
     * m[1]++
     * m[1]++
     * m[7]++
     */

    int sum = 0;
    // finding sum of repetitive elements
    // pair - element, frequency
    for (auto pair : m) {
        if (pair.second > 1) {
            sum = sum + pair.first;
        }
    }

    cout << "ANS - " << sum << endl;  // ANS - 4

    return 0;
}

// Time:- O(n log n) because every map operation is O(log n)
// Space:- O(n) for the frequency map