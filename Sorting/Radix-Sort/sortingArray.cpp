// Radix sort processes the digits from least significant to most significant.
// Each pass is a stable count sort on one decimal digit.
//
// To support negative values, shift every value by the minimum so all keys are
// non-negative, sort the shifted keys, then shift them back. The shift is done
// in long long so the full int range does not overflow.

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void countSort(vector<long long> &v, long long pos) {
    // create frequency array for digits 0..9
    vector<int> freq(10, 0);
    int n = v.size();

    for (int i = 0; i < n; i++) {
        freq[(int)((v[i] / pos) % 10)]++;
    }

    // calculate cumulative frequency
    for (int i = 1; i < 10; i++) {
        freq[i] += freq[i - 1];
    }

    // ans array
    vector<long long> ans(n);

    for (int i = n - 1; i >= 0; i--) {
        ans[--freq[(int)((v[i] / pos) % 10)]] = v[i];
    }

    for (int i = 0; i < n; i++) {
        v[i] = ans[i];
    }
}

void radixSort(vector<int> &v) {
    if (v.empty()) {
        return;
    }

    int min_ele = v[0];
    int max_ele = v[0];

    for (int x : v) {
        min_ele = min(min_ele, x);
        max_ele = max(max_ele, x);
    }

    vector<long long> shifted(v.size());

    for (int i = 0; i < (int)v.size(); i++) {
        shifted[i] = (long long)v[i] - min_ele;
    }

    long long max_shifted = (long long)max_ele - min_ele;

    for (long long pos = 1; pos <= max_shifted; pos *= 10) {
        countSort(shifted, pos);
    }

    for (int i = 0; i < (int)v.size(); i++) {
        v[i] = (int)(shifted[i] + min_ele);
    }
}

int main() {
    int n;
    cin >> n;

    vector<int> v(n);

    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    radixSort(v);

    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}

// Sample input:  6
//                170 45 75 90 802 24
// Sample output: 24 45 75 90 170 802
//
// Sample input:  4
//                -5 0 -12 7
// Sample output: -12 -5 0 7
//
// Time complexity -> O(d * (n + 10)), where d is the number of digits in the
//                    shifted range
// Space complexity -> O(n + 10)
