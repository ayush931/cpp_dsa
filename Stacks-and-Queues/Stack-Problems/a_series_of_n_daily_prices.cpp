/** Given a series of N daily prices for a stock, we need to calculate the span
 * of the stock's price for all N days. The span of the stock's price in one day
 * is the maximum number of consecutive days (starting from the day and going
 * backward) for which the stock price was less than or equal to the price of
 * that day.
 *
 * Example: prices = [100, 80, 60, 70, 60, 75, 85]
 * Spans:             1   1   1   2   1   4   6
 */

#include <bits/stdc++.h>
using namespace std;

vector<int> pge(vector<int> &arr) {
    int n = arr.size();
    vector<int> output(n, -1);  // indexes
    if (n == 0) {
        return output;
    }

    reverse(arr.begin(), arr.end());
    stack<int> st;
    st.push(0);

    for (int i = 1; i < n; i++) {
        while (!st.empty() and arr[i] > arr[st.top()]) {
            output[st.top()] = n - i - 1;
            st.pop();
        }
        st.push(i);
    }

    while (not st.empty()) {
        output[st.top()] = -1;
        st.pop();
    }

    reverse(output.begin(), output.end());
    reverse(arr.begin(), arr.end());

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

    vector<int> res = pge(v);

    for (int i = 0; i < res.size(); i++) {
        cout << (i - res[i]) << " ";
    }
    return 0;
}
