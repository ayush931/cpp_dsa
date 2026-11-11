// Rotate the given array 'a' by k steps, where k is non-negative.
// Note:- k can be greater than n as well where n is the size of array 'a'.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int array[] = {1, 2, 3, 4, 5};
    int size = sizeof(array) / sizeof(array[0]);
    int k = 4;

    // k can be greater than size.
    k = k % size;

    vector<int> ansArray(size);
    int j = 0;

    // inserting last k element in ans array.
    for (int i = size - k; i < size; i++) {
        ansArray[j++] = array[i];
    }

    // inserting first n-k elements in ans array.

    for (int i = 0; i < size - k; i++) {
        ansArray[j++] = array[i];
    }

    for (int i = 0; i < size; i++) {
        cout << ansArray[i] << endl;
    }

    return 0;
}