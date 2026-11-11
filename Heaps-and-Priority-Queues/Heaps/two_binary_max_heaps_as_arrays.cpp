/**
 * Given two binary max heaps as arrays, the task is to merge the given heaps
 */

#include <iostream>
#include <vector>
using namespace std;

class Solution {
   public:
    void heapify(vector<int> &v, int size, int curr) {
        while (2 * curr + 1 < size) {
            int leftChild = 2 * curr + 1;
            int rightChild = 2 * curr + 2;
            int maxChild = leftChild;

            if (rightChild < size && v[rightChild] > v[leftChild]) {
                maxChild = rightChild;
            }

            if (v[maxChild] <= v[curr]) {
                return;
            }

            swap(v[maxChild], v[curr]);
            curr = maxChild;
        }
    };

    vector<int> mergeHeaps(vector<int> &a, vector<int> &b, int n, int m) {
        vector<int> mergedHeap;

        for (int value : a) {
            mergedHeap.push_back(value);
        }
        for (int value : b) {
            mergedHeap.push_back(value);
        }

        // build a max heap from the combined array, bottom-up
        int lastParentNode = (n + m) / 2 - 1;

        for (int i = lastParentNode; i >= 0; i--) {
            heapify(mergedHeap, n + m, i);
        }

        return mergedHeap;
    }
};

int main() {
    vector<int> first = {10, 5, 6, 2};
    vector<int> second = {12, 7, 9};

    Solution solution;
    vector<int> merged =
        solution.mergeHeaps(first, second, first.size(), second.size());

    for (int value : merged) {
        cout << value << " ";
    }
    cout << endl;  // 12 10 9 2 5 7 6

    // verify the max heap property
    bool valid = true;

    for (int i = 0; 2 * i + 1 < merged.size(); i++) {
        if (2 * i + 1 < merged.size() && merged[i] < merged[2 * i + 1]) {
            valid = false;
        }
        if (2 * i + 2 < merged.size() && merged[i] < merged[2 * i + 2]) {
            valid = false;
        }
    }

    cout << valid << endl;  // 1

    return 0;
}

// Time complexity -> O(n + m)
// Space complexity -> O(n + m) for the merged heap
// Special cases: one heap empty, both heaps empty, duplicate values,
// different sizes, already merged array (heapify still works)
