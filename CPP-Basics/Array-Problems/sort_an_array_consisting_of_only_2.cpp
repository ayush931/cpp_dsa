// Sort an array consisting of only 0s and 1s.
// Different method (two pointers)

// Sort an array consisting of only 0s and 1s.

#include <iostream>
#include <vector>
using namespace std;

void sortZerosAndOnes(vector<int> &v) {
    int leftPointer = 0;
    int rightPointer = v.size() - 1;

    while (leftPointer < rightPointer) {
        if (v[leftPointer] == 1 && v[rightPointer] == 0) {
            v[leftPointer++] = 0;
            v[rightPointer--] = 1;
        }
        if (v[leftPointer] == 0) {
            leftPointer++;
        }
        if (v[rightPointer] == 1) {
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

    cout << "Enter the element of vector in 0s and 1s: " << endl;
    for (int i = 0; i < size; i++) {
        int element;
        cin >> element;
        v.push_back(element);
    }

    sortZerosAndOnes(v);

    cout << "Sorted vector is: " << endl;

    for (int i = 0; i < size; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}