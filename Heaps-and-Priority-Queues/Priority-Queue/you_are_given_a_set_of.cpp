/**
 * You are given a set of points in an XY plane. Find the k closest points to
 * the origin using Manhattan distance: |x| + |y|.
 * It is guaranteed that no two points have the same distance from the origin.
 */

#include <bits/stdc++.h>
using namespace std;

vector<pair<int, int>> kClosestPoints(vector<pair<int, int>> pts, int n,
                                      int k) {
    priority_queue<pair<int, pair<int, int>>> pq;  // max heap

    for (auto &pt : pts) {
        int distance = abs(pt.first) + abs(pt.second);
        pq.push(make_pair(distance, pt));

        if (pq.size() > k) {
            pq.pop();  // remove largest distance pt in k + 1 pts
        }
    }

    vector<pair<int, int>> ans(k);
    while (!pq.empty()) {
        ans[pq.size() - 1] = pq.top().second;
        pq.pop();
    }

    return ans;
};

int main() {
    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> pts(n);

    for (auto &pt : pts) {
        cin >> pt.first >> pt.second;
    }

    vector<pair<int, int>> ansPts = kClosestPoints(pts, n, k);

    for (auto &pt : ansPts) {
        cout << pt.first << " " << pt.second << endl;
    }

    return 0;
}