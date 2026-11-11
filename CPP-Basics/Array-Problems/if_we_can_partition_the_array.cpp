// Check if we can partition the array into two subarrays with equal sum. More
// formally, check that the prefix sum of a part of the array is equal to the
// suffix sum of rest of the array.

#include <iostream>
#include <vector>
using namespace std;

bool checkPrefixSuffixSum(vector<int> &v) {
    int totalSum = 0;

    for (int i = 0; i < v.size(); i++) {
        totalSum = totalSum + v[i];
    }

    int prefixSum = 0;

    for (int i = 0; i < v.size(); i++) {
        prefixSum = prefixSum + v[i];
        int suffixSum = totalSum - prefixSum;

        if (suffixSum == prefixSum) {
            return true;
        }
    }
    return false;
}

int main() {
    int size;
    cout << "Enter the size of an array: " << endl;
    cin >> size;

    vector<int> v;

    cout << "Enter the element of an array: " << endl;

    for (int i = 0; i < size; i++) {
        int element;
        cin >> element;
        v.push_back(element);
    }

    cout << checkPrefixSuffixSum(v) << endl;

    return 0;
}