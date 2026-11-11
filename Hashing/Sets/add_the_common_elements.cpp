/**
 * Add the common elements
 * Given 2 vectors v1 and v2. Find out the common elements between the two and
 return the sum of them.

 * Input:- v1 = {1,1,2,3,3,3}
 * v2 = {5,6,7,5,2,3,6}

 * Output:- 5

 * Explanation:- The values common between v1 and v2 are 2, 3. So sum is 2 + 3
 = 5.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<int> v1(n);
    vector<int> v2(m);

    cout << "Enter elements of v1: " << endl;
    for (int i = 0; i < n; i++) {
        cin >> v1[i];
    }

    cout << "Enter elements of v2: " << endl;
    for (int i = 0; i < m; i++) {
        cin >> v2[i];
    }

    int ans_sum = 0;

    // Use sets so a value repeated in either vector is counted only once.
    set<int> s1(v1.begin(), v1.end());
    set<int> s2(v2.begin(), v2.end());

    for (auto ele : s2) {
        if (s1.find(ele) != s1.end()) {
            ans_sum += ele;
        }
    }

    cout << "Ans: " << ans_sum << endl;  // 5

    return 0;
}