/**
 * Evaluate a prefix expression.
 *
 * Example:
 * Input:  "-9+*132"
 * Output: -4
 *
 * Assumes single-digit non-negative operands and a valid prefix expression.
 * Time and space complexity -> O(n)
 */

#include <bits/stdc++.h>
using namespace std;

int calc(int v1, int v2, char op) {
    if (op == '^') {
        return pow(v1, v2);
    }

    if (op == '*') {
        return v1 * v2;
    }

    if (op == '/') {
        return v1 / v2;
    }

    if (op == '+') {
        return v1 + v2;
    }

    return v1 - v2;
};

int eval(string &str) {
    stack<int> st;
    for (int i = str.size() - 1; i >= 0; i--) {
        char ch = str[i];
        if (isdigit(ch)) {
            st.push(ch - '0');
        } else {
            int v2 = st.top();
            st.pop();
            int v1 = st.top();
            st.pop();
            st.push(calc(v1, v2, ch));
        }
    }

    return st.top();
};

int main() {
    string str = "-9+*132";
    cout << eval(str) << endl;  // -4

    return 0;
}