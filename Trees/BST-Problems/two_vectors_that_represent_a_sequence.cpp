/**
 * Given two vectors that represent a sequence of keys. Imagine we make a Binary
 * Search Tree (BST) from each array. We need to tell whether two BSTs will be
 * identical or not without actually constructing the tree. Input: arr1 = {4, 2,
 * 5, 1, 3}, arr2 = {4, 5, 2, 3, 1} Output: BSTs are identical
 */

/**
 * 1. Find the first elements within range in both arrays.
 * 2. If both elements are leaf nodes -> return true
 * 3. If one elements is leaf nodes -> return false
 * 4. If elements not same -> return false
 * 5. Recursively check for right and left subtree
 */

#include <bits/stdc++.h>
using namespace std;

bool checkIdenticalBSTHelper(vector<int> &v1, vector<int> &v2, int a1, int a2,
                             int minValue, int maxValue) {
    // find the first element in v1 within range
    int i;
    for (i = a1; i < v1.size(); i++) {
        if (v1[i] > minValue && v1[i] < maxValue) {
            break;
        }
    }

    // find the first element in v2 within range
    int j;
    for (j = a2; j < v2.size(); j++) {
        if (v2[j] > minValue && v2[j] < maxValue) {
            break;
        }
    }

    // If no element is found within range (leaf nodes)
    if (i == v1.size() && j == v2.size()) {
        return true;
    }

    // If only one vector does not have an element within range
    if ((i == v1.size()) != (j == v2.size())) {
        return false;
    }

    // checking if both elements are equal
    if (v1[i] != v2[j]) {
        return false;
    }

    // recursively check for left and right subtree
    return checkIdenticalBSTHelper(v1, v2, i + 1, j + 1, minValue, v1[i]) &&
           checkIdenticalBSTHelper(v1, v2, i + 1, j + 1, v1[i], maxValue);
};

bool checkIdenticalBSTs(vector<int> &v1, vector<int> &v2) {
    return checkIdenticalBSTHelper(v1, v2, 0, 0, INT_MIN, INT_MAX);
};

int main() {
    cout << "Enter input - " << endl;
    int n;
    cin >> n;

    vector<int> v1(n), v2(n);

    for (int i = 0; i < n; i++) {
        cin >> v1[i];
    }

    for (int i = 0; i < n; i++) {
        cin >> v2[i];
    }

    if (checkIdenticalBSTs(v1, v2)) {
        cout << "BSTs are identical" << endl;
    } else {
        cout << "BSTs are not identical" << endl;
    }

    return 0;
}

/**
 * Time complexity -> O(n^2)
 * Space complexity -> O(h) -> height of the tree
 */