/**
 * Given an array of length n and a target, return a pair whose sum is equal to
 the target. If there is no pair present, return -1.

 * Input 1: n = 7
 * Element = [1, 4, 5, 11, 13, 10, 2]
 * Target = 13
 * Output 1: [3, 6]
 */

#include <bits/stdc++.h>
using namespace std;

vector<int> targetSumPair(vector<int> &v, int targetSum) {
    unordered_map<int, int> m;
    vector<int> ans(2, -1);

    for (int i = 0; i < v.size(); i++) {
        if (m.find(targetSum - v[i]) != m.end()) {
            ans[0] = m[targetSum - v[i]];
            ans[1] = i;
            return ans;
        } else {
            m[v[i]] = i;
        }
    }

    return ans;
};

int main() {
    int n;
    cin >> n;

    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    int targetSum;
    cin >> targetSum;

    vector<int> ans = targetSumPair(v, targetSum);

    if (ans[0] == -1) {
        cout << -1 << endl;
    } else {
        cout << ans[0] << ", " << ans[1] << endl;  // 3, 6
    }

    return 0;
}