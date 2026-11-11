// insert at bottom using recursion

#include <bits/stdc++.h>
using namespace std;

void f(stack<int> &st, int x) {
    /**
     * time: O(N)
     * space: O(N)
     */

    if (st.empty()) {
        st.push(x);
        return;
    }

    int curr = st.top();
    st.pop();
    f(st, x);
    st.push(curr);
};

int main() {
    stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);

    f(st, 100);

    while (not st.empty()) {
        int curr = st.top();
        st.pop();
        cout << curr << endl;
    }

    return 0;
}