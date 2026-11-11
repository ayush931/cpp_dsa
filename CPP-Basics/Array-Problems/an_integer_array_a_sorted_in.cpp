// Given an integer array 'a' sorted in non-decreasing order, return an array of
// the squares of each number sorted in non-decreasing order.

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void sortedSquaredArray(vector<int> &v) {
    vector<int> ans;

    int leftPointer = 0;
    int rightPointer = v.size() - 1;

    while (leftPointer <= rightPointer) {
        if (abs(v[leftPointer]) < abs(v[rightPointer])) {
            ans.push_back(v[rightPointer] * v[rightPointer]);
            rightPointer--;
        } else {
            ans.push_back(v[leftPointer] * v[leftPointer]);
            leftPointer++;
        }
    }
    reverse(ans.begin(), ans.end());

    cout << "Non-ascending squared sorted array: " << endl;

    for (int i = 0; i < ans.size(); i++) {
        cout << ans[i] << " ";
    }

    cout << endl;
}

int main() {
    int size;
    cout << "Enter the size of array: " << endl;
    cin >> size;

    vector<int> v;

    cout << "Enter the element of array: " << endl;

    for (int i = 0; i < size; i++) {
        int element;
        cin >> element;
        v.push_back(element);
    }

    sortedSquaredArray(v);

    return 0;
}