/**
 * Given n integers (can be duplicates), print the second smallest integer. If
 * it does not exists, print -1.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    set<int> s;
    for (auto val : v) {
        s.insert(val);
    }

    if (s.size() < 2) {
        cout << "Second smallest number: -1" << endl;
        return 0;
    }

    auto itr = s.begin();
    itr++;

    cout << "Second smallest number: " << *itr << endl;  // 2

    return 0;
}