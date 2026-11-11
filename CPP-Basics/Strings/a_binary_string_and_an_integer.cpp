// Given a binary string and an integer k, return the maximum number of
// consecutive 1's in the string if you can flip at mos k 0's Sliding window
// technique

#include <bits/stdc++.h>
using namespace std;

int longestOnes(string str, int k) {
    int start = 0;
    int end = 0;

    int zero_count = 0;
    int max_length = 0;

    for (; end < str.length(); end++) {
        if (str[end] == '0') {
            zero_count++;
        }
        while (zero_count > k) {
            if (str[start] == '0') {
                zero_count--;
            }
            start++;  // contracting our window
        }
        // zero_count <= k
        max_length = max(max_length, end - start + 1);
    }
    return max_length;
}

int main() {
    string str;
    cout << "Enter binary string: ";
    cin >> str;

    int k;
    cout << "Enter max flips: ";
    cin >> k;

    cout << longestOnes(str, k) << endl;

    return 0;
}

// Time complexity -> 0(n)