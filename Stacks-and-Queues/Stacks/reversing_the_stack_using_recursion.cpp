// reversing the stack using recursion

#include <bits/stdc++.h>
using namespace std;

void insertAtBottom(stack<int> &st, int x) {
    /**
     * time: O(N)
     * space: O(N)
     */
    stack<int> temp;
    while (not st.empty()) {
        int curr = st.top();
        st.pop();
        temp.push(curr);
    }

    st.push(x);  // this is the point where x got inserted at the bottom
    while (not temp.empty()) {
        int curr = temp.top();
        temp.pop();
        st.push(curr);
    }
};

void f(stack<int> &st) {
    if (st.empty()) {
        return;
    }
    int curr = st.top();
    st.pop();
    f(st);
    insertAtBottom(st, curr);
}

int main() {
    stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);

    f(st);

    while (not st.empty()) {
        int curr = st.top();
        st.pop();
        cout << curr << endl;
    }

    return 0;
}
