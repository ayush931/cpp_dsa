// Given an array of integer 'a', move all the even integers at the beginning of
// the array followed by all the odd integers. The relative order of odd or even
// integers does not matter. Return any array that satisfies the condition.

#include <iostream>
#include <vector>
using namespace std;

void sortByParity(vector<int> &v) {
    int leftPointer = 0;
    int rightPointer = v.size() - 1;

    while (leftPointer < rightPointer) {
        if (v[leftPointer] % 2 == 1 && v[rightPointer] % 2 == 0) {
            swap(v[leftPointer], v[rightPointer]);
            leftPointer++;
            rightPointer--;
        }
        if (v[leftPointer] % 2 == 0) {
            leftPointer++;
        }
        if (v[rightPointer] % 2 == 1) {
            rightPointer--;
        }
    }
    return;
}

int main() {
    int size;
    cout << "Enter the size of vector: " << endl;
    cin >> size;

    vector<int> v;

    cout << "Enter the element of vector: " << endl;
    for (int i = 0; i < size; i++) {
        int element;
        cin >> element;
        v.push_back(element);
    }

    sortByParity(v);

    cout << "Sorted vector is: " << endl;

    for (int i = 0; i < size; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}