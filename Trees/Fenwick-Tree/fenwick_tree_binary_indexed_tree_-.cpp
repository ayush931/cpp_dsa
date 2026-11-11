/**
 * Fenwick tree (Binary Indexed Tree) -> stores prefix sums in O(n) space
 * Every index stores the sum of a range whose length is the lowest set bit of
 * the index Point update -> O(log n) Prefix sum -> O(log n) Range sum -> O(log
 * n) Uses 1-based indexing
 */

#include <iostream>
#include <vector>
using namespace std;

class FenwickTree {
    vector<int> bit;
    int n;

   public:
    FenwickTree(int size) {
        n = size;
        bit.assign(n + 1, 0);
    }

    FenwickTree(vector<int> &arr) {
        n = arr.size();
        bit.assign(n + 1, 0);

        for (int i = 0; i < n; i++) {
            update(i + 1, arr[i]);
        }
    }

    void update(int index, int delta) {
        while (index <= n) {
            bit[index] += delta;
            index += index & (-index);
        }
    }

    int prefixSum(int index) {
        int sum = 0;

        while (index > 0) {
            sum += bit[index];
            index -= index & (-index);
        }

        return sum;
    }

    int rangeSum(int left, int right) {
        return prefixSum(right) - prefixSum(left - 1);
    }
};

int main() {
    vector<int> arr = {1, 3, 5, 7, 9};

    FenwickTree ft(arr);

    cout << ft.prefixSum(3) << endl;    // 1 + 3 + 5 = 9
    cout << ft.rangeSum(2, 4) << endl;  // 3 + 5 + 7 = 15

    ft.update(2, 4);                    // arr[2] becomes 7
    cout << ft.rangeSum(1, 3) << endl;  // 1 + 7 + 5 = 13

    return 0;
}

// index & (-index) isolates the lowest set bit
// Fenwick tree is faster and uses less memory than a segment tree for prefix
// sums
