/**
 * Segment tree -> tree used for range queries and point updates
 * Every node stores the answer for a range
 * Root covers [0, n - 1], children split the range into two halves
 * Build -> O(n)
 * Query -> O(log n)
 * Point update -> O(log n)
 * Space -> O(4 * n)
 */

#include <iostream>
#include <vector>
using namespace std;

class SegmentTree {
    vector<int> tree;
    int n;

    void build(vector<int> &arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }

        int mid = start + (end - start) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);

        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    int query(int node, int start, int end, int left, int right) {
        // no overlap
        if (right < start || end < left) {
            return 0;
        }

        // complete overlap
        if (left <= start && end <= right) {
            return tree[node];
        }

        // partial overlap
        int mid = start + (end - start) / 2;
        int leftSum = query(2 * node, start, mid, left, right);
        int rightSum = query(2 * node + 1, mid + 1, end, left, right);

        return leftSum + rightSum;
    }

    void update(int node, int start, int end, int index, int value) {
        if (start == end) {
            tree[node] = value;
            return;
        }

        int mid = start + (end - start) / 2;
        if (index <= mid) {
            update(2 * node, start, mid, index, value);
        } else {
            update(2 * node + 1, mid + 1, end, index, value);
        }

        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

   public:
    SegmentTree(vector<int> &arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }

    int query(int left, int right) {
        return query(1, 0, n - 1, left, right);
    }

    void update(int index, int value) {
        update(1, 0, n - 1, index, value);
    }
};

int main() {
    vector<int> arr = {1, 3, 5, 7, 9, 11};

    SegmentTree st(arr);

    cout << st.query(1, 3) << endl;  // 3 + 5 + 7 = 15

    st.update(2, 10);                // arr becomes {1, 3, 10, 7, 9, 11}
    cout << st.query(1, 3) << endl;  // 3 + 10 + 7 = 20

    return 0;
}
