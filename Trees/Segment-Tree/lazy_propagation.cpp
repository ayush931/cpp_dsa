/**
 * Lazy propagation -> optimization for segment tree range updates
 * Instead of updating every leaf, store the pending update at a node
 * Push the pending value to children only when they are visited
 * Range update -> O(log n)
 * Range query -> O(log n)
 */

#include <iostream>
#include <vector>
using namespace std;

class SegmentTree {
    vector<int> tree;
    vector<int> lazy;
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

    void push(int node, int start, int end) {
        if (lazy[node] == 0) {
            return;
        }

        tree[node] += (end - start + 1) * lazy[node];

        if (start != end) {
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }

        lazy[node] = 0;
    }

    void update(int node, int start, int end, int left, int right, int value) {
        push(node, start, end);

        if (right < start || end < left) {
            return;
        }

        if (left <= start && end <= right) {
            lazy[node] += value;
            push(node, start, end);
            return;
        }

        int mid = start + (end - start) / 2;
        update(2 * node, start, mid, left, right, value);
        update(2 * node + 1, mid + 1, end, left, right, value);

        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    int query(int node, int start, int end, int left, int right) {
        push(node, start, end);

        if (right < start || end < left) {
            return 0;
        }

        if (left <= start && end <= right) {
            return tree[node];
        }

        int mid = start + (end - start) / 2;
        int leftSum = query(2 * node, start, mid, left, right);
        int rightSum = query(2 * node + 1, mid + 1, end, left, right);

        return leftSum + rightSum;
    }

   public:
    SegmentTree(vector<int> &arr) {
        n = arr.size();
        tree.resize(4 * n);
        lazy.assign(4 * n, 0);
        build(arr, 1, 0, n - 1);
    }

    void update(int left, int right, int value) {
        update(1, 0, n - 1, left, right, value);
    }

    int query(int left, int right) {
        return query(1, 0, n - 1, left, right);
    }
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};

    SegmentTree st(arr);

    st.update(1, 3, 10);             // add 10 to indices 1 to 3
    cout << st.query(0, 4) << endl;  // 1 + 12 + 13 + 14 + 5 = 45
    cout << st.query(1, 2) << endl;  // 12 + 13 = 25

    return 0;
}
