// Given an integers array 'a', return the prefix sum / running sum in the same
// array without creating a new array.

#include <iostream>
#include <vector>
using namespace std;

void runningSum(vector<int> &v) {
    for (int i = 1; i < v.size(); i++) {
        v[i] = v[i - 1] + v[i];
    }

    return;
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

    runningSum(v);

    cout << "Prefix sum of the given array: " << endl;

    for (int i = 0; i < size; i++) {
        cout << v[i] << " ";
    }

    cout << endl;

    return 0;
}