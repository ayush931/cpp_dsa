/**
 * Given an array of n integers. You are supposed to perform k operations on it.
 * At each operation, the smallest 2 elements of the array are removed from the
 * array, multiplied by each other, and the product is added back to the array.
 *
 * Return the largest element of the array after the k operations.
 * It is guaranteed that at least one element will remain after the k
 * operations.
 *
 * Input: n = 5, k = 3, Array = {2, 4, 3, 1, 5}
 * Output: 20
 */

/**
 * Approach:-
 * 1. Min heap
 * 2. Pop 2 elements, push their product
 * 3. pop until only 1 element remains
 */

#include <bits/stdc++.h>
using namespace std;

int largestPoint(vector<int> a, int n, int k) {
    priority_queue<int, vector<int>, greater<int>> pq;  // min heap

    for (auto i : a) {
        pq.push(i);
    }

    while (k--) {
        int firstElement = pq.top();
        pq.pop();

        int secondElement = pq.top();
        pq.pop();

        int product = firstElement * secondElement;
        pq.push(product);
    }

    // find largest element in pq
    while (pq.size() > 1) {
        pq.pop();
    }

    return pq.top();
};

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> a(n);

    for (auto &x : a) {
        cin >> x;
    }

    cout << largestPoint(a, n, k) << endl;

    return 0;
}

/**
 * Time complexity -> O(n + k log n) for the heap operations, plus O(n) to
 * find the largest remaining element
 * Space complexity -> O(n) for the heap
 */