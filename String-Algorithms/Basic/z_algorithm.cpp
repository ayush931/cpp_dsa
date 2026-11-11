/**
 * Z algorithm -> builds the Z array in O(n)
 * Z[i] is the length of the longest substring starting at i
 * which is also a prefix of the string
 * Pattern matching -> concatenate pattern + '$' + text and find Z[i] == pattern
 * length
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> zFunction(string s) {
    int n = s.size();
    vector<int> z(n, 0);

    int left = 0;
    int right = 0;

    for (int i = 1; i < n; i++) {
        if (i <= right) {
            z[i] = min(right - i + 1, z[i - left]);
        }

        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }

        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }

    return z;
};

vector<int> search(string text, string pattern) {
    vector<int> result;

    if (pattern.empty()) {
        return result;
    }

    string combined = pattern + "$" + text;
    vector<int> z = zFunction(combined);

    int m = pattern.size();
    int offset = m + 1;

    for (int i = offset; i < combined.size(); i++) {
        if (z[i] == m) {
            result.push_back(i - offset);
        }
    }

    return result;
};

int main() {
    string text = "ababcababcabc";
    string pattern = "abc";

    vector<int> positions = search(text, pattern);

    for (int pos : positions) {
        cout << pos << " ";
    }
    cout << endl;

    return 0;
}

// Time complexity -> O(n + m)
// Space complexity -> O(n + m)
