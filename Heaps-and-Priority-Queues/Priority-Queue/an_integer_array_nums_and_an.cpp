/**
 * Given an integer array nums and an integer k, return the kth largest element
 * in the array. Note that it is the kth largest element in the sorted order,
 * not the kth distinct element.
 *
 * Input: nums: [3, 7, 2, 9, 5], k: 3
 * Output: Ans = 5
 */

#include <bits/stdc++.h>
using namespace std;

int kthLargestElement(vector<int> a, int n, int k) {
    priority_queue<int, vector<int>, greater<int>> pq;  // minheap

    for (int i = 0; i < n; i++) {
        pq.push(a[i]);
        if (pq.size() > k) {
            pq.pop();  // removing the smallest element out of k + 1 elements
        }
    }

    return pq.top();
};

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    cout << kthLargestElement(a, n, k) << endl;

    return 0;
}
