// next greater element

#include <bits/stdc++.h>
using namespace std;

vector<int> nge(vector<int> &arr) {
    int n = arr.size();
    vector<int> output(n, -1);
    if (n == 0) {
        return output;
    }

    stack<int> st;  // indexes
    st.push(0);

    // for smaller element just change the sign to <
    for (int i = 1; i < n; i++) {
        while (!st.empty() and arr[i] > arr[st.top()]) {
            output[st.top()] = arr[i];
            st.pop();
        }
        st.push(i);
    }

    // Unmatched indexes keep the -1 value they were initialized with.
    return output;
}

int main() {
    int n;
    cin >> n;
    vector<int> v;

    while (n--) {
        int x;
        cin >> x;
        v.push_back(x);
    }

    vector<int> res = nge(v);

    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << " ";
    }

    return 0;
}
