// check whether a given bracket sequence is balanced or not.

#include <bits/stdc++.h>
using namespace std;

bool isValid(string str) {
    /**
     * time complexity:- O(n)
     * space complexity:- O(n)
     */

    stack<char> st;
    for (int i = 0; i < str.size(); i++) {
        char ch = str[i];  // current character
        if (ch == '[' or ch == '{' or ch == '(') {
            st.push(ch);
        } else {
            // closing bracket
            if (ch == ')' and !st.empty() and st.top() == '(') {
                st.pop();
            } else if (ch == ']' and !st.empty() and st.top() == '[') {
                st.pop();
            } else if (ch == '}' and !st.empty() and st.top() == '{') {
                st.pop();
            } else {
                return false;
            }
        }
    }

    return st.empty();
}

int main() {
    string str = "()()(){";
    cout << isValid(str) << endl;

    return 0;
}
