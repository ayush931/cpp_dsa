/**
 * Given N (very large), the task is to print the largest palindromic number
 * obtained by permuting the digits of N. If it is not possible to make a
 * palindromic number, then print an appropriate message. Input: 313551 Output:
 * 531135
 */

#include <bits/stdc++.h>
using namespace std;

bool isPossible(unordered_map<int, int> &mp) {
    int count = 0;
    for (int i = 0; i <= 9; i++) {
        if (mp.count(i)) {
            if (mp[i] % 2 != 0) {
                count++;
            }
            if (count > 1) {
                return false;
            }
        }
    }
    return true;
};

string max_palindrome(string num) {
    int l = num.size();
    unordered_map<int, int> mp;
    for (int i = 0; i < l; i++) {
        mp[num[i] - '0']++;
    }

    if (!isPossible(mp)) {
        return "NP";
    }

    vector<char> v(l);
    int front = 0;
    for (int i = 9; i >= 0; i--) {
        if (mp[i] % 2 != 0) {
            v[l / 2] = char(i + 48);
            mp[i]--;
        }
        while (mp[i] > 0) {
            v[front] = char(i + 48);
            v[l - front - 1] = char(i + 48);
            mp[i] -= 2;
            front++;
        }
    }

    string res = "";
    for (int i = 0; i < v.size(); i++) {
        res += v[i];
    }
    return res;
};

int main() {
    cout << max_palindrome("313551") << endl;
    return 0;
}

// Time:- O(length of string)