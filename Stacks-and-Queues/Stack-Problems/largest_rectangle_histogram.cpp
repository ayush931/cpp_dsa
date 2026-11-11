/**
 * Given an array of integer heights representing the histogram's bar height
 * where the width of each bar is 1, return the area of the largest rectangle in
 * the histogram.
 */

#include <bits/stdc++.h>

#include <algorithm>
using namespace std;

int histogram(vector<int> &arr) {
    int n = arr.size();
    stack<int> st;  // indexes
    int ans = 0;
    if (n == 0) {
        return ans;
    }

    st.push(0);

    for (int i = 1; i < n; i++) {
        while (!st.empty() and arr[i] < arr[st.top()]) {
            int el = arr[st.top()];  // current bar to be removed and whose ans
                                     // will be calculated
            st.pop();
            int nsi = i;
            int psi = (st.empty()) ? (-1) : st.top();
            ans = max(ans, el * (nsi - psi - 1));
        }

        st.push(i);
    }

    while (not st.empty()) {
        int el = arr[st.top()];
        st.pop();
        int nsi = n;
        int psi = (st.empty()) ? (-1) : st.top();
        ans = max(ans, el * (nsi - psi - 1));
    }

    return ans;
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

    int ans = histogram(v);

    cout << ans << endl;

    return 0;
}
