#include <bits/stdc++.h>
using namespace std;

string longestCommonPrefix(vector<string> &str) {
    if (str.empty()) {
        return "";
    }

    // Keeping first string as constant and comparing it with all other strings
    string s1 = str[0];
    int ans_length = s1.size();

    for (int i = 1; i < str.size(); i++) {
        int j = 0;
        while (j < s1.size() && j < str[i].size() &&
               s1[j] == str[i][j]) {  // finding the common prefix string length
            j++;
        }
        ans_length = min(ans_length, j);  // updating length of ans string
    }
    string ans = s1.substr(0, ans_length);
    return ans;
}

int main() {
    int n;
    cout << "Enter no of strings: ";
    cin >> n;

    cout << "Enter the string: ";
    vector<string> str(n);
    for (int i = 0; i < n; i++) {
        cin >> str[i];
    }

    cout << "Longest common prefix: " << longestCommonPrefix(str) << endl;

    return 0;
}

// Time complexity -> 0(n * m)